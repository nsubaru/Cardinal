module;

#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "ntapi.h"

module Cardinal.CsHelper;
import Cardinal.Core;

namespace Cardinal::CsHelper
{
	Cardinal::UInt64 StartRemKernel(wchar_t* path) {
		NTSTATUS status;
		PRTL_USER_PROCESS_PARAMETERS pp;
		RTL_USER_PROCESS_INFORMATION pi;
		UNICODE_STRING str;

		if (RtlDosPathNameToNtPathName_U(path, &str, NULL, NULL) == FALSE)
		{
			return 0;
		}

		status = RtlCreateProcessParameters(&pp,
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
			return 0;
		}

		status = RtlCreateUserProcess(
			&str,
			OBJ_CASE_INSENSITIVE | OBJ_INHERIT,
			pp,
			NULL,
			NULL,
			NULL,
			TRUE,
			NULL,
			NULL,
			&pi
		);

		if (STATUS_SUCCESS != status)
		{
			return 0;
		}

		RtlDestroyProcessParameters(pp);

		NtClose(pi.ProcessHandle);
		NtClose(pi.ThreadHandle);

		return reinterpret_cast<Cardinal::UInt64>(pi.ClientId.UniqueProcess);
	}
}