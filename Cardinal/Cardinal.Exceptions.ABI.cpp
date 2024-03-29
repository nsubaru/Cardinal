module;
#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

module Cardinal.Exceptions.ABI;

EXCEPTION_DISPOSITION __cdecl __CxxFrameHandler4(
	EHExceptionRecord* pExcept,
	EHRegistrationNode RN,
	CONTEXT* pContext,
	DispatcherContext* pDC
) {
	FuncInfo4          FuncInfo;
	EXCEPTION_DISPOSITION   result{};
	EHRegistrationNode      EstablisherFrame = RN;
	GetThreadExceptionData()->_ImageBase = pDC->ImageBase;
	GetThreadExceptionData()->_ThrowImageBase = (uintptr_t) pExcept->params.pThrowImageBase;

	PBYTE buffer = (PBYTE) (pDC->ImageBase + *(PULONG) pDC->HandlerData);

	DecompFuncInfo(buffer, FuncInfo, pDC->ImageBase, pDC->FunctionEntry->BeginAddress);

	result = InternalCxxFrameHandler(pExcept, &EstablisherFrame, (_CONTEXT*) pContext, pDC, &FuncInfo, 0, nullptr, FALSE);
	return result;
}