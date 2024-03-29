export module Cardinal.Threading.SRWLock;

export import Cardinal.Core;

import Cardinal.Threading.Internal;

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

export namespace Cardinal::Threading
{
	template<bool isShared>
	class ScopedLock;

	/// <summary>
	/// Synchronization primitive which allows to simultaneous reading and exclusive writing
	/// </summary>
	class SRWLock final {
	private:
		struct ImplT {
			volatile ImplSpecificT _;
		};

		ImplT mutable lock;

	public:
		/// <summary>
		/// Empty constructor
		/// </summary>
		constexpr ForceInline SRWLock() {
			lock._ = nullptr;
		}

		/// <summary>
		/// Performs the <see cref="Funct"/> with exclusive lock
		/// </summary>
		/// <param name="Funct">A function or lamda which will run in exclusive lock</param>
		/// <returns>Same as "Funct"</returns>
		template<typename T> requires Concepts::IsInvockable<T>
		constexpr ForceInline decltype(auto) ExclusiveLock(T&& Funct) const {
			if (TypeTraits::IsCompileTimeEvaluating())
			{
				if constexpr (TypeTraits::IsSameT<decltype(Funct()), void>())
				{
					Funct();
					return;
				}
				else
				{
					return Funct();
				}
			}
			else if constexpr (TypeTraits::IsSameT<decltype(Funct()), void>())
			{
				Internals::AcquireLockExclusiveSRWLock(&lock);
				try
				{
					Funct();
				}
				catch (...)
				{
					Internals::ReleaseLockExclusiveSRWLock(&lock);
					Internals::__CxxRethrowImpl();
				}
				Internals::ReleaseLockExclusiveSRWLock(&lock);
			}
			else
			{
				Internals::AcquireLockExclusiveSRWLock(&lock);
				try
				{
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
				catch (...)
				{
					Internals::ReleaseLockExclusiveSRWLock(&lock);
					Internals::__CxxRethrowImpl();
				}
			}
		}

		/// <summary>
		/// Performs the <see cref="Funct"/> with shared lock
		/// </summary>
		/// <param name="Funct">A function or lamda which will run in shared lock</param>
		/// <returns>Same as "Funct"</returns>
		template<typename T> requires Concepts::IsInvockable<T>
		constexpr ForceInline auto SharedLock(T&& Funct) -> decltype(Funct()) const {
			if (TypeTraits::IsCompileTimeEvaluating())
			{
				if constexpr (TypeTraits::IsSameT<decltype(Funct()), void>())
				{
					Funct();
					return;
				}
				else
				{
					return Funct();
				}
			}
			else if constexpr (TypeTraits::IsSameT<decltype(Funct()), void>())
			{
				Internals::AcquireLockSharedSRWLock(&lock);
				try
				{
					Funct();
				}
				catch (...)
				{
					Internals::ReleaseLockSharedSRWLock(&lock);
					Internals::__CxxRethrowImpl();
				}
				Internals::ReleaseLockSharedSRWLock(&lock);
				return;
			}
			else
			{
				Internals::AcquireLockSharedSRWLock(&lock);
				try
				{
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
				catch (...)
				{
					Internals::ReleaseLockSharedSRWLock(&lock);
					Internals::__CxxRethrowImpl();
				}
			}
		}

		/// <summary>
		/// Attempt to capture shared lock
		/// </summary>
		/// <typeparam name="T">A lambda or function type</typeparam>
		/// <param name="Funct">A lambda or function</param>
		/// <returns>"True" if the lock capture was successful</returns>
		template<typename T> requires Concepts::IsInvockableWithResult<T, void>
		ForceInline Boolean TryInShareLock(T&& Funct) const {
			if (TypeTraits::IsCompileTimeEvaluating())
			{
				Funct();
				return true;
			}
			else if constexpr (TypeTraits::IsSameT<decltype(Funct()), void>())
			{
				if (!Internals::TryAcquireLockSharedSRWLock(&lock))
					return false;

				try
				{
					Funct();
				}
				catch (...)
				{
					Internals::ReleaseLockSharedSRWLock(&lock);
					Internals::__CxxRethrowImpl();
				}

				Internals::ReleaseLockSharedSRWLock(&lock);
				return true;
			}
			else
			{
				static_assert(TypeTraits::IsSameT<decltype(Funct()), void>(), "Try acquire can use only with void return function\\lambda");
				__assume(0);
			}
		}

	public:
		template<bool isShared>
		friend class ScopedLock;

	public:
		/// <summary>
		/// Destructor
		/// </summary>
		ForceInline ~SRWLock() = default;
	};
}

#pragma warning(pop)