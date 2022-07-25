module;

#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

#include "Cardinal.SEH.h"

module EntryPoint;

#pragma warning(disable: 6262)

using namespace Cardinal;

constexpr Char IOPipeNameReq[] = L"Please specify IO pipe name";
constexpr auto ConnectionRetryTimes = 5;

volatile bool IsManual;
static KMS::Console::LineBufferT printBuffer;

void __cdecl terminate(void) {
	; // do nothing as we work without CRT
}

void PrintLineWithHex(const wchar_t* msg, LONGLONG code, int mode) {
	UNICODE_STRING str;
	RtlInitUnicodeString(&str, (wchar_t*)msg);
	NtDisplayString(&str);

	RtlZeroMemory(printBuffer, 1000 * sizeof(Char));
	str.Buffer = printBuffer;
	str.Length = 0;
	str.MaximumLength = 1000 * 2;
	union {
		LONGLONG v1;
		ULONGLONG v2;
	};
	v1 = code;
	RtlInt64ToUnicodeString(v2, mode, &str);
	NtDisplayString(&str);

	RtlInitUnicodeString(&str, (wchar_t*)L"\r\n");
	NtDisplayString(&str);
}

int filter(unsigned int code, struct _EXCEPTION_POINTERS* ep)
{
#ifdef _DEBUG
	__debugbreak();
#endif // DEBUG
	PrintLineWithHex(L"Catched SEH exception with code :", code, 16);
	LARGE_INTEGER delay = { .QuadPart = -30LL * 1000LL * 10000LL };
	NtDelayExecution(TRUE, &delay);
	return EXCEPTION_EXECUTE_HANDLER;
}

struct ThreadArgs {
	KMS::Console console;
	KMS::Shell shell;
};

void ReadThread(void* args) {
	ThreadArgs* threadArgs = (ThreadArgs*)args;
	KMS::Shell::TBuffer buffer;
	do {
		threadArgs->console.ReadLine(buffer);
		if (!threadArgs->shell.Write(buffer)) {
			break;
		}
	} while (true);
}

void WriteThread(void* args) {
	__try {
		ThreadArgs* threadArgs = (ThreadArgs*)args;
		KMS::Shell::TBuffer buffer;
		SizeT count = 0;

		do {
			if (!threadArgs->shell.Read(buffer)) {
				break;
			}

			threadArgs->console.SetLastCommandType(threadArgs->shell.GetLastCommandType());
			threadArgs->console.SetLastDatagramType(threadArgs->shell.GetLastDatagramType());
			threadArgs->console.SetLastLogLevel(threadArgs->shell.GetLastLogLevel());
			threadArgs->console.SetLastMessageType(threadArgs->shell.GetLastMessageType());

			threadArgs->console.WriteMultiLineMessage(buffer);
		} while (true);
	}
	__except (filter(GetExceptionCode(), GetExceptionInformation())) {
		;;
	}
}

struct WaitThreadArgs {
	HANDLE hParentThread;
	KMS::Keyboard::Keyboard keyboard;
};

void WaitThread(void* arg) {
	__try {
		using namespace Cardinal::KMS::Keyboard;
		WaitThreadArgs* data = (WaitThreadArgs*)arg;
		do {
			auto code = data->keyboard.ReadCode();

			if (Bits::BitCheck(code.Modificator, (SizeT)EModificators::SHIFT) &&
				Bits::BitCheck(code.Modificator, (SizeT)EModificators::CTRL) &&
				code.VkCode == VkCodes::C_KEY
				) {
				NtAlertThread(data->hParentThread);
				break;
			}
			if (Bits::BitCheck(code.Modificator, (SizeT)EModificators::SHIFT) &&
				Bits::BitCheck(code.Modificator, (SizeT)EModificators::CTRL) &&
				code.VkCode == VkCodes::M_KEY
				) {
				Cardinal::Interlocked::Store(IsManual, true);
				NtAlertThread(data->hParentThread);
				break;
			}

		} while (true);
	}
	__except (filter(GetExceptionCode(), GetExceptionInformation())) {
		;;
	}
}

DECLSPEC_NORETURN void NTAPI NtProcessStartup() {
	Memory::InitHeap();
	Cardinal::Interlocked::Store(IsManual, false);

	__try {
		KMS::Keyboard::Keyboard* keyboard = new KMS::Keyboard::Keyboard();
		KMS::Console* console = new KMS::Console(*keyboard);
		KMS::Shell* shell = new KMS::Shell(*console);

		if (!NT_SUCCESS((NTSTATUS)shell->GetStatus())) {
			__leave;
		}

		HANDLE hCurrentThread = NULL;
		NtDuplicateObject(
			NtCurrentProcess(),
			NtCurrentThread(),
			NtCurrentProcess(),
			&hCurrentThread,
			DUPLICATE_SAME_ACCESS,
			DUPLICATE_SAME_ATTRIBUTES,
			0
		);

		HANDLE hWaitThread = Threading::StartThread(WaitThread, new WaitThreadArgs{ hCurrentThread , *keyboard });

		LARGE_INTEGER delay = { .QuadPart = -10LL * Cardinal::KMS::Shell::SecondIntNtTime };
		auto status = NtWaitForSingleObject(hWaitThread, TRUE, &delay);

		if (status == STATUS_ALERTED || status == STATUS_SUCCESS)
		{
			if (Cardinal::Interlocked::Load(IsManual))
			{
				KMS::Shell::TBuffer pipeGenericName = { 0 };

				RtlCopyMemory(pipeGenericName, IOPipeNameReq, sizeof(IOPipeNameReq));
				console->WriteLine(pipeGenericName);
				RtlZeroMemory(pipeGenericName, sizeof(pipeGenericName));
				console->ReadLine(pipeGenericName);

				shell->SetPipes(pipeGenericName);
			}
			else
			{
				shell->SetDefaultPipes();
			}

			console->DisplayToBootScreen(L"Wait for kernel connecting...");

			for (size_t i = 0; i < ConnectionRetryTimes; i++)
				if (shell->WaitForConnection(static_cast<Cardinal::Int64>(15) * 1000)) break;

			HANDLE hThread[] = {
				Threading::StartThread(ReadThread, new ThreadArgs({ *console, *shell })),
				Threading::StartThread(WriteThread, new ThreadArgs({ *console, *shell }))
			};

			NtWaitForMultipleObjects(2, hThread, WAIT_TYPE::WaitAny, TRUE, NULL);
		}

		console->DisplayToBootScreen(L"Exit");
		delete keyboard;
		delete shell;
		delete console;
	}
	__except (filter(GetExceptionCode(), GetExceptionInformation())) {
		NtTerminateProcess(NtCurrentProcess(), STATUS_SUCCESS);
	}

	Memory::DestroyHeap();
	NtTerminateProcess(NtCurrentProcess(), STATUS_SUCCESS);
}