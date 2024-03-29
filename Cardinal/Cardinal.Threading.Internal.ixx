export module Cardinal.Threading.Internal;

export import Cardinal.Core;

export namespace Cardinal::Threading::Internals
{
	InternalApiCall void InitSRWLock(void* PSRWLock);

	InternalApiCall void InitCondVal(void* handle);

	InternalApiCall void AcquireLockExclusiveSRWLock(void* PSRWLock);

	InternalApiCall void ReleaseLockExclusiveSRWLock(void* PSRWLock);

	InternalApiCall void AcquireLockSharedSRWLock(void* PSRWLock);

	InternalApiCall void ReleaseLockSharedSRWLock(void* PSRWLock);

	InternalApiCall bool TryAcquireLockSharedSRWLock(void* PSRWLock);

	InternalApiCall bool TryAcquireLockExclusiveSRWLock(void* PSRWLock);

	InternalApiCall void WakeOneCondVal(void* condVar);

	InternalApiCall void WaitCondVal(void* condVar, void* srwLock);

	InternalApiCall void PulseNtEvent(void* handle);
	InternalApiCall void SetNtEvent(void* handle);

	InternalApiCall void ResetNtEvent(void* handle);

	InternalApiCall __declspec(noreturn) void __CxxRethrowImpl();

	InternalApiCall __declspec(noreturn) void ThrowDeadlock();
}