export module Cardinal.Containers.Concurency.DynamicQueue;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;
export import Cardinal.Containers.InitilizerList;
export import Cardinal.Settings;
export import Cardinal.Threading.DataFlowControler;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

#pragma warning(disable: 4172)

export namespace Cardinal::Containers
{
	/// <summary>
	/// A queue with multithreading support (also it implements a thread control)
	/// </summary>
	template<typename THeap, typename T>
	class BaseConcurencyDynamicQueue final :
		public LINQ::IDynamicStack<BaseConcurencyDynamicQueue, THeap, T, true> {
	private:
		SizeT front;
		SizeT rear;
		RefCountT count;
		T* Restrict queue;
		SizeT bufferSize;

		Threading::DataFlowControler mutable flowControler;

	protected:
		ForceInline T& GetByIdx(SizeT index) {
			return queue[(front + index) % bufferSize];
		}

		ForceInline const T& GetByIdx(SizeT index) const {
			return queue[(front + index) % bufferSize];
		}

	public:
		~BaseConcurencyDynamicQueue() {
			delete[] queue;
			front = rear = count = bufferSize = 0;
			queue = nullptr;
		}

	public:
		/// <summary>
		/// A dynamic queue empty constructor
		/// </summary>
		ForceInline BaseConcurencyDynamicQueue() :
			BaseConcurencyDynamicQueue(CurrentSettings().MemoryManager.QueueReserveSize) {}

		/// <summary>
		/// A queue constructor with the certain maximal number of elements
		/// </summary>
		/// <param name="startCount">A maximal number of elements in queue</param>
		ForceInline BaseConcurencyDynamicQueue(SizeT startCount) :
			front(0),
			rear(0),
			count(0),
			queue(new T[startCount]),
			bufferSize(startCount),
			flowControler() {
		}

		/// <summary>
		/// A queue constructor with elements from initialization list
		/// </summary>
		/// <param name="list">Initialization list</param>
		BaseConcurencyDynamicQueue(InitilizerList<T>&& list) :
			front(0),
			rear(0),
			count(0),
			queue(nullptr),
			bufferSize(0),
			flowControler() {
			if (list.size() != 0)
			{
				SizeT actuallyCopyied = list.size();
				bufferSize = list.size();
				queue = new T[bufferSize];
				auto it = list.begin();
				for (SizeT i = 0; i < actuallyCopyied; i++)
				{
					queue[i] = MoveRValue(*it); ++it;
				}
				rear = count = actuallyCopyied;
				front = 0;
			}
		}

		/// <summary>
		/// Copying constructor from existing queue
		/// </summary>
		/// <param name="queue">Existing queue</param>
		BaseConcurencyDynamicQueue(const BaseConcurencyDynamicQueue& queue) :
			front(0),
			rear(0),
			count(0),
			queue(nullptr),
			bufferSize(0),
			flowControler() {
			queue.flowControler.SharedLock([&] () {
				this->front = queue.front;
				this->rear = queue.rear;
				this->count = queue.count;
				this->bufferSize = queue.bufferSize;

				if constexpr (TypeTraits::IsPodV<T>)
				{
					Memory::Copy(queue.queue, this->queue, sizeof(T) * this->bufferSize);
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
		/// Moving constructor from existing queue
		/// </summary>
		/// <param name="queue">Existing queue</param>
		BaseConcurencyDynamicQueue(BaseConcurencyDynamicQueue&& queue) {
			queue.flowControler.ExclusiveLock([&] () mutable {
				this->front = queue.front;
				queue.front = 0;
				this->count = queue.count;
				queue.count = 0;
				this->rear = queue.rear;
				queue.rear = 0;
				this->bufferSize = queue.bufferSize;
				queue.bufferSize = 0;
				this->queue = queue.queue;
				queue.queue = nullptr;
				});
		}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"LValue" reference to queue</returns>
		BaseConcurencyDynamicQueue& operator=(const BaseConcurencyDynamicQueue& original) {
			this->~BaseConcurencyDynamicQueue();
			this->BaseConcurencyDynamicQueue::BaseConcurencyDynamicQueue(original);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"RValue" reference to queue</returns>
		BaseConcurencyDynamicQueue& operator=(BaseConcurencyDynamicQueue&& original) {
			this->~BaseConcurencyDynamicQueue();
			this->BaseConcurencyDynamicQueue::BaseConcurencyDynamicQueue(MoveRValue(original));
			return *this;
		}

		/// <summary>
		/// Adding the elements to the queue
		/// </summary>
		/// <param name="value">A value of element</param>
		/// <param name="isResizeIfFull">If "true" then queue will be extended automatically if needed, 
		/// if "false" and queue is full then it will stop command execution
		/// until the free space in queue appears</param>
		void Enqueue(const T& value, bool isResizeIfFull = false) {
			this->flowControler.Produce([&]() InlineLambda{
				if (this->count >= this->bufferSize)
 {
if (!isResizeIfFull)
	this->flowControler.WaitForConsumer();
else
	ExtendImpl(CurrentSettings().MemoryManager.QueueReserveSize);
}

SizeT newRear = (this->rear + 1) % this->bufferSize;
this->queue[this->rear] = value;
this->rear = newRear;
++this->count;
				});
		}

		/// <summary>
		/// Optimized elements addition to the queue
		/// </summary>
		/// <param name="value">"RValue" reference to element</param>
		/// <param name="isResizeIfFull">If "true" then queue will be extended automatically if needed, 
		/// if "false" and queue is full then it will stop command execution 
		/// until the free space in queue appears</param>
		void Enqueue(T&& value, bool isResizeIfFull = false) {
			this->flowControler.Produce([&, value = MoveRValue(value)]() mutable InlineLambda{
				if (this->count >= this->bufferSize)
 {
if (!isResizeIfFull)
	this->flowControler.WaitForConsumer();
else
	ExtendImpl(CurrentSettings().MemoryManager.QueueReserveSize);
}

SizeT newRear = (this->rear + 1) % this->bufferSize;
this->queue[this->rear] = MoveRValue(value);
this->rear = newRear;
++this->count;
				});
		}

		/// <summary>
		/// Element extraction from queue (if there are no elements
		/// then method will stop the thread execution until element appears)
		/// </summary>
		/// <returns>A value of element</returns>
		T Dequeue() {
			return this->flowControler.Consume([&]() InlineLambda{
				while (this->count == 0) this->flowControler.WaitForProducer();

				T res = MoveRValue(this->queue[this->front]);
				this->front = (this->front + 1) % this->bufferSize;
				--this->count;

				return MoveRValue(res);
				});
		}

		/// <summary>
		/// Returns element value without extraction from queue (if queue is empty, generates an exception)
		/// </summary>
		ForceInline const T& Top() {
			return this->flowControler.SharedLock([&]() -> const T & InlineLambda{
				if (this->front == this->rear)
				Internals::Exceptions::ThrowEmptyQueueException(SourcePosition);

				return this->queue[this->front];
				});
		}

		/// <summary>
		/// Returns element value without extraction from queue (if queue is empty, generates an exception)
		/// </summary>
		ForceInline const T& Top() const {
			return this->flowControler.SharedLock([&]() -> const T & InlineLambda{
				if (this->front == this->rear)
				Internals::Exceptions::ThrowEmptyQueueException(SourcePosition);

				return this->queue[this->front];
				});
		}

		/// <summary>
		/// Queue extension on given number of elements
		/// </summary>
		/// <param name="AdditionalSize">Number of elements to extend</param>
		ForceInline void Extend(SizeT AdditionalSize) {
			this->flowControler.ExclusiveLock([&]() InlineLambda{
				ExtendImpl(AdditionalSize);
				});
		}

		/// <summary>
		/// Queue shrinking on given number of elements
		/// </summary>
		void Shrink() {
			this->flowControler.ExclusiveLock([&]() InlineLambda{
				const SizeT newSize = this->count;
				T * newBuffer = new T[newSize];
				for (SizeT i = 0; i < this->count; i++)
				{
					newBuffer[i] = GetByIdx(i);
				}
				this->front = 0; this->rear = this->count;
				delete this->queue;
				this->queue = newBuffer;
				});
		}

		template<typename TU>
		ForceInline friend void Cardinal::Swap(TU& a, TU& b);

		/// <summary>
		/// A number of elements in queue
		/// </summary>
		/// <returns>A current number of elements</returns>
		ForceInline SizeT GetCount() const { return this->count; }

		/// <summary>
		/// A current number of elements
		/// </summary>
		SizeT Property(GetCount) Count;

		/// <summary>
		/// A maximal number of elements in queue
		/// </summary>
		/// <returns>A maximal number of elements</returns>
		ForceInline SizeT GetMaxCount() const { return this->bufferSize; }

		/// <summary>
		/// A maximal number of elements
		/// </summary>
		SizeT Property(GetMaxCount) MaxCount;

		/// <summary>
		/// Indicates whether queue is full
		/// </summary>
		/// <returns>Indicates whether queue is full</returns>
		Boolean GetIsFull() const {
			return this->count == this->bufferSize;
		}

		/// <summary>
		/// Indicates whether queue is full
		/// </summary>
		Boolean Property(GetIsFull) IsFull;

		/// <summary>
		/// Queue cloning
		/// </summary>
		/// <returns>Returnt a new queue with copies of all elements</returns>
		template<typename = void> requires (Concepts::IsClonable<T>)
			BaseConcurencyDynamicQueue Clone() const {
			return this->flowControler.SharedLock([&] () -> BaseConcurencyDynamicQueue InlineLambda {
				auto item = BaseConcurencyDynamicQueue(this->count);

				if constexpr (Concepts::IsPod<T>)
				{
					Memory::Copy(static_cast<const void*>(this->queue), static_cast<void*>(item.queue), this->count * sizeof(T));
				}
				else
				{
					for (SizeT i = 0; i < this->count; i++)
					{
						item.GetByIdx(i) = Clone(this->GetByIdx(i));
					}
				}

				item.front = this->front;
				item.rear = this->rear;
				item.count = this->count;
				return item;
				});
		}

		/// <summary>
		/// Queue cloning using custom lambda or functor
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">A function with which occurs cloning</param>
		/// <returns>Returnt a new queue with copies of all elements</returns>
		template<typename TLambda> requires (Concepts::IsInvockableWithResult<TLambda, T, const T&>)
			BaseConcurencyDynamicQueue Clone(TLambda&& lambda) const {
			return this->flowControler.SharedLock([&] () -> BaseConcurencyDynamicQueue InlineLambda {
				auto item = BaseConcurencyDynamicQueue(this->count);

				for (SizeT i = 0; i < this->count; i++)
				{
					item.GetByIdx(i) = lambda(this->GetByIdx(i));
				}

				item.front = this->front;
				item.rear = this->rear;
				item.count = this->count;
				return item;
				});
		}

	private:
		void ExtendImpl(SizeT additionalSize) {
			const SizeT newSize = this->bufferSize + additionalSize;
			T* newBuffer = new T[newSize];

			for (SizeT i = 0; i < this->count; i++)
			{
				newBuffer[i] = GetByIdx(i);
			}

			this->front = 0; this->rear = this->count;
			delete this->queue;
			this->queue = newBuffer;
			this->bufferSize = newSize;
		}

	protected:
		template <
			template<typename, typename> typename TCollection,
			typename TAnyHeap,
			typename TAny,
			Boolean TIsDynamic,
			Boolean TIsConcurencySuport,
			Boolean TIsReadonly,
			Boolean TIsCollectionCanReturnRef
		>
		friend class LINQ::ICollection;

		template<
			template<typename, SizeT, typename> typename TCollection,
			typename TAnyHeap,
			typename TAny,
			SizeT TBufferSize,
			Boolean TIsConcurencySuport
		>
		friend class LINQ::IFixedCollection;

		template<
			template<typename, SizeT, typename> typename TCollection,
			typename TAnyHeap,
			typename TAny,
			SizeT TBufferSize,
			Boolean TIsConcurencySuport
		>
		friend class LINQ::IFixedStack;

		template<
			template<typename, typename> typename TCollection,
			typename TAnyHeap,
			typename TAny,
			Boolean TIsDynamic,
			Boolean TIsConcurencySuport,
			Boolean TIsReadonly,
			Boolean TIsCollectionCanReturnRef
		>
		friend class LINQ::ICollectionBase;

		template<typename U>
		friend struct Cardinal::Serialization::SerializationHandler;

		template<template<typename UHeap, typename TKey, typename TValue> typename TCollection, typename UHeap, typename TKey, typename TValue, typename TPair, bool IsConcurencySuportT>
		friend class LINQ::IAssociableCollection;

		template<template<typename, SizeT, typename> typename TCollection, typename THeap, typename T, SizeT TSize, Boolean TIsConcurencySuport>
		friend class LINQ::IFixedCollection;

		template<template<typename, SizeT, typename> typename, typename, typename, SizeT, Boolean>
		friend class LINQ::IFixedCollectionBase;

	protected:
		template<typename TLambda>
		BaseConcurencyDynamicQueue& ForEachImpl(TLambda&& lambda) {
			if constexpr (TypeTraits::IsInvockable<TLambda, T&> ||
				TypeTraits::IsInvockable<TLambda, const T&>)
			{
				return this->flowControler.SharedLock([&]()->BaseConcurencyDynamicQueue & InlineLambda{
					for (SizeT i = 0; i < this->count; i++)
					{
						lambda(GetByIdx(i));
					}
					return *this;
					});
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, T&, SizeT> ||
				TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				return this->flowControler.SharedLock([&]()->BaseConcurencyDynamicQueue & InlineLambda{
					SizeT index = 0;

					for (SizeT i = 0; i < this->count; i++, index++)
					{
						lambda(GetByIdx(i), index);
					}

					return *this;
					});
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, T&> ||
					TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, T&, SizeT> ||
					TypeTraits::IsInvockable<TLambda, const T&, SizeT>);
				__assume(0);
			}
		}

		template<typename TLambda>
		const BaseConcurencyDynamicQueue& ForEachImpl(TLambda&& lambda) const {
			if constexpr (TypeTraits::IsInvockable<TLambda, const T&>)
			{
				return this->flowControler.SharedLock([&]() -> const BaseConcurencyDynamicQueue & InlineLambda{
					for (SizeT i = 0; i < this->count; i++)
					{
						lambda(GetByIdx(i));
					}
					return *this;
					});
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				return this->flowControler.SharedLock([&]() -> const BaseConcurencyDynamicQueue & InlineLambda{
					SizeT Index = 0;

					for (SizeT i = 0; i < this->count; i++, Index++)
					{
						lambda(GetByIdx(i), Index);
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

		template<typename TLambda>
		BaseConcurencyDynamicQueue& ForEachImpl(TLambda&& lambda, SizeT requiredCount) {
			if (requiredCount > this->Count)
				requiredCount = this->Count;

			if constexpr (TypeTraits::IsInvockable<TLambda, T&> ||
				TypeTraits::IsInvockable<TLambda, const T&>)
			{
				return this->flowControler.SharedLock([&]()->BaseConcurencyDynamicQueue & InlineLambda{
					for (SizeT i = 0; i < requiredCount; i++)
					{
						lambda(GetByIdx(i));
					}
					return *this;
					});
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, T&, SizeT> ||
				TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				return this->flowControler.SharedLock([&]()->BaseConcurencyDynamicQueue & InlineLambda{
					SizeT index = 0;

					for (SizeT i = 0; i < requiredCount; i++, index++)
					{
						lambda(GetByIdx(i), index);
					}

					return *this;
					});
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, T&> ||
					TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, T&, SizeT> ||
					TypeTraits::IsInvockable<TLambda, const T&, SizeT>);
				__assume(0);
			}
		}

		template<typename TLambda>
		const BaseConcurencyDynamicQueue& ForEachImpl(TLambda&& lambda, SizeT requiredCount) const {
			if (requiredCount > this->Count)
				requiredCount = this->Count;

			if constexpr (TypeTraits::IsInvockable<TLambda, const T&>)
			{
				return this->flowControler.SharedLock([&]() -> const BaseConcurencyDynamicQueue & InlineLambda{
					for (SizeT i = 0; i < requiredCount; i++)
					{
						lambda(GetByIdx(i));
					}
					return *this;
					});
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				return this->flowControler.SharedLock([&]() -> const BaseConcurencyDynamicQueue & InlineLambda{
					SizeT Index = 0;

					for (SizeT i = 0; i < requiredCount; i++, Index++)
					{
						lambda(GetByIdx(i), Index);
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
		data = lambda(data, GetByIdx(i));
	}
	else
	{
		lambda(data, GetByIdx(i));
	}
}

return MoveRValue(result);
				});
		}

		template<typename U, typename TLambda>
		U AggregateImpl(TLambda&& lambda, U&& value) const {
			return this->flowControler.SharedLock([&]() mutable InlineLambda{
				if (this->count == 0)
					return MoveRValue(value);

				for (SizeT i = 0; i < this->count; i++)
				{
					if constexpr (
						Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&> ||
						Concepts::IsInvockableWithResult<TLambda, U, U&, const T&>)
					{
						value = lambda(value, GetByIdx(i));
					}
					else
					{
						lambda(value, GetByIdx(i));
					}
				}

				return MoveRValue(value);
				});
		}

		ForceInline bool AnyImpl() const { return this->count > 0; }

		template<typename Lambda>
		bool AnyImpl(Lambda&& lambda) const {
			return this->flowControler.SharedLock([&]() InlineLambda{
				if (this->count == 0)
					return false;

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(GetByIdx(i)))
						return true;
				}

				return false;
				});
		}

		template<typename Lambda>
		bool AllImpl(Lambda&& lambda) const {
			return this->flowControler.SharedLock([&]() InlineLambda{
				if (this->count == 0)
					return false;

				for (SizeT i = 0; i < this->count; i++)
				{
					if (!lambda(GetByIdx(i)))
						return false;
				}

				return true;
				});
		}

		template<typename Lambda>
		T& FirstImpl(Lambda&& lambda) {
			return this->flowControler.SharedLock([&]()->T & InlineLambda{
				if (this->count == 0)
					Internals::Exceptions::ThrowNotFound(SourcePosition);

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(GetByIdx(i)))
						return GetByIdx(i);
				}

				Internals::Exceptions::ThrowNotFound(SourcePosition);
				});
		}

		template<typename Lambda>
		T& LastImpl(Lambda&& lambda) {
			return this->flowControler.SharedLock([&]()->T & InlineLambda{
				if (this->count == 0)
					Internals::Exceptions::ThrowNotFound(SourcePosition);

				SizeT idx = -1;

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(GetByIdx(i)))
						idx = i;
				}

				if (idx == -1)
				{
					Internals::Exceptions::ThrowNotFound(SourcePosition);
				}

				return GetByIdx(idx);
				});
		}

		template<typename Lambda>
		const T& FirstImpl(Lambda&& lambda) const {
			return this->flowControler.SharedLock([&]() -> const T & InlineLambda{
				if (this->count == 0)
					Internals::Exceptions::ThrowNotFound(SourcePosition);

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(GetByIdx(i)))
						return GetByIdx(i);
				}

				Internals::Exceptions::ThrowNotFound(SourcePosition);
				});
		}

		template<typename Lambda>
		const T& LastImpl(Lambda&& lambda) const {
			return this->flowControler.SharedLock([&]() -> const T & InlineLambda{
				if (this->count == 0)
					Internals::Exceptions::ThrowNotFound(SourcePosition);

				SizeT idx = -1;

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(GetByIdx(i)))
						idx = i;
				}

				if (idx == -1)
				{
					Internals::Exceptions::ThrowNotFound(SourcePosition);
				}

				return GetByIdx(idx);
				});
		}

		template<typename TLambda>
		T FirstOrImpl(TLambda&& lambda, T&& item) const {
			return this->flowControler.SharedLock([&, item = MoveRValue(item)]() InlineLambda{
				if (count == 0)
					return MoveRValue(item);

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(GetByIdx(i)))
						return GetByIdx(i);
				}

				return MoveRValue(item);
				});
		}

		template<typename TLambda>
		T LastOrImpl(TLambda&& lambda, T&& item) const {
			return this->flowControler.SharedLock([&, item = MoveRValue(item)]() InlineLambda{
				if (count == 0)
					return MoveRValue(item);

				SizeT idx = -1;

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(GetByIdx(i)))
						idx = i;
				}

				if (idx == -1)
				{
					return MoveRValue(item);
				}

				return GetByIdx(idx);
				});
		}

		template<typename TLambda>
		T FirstOrImpl(TLambda&& lambda, const T& item) const {
			return this->flowControler.SharedLock([&]() InlineLambda{
				if (count == 0)
					return item;

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(GetByIdx(i)))
						return GetByIdx(i);
				}

				return item;
				});
		}

		template<typename TLambda>
		T LastOrImpl(TLambda&& lambda, const T& item) const {
			return this->flowControler.SharedLock([&]() InlineLambda{
				if (count == 0)
					return item;

				SizeT idx = -1;

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(GetByIdx(i)))
						idx = i;
				}

				if (idx == -1)
				{
					return item;
				}

				return GetByIdx(idx);
				});
		}

		template<typename TLambda, typename TGetterLambda>
		T FirstOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			return this->flowControler.SharedLock([&]() InlineLambda{
				if (count == 0)
					return otherLambda();

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(GetByIdx(i)))
						return GetByIdx(i);
				}

				return otherLambda();
				});
		}

		template<typename TLambda, typename TGetterLambda>
		T LastOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			return this->flowControler.SharedLock([&]() InlineLambda{
				if (count == 0)
					return otherLambda();

				SizeT idx = -1;

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(GetByIdx(i)))
						idx = i;
				}

				if (idx == -1)
				{
					return otherLambda();
				}

				return GetByIdx(idx);
				});
		}

		BaseConcurencyDynamicQueue& ReverseImpl() {
			return this->flowControler.SharedLock([&]()->BaseConcurencyDynamicQueue & InlineLambda{
				for (SizeT i = 0; i < this->count / 2; i++)
				{
					Cardinal::Swap(GetByIdx(i), GetByIdx((this->count - 1) - i));
				}
				return *this;
				});
		}

		ForceInline void UnsafeSet(const T& value, SizeT index) {
			this->queue[index] = value;
			this->rear = index;
			this->count = this->bufferSize;
		}

		ForceInline void UnsafeSet(T&& value, SizeT index) {
			this->queue[index] = MoveRValue(value);
			this->rear = index;
			this->count = this->bufferSize;
		}

		BaseConcurencyDynamicQueue& RemoveAllImpl() {
			return this->flowControler.SharedLock([&]()->BaseConcurencyDynamicQueue & InlineLambda{
				if (this->count == 0)
					return *this;
				delete[] this->queue;
				this->front = this->rear = this->count = this->bufferSize = 0;
				this->queue = nullptr;
				return *this;
				});
		}

		template<typename Lambda>
		BaseConcurencyDynamicQueue& RemoveAllImpl(Lambda&& lambda) {
			return this->flowControler.SharedLock([&, lambda = MoveRValue(lambda)]()->BaseConcurencyDynamicQueue & InlineLambda{
				if (this->count == 0)
					return *this;
				for (SizeT i = 0; i < this->count; i++)
				{
					lambda(MoveRValue(GetByIdx(i)));
				}
				delete[] this->queue;
				this->front = this->rear = this->count = this->bufferSize = 0;
				this->queue = nullptr;
				return *this;
				});
		}

		template<typename Lambda>
		SizeT GetCountIfImpl(Lambda&& lambda) const {
			return this->flowControler.SharedLock([&]() InlineLambda{
				SizeT selectedCount = 0;

				for (SizeT i = 0; i < this->count; i++)
				{
					if (lambda(GetByIdx(i)))
						selectedCount++;
				}

				return selectedCount;
				});
		}
	};

	/// <summary>
	/// A queue with multithreading support (also it implements a thread control)
	/// </summary>
	template<typename T>
	using ConcurencyDynamicQueue = BaseConcurencyDynamicQueue<Cardinal::Memory::Heap, T>;

	/// <summary>
	/// A queue with multithreading support in protected heap (also it implements a thread control)
	/// </summary>
	template<typename T>
	using ConcurencySensitiveDynamicQueue = BaseConcurencyDynamicQueue<Cardinal::Memory::SensitiveDataHeap, T>;
}

export namespace Cardinal::TypeTraits::Collection
{
	template<typename THeap, typename T>
	struct IsDynamicQueueT<Cardinal::Containers::BaseConcurencyDynamicQueue<THeap, T>> : LogicTrue {};
}

export namespace Cardinal
{
	template<typename THeap, typename T>
	ForceInline void Swap(Containers::BaseConcurencyDynamicQueue<THeap, T>& a, Containers::BaseConcurencyDynamicQueue<THeap, T>& b) {
		a.flowControler.ExclusiveLock([&a, &b] () {
			b.flowControler.ExclusiveLock([&a, &b] () {
				Swap(a.front, b.front);
				Swap(a.rear, b.rear);
				Swap(a.count, b.count);
				Swap(a.queue, b.queue);
				Swap(a.bufferSize, b.bufferSize);
				});
			});
	}
}

#pragma warning(pop)