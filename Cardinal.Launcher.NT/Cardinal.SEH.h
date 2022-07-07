#pragma once
#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

// Exception disposition return values
struct _EXCEPTION_RECORD;
struct _CONTEXT;
struct _DISPATCHER_CONTEXT;

// C SEH handle declaration
EXCEPTION_DISPOSITION __cdecl __C_specific_handler(
	_In_    struct _EXCEPTION_RECORD* ExceptionRecord,
	_In_    void* EstablisherFrame,
	_Inout_ struct _CONTEXT* ContextRecord,
	_Inout_ struct _DISPATCHER_CONTEXT* DispatcherContext
);

// SEH intrinsics
#define GetExceptionCode        _exception_code
#define exception_code          _exception_code
#define GetExceptionInformation (struct _EXCEPTION_POINTERS*)_exception_info
#define exception_info          (struct _EXCEPTION_POINTERS*)_exception_info
#define AbnormalTermination     _abnormal_termination
#define abnormal_termination    _abnormal_termination

extern "C" {
	unsigned long __cdecl _exception_code(void);
	void* __cdecl _exception_info(void);
	int           __cdecl _abnormal_termination(void);
}

// Defined values for the exception filter expression
#define EXCEPTION_EXECUTE_HANDLER      1
#define EXCEPTION_CONTINUE_SEARCH      0
#define EXCEPTION_CONTINUE_EXECUTION (-1)