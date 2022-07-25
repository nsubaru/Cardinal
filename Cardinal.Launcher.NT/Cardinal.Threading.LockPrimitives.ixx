export module Cardinal.Threading:LockPrimitives;

import :Base;
import :Internal;

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

export namespace Cardinal::Threading
{
	/// <summary>
	/// Synchronization primitive, which allows simultaneous reading and exclusive write
	/// </summary>
	class SRWLock final
	{
	private:
		struct ImplT {
			void* _;
		};

		ImplT lock;
	public:
		constexpr ForceInline SRWLock()
		{
			lock._ = nullptr;
		}

		/// <summary>
		/// Executes <see cref="Funct"/> with exclusive lock
		/// </summary>
		/// <param name="Funct">Function or lambda which executes in exclusive lock</param>
		/// <returns>Same as "Funct"</returns>
		template<typename T>
		ForceInline decltype(auto) ExclusiveLock(T&& Funct)
		{
			if constexpr (TypeTraits::IsSameT<decltype(Funct()), void>())
			{
				Internals::AcquireLockExclusiveSRWLock(&lock);

				Funct();

				Internals::ReleaseLockExclusiveSRWLock(&lock);
			}
			else
			{
				Internals::AcquireLockExclusiveSRWLock(&lock);

				if constexpr (TypeTraits::IsLValueReferenceV<decltype(Funct())>)
				{
					decltype(Funct()) ret(Funct());
					Internals::ReleaseLockExclusiveSRWLock(&lock);
					return ret;
				}
				else
				{
					decltype(Funct()) ret(MoveRValue(Funct()));
					Internals::ReleaseLockExclusiveSRWLock(&lock);
					return ret;
				}
			}
		}

		/// <summary>Executes <see cref="Funct"/> with shared lock</summary>
		/// <param name="Funct">Function or lambda which executes in shared lock</param>
		/// <returns>Same as "Funct"</returns>
		template<typename T>
		ForceInline auto ShareLock(T&& Funct) -> decltype(Funct())
		{
			if constexpr (TypeTraits::IsSameT<decltype(Funct()), void>())
			{
				Internals::AcquireLockSharedSRWLock(&lock);

				Funct();

				Internals::ReleaseLockSharedSRWLock(&lock);
			}
			else
			{
				Internals::AcquireLockSharedSRWLock(&lock);

				if constexpr (TypeTraits::IsLValueReferenceV<decltype(Funct())>)
				{
					decltype(Funct()) ret(Funct());
					Internals::ReleaseLockSharedSRWLock(&lock);
					return ret;
				}
				else
				{
					decltype(Funct()) ret(MoveRValue(Funct()));
					Internals::ReleaseLockSharedSRWLock(&lock);
					return ret;
				}
			}
		}
	public:
		ForceInline ~SRWLock() = default;
	};
}

#pragma warning(pop)