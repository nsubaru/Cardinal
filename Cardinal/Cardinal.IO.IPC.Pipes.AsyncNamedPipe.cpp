// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

// ReSharper disable CppZeroConstantCanBeReplacedWithNullptr
#include "Cardinal.IO.IPC.Pipes.AsyncNamedPipe.h"

#define Cardinal_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

#pragma region Named pipe client

Cardinal::IO::IPC::Pipes::AsyncNamedPipeClient::BaseAsyncNamedPipeClient::BaseAsyncNamedPipeClient(String&& pipeName, PipeMode mode, Boolean isNoCache)
	: hPipe(nullptr), pipeName(MoveRValue(pipeName)), isNoCache(isNoCache), mode(mode), poolTask(nullptr), RefCount(1) {
	IO_STATUS_BLOCK ioStatusBlock = { 0 };
	OBJECT_ATTRIBUTES objectAttributes;
	UNICODE_STRING usPipeName = { 0 };

	String realPipeName = L"\\??\\pipe\\";
	realPipeName += this->pipeName;

	Unsafe(RtlInitUnicodeStringEx(&usPipeName, const_cast<PWSTR>(static_cast<ConstCharStr>(realPipeName))));

	InitializeObjectAttributes(&objectAttributes,
		&usPipeName,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL);

	ULONG pipeMode = NULL;

	if (mode & EPipeMode::In)
	{
		pipeMode |= GENERIC_READ;
	}

	if (mode & EPipeMode::Out)
	{
		pipeMode |= GENERIC_WRITE;
	}

	// Call NT service function
	NTSTATUS status = NtCreateFile(
		&this->hPipe,
		pipeMode | PIPE_WAIT,
		&objectAttributes,
		&ioStatusBlock,
		NULL,
		NULL,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		FILE_OPEN,
		(isNoCache ? FILE_WRITE_THROUGH : NULL),
		NULL,
		0);

	if (!NT_SUCCESS(status))
		Throw(NtException(SourcePosition, status));

	realPipeName.LeftResize(realPipeName.Lenght - 9);
}

void Cardinal::IO::IPC::Pipes::AsyncNamedPipeClient::BaseAsyncNamedPipeClient::InitAsyncRead(IIOTask*&& task, void* buffer, Memory::DataSizeT size, void* ioStatusBlk) {
	NTSTATUS status = NtReadFile(
		hPipe,
		NULL,
		NULL,
		task,
		static_cast<IO_STATUS_BLOCK*>(ioStatusBlk),
		buffer,
		static_cast<ULONG>(size.Bytes),
		0,
		NULL
	);

	if (status == STATUS_PIPE_BROKEN)
	{
		this->Dispose();
		Throw(PipeBrokenException(SourcePosition));
	}
}

void Rem::Core::IO::IPC::Pipes::AsyncNamedPipeClient::BaseAsyncNamedPipeClient::InitAsyncWrite(IIOTask*&& task, void* buffer, Memory::DataSizeT size, void* ioStatusBlk) {
	NTSTATUS status = NtWriteFile(
		hPipe,
		NULL,
		NULL,
		task,
		static_cast<IO_STATUS_BLOCK*>(ioStatusBlk),
		buffer,
		static_cast<ULONG>(size.Bytes),
		0,
		NULL
	);

	if (status == STATUS_PIPE_BROKEN)
	{
		this->Dispose();
		Throw(PipeBrokenException(SourcePosition));
	}
}

void Cardinal::IO::IPC::Pipes::AsyncNamedPipeClient::BaseAsyncNamedPipeClient::CloseConnection() {
	if (this->hPipe != nullptr)
	{
		NtClose(this->hPipe);
		this->hPipe = nullptr;
	}
}

void Cardinal::IO::IPC::Pipes::AsyncNamedPipeClient::BaseAsyncNamedPipeClient::Flush() {
	if (this->hPipe == nullptr)
		Throw(NullPointException(SourcePosition));

	IO_STATUS_BLOCK ioStatusBlock;

	NTSTATUS status = NtFlushBuffersFile(this->hPipe, &ioStatusBlock);

	if (status == STATUS_PIPE_BROKEN)
	{
		this->CloseConnection();
		Throw(PipeBrokenException(SourcePosition));
	}
}

void Cardinal::IO::IPC::Pipes::AsyncNamedPipeClient::BaseAsyncNamedPipeClient::WaitClosing() const {
	if (this->hPipe == nullptr)
		Throw(NullPointException(SourcePosition));

	NtWaitForSingleObject(hPipe, TRUE, NULL);
}

void Cardinal::IO::IPC::Pipes::AsyncNamedPipeClient::BaseAsyncNamedPipeClient::WaitClosing(Time::TimeSpan timeout) const {
	if (this->hPipe == nullptr)
		Throw(NullPointException(SourcePosition));

	LARGE_INTEGER ntTimeout = { .QuadPart = -static_cast<Int64>(timeout.Ticks) };

	NtWaitForSingleObject(hPipe, TRUE, &ntTimeout);
}

void Cardinal::IO::IPC::Pipes::AsyncNamedPipeClient::BaseAsyncNamedPipeClient::Dispose() {
	CloseConnection();
}

#pragma endregion

#pragma region Named pipe server 

Cardinal::Handle Rem::Core::IO::IPC::Pipes::AsyncNamedPipeServer::BaseAsyncNamedPipeServer::CreateServerPipeHandle() {
	IO_STATUS_BLOCK ioStatusBlock;
	OBJECT_ATTRIBUTES objectAttributes;
	UNICODE_STRING usPipeName = { 0 };
	LARGE_INTEGER waitTime;

	String realPipeName = String(L"\\??\\pipe\\") + pipeName;

	Unsafe(RtlInitUnicodeStringEx(&usPipeName, const_cast<PWSTR>(static_cast<ConstCharStr>(realPipeName))));

	InitializeObjectAttributes(&objectAttributes,
		&usPipeName,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL);

	waitTime.QuadPart = -1 * Time::MiliSecondsTicksCount;

	HANDLE hPipe = NULL;

	ULONG pipeMode = NULL, inboundQuota = 0, outboundQuota = 0;

	if (mode & EPipeMode::In)
	{
		pipeMode |= GENERIC_READ;
		inboundQuota = 2048;
	}

	if (mode & EPipeMode::Out)
	{
		pipeMode |= GENERIC_WRITE;
		outboundQuota = 2048;
	}

	// Call NT service function
	NTSTATUS status = NtCreateNamedPipeFile(
		&hPipe,
		// Check is it new pipe or only new instance of exist pipe and set flags
		pipeMode | PIPE_WAIT | ((instances.Count > 0) ? FILE_CREATE_PIPE_INSTANCE : NULL),
		&objectAttributes,
		&ioStatusBlock,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		// If pipe new than create i else open
		((instances.Count > 0) ? FILE_CREATE : NULL) | FILE_OPEN_IF,
		(isNoCache ? FILE_WRITE_THROUGH : NULL),
		TRUE,
		TRUE,
		// Completition mode is same as PIPE_NOWAIT, 
		// in this mode pipe operation complete imediatly and return state as error code
		FALSE,
		maxConnectionCount,
		inboundQuota,
		outboundQuota,
		&waitTime);

	if (!NT_SUCCESS(status))
		Throw(NtException(SourcePosition, status));

	return hPipe;
}

void Cardinal::IO::IPC::Pipes::AsyncNamedPipeServer::BaseAsyncNamedPipeServer::CloseAllInstances() {
	// We copy list for non-blocking removing as 
	// closing instance also remove it from list of instances
	instances.CopyTo<Containers::List>().RemoveAll([] (auto&& instance) { instance->CloseConnection(); });
}

void Cardinal::IO::IPC::Pipes::AsyncNamedPipeServer::BaseAsyncNamedPipeServer::Dispose() {
	CloseAllInstances();
}

#pragma endregion

#pragma region Named pipe server instance

void Cardinal::IO::IPC::Pipes::AsyncNamedPipeServerInstance::BaseAsyncNamedPipeServerInstance::InitAsyncConnect(IIOTask*&& task, void* ioStatusBlk) {
	// Use IOCTL for async waiting to client
	NTSTATUS status = NtFsControlFile(hPipe,
		NULL,
		NULL,
		task,
		static_cast<IO_STATUS_BLOCK*>(ioStatusBlk),
		FSCTL_PIPE_LISTEN,
		NULL,
		NULL,
		NULL,
		NULL);

	if (!NT_SUCCESS(status))
	{
		Dispose();
		Throw(NullPointException(SourcePosition));
	}
}

void Cardinal::IO::IPC::Pipes::AsyncNamedPipeServerInstance::BaseAsyncNamedPipeServerInstance::InitAsyncRead(IIOTask*&& task, void* buffer, Memory::DataSizeT size, void* ioStatusBlk) {
	NTSTATUS status = NtReadFile(
		hPipe,
		NULL,
		NULL,
		task,
		static_cast<IO_STATUS_BLOCK*>(ioStatusBlk),
		buffer,
		static_cast<ULONG>(size.Bytes),
		0,
		NULL
	);

	if (status == STATUS_PIPE_BROKEN)
	{
		this->Dispose();
		Throw(PipeBrokenException(SourcePosition));
	}
}

void Cardinal::IO::IPC::Pipes::AsyncNamedPipeServerInstance::BaseAsyncNamedPipeServerInstance::InitAsyncWrite(IIOTask*&& task, void* buffer, Memory::DataSizeT size, void* ioStatusBlk) {
	NTSTATUS status = NtWriteFile(
		hPipe,
		NULL,
		NULL,
		task,
		static_cast<IO_STATUS_BLOCK*>(ioStatusBlk),
		buffer,
		static_cast<ULONG>(size.Bytes),
		0,
		NULL
	);

	if (status == STATUS_PIPE_BROKEN)
	{
		this->Dispose();
		Throw(PipeBrokenException(SourcePosition));
	}
}

void Cardinal::IO::IPC::Pipes::AsyncNamedPipeServerInstance::BaseAsyncNamedPipeServerInstance::CloseConnection() {
	if (this->hPipe != nullptr)
	{
		NtClose(this->hPipe);
		this->serverRef.Remove(true);
		this->hPipe = nullptr;
		this->ownerPipe.~AsyncNamedPipeServer();
	}
}

void Cardinal::IO::IPC::Pipes::AsyncNamedPipeServerInstance::BaseAsyncNamedPipeServerInstance::Flush() {
	if (this->hPipe == nullptr)
		Throw(NullPointException(SourcePosition));

	IO_STATUS_BLOCK ioStatusBlock = { 0 };
	NTSTATUS status = NtFlushBuffersFile(this->hPipe, &ioStatusBlock);

	if (status == STATUS_PIPE_BROKEN)
	{
		this->CloseConnection();
		Throw(PipeBrokenException(SourcePosition));
	}
}

void Cardinal::IO::IPC::Pipes::AsyncNamedPipeServerInstance::BaseAsyncNamedPipeServerInstance::WaitClosing() const {
	if (this->hPipe == nullptr)
		Throw(NullPointException(SourcePosition));

	NtWaitForSingleObject(hPipe, TRUE, NULL);
}

void Cardinal::IO::IPC::Pipes::AsyncNamedPipeServerInstance::BaseAsyncNamedPipeServerInstance::WaitClosing(Time::TimeSpan timeout) const {
	if (this->hPipe == nullptr)
		Throw(NullPointException(SourcePosition));

	LARGE_INTEGER ntTimeout = { .QuadPart = -static_cast<Int64>(timeout.Ticks) };

	NtWaitForSingleObject(hPipe, TRUE, &ntTimeout);
}

void Cardinal::IO::IPC::Pipes::AsyncNamedPipeServerInstance::BaseAsyncNamedPipeServerInstance::Dispose() {
	if (this->hPipe != nullptr)
	{
		// Set fake ref count value as pipe dtor can call in close connection due removing server ref
		this->RefCount = 2;
		CloseConnection();
		// Restore real ref count value
		this->RefCount = 0;
	}
}

#pragma endregion