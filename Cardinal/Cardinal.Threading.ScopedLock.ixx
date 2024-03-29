export module Cardinal.Threading:ScopedLock;

export import :Base;

import Cardinal.Threading.Internal;
import Cardinal.Threading.SRWLock;

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)
	
export namespace Cardinal::Threading
{
	/// <summary>
	/// Automatically acquire lock on construct and release on destroy
	/// </summary>
	/// <typeparam name = "TIsShared">Lock mode</typeparam>
	template<bool TIsShared>
	class ScopedLock final {
	private:
		SRWLock& lock;

	public:
		/// <summary>
		/// Construct using SRWLock, automatically acquire lock
		/// </summary>
		/// <param name = "lock">Lock object</param>
		ForceInline ScopedLock(SRWLock& lock) : lock(lock) {
			if constexpr (TIsShared)
			{
				Internals::AcquireLockSharedSRWLock(&lock.lock);
			}
			else
			{
				Internals::AcquireLockExclusiveSRWLock(&lock.lock);
			}
		}

		/// <summary>
		/// Destroy object and automatically release lock
		/// </summary>
		ForceInline ~ScopedLock() {
			if constexpr (TIsShared)
			{
				Internals::ReleaseLockSharedSRWLock(&lock.lock);
			}
			else
			{
				Internals::ReleaseLockExclusiveSRWLock(&lock.lock);
			}
		}

	public:
		ScopedLock(const ScopedLock&) = delete;
		ScopedLock(ScopedLock&&) = delete;

		ScopedLock& operator=(const ScopedLock&) = delete;
		ScopedLock& operator=(ScopedLock&&) = delete;
	};

	/// <summary>
	/// Automatically acquire shared lock on construct and release it on destroy
	/// </summary>
	using SharedScoppedLock = ScopedLock<true>;

	/// <summary>
	/// Automatically acquire exclusive lock on construct and release it on destroy
	/// </summary>
	using ExclusiveScoppedLock = ScopedLock<false>;
}