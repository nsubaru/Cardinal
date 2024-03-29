export module Cardinal.Containers.NonConcurency.FixedQueue;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;
export import Cardinal.Containers.InitilizerList;
export import Cardinal.Settings;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
#pragma warning(disable: 4172)
#pragma warning(disable: 4996)

export namespace Cardinal::Containers
{
	/// <summary>
	/// A queue with fixed maximal number of elements
	/// </summary>
	template<typename THeap, SizeT TSize, typename T>
		requires (TSize > 0)
	class BaseFixedQueue :
		public LINQ::IFixedQueue<BaseFixedQueue, THeap, T, TSize, true> {
	private:
		SizeT front;
		SizeT rear;
		SizeT count;
		T queue[TSize] = {};

	public:
		~BaseFixedQueue() {
			if constexpr (TypeTraits::IsSameV<THeap, Memory::SensitiveDataHeap> &&
				TypeTraits::IsPodV<T>)
			{
				Memory::SecureZeroMemory(queue);
			}
		}

	private:
		/// <summary>
		/// A queue element gaining by index
		/// </summary>
		/// <param name="Index">Index of element</param>
		ForceInline T& GetByIdx(SizeT Index) {
			return queue[(front + Index) % TSize];
		}

		/// <summary>
		/// A queue element gaining by index
		/// </summary>
		/// <param name="Index">Index of element</param>
		ForceInline const T& GetByIdx(SizeT Index) const {
			return queue[(front + Index) % TSize];
		}

	public:
		/// <summary>
		/// An empty constructor (initializes the empty queue)
		/// </summary>
		ForceInline BaseFixedQueue() :
			front(0),
			rear(0),
			count(0),
			queue() {
		}

		/// <summary>
		/// Copying constructor
		/// </summary>
		/// <param name="original">Original</param>
		BaseFixedQueue(const BaseFixedQueue& original) :
			front(original.front),
			rear(original.rear),
			count(original.count) {
			if (this->count != 0)
			{
				if constexpr (!TypeTraits::IsPodV<T>)
				{
					for (SizeT i = 0; i < count; i++)
					{
						this->queue[i] = original.GetByIdx(i);
					}
				}
				else
				{
					Memory::Copy(original.queue, this->queue);
				}
			}
		}

		/// <summary>
		/// Moving constructor
		/// </summary>
		/// <param name="original">Original</param>
		BaseFixedQueue(BaseFixedQueue&& original) :
			front(original.front),
			rear(original.rear),
			count(original.count) {
			if (count != 0)
			{
				if constexpr (!TypeTraits::IsPodV<T>)
				{
					for (SizeT i = 0; i < count; i++)
					{
						this->GetByIdx(i) = MoveRValue(original.GetByIdx(i));
					}
				}
				else
				{
					Memory::Copy(original.queue, this->queue);
				}
			}

			original.front = 0;
			original.rear = 0;
			original.count = 0;
		}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"LValue" reference to array</returns>
		BaseFixedQueue& operator=(const BaseFixedQueue& original) {
			this->~BaseFixedQueue();
			this->BaseFixedQueue::BaseFixedQueue(original);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"RValue" reference to array</returns>
		BaseFixedQueue& operator=(BaseFixedQueue&& original) {
			this->~BaseFixedQueue();
			this->BaseFixedQueue::BaseFixedQueue(MoveRValue(original));
			return *this;
		}

		/// <summary>
		/// List of elements constructor (if number of elements are more than maximal, the rest will be ignored)
		/// </summary>
		/// <param name="list">List of elements</param>
		BaseFixedQueue(InitilizerList<T>&& list) : BaseFixedQueue() {
			if (list.size() != 0)
			{
				SizeT ActuallyCopyied = (TSize < list.size()) ? TSize : list.size();
				auto it = list.begin();
				SizeT i = ActuallyCopyied;

				for (SizeT i = 0; i < ActuallyCopyied; i++)
				{
					queue[i] = MoveRValue(*it); it++;
				}

				rear = count = ActuallyCopyied;
				front = 0;
			}
		}

		/// <summary>
		/// An element addition to the queue 
		/// (if queue is full then generates an exception)
		/// </summary>
		/// <param name="Value">A value of element</param>
		ForceInline void Enqueue(const T& Value) {
			if (++count > TSize)
				Internals::Exceptions::ThrowFullQueueException(SourcePosition, TSize);

			SizeT newRear = (rear + 1) % TSize;
			queue[rear] = Value;
			rear = newRear;
		}

		/// <summary>
		/// An element addition to the queue 
		/// (if queue is full then generates an exception)
		/// </summary>
		/// <param name="Value">A value of element</param>
		ForceInline void Enqueue(T&& Value) {
			if (++count > TSize)
				Internals::Exceptions::ThrowFullQueueException(SourcePosition, TSize);

			SizeT newRear = (rear + 1) % TSize;
			queue[rear] = MoveRValue(Value);
			rear = newRear;
		}

		/// <summary>
		/// Popping elements from queue (if queue is empty then generates an exception)
		/// </summary>
		/// <returns>Popped element</returns>
		ForceInline T Dequeue() {
			if (count == 0)
				Internals::Exceptions::ThrowEmptyQueueException(SourcePosition);

			T res = MoveRValue(queue[front]);
			front = (front + 1) % TSize;
			--count;
			return res;
		}

		/// <summary>
		/// Returns the first element from queue without popping (is queue is empty, generates an exception)
		/// </summary>
		/// <returns>A value of element</returns>
		ForceInline const T& Top() {
			if (front == rear)
				Internals::Exceptions::ThrowEmptyQueueException(SourcePosition);

			return queue[front];
		}

		/// <summary>
		/// Returns the first element from queue without popping (is queue is empty, generates an exception)
		/// </summary>
		/// <returns>A value of element</returns>
		ForceInline const T& Top() const {
			if (front == rear)
				Internals::Exceptions::ThrowEmptyQueueException(SourcePosition);

			return queue[front];
		}

		template<typename U>
		friend void Cardinal::Swap(U& A, U& B);

		/// <summary>
		/// Returns the amount of elements in queue
		/// </summary>
		/// <returns>A current number of elements in queue</returns>
		ForceInline SizeT GetCount() const { return count; }

		/// <summary>
		/// A current number of elements in queue
		/// </summary>
		SizeT Property(GetCount) Count;

		/// <summary>
		/// Queue object cloning
		/// </summary>
		/// <returns>Returns new queue object with copy of all elements</returns>
		template<typename = void> requires (Concepts::IsClonable<T>)
			BaseFixedQueue Clone() const {
			BaseFixedQueue result;
			result.front = front;
			result.rear = rear;
			result.count = count;

			if constexpr (Concepts::IsPod<T>)
			{
				Memory::Copy(this->queue, result.queue);
			}
			else
			{
				for (SizeT i = 0; i < count; i++)
				{
					result.GetByIdx(i) = Clone(GetByIdx(i));
				}
			}
			return result;
		}

		/// <summary>
		/// Queue object cloning using custom lambda or functor
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">A function with which occurs cloning</param>
		/// <returns>Returns new queue object with copy of all elements</returns>
		template<typename TLambda> requires (Concepts::IsInvockableWithResult<TLambda, T, const T&>)
			BaseFixedQueue Clone(TLambda&& lambda) const {
			BaseFixedQueue result;
			result.front = front;
			result.rear = rear;
			result.count = count;

			for (SizeT i = 0; i < count; i++)
			{
				result.GetByIdx(i) = lambda(GetByIdx(i));
			}
			return result;
		}

	protected:
		template<template<typename, typename> typename TCollection, typename THeap, typename T, Boolean TIsDynamic, Boolean TIsConcurencySuport, Boolean TIsReadonly, Boolean TIsCollectionCanReturnRef>
		friend class LINQ::ICollection;

		template<template<typename, SizeT, typename> typename TCollection, typename THeap, typename T, SizeT TSize, Boolean TIsConcurencySuport>
		friend class LINQ::IFixedCollection;

		template<template<typename, SizeT, typename> typename TCollection, typename THeap, typename T, SizeT TSize, Boolean TIsConcurencySuport>
		friend class LINQ::IFixedStack;

		template<template<typename THeap, typename T> typename TCollection, typename THeap, typename T, Boolean TIsDynamic, Boolean TIsConcurencySuport, Boolean TIsReadonly, Boolean TIsCollectionCanReturnRef>
		friend class LINQ::ICollectionBase;

		template<typename U>
		friend struct Cardinal::Serialization::SerializationHandler;

	protected:
		template<typename TLambda>
		BaseFixedQueue& ForEachImpl(TLambda&& lambda) {
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
				TypeTraits::IsInvockable<TLambda, const T&>)
			{
				for (SizeT i = 0; i < count; i++)
				{
					lambda(GetByIdx(i), i);
				}

				return *this;
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, T&> ||
					TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, T&, SizeT> ||
					TypeTraits::IsInvockable<TLambda, const T&>);
			}
		}

		template<typename TLambda>
		const BaseFixedQueue& ForEachImpl(TLambda&& lambda) const {
			if constexpr (TypeTraits::IsInvockable<TLambda, const T&>)
			{
				for (SizeT i = 0; i < count; i++)
				{
					lambda(GetByIdx(i));
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				for (SizeT i = 0; i < count; i++)
				{
					lambda(GetByIdx(i), i);
				}

				return *this;
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, const T&, SizeT>);
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
			U result = MoveRValue(value);

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
				Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
				{
					return GetByIdx(i);
				}
			}

			Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
		}

		template<typename TLambda>
		T& LastImpl(TLambda&& lambda) {
			if (count == 0)
				Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);

			T* item = nullptr;

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
				{
					item = &GetByIdx(i);
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
		}

		template<typename TLambda>
		const T& FirstImpl(TLambda&& lambda) const {
			if (count == 0)
				Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
				{
					return GetByIdx(i);
				}
			}

			Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
		}

		template<typename TLambda>
		const T& LastImpl(TLambda&& lambda) const {
			if (count == 0)
				Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);

			T* item = nullptr;

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
				{
					item = &GetByIdx(i);
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
		}

		template<typename TLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, T&& element) const {
			if (count == 0)
				return MoveRValue(element);

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
				{
					return GetByIdx(i);
				}
			}

			return MoveRValue(element);
		}

		template<typename TLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, T&& element) const {
			if (count == 0)
				return MoveRValue(element);

			const T* item = nullptr;

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
				{
					item = &GetByIdx(i);
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
		}

		template<typename TLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, const T& element) const {
			if (count == 0)
				return element;

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
				{
					return GetByIdx(i);
				}
			}

			return element;
		}

		template<typename TLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, const T& element) const {
			if (count == 0)
				return element;

			const T* item = nullptr;

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
				{
					item = &GetByIdx(i);
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
		}

		template<typename TLambda, typename TGetterLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			if (count == 0)
				return otherLambda();

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
				{
					return GetByIdx(i);
				}
			}

			return otherLambda();
		}

		template<typename TLambda, typename TGetterLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			if (count == 0)
				return otherLambda();

			const T* item = nullptr;

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(GetByIdx(i)))
				{
					item = &GetByIdx(i);
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
		}

		BaseFixedQueue& ReverseImpl() {
			for (SizeT i = 0; i < count / 2; i++)
			{
				Cardinal::Swap(GetByIdx(i), GetByIdx((count - 1) - i));
			}
			return *this;
		}

		ForceInline void UnsafeSet(T&& Value, SizeT Index) {
			queue[Index] = MoveRValue(Value);
			count = Index + 1;
			rear = Index;
		}

		ForceInline void UnsafeSet(const T& Value, SizeT Index) {
			queue[Index] = Value;
			count = Index + 1;
			rear = Index;
		}

		BaseFixedQueue& RemoveAllImpl() {
			if (count == 0)
				return *this;

			for (SizeT i = 0; i < count; i++)
			{
				GetByIdx(i).~T();
			}
			this->count = 0;
			return *this;
		}

		template<typename TLambda>
		BaseFixedQueue& RemoveAllImpl(TLambda&& lambda) {
			if (count == 0)
				return *this;

			for (SizeT i = 0; i < count; i++)
			{
				lambda(MoveRValue(GetByIdx(i)));
			}
			this->count = 0;
			return *this;
		}
	};

	/// <summary>
	/// A queue with fixed maximal number of elements in standard heap
	/// </summary>
	template<SizeT TSize, typename T>
	using FixedQueue = BaseFixedQueue<Cardinal::Memory::Heap, TSize, T>;

	/// <summary>
	/// A queue with fixed maximal number of elements in protected heap
	/// </summary>
	template<SizeT TSize, typename T>
	using SensitiveFixedQueue = BaseFixedQueue<Cardinal::Memory::SensitiveDataHeap, TSize, T>;
}

export namespace Cardinal::TypeTraits::Collection
{
	template<typename THeap, SizeT TSize, typename T>
	struct IsFixedQueueT<Cardinal::Containers::BaseFixedQueue<THeap, TSize, T>> : LogicTrue {};
}

export namespace Cardinal
{
	template<typename THeap, SizeT TSize, typename T>
	ForceInline void Swap(Containers::BaseFixedQueue<THeap, TSize, T>& A, Containers::BaseFixedQueue<THeap, TSize, T>& B) {
		Swap(A.queue, B.queue);
		Swap(A.front, B.front);
		Swap(A.rear, B.rear);
		Swap(A.count, B.count);
	}
}

#pragma warning(pop)