export module Cardinal.Containers.NonConcurency.DynamicQueue;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;
export import Cardinal.Containers.InitilizerList;
export import Cardinal.Settings;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

#pragma warning(disable: 4172)

export namespace Cardinal::Containers
{
	/// <summary>
	/// A queue of elements
	/// </summary>
	template<typename THeap, typename T>
	class BaseDynamicQueue final
		:
		public LINQ::IDynamicStack<BaseDynamicQueue, THeap, T, true> {
	private:
		SizeT front;
		SizeT rear;
		SizeT count;
		T* Restrict queue;
		SizeT bufferSize;

	public:
		~BaseDynamicQueue() {
			delete[] queue;
			front = rear = count = bufferSize = 0;
			queue = nullptr;
		}

	private:
		/// <summary>
		/// A queue element gaining by index
		/// </summary>
		/// <param name="Index">Index of element</param>
		ForceInline T& GetByIdx(SizeT Index) {
			return queue[(front + Index) % bufferSize];
		}

		/// <summary>
		/// A queue element gaining by index
		/// </summary>
		/// <param name="Index">Index of element</param>
		ForceInline const T& GetByIdx(SizeT Index) const {
			return queue[(front + Index) % bufferSize];
		}

	public:
		/// <summary>
		/// A dynamic queue empty constructor
		/// </summary>
		ForceInline BaseDynamicQueue() :
			BaseDynamicQueue(CurrentSettings().MemoryManager.QueueReserveSize) {}

		/// <summary>
		/// A queue constructor with the certain maximal number of elements
		/// </summary>
		/// <param name="startCount">A maximal number of elements in queue</param>
		ForceInline BaseDynamicQueue(SizeT startCount) :
			front(0),
			rear(0),
			count(0),
			queue(new T[startCount]),
			bufferSize(startCount) {
		}

		/// <summary>
		/// Copying constructor
		/// </summary>
		/// <param name="original">Original</param>
		BaseDynamicQueue(const BaseDynamicQueue& original) :
			front(original.front),
			rear(original.rear),
			count(original.count),
			queue(nullptr),
			bufferSize(original.bufferSize) {
			if (original.count != 0)
			{
				this->queue = new T[original.bufferSize];

				if constexpr (!TypeTraits::IsPodV<T>)
				{
					for (SizeT i = 0; i < count; i++)
					{
						this->GetByIdx(i) = original.GetByIdx(i);
					}
				}
				else
				{
					Memory::Copy(original.queue, this->queue, sizeof(T) * bufferSize);
				}
			}
		}

		/// <summary>
		/// Moving constructor
		/// </summary>
		/// <param name="original">Original</param>
		BaseDynamicQueue(BaseDynamicQueue&& original) :
			front(MoveRValue(original.front)),
			rear(MoveRValue(original.rear)),
			count(MoveRValue(original.count)),
			queue(MoveRValue(original.queue)),
			bufferSize(MoveRValue(original.bufferSize)) {
			original.queue = nullptr;
			original.front = 0;
			original.rear = 0;
			original.count = 0;
			original.bufferSize = 0;
		}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"LValue" reference to array</returns>
		BaseDynamicQueue& operator=(const BaseDynamicQueue& original) {
			this->~BaseDynamicQueue();
			this->BaseDynamicQueue::BaseDynamicQueue(original);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"RValue" reference to array</returns>
		BaseDynamicQueue& operator=(BaseDynamicQueue&& original) {
			this->~BaseDynamicQueue();
			this->BaseDynamicQueue::BaseDynamicQueue(MoveRValue(original));
			return *this;
		}

		/// <summary>
		/// A queue constructor with elements from initialization list
		/// </summary>
		/// <param name="list">Initialization list</param>
		BaseDynamicQueue(InitilizerList<T>&& list) :
			front(0),
			rear(0),
			count(0),
			queue(nullptr),
			bufferSize(0) {
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
		/// Adding the elements to the queue
		/// </summary>
		/// <param name="value">A value of element</param>
		/// <param name="isResizeIfFull">If "true" then queue will be extended automatically if needed, 
		/// if "false" and queue is full, then it generates an exception
		/// </param>
		void Enqueue(const T& value, bool isResizeIfFull = false) {
			if (count >= bufferSize)
			{
				if (isResizeIfFull)
					this->ExtendImpl(CurrentSettings().MemoryManager.QueueReserveSize);
				else
					Internals::Exceptions::ThrowFullQueueException(SourcePosition, bufferSize);
			}

			SizeT newRear = (rear + 1) % bufferSize;
			queue[rear] = value;
			rear = newRear;
			++count;
		}

		/// <summary>
		/// Optimized elements addition to the queue
		/// </summary>
		/// <param name="value">"RValue" reference to element</param>
		/// <param name="isResizeIfFull">If "true" then queue will be extended automatically if needed, 
		/// if "false" and queue is full, then it generates an exception
		/// </param>
		void Enqueue(T&& value, bool isResizeIfFull = false) {
			if (count >= bufferSize)
			{
				if (isResizeIfFull)
					this->ExtendImpl(CurrentSettings().MemoryManager.QueueReserveSize);
				else
					Internals::Exceptions::ThrowFullQueueException(SourcePosition, bufferSize);
			}

			SizeT newRear = (rear + 1) % bufferSize;
			queue[rear] = MoveRValue(value);
			rear = newRear;
			++count;
		}

		/// <summary>
		/// Element extraction from queue 
		/// (if queue is empty then generates an exception)
		/// </summary>
		/// <returns>A value of element</returns>
		T Dequeue() {
			if (count == 0)
				Internals::Exceptions::ThrowEmptyQueueException(SourcePosition);

			T res = MoveRValue(queue[front]);
			front = (front + 1) % bufferSize;
			--count;

			return MoveRValue(res);
		}

		/// <summary>
		/// Returns element value without extraction from queue
		/// (if queue is empty then generates an exception)
		/// </summary>
		ForceInline const T& Top() {
			if (front == rear)
				Internals::Exceptions::ThrowEmptyQueueException(SourcePosition);

			return queue[front];
		}

		/// <summary>
		/// Returns element value without extraction from queue
		/// (if queue is empty then generates an exception)
		/// </summary>
		ForceInline const T& Top() const {
			if (front == rear)
				Internals::Exceptions::ThrowEmptyQueueException(SourcePosition);

			return queue[front];
		}

		/// <summary>
		/// Queue extension on given number of elements
		/// </summary>
		/// <param name="additionalSize">A number of elements, to which the queue is need to extend</param>
		ForceInline void Extend(SizeT additionalSize) {
			ExtendImpl(additionalSize);
		}

		/// <summary>
		/// Queue shrinking on given number of elements
		/// </summary>
		void Shrink() {
			const SizeT newSize = count;
			T* newBuffer = new T[newSize];
			for (SizeT i = 0; i < count; i++)
			{
				newBuffer[i] = GetByIdx(i);
			}
			front = 0; rear = count;
			delete queue;
			queue = newBuffer;
		}

		template<typename TU>
		friend void Cardinal::Swap(TU& a, TU& b);

		/// <summary>
		/// A number of elements in queue
		/// </summary>
		/// <returns>A current number of elements</returns>
		ForceInline SizeT GetCount() const { return count; }

		/// <summary>
		/// A current number of elements
		/// </summary>
		SizeT Property(GetCount) Count;

		/// <summary>
		/// A maximal number of elements in queue
		/// </summary>
		/// <returns>A maximal number of elements</returns>
		ForceInline SizeT GetMaxCount() const { return bufferSize; }

		/// <summary>
		/// A maximal number of elements
		/// </summary>
		SizeT Property(GetMaxCount) MaxCount;

		/// <summary>
		/// Indicates whether queue is full
		/// </summary>
		/// <returns>Indicates whether queue is full</returns>
		Boolean GetIsFull() const {
			return count == bufferSize;
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
			BaseDynamicQueue Clone() const {
			auto item = BaseDynamicQueue(this->bufferSize);
			item.front = this->front;
			item.rear = this->rear;
			item.count = this->count;

			if constexpr (TypeTraits::IsPodV<T>)
			{
				Memory::Copy(this->queue, item.queue, sizeof(T) * this->bufferSize);
			}
			else
			{
				for (SizeT i = 0; i < count; i++)
				{
					item.GetByIdx(i) = Clone(GetByIdx(i));
				}
			}

			return item;
		}

		/// <summary>
		/// Queue cloning using custom lambda or functor
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">A function with which occurs cloning</param>
		/// <returns>Returnt a new queue with copies of all elements</returns>
		template<typename TLambda> requires (Concepts::IsInvockableWithResult<TLambda, T, const T&>)
			BaseDynamicQueue Clone(TLambda&& lambda) const {
			auto item = BaseDynamicQueue(this->count);
			item.front = this->front;
			item.rear = this->rear;
			item.count = this->count;

			for (SizeT i = 0; i < count; i++)
			{
				item.GetByIdx(i) = lambda(GetByIdx(i));
			}

			return item;
		}

	private:
		void ExtendImpl(SizeT additionalSize) {
			const SizeT newSize = bufferSize + additionalSize;
			T* newBuffer = nullptr;
			try
			{
				newBuffer = new T[newSize];
				for (SizeT i = 0; i < count; i++)
				{
					newBuffer[i] = GetByIdx(i);
				}
				front = 0; rear = count;
				Swap(queue, newBuffer);
				delete[] newBuffer; newBuffer = nullptr;
				bufferSize = newSize;
			}
			catch (...)
			{
				delete newBuffer; newBuffer = nullptr;
				Internals::Exceptions::__ReThrow();
			}

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

		template<template<typename THeap, typename TKey, typename TValue> typename TCollection, typename THeap, typename TKey, typename TValue, typename TPair, bool IsConcurencySuportT>
		friend class LINQ::IAssociableCollection;

		template<template<typename, SizeT, typename> typename TCollection, typename THeap, typename T, SizeT TSize, Boolean TIsConcurencySuport>
		friend class LINQ::IFixedCollection;

		template<template<typename, SizeT, typename> typename, typename, typename, SizeT, Boolean>
		friend class LINQ::IFixedCollectionBase;

	protected:
		template<typename TLambda>
		BaseDynamicQueue& ForEachImpl(TLambda&& lambda) {
			if constexpr (TypeTraits::IsInvockable<TLambda, T&> ||
				TypeTraits::IsInvockable<TLambda, const T&>)
			{

				for (SizeT i = 0; i < count; i++)
				{
					lambda(GetByIdx(i));
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, T&, SizeT> ||
				TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{

				SizeT index = 0;

				for (SizeT i = 0; i < count; i++, index++)
				{
					lambda(GetByIdx(i), index);
				}

				return *this;
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, T&> ||
					TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, T&, SizeT> ||
					TypeTraits::IsInvockable<TLambda, const T&, SizeT>);
				__assume(false);
				return *this;
			}
		}

		template<typename TLambda>
		const BaseDynamicQueue& ForEachImpl(TLambda&& lambda) const {
			if constexpr (TypeTraits::IsInvockable<TLambda, const T&>)
			{
				for (SizeT i = 0; i < this->count; i++)
				{
					lambda(GetByIdx(i));
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				SizeT index = 0;

				for (SizeT i = 0; i < this->count; i++, index++)
				{
					lambda(GetByIdx(i), index);
				}

				return *this;
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, const T&, SizeT>);
				__assume(false);
				return *this;
			}
		}

		template<typename TLambda>
		BaseDynamicQueue& ForEachImpl(TLambda&& lambda, SizeT requiredCount) {
			if (requiredCount > this->count)
				requiredCount = this->count;

			if constexpr (TypeTraits::IsInvockable<TLambda, T&> ||
				TypeTraits::IsInvockable<TLambda, const T&>)
			{

				for (SizeT i = 0; i < requiredCount; i++)
				{
					lambda(GetByIdx(i));
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, T&, SizeT> ||
				TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{

				SizeT index = 0;

				for (SizeT i = 0; i < requiredCount; i++, index++)
				{
					lambda(GetByIdx(i), index);
				}

				return *this;
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, T&> ||
					TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, T&, SizeT> ||
					TypeTraits::IsInvockable<TLambda, const T&, SizeT>);
				__assume(false);
				return *this;
			}
		}

		template<typename TLambda>
		const BaseDynamicQueue& ForEachImpl(TLambda&& lambda, SizeT requiredCount) const {
			if (requiredCount > this->count)
				requiredCount = this->count;

			if constexpr (TypeTraits::IsInvockable<TLambda, const T&>)
			{
				for (SizeT i = 0; i < requiredCount; i++)
				{
					lambda(GetByIdx(i));
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				SizeT index = 0;

				for (SizeT i = 0; i < requiredCount; i++, index++)
				{
					lambda(GetByIdx(i), index);
				}

				return *this;
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, const T&, SizeT>);
				__assume(false);
				return *this;
			}
		}

		template<typename U, typename TLambda>
		U AggregateImpl(TLambda&& lambda, const U& value) const {
			U result = value;
			U& data = result;

			if (count == 0)
			{
				return MoveRValue(result);
			}

			for (SizeT i = 0; i < count; i++)
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
		}

		template<typename U, typename TLambda>
		U AggregateImpl(TLambda&& lambda, U&& value) const {
			if (count == 0)
				return MoveRValue(value);

			for (SizeT i = 0; i < count; i++)
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
		}

		ForceInline bool AnyImpl() const { return count > 0; }

		template<typename TLambda>
		bool AnyImpl(TLambda&& lambda) const {
			if (count == 0)
				return false;

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
					return true;
			}

			return false;
		}

		template<typename TLambda>
		bool AllImpl(TLambda&& lambda) const {
			if (count == 0)
				return false;

			for (SizeT i = 0; i < count; i++)
			{
				if (!lambda(GetByIdx(i)))
					return false;
			}

			return true;
		}

		template<typename TLambda>
		T& FirstImpl(TLambda&& lambda) {
			if (count == 0)
				Internals::Exceptions::ThrowNotFound(SourcePosition);

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
					return GetByIdx(i);
			}

			Internals::Exceptions::ThrowNotFound(SourcePosition);
		}

		template<typename TLambda>
		T& LastImpl(TLambda&& lambda) {
			if (count == 0)
				Internals::Exceptions::ThrowNotFound(SourcePosition);

			SizeT idx = Math::Max<SizeT>();

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
					idx = i;
			}

			if (idx == Math::Max<SizeT>())
			{
				Internals::Exceptions::ThrowNotFound(SourcePosition);
			}

			return GetByIdx(idx);
		}

		template<typename TLambda>
		const T& FirstImpl(TLambda&& lambda) const {
			if (count == 0)
				Internals::Exceptions::ThrowNotFound(SourcePosition);

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
					return GetByIdx(i);
			}

			Internals::Exceptions::ThrowNotFound(SourcePosition);
		}

		template<typename TLambda>
		const T& LastImpl(TLambda&& lambda) const {
			if (count == 0)
				Internals::Exceptions::ThrowNotFound(SourcePosition);

			SizeT idx = Math::Max<SizeT>();

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
					idx = i;
			}

			if (idx == Math::Max<SizeT>())
			{
				Internals::Exceptions::ThrowNotFound(SourcePosition);
			}

			return GetByIdx(idx);
		}

		template<typename TLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, T&& item) const {
			if (count == 0)
				return MoveRValue(item);

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
					return GetByIdx(i);
			}

			return MoveRValue(item);
		}

		template<typename TLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, T&& item) const {
			if (count == 0)
				return MoveRValue(item);

			SizeT idx = Math::Max<SizeT>();

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
					idx = i;
			}

			if (idx == Math::Max<SizeT>())
			{
				return MoveRValue(item);
			}

			return GetByIdx(idx);
		}

		template<typename TLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, const T& item) const {
			if (count == 0)
				return item;

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
					return GetByIdx(i);
			}

			return item;
		}

		template<typename TLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, const T& item) const {
			if (count == 0)
				return item;

			SizeT idx = Math::Max<SizeT>();

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
					idx = i;
			}

			if (idx == Math::Max<SizeT>())
			{
				return item;
			}

			return GetByIdx(idx);
		}

		template<typename TLambda, typename TGetterLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			if (count == 0)
				return otherLambda();

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
					return GetByIdx(i);
			}

			return otherLambda();
		}

		template<typename TLambda, typename TGetterLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			if (count == 0)
				return otherLambda();

			SizeT idx = Math::Max<SizeT>();

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
					idx = i;
			}

			if (idx == Math::Max<SizeT>())
			{
				return otherLambda();
			}

			return GetByIdx(idx);
		}

		BaseDynamicQueue& ReverseImpl() {
			for (SizeT i = 0; i < count / 2; i++)
			{
				Swap(GetByIdx(i), GetByIdx((count - 1) - i));
			}
			return *this;
		}

		ForceInline void UnsafeSet(const T& value, SizeT index) {
			queue[index] = value;
			rear = index;
			count = bufferSize;
		}

		ForceInline void UnsafeSet(T&& value, SizeT index) {
			queue[index] = MoveRValue(value);
			rear = index;
			count = bufferSize;
		}

		ForceInline BaseDynamicQueue& RemoveAllImpl() {
			if (count == 0)
				return *this;

			delete[] queue;
			front = rear = count = bufferSize = 0;
			queue = nullptr;
			this->count = 0;
			return *this;
		}

		template<typename TLambda>
		ForceInline BaseDynamicQueue& RemoveAllImpl(TLambda&& lambda) {
			if (count == 0)
				return *this;

			for (SizeT i = 0; i < count; i++)
			{
				lambda(MoveRValue(GetByIdx(i)));
			}

			delete[] queue;
			front = rear = count = bufferSize = 0;
			queue = nullptr;
			this->count = 0;
			return *this;
		}

		template<typename TLambda>
		SizeT GetCountIfImpl(TLambda&& lambda) const {
			SizeT selectedCount = 0;

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
					selectedCount++;
			}

			return selectedCount;
		}
	};

	/// <summary>
	/// A queue of elements
	/// </summary>
	template<typename T>
	using DynamicQueue = BaseDynamicQueue<Cardinal::Memory::Heap, T>;

	/// <summary>
	/// A queue of elements in protected heap
	/// </summary>
	template<typename T>
	using SensitiveDynamicQueue = BaseDynamicQueue<Cardinal::Memory::SensitiveDataHeap, T>;
}

export namespace Cardinal::TypeTraits::Collection
{
	template<typename THeap, typename T>
	struct IsDynamicQueueT<Cardinal::Containers::BaseDynamicQueue<THeap, T>> : LogicTrue {};
}

export namespace Cardinal
{
	template<typename THeap, typename T>
	ForceInline void Swap(Containers::BaseDynamicQueue<THeap, T>& A, Containers::BaseDynamicQueue<THeap, T>& B) {
		Swap(A.front, B.front);
		Swap(A.rear, B.rear);
		Swap(A.count, B.count);
		Swap(A.bufferSize, B.bufferSize);
		Swap(A.queue, B.queue);
	}
}

#pragma warning(pop)