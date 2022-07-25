module;

#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

module Cardinal.Debug;
import Cardinal.KMS;

using namespace Cardinal;
using namespace Cardinal;

static Cardinal::KMS::Console::LineBufferT printBuffer;

void Cardinal::Internals::PrintLineWithHexImpl(const wchar_t* msg, Cardinal::Int64 code, int mode) {
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