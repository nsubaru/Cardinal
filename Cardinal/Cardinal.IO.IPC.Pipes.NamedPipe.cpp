// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// ReSharper disable CppZeroConstantCanBeReplacedWithNullptr
#include "Cardinal.IO.IPC.Pipes.NamedPipe.h"

#define Cardinal_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

using namespace Cardinal;
using namespace Cardinal::IO::IPC::Pipes;

#pragma region Named pipe client

void Cardinal::IO::IPC::Pipes::NamedPipeClient::Dispose() {
	if (This->pipe != nullptr)
	{
		this->Close();
	}
}

void Cardinal::IO::IPC::Pipes::NamedPipeClient::ReadRaw(Containers::Blob& data) {
	if (this->This == nullptr || this->This->pipe == nullptr)
		Throw(NullPointException(SourcePosition));

	if (!(This->mode & EPipeMode::In || This->mode & InOutPipeMode))
		Throw(InvalidPipeException(SourcePosition));

	NTSTATUS status = STATUS_SUCCESS;

	IO_STATUS_BLOCK ioStatusBlock = { 0 };

	This->inputLock.ExclusiveLock([&]() InlineLambda
		{
			status = NtReadFile(
				This->pipe,
				NULL,
				NULL,
				NULL,
				&ioStatusBlock,
				(void*) (const Byte*) data,
				ULONG(data.Count),
				0,
				NULL
			);
		});

	if (status == STATUS_PIPE_BROKEN)
	{
		this->Close();
		Throw(PipeBrokenException(SourcePosition));
	}
}

void Cardinal::IO::IPC::Pipes::NamedPipeClient::WriteRawData(const Containers::Blob& data) {
	if (this->This == nullptr || this->This->pipe == nullptr)
		Throw(NullPointException(SourcePosition));

	if (!(This->mode & EPipeMode::Out || This->mode & InOutPipeMode))
		Throw(InvalidPipeException(SourcePosition));

	IO_STATUS_BLOCK ioStatusBlock = { 0 };
	NTSTATUS status = STATUS_SUCCESS;

	This->ouputLock.ExclusiveLock([&]() InlineLambda
		{
			NtFlushBuffersFile(This->pipe, &ioStatusBlock);
			status = NtWriteFile( //-V107
				This->pipe,
				NULL,
				NULL,
				NULL,
				&ioStatusBlock,
				(void*) (const Byte*) data,
				ULONG(data.Count),
				0,
				NULL
			);
		});

	if (status == STATUS_PIPE_BROKEN)
	{
		this->Close();
		Throw(PipeBrokenException(SourcePosition));
	}
}

void Cardinal::IO::IPC::Pipes::NamedPipeClient::Flush() {
	if (this->This == nullptr || this->This->pipe == nullptr)
		Throw(NullPointException(SourcePosition));

	IO_STATUS_BLOCK ioStatusBlock = { 0 };
	NTSTATUS status = NtFlushBuffersFile(This->pipe, &ioStatusBlock);

	if (status == STATUS_PIPE_BROKEN)
	{
		this->Close();
		Throw(PipeBrokenException(SourcePosition));
	}
}

void Cardinal::IO::IPC::Pipes::NamedPipeClient::Close() {
	if (this->This != nullptr)
		if (this->This->pipe != nullptr)
		{
			NtClose(This->pipe);
			this->This->pipe = nullptr;
		}
}

void Cardinal::IO::IPC::Pipes::NamedPipeClient::WaitClosing() const {
	if (this->This == nullptr || this->This->pipe == nullptr)
		Throw(NullPointException(SourcePosition));

	NtWaitForSingleObject(This->pipe, TRUE, NULL);
}

#pragma endregion

#pragma region Named pipe server

Core::Handle Cardinal::IO::IPC::Pipes::NamedPipeServer::CreateServerPipeHandle(bool isNoCache) {
	IO_STATUS_BLOCK ioStatusBlock = { 0 };
	OBJECT_ATTRIBUTES objectAttributes;
	UNICODE_STRING usPipeName = { 0 };
	LARGE_INTEGER waitTime = { .QuadPart = -1 * static_cast<LONGLONG>(Time::MiliSecondsTicksCount) };

	String realPipeName = String(L"\\??\\pipe\\") + This->name;

	RtlInitUnicodeStringEx(&usPipeName, const_cast<PWSTR>(static_cast<ConstCharStr>(realPipeName)));

	InitializeObjectAttributes(&objectAttributes,
		&usPipeName,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL);

	HANDLE hPipe = NULL;

	ULONG pipeMode = NULL, inBuffSize = 0, outBuffSize = 0;

	if (This->mode & EPipeMode::In)
	{
		pipeMode |= GENERIC_READ;
		inBuffSize = 2048;
	}

	if (This->mode & EPipeMode::Out)
	{
		pipeMode |= GENERIC_WRITE;
		outBuffSize = 2048;
	}

	NTSTATUS status = NtCreateNamedPipeFile(
		&hPipe,
		pipeMode | SYNCHRONIZE | PIPE_WAIT | ((This->instances.Count > 0) ? FILE_CREATE_PIPE_INSTANCE : NULL),
		&objectAttributes,
		&ioStatusBlock,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		FILE_OPEN_IF,
		FILE_SYNCHRONOUS_IO_ALERT | (isNoCache ? FILE_WRITE_THROUGH : NULL),
		TRUE,
		TRUE,
		FALSE,
		This->maxInstanceCount,
		inBuffSize,
		outBuffSize,
		&waitTime);

	if (!NT_SUCCESS(status))
		Throw(NtException(SourcePosition, status));

	return hPipe;
}

void Cardinal::IO::IPC::Pipes::NamedPipeServer::Dispose() {
	this->DestroyPipe();
}

NamedPipeServerInstance Rem::Core::IO::IPC::Pipes::NamedPipeServer::Connect(Boolean isNoCache) {
	if (This == nullptr)
		Throw(NullPointException(SourcePosition));

	Handle pipeHandle = CreateServerPipeHandle(isNoCache);

	try
	{

		IO_STATUS_BLOCK IoStatusBlock = { 0 };

		NTSTATUS status = NtFsControlFile(pipeHandle,
			NULL,
			NULL,
			NULL,
			&IoStatusBlock,
			FSCTL_PIPE_LISTEN,
			NULL,
			NULL,
			NULL,
			NULL);

		if (!NT_SUCCESS(status))
			Throw(NtException(SourcePosition, status));

		auto pipe = NamedPipeServerInstance(
			new NamedPipeServerInstance::Data(
				String(This->name),
				isNoCache,
				pipeHandle,
				This->mode)
		);

		auto it = This->instances.AddLast(pipe);

		pipe.This->ThisInstanceLink = it;
		return pipe;
	}
	catch (...)
	{
		if (pipeHandle != nullptr)
			NtClose(pipeHandle);

		ReThrow;
	}
}

void Cardinal::IO::IPC::Pipes::NamedPipeServer::CloseAllInstance() {
	if (this->This != nullptr)
	{
		this->This->instances
			.CopyTo<Containers::List>()
			.ForEach([&](NamedPipeServerInstance& item) InlineLambda{
				item.Close();
				})
			.RemoveAll();
	}
}

void Cardinal::IO::IPC::Pipes::NamedPipeServer::DestroyPipe() {
	if (this->This != nullptr)
	{
		this->CloseAllInstance();
	}
}

#pragma endregion

#pragma region NamedPipeServerInstance

void Cardinal::IO::IPC::Pipes::NamedPipeServerInstance::Dispose() {
	this->Close();
}

void Cardinal::IO::IPC::Pipes::NamedPipeServerInstance::ReadRaw(Containers::Blob& data) {
	if (this->This == nullptr || this->This->pipe == nullptr)
		Throw(NullPointException(SourcePosition));

	if (!(This->mode & EPipeMode::In || This->mode & InOutPipeMode))
		Throw(InvalidPipeException(SourcePosition));

	NTSTATUS status = STATUS_SUCCESS;

	IO_STATUS_BLOCK ioStatusBlock = { 0 };

	This->inputLock.ExclusiveLock([&]() InlineLambda
		{
			status = NtReadFile(
				This->pipe,
				NULL,
				NULL,
				NULL,
				&ioStatusBlock,
				(void*) (const Byte*) data,
				ULONG(data.Count),
				0,
				NULL
			);
		});

	if (status == STATUS_PIPE_BROKEN)
	{
		this->Close();
		Throw(PipeBrokenException(SourcePosition));
	}
}

void Cardinal::IO::IPC::Pipes::NamedPipeServerInstance::WriteRawData(const Containers::Blob& data) {
	if (this->This == nullptr || this->This->pipe == nullptr)
		Throw(NullPointException(SourcePosition));

	if (!(This->mode & EPipeMode::Out || This->mode & InOutPipeMode))
		Throw(InvalidPipeException(SourcePosition));

	IO_STATUS_BLOCK ioStatusBlock = { 0 };
	NTSTATUS status = STATUS_SUCCESS;

	This->ouputLock.ExclusiveLock([&]() InlineLambda
		{
			NtFlushBuffersFile(This->pipe, &ioStatusBlock);
			status = NtWriteFile( //-V107
				This->pipe,
				NULL,
				NULL,
				NULL,
				&ioStatusBlock,
				(void*) (const Byte*) data,
				ULONG(data.Count),
				0,
				NULL
			);
		});

	if (status == STATUS_PIPE_BROKEN)
	{
		this->Close();
		Throw(PipeBrokenException(SourcePosition));
	}
}

void Cardinal::IO::IPC::Pipes::NamedPipeServerInstance::Close() {
	if (this->This != nullptr)
		if (this->This->pipe != nullptr)
		{
			This->ThisInstanceLink.Remove(false);
			NtClose(This->pipe);
			this->This->pipe = nullptr;
		}
}

void Cardinal::IO::IPC::Pipes::NamedPipeServerInstance::WaitClosing() const {
	if (this->This == nullptr || this->This->pipe == nullptr)
		Throw(NullPointException(SourcePosition));

	NtWaitForSingleObject(This->pipe, TRUE, NULL);
}

void Cardinal::IO::IPC::Pipes::NamedPipeServerInstance::Flush() {
	if (this->This == nullptr || this->This->pipe == nullptr)
		Throw(NullPointException(SourcePosition));

	IO_STATUS_BLOCK ioStatusBlock = { 0 };
	NTSTATUS status = NtFlushBuffersFile(This->pipe, &ioStatusBlock);

	if (status == STATUS_PIPE_BROKEN)
	{
		this->Close();
		Throw(PipeBrokenException(SourcePosition));
	}
}

#pragma endregion

NamedPipeServer Cardinal::IO::IPC::Pipes::CreatePipe(
	const String& name,
	UInt8 MaxInstanceCount,
	PipeMode mode,
	bool isNoCache) {
	return NamedPipeServer(new NamedPipeServer::Data(mode, isNoCache, MaxInstanceCount, name));
}

NamedPipeClient Cardinal::IO::IPC::Pipes::ConnectPipe(const String& name, PipeMode mode, bool isNoCache) {
	IO_STATUS_BLOCK ioStatusBlock = { 0 };
	OBJECT_ATTRIBUTES objectAttributes;
	UNICODE_STRING usPipeName = { 0 };

	String realPipeName = L"\\??\\pipe\\";
	realPipeName += name;

	RtlInitUnicodeStringEx(&usPipeName, const_cast<PWSTR>(static_cast<ConstCharStr>(realPipeName)));

	InitializeObjectAttributes(&objectAttributes,
		&usPipeName,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL);

	HANDLE pipe = NULL;

	ULONG pipeMode = NULL;

	if (mode & EPipeMode::In)
	{
		pipeMode |= GENERIC_READ;
	}

	if (mode & EPipeMode::Out)
	{
		pipeMode |= GENERIC_WRITE;
	}

	NTSTATUS status = NtCreateFile(
		&pipe,
		pipeMode | SYNCHRONIZE | PIPE_WAIT,
		&objectAttributes,
		&ioStatusBlock,
		NULL,
		NULL,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		FILE_OPEN,
		FILE_SYNCHRONOUS_IO_ALERT | (isNoCache ? FILE_WRITE_THROUGH : NULL),
		NULL,
		0);

	if (!NT_SUCCESS(status))
		Throw(NullPointException(SourcePosition));

	realPipeName.LeftResize(realPipeName.Lenght - 9);
	return NamedPipeClient(new NamedPipeClient::Data(pipe, isNoCache, mode, MoveRValue(realPipeName)));
}

#pragma warning(pop)