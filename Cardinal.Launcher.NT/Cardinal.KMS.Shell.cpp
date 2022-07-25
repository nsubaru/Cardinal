module;

#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

module Cardinal.KMS;

using namespace Cardinal;
using namespace Cardinal;

void Cardinal::KMS::Shell::Dispose()
{
	if (This->hKernel != NULL)
		NtClose(This->hKernel);

	if (This->io.hGenericIoPipe != NULL)
		NtClose(This->io.hGenericIoPipe);
}

KMS::Shell::IO Cardinal::KMS::Shell::CreateIoPipe()
{
	if (This->io.hGenericIoPipe != NULL)
		NtClose(This->io.hGenericIoPipe);

	if (This->io.hGenericIoPipeEventRead != NULL)
		NtClose(This->io.hGenericIoPipeEventRead);

	if (This->io.hGenericIoPipeEventWrite != NULL)
		NtClose(This->io.hGenericIoPipeEventWrite);

	NTSTATUS status = STATUS_SUCCESS;
	IO io;
	IO_STATUS_BLOCK ioBlk;
	UNICODE_STRING usPipeName = { 0 };
	OBJECT_ATTRIBUTES objectAttributes = { 0 };

	status = RtlInitUnicodeStringEx(&usPipeName, (Char*)GenericIoPipeName);

	if (!NT_SUCCESS(status))
	{
		ioBlk.Status = status;
	}

	InitializeObjectAttributes(&objectAttributes,
		&usPipeName,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL);

	LARGE_INTEGER WaitTime = { 0 };
	WaitTime.QuadPart = -1 * 3600 * 1000ULL * 10000ULL;

	status = NtCreateNamedPipeFile(
		&io.hGenericIoPipe,
		GENERIC_READ | GENERIC_WRITE,
		&objectAttributes,
		&ioBlk,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		FILE_OPEN_IF,
		NULL,
		TRUE,
		TRUE,
		FALSE,
		2,
		1024 * 20,
		1024 * 20,
		&WaitTime);

	if (!NT_SUCCESS(status))
	{
		ioBlk.Status = status;
	}

	InitializeObjectAttributes(&objectAttributes, NULL, 0, NULL, NULL);
	status = NtCreateEvent(&io.hGenericIoPipeEventRead, GENERIC_ALL, &objectAttributes, SynchronizationEvent, FALSE);

	if (!NT_SUCCESS(status))
	{
		ioBlk.Status = status;
	}

	status = NtCreateEvent(&io.hGenericIoPipeEventWrite, GENERIC_ALL, &objectAttributes, SynchronizationEvent, FALSE);

	if (!NT_SUCCESS(status))
	{
		ioBlk.Status = status;
	}

	return io;
}

void Cardinal::KMS::Shell::Sleep(Int64 seconds)
{
	LARGE_INTEGER time;
	time.QuadPart = -1LL * seconds * Shell::SecondIntNtTime;
	NtDelayExecution(FALSE, &time);
}

Cardinal::KMS::Shell::Shell(Console& console) : This(new Data())
{
	NTSTATUS& status = *(NTSTATUS*)(&This->status);
	PRTL_USER_PROCESS_PARAMETERS procParam;
	RTL_USER_PROCESS_INFORMATION procInfo;
	UNICODE_STRING str;

	wchar_t kernelExcecutableName[] = L"Natsuki Cardinal.exe";

	console.DisplayToBootScreen(L"Get full path to Cardinal excecutable...");

	RtlDosPathNameToNtPathName_U(kernelExcecutableName, &str, NULL, NULL);
	console.DisplayToBootScreen(L"Create process params...");

	status = RtlCreateProcessParameters(&procParam,
		&str,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL
	);

	if (STATUS_SUCCESS != status)
	{
		console.DisplayToBootScreen(L"Error...");
		Sleep(10);
		return;
	}

	console.DisplayToBootScreen(L"Load kernel...");

	status = RtlCreateUserProcess(
		&str,
		OBJ_CASE_INSENSITIVE | OBJ_INHERIT,
		procParam,
		NULL,
		NULL,
		NULL,
		TRUE,
		NULL,
		NULL,
		&procInfo
	);

	if (STATUS_SUCCESS != status)
	{
		console.DisplayToBootScreen(L"Error...");
		Sleep(10);
		return;
	}

	RtlDestroyProcessParameters(procParam);

	console.DisplayToBootScreen(L"Start kernel...");

	status = NtResumeThread(procInfo.ThreadHandle, NULL);

	if (STATUS_SUCCESS != status)
	{
		console.DisplayToBootScreen(L"Error...");
		Sleep(10);
		return;
	}

	NtClose(procInfo.ThreadHandle);
	This->hKernel = procInfo.ProcessHandle;
	console.DisplayToBootScreen(L"Kernel started");
}

bool Cardinal::KMS::Shell::WaitForConnection(Int64 timeoutInMiliseconds)
{
	IO_STATUS_BLOCK ioStatusBlock = { 0 };
	NTSTATUS status = NtFsControlFile(This->io.hGenericIoPipe,
		This->io.hGenericIoPipeEventRead,
		NULL,
		NULL,
		&ioStatusBlock,
		FSCTL_PIPE_LISTEN,
		NULL,
		NULL,
		NULL,
		NULL);

	if (status == STATUS_PENDING)
	{
		LARGE_INTEGER delay = { 0 };
		delay.QuadPart = -10000LL * timeoutInMiliseconds;
		status = NtWaitForSingleObject(This->io.hGenericIoPipeEventRead, TRUE, &delay);

		if (status == STATUS_TIMEOUT)
			return false;

		status = NtFlushBuffersFile(This->io.hGenericIoPipe, &ioStatusBlock);
	}

	return NT_SUCCESS(status);
}

bool Cardinal::KMS::Shell::Read(TBuffer& buffer)
{
	IO_STATUS_BLOCK ioStatusBlock = { 0 };
	RtlZeroMemory(buffer, BufferSize);

	API::FixedSizeMessage<Shell::MessageSize> data{};

	NTSTATUS status = NtFlushBuffersFile(This->io.hGenericIoPipe, &ioStatusBlock);

	if (status == STATUS_PIPE_BROKEN)
		return false;

	if (!NT_SUCCESS(status))
		return false;

	status = NtReadFile( 
		This->io.hGenericIoPipe,
		This->io.hGenericIoPipeEventRead,
		NULL,
		NULL,
		&ioStatusBlock,
		&data,
		(ULONG)MessageSize, 
		0,
		NULL
	);

	if (status == STATUS_PENDING)
	{
		status = NtWaitForSingleObject(This->io.hGenericIoPipeEventRead, NULL, NULL);
	}

	if (status == STATUS_PIPE_BROKEN)
		return false;

	Interlocked::Store(This->commandType, data.CommandType);
	Interlocked::Store(This->datagramType, data.DatagramType);
	Interlocked::Store(This->logLevel, data.LogLevel);
	Interlocked::Store(This->messageType, data.MessageType);

	RtlCopyMemory(buffer, data.Data, BufferSize);

	return true;
}

void Cardinal::KMS::Shell::ProccesNewMessageType(API::MessageHeader& dest) {
	dest.CommandType = API::ECommandType::Undefined;
	dest.DatagramType = API::EDatagramType::Undefined;
	dest.LogLevel = API::ELogLevel::Undefined;
	dest.MessageType = API::EMessageType::Undefined;

	if (GetLastMessageType() == API::EMessageType::Command)
	{
		switch (GetLastCommandType())
		{
		case API::ECommandType::ArgumentRequest:
			dest.MessageType = API::EMessageType::Command;
			dest.CommandType = API::ECommandType::ArgumentAnswer;
			break;

		case API::ECommandType::PasswordRequest:
			dest.MessageType = API::EMessageType::Command;
			dest.CommandType = API::ECommandType::PasswordAnswer;
			break;

		default:
			break;
		}
	}
	else
	{
		dest.MessageType = API::EMessageType::Command;
		dest.CommandType = API::ECommandType::CommandRequest;
	}
}

bool Cardinal::KMS::Shell::Write(TBuffer& buffer)
{
	API::FixedSizeMessage<MessageSize> data{};
	IO_STATUS_BLOCK ioStatusBlock = { 0 };

	ProccesNewMessageType(data);
	RtlCopyMemory(data.Data, buffer, BufferSize);

	NTSTATUS status = NtFlushBuffersFile(This->io.hGenericIoPipe, &ioStatusBlock);

	if (status == STATUS_PIPE_BROKEN)
		return false;

	if (!NT_SUCCESS(status))
		return false;

	status = NtWriteFile( 
		This->io.hGenericIoPipe,
		This->io.hGenericIoPipeEventWrite,
		NULL,
		NULL,
		&ioStatusBlock,
		&data,
		(ULONG)MessageSize, 
		0,
		NULL
	);

	if (status == STATUS_PENDING)
	{
		status = NtWaitForSingleObject(This->io.hGenericIoPipeEventWrite, NULL, NULL);
	}

	if (status == STATUS_PIPE_BROKEN)
		return false;

	return true;
}

bool Cardinal::KMS::Shell::SetPipes(const KMS::Shell::TBuffer& pipeGenericName)
{
	if (This->io.hGenericIoPipe != NULL)
		NtClose(This->io.hGenericIoPipe);

	if (This->io.hGenericIoPipeEventRead != NULL)
		NtClose(This->io.hGenericIoPipeEventRead);

	if (This->io.hGenericIoPipeEventWrite != NULL)
		NtClose(This->io.hGenericIoPipeEventWrite);

	IO_STATUS_BLOCK ioBlk;
	UNICODE_STRING usPipeName = { 0 };
	OBJECT_ATTRIBUTES objectAttributes = { 0 };

	KMS::Shell::TBuffer buffer = L"\\Device\\NamedPipe\\";

	RtlInitUnicodeString(&usPipeName, buffer);
	usPipeName.MaximumLength = (KMS::Console::MessageSize - sizeof(L"\\Device\\NamedPipe\\"));

	RtlAppendUnicodeToString(&usPipeName, pipeGenericName);

	InitializeObjectAttributes(&objectAttributes,
		&usPipeName,
		OBJ_CASE_INSENSITIVE,
		NULL,
		NULL);

	LARGE_INTEGER WaitTime = { 0 };
	WaitTime.QuadPart = -1LL * 3600LL * 1000LL * 10000LL;

	NTSTATUS status = NtCreateNamedPipeFile(
		&This->io.hGenericIoPipe,
		GENERIC_READ | GENERIC_WRITE,
		&objectAttributes,
		&ioBlk,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		FILE_OPEN_IF,
		NULL,
		TRUE,
		TRUE,
		FALSE,
		2,
		MessageSize,
		MessageSize,
		&WaitTime);

	if (!NT_SUCCESS(status))
	{
		return false;
	}

	InitializeObjectAttributes(&objectAttributes, NULL, 0, NULL, NULL);
	status = NtCreateEvent(&This->io.hGenericIoPipeEventRead, GENERIC_ALL, &objectAttributes, SynchronizationEvent, FALSE);

	if (!NT_SUCCESS(status))
	{
		return false;
	}

	status = NtCreateEvent(&This->io.hGenericIoPipeEventWrite, GENERIC_ALL, &objectAttributes, SynchronizationEvent, FALSE);

	if (!NT_SUCCESS(status))
	{
		return false;
	}

	return true;
}