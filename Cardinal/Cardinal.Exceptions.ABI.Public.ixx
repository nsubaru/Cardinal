export module Cardinal.Exceptions.ABI:Public;

import Cardinal.ConstString;

//Implement in public Exception subsystem API use in private ABI
__declspec(noreturn)
void ThrowBadException();

decltype(typeid(int)) GetBadExceptionTypeId();

void SehTranslatorFunction(unsigned int exCode, struct _EXCEPTION_POINTERS* exData);

__declspec(noreturn)
void EmergencyShutdown(
    Cardinal::SourceLocation sourceLocation,
    Cardinal::ConstString Reason
);

//Implement in Exception private ABI and use in API 
void StoreException(void*& ptr, void* (*alocator)(Cardinal::SizeT));

__declspec(noreturn) void ThrowStoredException(void* ptr);

void DestroyStoredException(void*& ptr, void(*dealocator)(void*));

