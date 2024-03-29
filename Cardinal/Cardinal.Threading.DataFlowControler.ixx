export module Cardinal.Threading.DataFlowControler;

export import Cardinal.Core;

import Cardinal.Threading.Internal;

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

export namespace Cardinal::Threading
{
	/// <summary>
	/// Data flow balancing object class
	/// </summary>
	class DataFlowControler {
	private:
		struct ImplT {
			volatile ImplSpecificT _;
		};

	private:
		mutable ImplT lock;
		mutable ImplT producer;
		mutable ImplT consumer;
		mutable volatile bool isConsumerWork;
		mutable volatile bool isProducerWork;

	public:
		/// <summary>
		/// Data flow balancing object constructor
		/// </summary>
		DataFlowControler();

		/// <summary>
		/// Data addition method, allows to perform waiting if there is no free space
		/// </summary>
		/// <typeparam name="Lambda">A lambda or function type which will be called for the processing</typeparam>
		/// <param name="lambda">A lambda or function which will be called for the processing</param>
		/// <returns>A lambda or function result</returns>
		template<typename Lambda>
		ForceInline decltype(auto) Consume(Lambda&& lambda) const {
			if constexpr (TypeTraits::IsSameV<decltype(lambda()), void>)
			{
				try
				{
					Internals::AcquireLockExclusiveSRWLock(&lock);
					Interlocked::Store(isProducerWork, true);

					lambda();

					Interlocked::Store(isProducerWork, false);
					Internals::ReleaseLockExclusiveSRWLock(&lock);
					Internals::WakeOneCondVal(&consumer);
				}
				catch (...)
				{
					Interlocked::Store(isProducerWork, false);
					Internals::ReleaseLockExclusiveSRWLock(&lock);
					Internals::__CxxRethrowImpl();
				}
			}
			else
			{
				try
				{
					Internals::AcquireLockExclusiveSRWLock(&lock);
					Interlocked::Store(isProducerWork, true);

					if constexpr (TypeTraits::IsLValueReferenceV<decltype(lambda())>)
					{
						decltype(lambda()) ret(lambda());

						Interlocked::Store(isProducerWork, false);
						Internals::ReleaseLockExclusiveSRWLock(&lock);
						Internals::WakeOneCondVal(&consumer);

						return ret;
					}
					else
					{
						decltype(lambda()) ret(MoveRValue(lambda()));

						Interlocked::Store(isProducerWork, false);
						Internals::ReleaseLockExclusiveSRWLock(&lock);
						Internals::WakeOneCondVal(&consumer);

						return ret;
					}
				}
				catch (...)
				{
					Interlocked::Store(isProducerWork, false);
					Internals::ReleaseLockExclusiveSRWLock(&lock);
					Internals::__CxxRethrowImpl();
				}
			}
		}

		/// <summary>
		/// Data returning method, allows to perform waiting if there is no free space
		/// </summary>
		/// <typeparam name="Lambda">A lambda or function type which will be called for the processing</typeparam>
		/// <param name="lambda">A lambda or function which will be called for the processing</param>
		/// <returns>A lambda or function result</returns>
		template<typename Lambda>
		ForceInline decltype(auto) Produce(Lambda&& lambda) const {
			if constexpr (TypeTraits::IsSameV<decltype(lambda()), void>)
			{
				try
				{
					Internals::AcquireLockExclusiveSRWLock(&lock);
					Interlocked::Store(isConsumerWork, true);

					lambda();

					Interlocked::Store(isConsumerWork, false);
					Internals::ReleaseLockExclusiveSRWLock(&lock);
					Internals::WakeOneCondVal(&producer);
				}
				catch (...)
				{
					Interlocked::Store(isConsumerWork, false);
					Internals::ReleaseLockExclusiveSRWLock(&lock);
					Internals::__CxxRethrowImpl();
				}
			}
			else
			{
				try
				{
					Internals::AcquireLockExclusiveSRWLock(&lock);
					Interlocked::Store(isConsumerWork, true);

					if constexpr (TypeTraits::IsLValueReferenceV<decltype(lambda())>)
					{
						decltype(lambda()) ret(lambda());

						Interlocked::Store(isConsumerWork, false);
						Internals::ReleaseLockExclusiveSRWLock(&lock);
						Internals::WakeOneCondVal(&producer);

						return ret;
					}
					else
					{
						decltype(lambda()) ret(MoveRValue(lambda()));

						Interlocked::Store(isConsumerWork, false);
						Internals::ReleaseLockExclusiveSRWLock(&lock);
						Internals::WakeOneCondVal(&producer);

						return ret;
					}
				}
				catch (...)
				{
					Interlocked::Store(isConsumerWork, false);
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
		template<typename Lambda>
		ForceInline auto SharedLock(Lambda&& lambda) -> decltype(lambda()) const {
			if constexpr (TypeTraits::IsSameV<decltype(lambda()), void>)
			{
				try
				{
					Internals::AcquireLockSharedSRWLock(&lock);

					lambda();

					Internals::ReleaseLockSharedSRWLock(&lock);
				}
				catch (...)
				{
					Internals::ReleaseLockExclusiveSRWLock(&lock);
					Internals::__CxxRethrowImpl();
				}
			}
			else
			{
				try
				{
					Internals::AcquireLockSharedSRWLock(&lock);

					if constexpr (TypeTraits::IsLValueReferenceV<decltype(lambda())>)
					{
						decltype(lambda()) ret(lambda());

						Internals::ReleaseLockSharedSRWLock(&lock);

						return ret;
					}
					else
					{
						decltype(lambda()) ret(MoveRValue(lambda()));

						Internals::ReleaseLockSharedSRWLock(&lock);

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
		/// Performs the <see cref="Funct"/> with exclusive lock
		/// </summary>
		/// <param name="Funct">A function or lamda which will run in exclusive lock</param>
		/// <returns>Same as "Funct"</returns>
		template<typename Lambda>
		ForceInline auto ExclusiveLock(Lambda&& lambda) -> decltype(lambda()) const {
			if constexpr (TypeTraits::IsSameV<decltype(lambda()), void>)
			{
				try
				{
					Internals::AcquireLockExclusiveSRWLock(&lock);

					lambda();

					Internals::ReleaseLockExclusiveSRWLock(&lock);
				}
				catch (...)
				{
					Internals::ReleaseLockExclusiveSRWLock(&lock);
					Internals::__CxxRethrowImpl();
				}
			}
			else
			{
				try
				{
					Internals::AcquireLockExclusiveSRWLock(&lock);

					if constexpr (TypeTraits::IsLValueReferenceV<decltype(lambda())>)
					{
						decltype(lambda()) ret(lambda());

						Internals::ReleaseLockExclusiveSRWLock(&lock);

						return ret;
					}
					else
					{
						decltype(lambda()) ret(MoveRValue(lambda()));

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
		/// Waiting for data provider
		/// </summary>
		ForceInline void WaitForProducer() const {
			Internals::WaitCondVal(&producer, &lock);
		}

		/// <summary>
		/// Waiting for data consumer
		/// </summary>
		ForceInline void WaitForConsumer() const {
			Internals::WaitCondVal(&consumer, &lock);
		}

	public:
		/// <summary>
		/// Destructor
		/// </summary>
		constexpr ForceInline ~DataFlowControler() = default;
	};
}

#pragma warning(pop)