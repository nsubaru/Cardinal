export module Cardinal.Containers.Concurency.FixedQueue;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;
export import Cardinal.Threading.DataFlowControler;
export import Cardinal.Containers.InitilizerList;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
#pragma warning(disable: 4172)
#pragma warning(disable: 4996)

export namespace Cardinal::Containers
{
	/// <summary>
	/// A queue with fixed maximal number of elements, flow control and multithreading support
	/// </summary>
	template<typename THeap, SizeT TSize, typename T>
		requires (TSize > 0)
	class BaseConcurencyFixedQueue :
		public LINQ::IFixedQueue<BaseConcurencyFixedQueue, THeap, T, TSize, true> {
	private:
		SizeT front;
		SizeT rear;
		RefCountT count;
		T queue[TSize];
		Threading::DataFlowControler mutable flowControler;

	protected:
		T& GetByIdx(SizeT Index) {
			return queue[(front + Index) % TSize];
		}

		const T& GetByIdx(SizeT Index) const {
			return queue[(front + Index) % TSize];
		}

	public:
		~BaseConcurencyFixedQueue() {
			if constexpr (TypeTraits::IsSameV<THeap, Memory::SensitiveDataHeap> &&
				TypeTraits::IsPodV<T>)
			{
				Memory::SecureZeroMemory(queue);
			}
		}

	public:
		/// <summary>
		/// An empty constructor (initializes an empty queue)
		/// </summary>
		ForceInline BaseConcurencyFixedQueue() :
			front(0),
			rear(0),
			count(0),
			queue(),
			flowControler() {
		}

		/// <summary>
		/// A constructor with list of elements (if too much elements then the rest are ignored)
		/// </summary>
		/// <param name="list">List of elements</param>
		BaseConcurencyFixedQueue(InitilizerList<T>&& list) : BaseConcurencyFixedQueue() {
			if (list.size() != 0)
			{
				SizeT actuallyCopyied = (TSize < list.size()) ? TSize : list.size();
				auto it = list.begin();
				for (SizeT i = 0; i < actuallyCopyied; i++)
				{
					this->queue[i] = MoveRValue(*it); ++it;
				}
				this->rear = this->count = actuallyCopyied;
				this->front = 0;
			}
		}

		/// <summary>
		/// A queue copying constructor from original
		/// </summary>
		/// <param name="queue">Original queue</param>
		BaseConcurencyFixedQueue(const BaseConcurencyFixedQueue& queue) : flowControler() {
			queue.flowControler.SharedLock([&] () {
				this->front = queue.front;
				this->rear = queue.rear;
				this->count = queue.count;

				if constexpr (Concepts::IsPod<T>)
				{
					Memory::Copy(queue.queue, this->queue);
				}
				else
				{
					for (SizeT i = 0; i < this->count; i++)
					{
						this->GetByIdx(i) = queue.GetByIdx(i);
					}
				}
				});
		}

		/// <summary>
		/// A queue moving constructor from original
		/// </summary>
		/// <param name="queue">Original queue</param>
		BaseConcurencyFixedQueue(BaseConcurencyFixedQueue&& queue) : flowControler() {
			queue.flowControler.SharedLock([&]() InlineLambda{
				this->front = queue.front;
				queue.front = 0;
				this->rear = queue.rear;
				queue.rear = 0;
				this->count = queue.count;
				queue.count = 0;

				if constexpr (Concepts::IsPod<T>)
				{
					Memory::Copy(queue.queue, this->queue);
				}
				else
				{
					for (SizeT i = 0; i < this->count; i++)
					{
						this->GetByIdx(i) = MoveRValue(queue.GetByIdx(i));
					}
				}
				});
		}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"LValue" reference to queue</returns>
		BaseConcurencyFixedQueue& operator=(const BaseConcurencyFixedQueue& original) {
			this->~BaseConcurencyFixedQueue();
			this->BaseConcurencyFixedQueue::BaseConcurencyFixedQueue(original);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"RValue" reference to queue</returns>
		BaseConcurencyFixedQueue& operator=(BaseConcurencyFixedQueue&& original) {
			this->~BaseConcurencyFixedQueue();
			this->BaseConcurencyFixedQueue::BaseConcurencyFixedQueue(MoveRValue(original));
			return *this;
		}

		/// <summary>
		/// An element addition to the queue 
		/// (if queue is full then the thread will be waiting for freeing the memory in queue)
		/// </summary>
		/// <param name="Value">A value of element</param>
		void Enqueue(const T& Value) {
			this->flowControler.Produce([&]() InlineLambda{
				if (++this->count > TSize)
					this->flowControler.WaitForConsumer();

				SizeT newRear = (this->rear + 1) % TSize;
				this->GetByIdx(this->rear - this->front) = Value;
				this->rear = newRear;
				});
		}

		/// <summary>
		/// An element addition to the queue 
		/// (if queue is full then the thread will be waiting for freeing the memory in queue)
		/// </summary>
		/// <param name="Value">A value of element</param>
		void Enqueue(T&& Value) {
			this->flowControler.Produce([&, Value = MoveRValue(Value)]() InlineLambda{
				if (++this->count > TSize)
					this->flowControler.WaitForConsumer();

				SizeT newRear = (this->rear + 1) % TSize;
				this->GetByIdx(this->rear - this->front) = MoveRValue(Value);
				this->rear = newRear;
				});
		}

		/// <summary>
		/// Popping element from queue (if queue is empty then thread will stop executing)
		/// </summary>
		/// <returns>Popped element</returns>
		T Dequeue() {
			return this->flowControler.Consume([&] () -> T InlineLambda {
				if (this->count == 0)
					this->flowControler.WaitForProducer();

				T res = MoveRValue(this->GetByIdx(0));
				this->front = (this->front + 1) % TSize;
				--this->count;
				return res;
				});
		}

		/// <summary>
		/// Returns the first element from queue without popping (is queue is empty, generates an exception)
		/// </summary>
		/// <returns>A value of element</returns>
		ForceInline const T& Top() {
			return this->flowControler.SharedLock([&]() -> const T & InlineLambda{
				if (front == rear)
				Internals::Exceptions::ThrowEmptyQueueException(SourcePosition);

				return queue[front];
				});
		}

		/// <summary>
		/// Returns the first element from queue without popping (is queue is empty, generates an exception)
		/// </summary>
		/// <returns>A value of element</returns>
		ForceInline const T& Top() const {
			return this->flowControler.SharedLock([&]() -> const T & InlineLambda{
				if (front == rear)
				Internals::Exceptions::ThrowEmptyQueueException(SourcePosition);

				return queue[front];
				});
		}

		template<typename U>
		friend void Cardinal::Swap(U& A, U& B);

		/// <summary>
		/// Returns the amount of elements in queue
		/// </summary>
		/// <returns>A current number of elements in queue</returns>
		ForceInline SizeT GetCount() const { return this->count; }

		/// <summary>
		/// A current number of elements in queue
		/// </summary>
		SizeT Property(GetCount) Count;

		/// <summary>
		/// Queue object cloning
		/// </summary>
		/// <returns>Returns new queue object with copy of all elements</returns>
		template<typename = void> requires (Concepts::IsClonable<T>)
			BaseConcurencyFixedQueue Clone() const {
			return this->flowControler.SharedLock([&]() InlineLambda{
				BaseConcurencyFixedQueue result;
				result.front = this->front;
				result.rear = this->rear;
				result.count = this->count;

				if constexpr (Concepts::IsPod<T>)
				{
					Memory::Copy(this->queue, result.queue);
				}
				else
				{
					for (SizeT i = 0; i < this->count; i++)
					{
						result.GetByIdx(i) = Clone(this->GetByIdx(i));
					}
				}
				return result;
				});
		}

		/// <summary>
		/// Queue object cloning
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">A function with which occurs cloning</param>
		/// <returns>Returns new queue object with copy of all elements</returns>
		template<typename TLambda> requires (Concepts::IsInvockableWithResult<TLambda, T, const T&>)
			BaseConcurencyFixedQueue Clone(TLambda&& lambda) const {
			return this->flowControler.SharedLock([&]() InlineLambda{
				BaseConcurencyFixedQueue result;
				result.front = this->front;
				result.rear = this->rear;
				result.count = this->count;

				for (SizeT i = 0; i < this->count; i++)
				{
					result.GetByIdx(i) = lambda(this->GetByIdx(i));
				}
				return result;
				});
		}

	protected:
		template<template<typename, typename> typename TCollection, typename UHeap, typename U, Boolean TIsDynamic, Boolean TIsConcurencySuport, Boolean TIsReadonly, Boolean TIsCollectionCanReturnRef>
		friend class LINQ::ICollection;

		template<template<typename, SizeT, typename> typename TCollection, typename UHeap, typename U, SizeT USize, Boolean TIsConcurencySuport>
		friend class LINQ::IFixedCollection;

		template<template<typename, SizeT, typename> typename TCollection, typename UHeap, typename U, SizeT USize, Boolean TIsConcurencySuport>
		friend class LINQ::IFixedStack;

		template<template<typename UHeap, typename U> typename TCollection, typename UHeap, typename U, Boolean TIsDynamic, Boolean TIsConcurencySuport, Boolean TIsReadonly, Boolean TIsCollectionCanReturnRef>
		friend class LINQ::ICollectionBase;

		template<typename U>
		friend struct Cardinal::Serialization::SerializationHandler;

	protected:
		template<typename TLambda>
		BaseConcurencyFixedQueue& ForEachImpl(TLambda&& lambda) {
			if constexpr (TypeTraits::IsInvockable<TLambda, T&> ||
				TypeTraits::IsInvockable<TLambda, const T&>)
			{
				return this->flowControler.SharedLock([&]()->BaseConcurencyFixedQueue & InlineLambda{
					for (SizeT i = 0; i < this->count; i++)
					{
						lambda(this->GetByIdx(i));
					}
					return *this;
					});
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, T&, SizeT> ||
				TypeTraits::IsInvockable<TLambda, const T&>)
			{
				return this->flowControler.SharedLock([&]()->BaseConcurencyFixedQueue & InlineLambda{
					for (SizeT i = 0; i < this->count; i++)
					{
						lambda(this->GetByIdx(i), i);
					}

					return *this;
					});
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, T&> ||
					TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, T&, SizeT> ||
					TypeTraits::IsInvockable<TLambda, const T&>);
				__assume(0);
			}
		}

		template<typename TLambda>
		const BaseConcurencyFixedQueue& ForEachImpl(TLambda&& lambda) const {
			if constexpr (TypeTraits::IsInvockable<TLambda, const T&>)
			{
				return this->flowControler.SharedLock([&]() -> const BaseConcurencyFixedQueue & InlineLambda{
					for (SizeT i = 0; i < this->count; i++)
					{
						lambda(this->GetByIdx(i));
					}
					return *this;
					});
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				return this->flowControler.SharedLock([&]() -> const BaseConcurencyFixedQueue & InlineLambda{
					for (SizeT i = 0; i < this->count; i++)
					{
						lambda(this->GetByIdx(i), i);
					}

					return *this;
					});
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, const T&, SizeT>);
				__assume(0);
			}
		}

		template<typename U, typename TLambda>
		U AggregateImpl(TLambda&& lambda, const U& value) const {
			return this->flowControler.SharedLock([&]() mutable InlineLambda{
				U result = value;
				U & data = result;
				if (this->count == 0)
				{
					return MoveRValue(result);
				}

				for (SizeT i = 0; i < this->count; i++)
				{
					if constexpr (
						Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&> ||
						Concepts::IsInvockableWithResult<TLambda, U, U&, const T&>)
					{
						data = lambda(data, this->GetByIdx(i));
					}
					else
					{
						lambda(data, this->GetByIdx(i));
					}
				}

				return MoveRValue(result);
				});
		}

		template<typename U, typename TLambda>
		U AggregateImpl(TLambda&& lambda, U&& value) const {
			return this->flowControler.SharedLock([&, value = MoveRValue(value)]() mutable InlineLambda{
				U result = MoveRValue(value);

				if (this->count == 0)
					return MoveRValue(result);

				for (SizeT i = 0; i < this->count; i++)
				{
					if constexpr (
						Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&> ||
						Concepts::IsInvockableWithResult<TLambda, U, U&, const T&>)
					{
						value = lambda(value, this->GetByIdx(i));
					}
					else
					{
						lambda(value, this->GetByIdx(i));
					}
				}

				return MoveRValue(value);
				});
		}

		ForceInline bool AnyImpl() const { return this->count > 0; }

		template<typename TLambda>
		bool AnyImpl(TLambda&& lambda) const {
			return this->flowControler.SharedLock([&]() InlineLambda{
				if (this->count == 0)
					return false;

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(this->GetByIdx(i)))
						return true;
				}

				return false;
				});
		}

		template<typename TLambda>
		bool AllImpl(TLambda&& lambda) const {
			return this->flowControler.SharedLock([&]() InlineLambda{
				if (this->count == 0)
					return false;

				for (SizeT i = 0; i < this->count; i++)
				{
					if (!lambda(this->GetByIdx(i)))
						return false;
				}

				return true;
				});
		}

		template<typename TLambda>
		T& FirstImpl(TLambda&& lambda) {
			return this->flowControler.SharedLock([&]()->T & InlineLambda{
				if (this->count == 0)
					Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(this->GetByIdx(i)))
 {
return this->GetByIdx(i);
}
}

Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
				});
		}

		template<typename TLambda>
		T& LastImpl(TLambda&& lambda) {
			return this->flowControler.SharedLock([&]()->T & InlineLambda{
				if (this->count == 0)
					Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);

				T* item = nullptr;

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(this->GetByIdx(i)))
 {
item = &this->GetByIdx(i);
}
}

if (item != nullptr)
{
	return *item;
}
else
{
	Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
}
				});
		}

		template<typename TLambda>
		const T& FirstImpl(TLambda&& lambda) const {
			return this->flowControler.SharedLock([&]() -> const T & InlineLambda{
				if (this->count == 0)
					Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(this->GetByIdx(i)))
					{
						return this->GetByIdx(i);
					}
				}

				Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
				});
		}

		template<typename TLambda>
		const T& LastImpl(TLambda&& lambda) const {
			return this->flowControler.SharedLock([&]() -> const T & InlineLambda{
				if (this->count == 0)
					Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);

				const T* item = nullptr;

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(this->GetByIdx(i)))
					{
						item = &this->GetByIdx(i);
					}
				}

				if (item != nullptr)
				{
					return *item;
				}
				else
				{
					Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
				}
				});
		}

		template<typename TLambda>
		T FirstOrImpl(TLambda&& lambda, T&& element) const {
			return this->flowControler.SharedLock([&, element = MoveRValue(element)]() InlineLambda{
				if (count == 0)
					return MoveRValue(element);

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(this->GetByIdx(i)))
					{
						return this->GetByIdx(i);
					}
				}

				return MoveRValue(element);
				});
		}

		template<typename TLambda>
		T LastOrImpl(TLambda&& lambda, T&& element) const {
			return this->flowControler.SharedLock([&, element = MoveRValue(element)]() InlineLambda{
				if (count == 0)
					return MoveRValue(element);

				const T* item = nullptr;

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(this->GetByIdx(i)))
					{
						item = &this->GetByIdx(i);
					}
				}

				if (item != nullptr)
				{
					return *item;
				}
				else
				{
					return MoveRValue(element);
				}
				});
		}

		template<typename TLambda>
		T FirstOrImpl(TLambda&& lambda, const T& element) const {
			return this->flowControler.SharedLock([&]() InlineLambda{
				if (count == 0)
					return element;

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(this->GetByIdx(i)))
					{
						return this->GetByIdx(i);
					}
				}

				return element;
				});
		}

		template<typename TLambda>
		T LastOrImpl(TLambda&& lambda, const T& element) const {
			return this->flowControler.SharedLock([&]() InlineLambda{
				if (count == 0)
					return element;

				const T* item = nullptr;

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(this->GetByIdx(i)))
					{
						item = &this->GetByIdx(i);
					}
				}

				if (item != nullptr)
				{
					return *item;
				}
				else
				{
					return element;
				}
				});
		}

		template<typename TLambda, typename TGetterLambda>
		T FirstOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			return this->flowControler.SharedLock([&]() InlineLambda{
				if (count == 0)
					return otherLambda();

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(this->GetByIdx(i)))
					{
						return this->GetByIdx(i);
					}
				}

				return otherLambda();
				});
		}

		template<typename TLambda, typename TGetterLambda>
		T LastOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			return this->flowControler.SharedLock([&]() InlineLambda{
				if (count == 0)
					return otherLambda();

				const T* item = nullptr;

				for (SizeT i = 0; i < this->count; i++)
				{
					if (!lambda(this->GetByIdx(i)))
					{
						item = &this->GetByIdx(i);
					}
				}

				if (item != nullptr)
				{
					return *item;
				}
				else
				{
					return otherLambda();
				}
				});
		}

		BaseConcurencyFixedQueue& ReverseImpl() {
			return this->flowControler.SharedLock([&]()->BaseConcurencyFixedQueue & InlineLambda{
				for (SizeT i = 0; i < this->count / 2; i++)
				{
					Cardinal::Swap(this->GetByIdx(i), this->GetByIdx((this->count - 1) - i));
				}
				return *this;
				});
		}

		ForceInline void UnsafeSet(const T& value, SizeT index) {
			this->GetByIdx(index) = value;
			this->count = index + 1;
			this->rear = index;
		}

		ForceInline void UnsafeSet(T&& value, SizeT index) {
			this->GetByIdx(index) = MoveRValue(value);
			this->count = index + 1;
			this->rear = index;
		}

		BaseConcurencyFixedQueue& RemoveAllImpl() {
			return this->flowControler.SharedLock([&]()->BaseConcurencyFixedQueue & InlineLambda{
				if (this->count == 0)
					return *this;
				for (SizeT i = 0; i < this->count; i++)
				{
					this->GetByIdx(i).~T();
				}
				this->count = 0;
				return *this;
				});
		}

		template<typename TLambda>
		BaseConcurencyFixedQueue& RemoveAllImpl(TLambda&& lambda) {
			return this->flowControler.SharedLock([&, lambda = MoveRValue(lambda)]()->BaseConcurencyFixedQueue & InlineLambda{
				if (this->count == 0)
					return *this;
				for (SizeT i = 0; i < this->count; i++)
				{
					lambda(MoveRValue(this->GetByIdx(i)));
				}
				this->count = 0;
				return *this;
				});
		}
	};

	/// <summary>
	/// A queue with fixed maximal number of elements in an ordinary heap,
	/// flow control and multithreading support
	/// </summary>
	template<SizeT TSize, typename T>
	using ConcurencyFixedQueue = BaseConcurencyFixedQueue<Cardinal::Memory::Heap, TSize, T>;

	/// <summary>
	/// A queue with fixed maximal number of elements in a protected heap, 
	/// flow control and multithreading support
	/// </summary>
	template<SizeT TSize, typename T>
	using ConcurencySensitiveFixedQueue = BaseConcurencyFixedQueue<Cardinal::Memory::SensitiveDataHeap, TSize, T>;
}

export namespace Cardinal::TypeTraits::Collection
{
	template<typename THeap, SizeT TSize, typename T>
	struct IsFixedQueueT<Cardinal::Containers::BaseConcurencyFixedQueue<THeap, TSize, T>> : LogicTrue {};
}

export namespace Cardinal
{

	template<typename THeap, SizeT TSize, typename T>
	ForceInline void Swap(Containers::BaseConcurencyFixedQueue<THeap, TSize, T>& a, Containers::BaseConcurencyFixedQueue<THeap, TSize, T>& b) {
		a.flowControler.ExclusiveLock([&] () {
			b.flowControler.ExclusiveLock([&] () {
				Swap(a.front, b.front);
				Swap(a.rear, b.rear);
				Swap(a.count, b.count);
				for (SizeT i = 0; i < b.count; i++)
				{
					Swap(a.GetByIdx(i), b.GetByIdx(i));
				}
				});
			});
	}
}

#pragma warning(pop)