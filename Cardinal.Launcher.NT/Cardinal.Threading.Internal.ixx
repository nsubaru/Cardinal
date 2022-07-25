export module Cardinal.Threading:Internal;

import :Base;

namespace Cardinal::Threading::Internals
{
	void InitSRWLock(void* PSRWLock);

	void InitCondVal(void* handle);

	void AcquireLockExclusiveSRWLock(void* PSRWLock);

	void ReleaseLockExclusiveSRWLock(void* PSRWLock);

	void AcquireLockSharedSRWLock(void* PSRWLock);

	void ReleaseLockSharedSRWLock(void* PSRWLock);

	bool TryAcquireLockSharedSRWLock(void* PSRWLock);

	void WakeOneCondVal(void* condVar);

	void WaitCondVal(void* condVar, void* srwLock);
}