module;

#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

module Cardinal.Threading;

using namespace Cardinal::Core;

void Cardinal::Core::Threading::Internals::InitSRWLock(void* PSRWLock) {
	RtlInitializeSRWLock(reinterpret_cast<PRTL_SRWLOCK>(PSRWLock));
}

void Cardinal::Core::Threading::Internals::InitCondVal(void* handle) {
	RtlInitializeConditionVariable((PRTL_CONDITION_VARIABLE)handle);
}

void Cardinal::Core::Threading::Internals::AcquireLockExclusiveSRWLock(void* PSRWLock)
{
	RtlAcquireSRWLockExclusive(reinterpret_cast<PRTL_SRWLOCK>(PSRWLock));
}

void Cardinal::Core::Threading::Internals::ReleaseLockExclusiveSRWLock(void* PSRWLock) {
	RtlReleaseSRWLockExclusive(reinterpret_cast<PRTL_SRWLOCK>(PSRWLock));
}

void Cardinal::Core::Threading::Internals::AcquireLockSharedSRWLock(void* PSRWLock) {
	RtlAcquireSRWLockShared(reinterpret_cast<PRTL_SRWLOCK>(PSRWLock));
}

void Cardinal::Core::Threading::Internals::ReleaseLockSharedSRWLock(void* PSRWLock) {
	RtlReleaseSRWLockShared(reinterpret_cast<PRTL_SRWLOCK>(PSRWLock));
	_ReadWriteBarrier();
}

bool Cardinal::Core::Threading::Internals::TryAcquireLockSharedSRWLock(void* PSRWLock) {
	return RtlTryAcquireSRWLockShared(reinterpret_cast<PRTL_SRWLOCK>(PSRWLock)) == 1;
}

struct ThreadArgs {
	void(*threadEntryPoint)(void*);
	void* arg;
};

NTSTATUS NTAPI nativeEntryPoint(
	_In_ PVOID ThreadParameter
) {
	ThreadArgs* args = (ThreadArgs*)ThreadParameter;
	args->threadEntryPoint(args->arg);
	return STATUS_SUCCESS;
}

Cardinal::Core::Handle Cardinal::Core::Threading::StartThread(void(*threadEntryPoint)(void*), void* arg)
{
	ThreadArgs* args = new ThreadArgs{ threadEntryPoint, arg };
	HANDLE hThread;
	CLIENT_ID id;
	size_t stack_max = 10485760, stack_alloc = 524288;
	RtlCreateUserThread(
		NtCurrentProcess(),
		NULL,
		false,
		0,
		stack_max,
		stack_alloc,
		nativeEntryPoint,
		args,
		&hThread,
		&id);
	return hThread;
}
