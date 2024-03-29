export module Cardinal.Containers.Concurency.DynamicStack;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;
export import Cardinal.Threading.SRWLock;
export import Cardinal.Settings;
export import Cardinal.Containers.InitilizerList;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
#pragma warning(disable: 4172)

export namespace Cardinal::Containers
{
	/// <summary>
	/// A stack with multithreading support
	/// </summary>
	template<typename THeap, typename T>
	class BaseConcurencyDynamicStack :
		public LINQ::IDynamicStack<BaseConcurencyDynamicStack, THeap, T, true> {
	private:
		RefCountT count;
		SizeT bufferSize;
		Threading::SRWLock mutable lock;
		T* Restrict stack;

	public:
		/// <summary>
		/// An empty constructor (initializes a stack with maximal number of elements)
		/// </summary>
		ForceInline BaseConcurencyDynamicStack() :
			count(0),
			lock(),
			bufferSize(CurrentSettings().MemoryManager.StackReserveSize),
			stack(new T[CurrentSettings().MemoryManager.StackReserveSize]) {
		}

		/// <summary>
		/// A constructor with buffer size
		/// </summary>
		/// <param name="bufferSize">Buffer size</param>
		ForceInline BaseConcurencyDynamicStack(SizeT bufferSize) :
			count(0),
			lock(),
			stack(new T[bufferSize]),
			bufferSize(bufferSize) {
		}

		/// <summary>
		/// Copying constructor from existing stack
		/// </summary>
		/// <param name="stack">Existing stack</param>
		BaseConcurencyDynamicStack(const BaseConcurencyDynamicStack& stack) : lock() {
			stack.lock.SharedLock([&] () {
				this->bufferSize = stack.bufferSize;
				this->stack = new T[this->bufferSize];

				if constexpr (TypeTraits::IsPodV<T>)
				{
					Memory::Copy(stack.stack, this->stack, sizeof(T) * this->bufferSize);
				}
				else
				{
					for (SizeT i = 0; i < stack.count; i++)
					{
						this->stack[i] = stack.stack[i];
					}
				}

				this->count = stack.count;
				});
		}

		/// <summary>
		/// Moving constructor from existing stack
		/// </summary>
		/// <param name="stack">Existing stack</param>
		BaseConcurencyDynamicStack(BaseConcurencyDynamicStack&& stack) : lock() {
			stack.lock.SharedLock([&] () {
				this->count = stack.count;
				stack.count = 0;
				this->bufferSize = stack.bufferSize;
				stack.bufferSize = 0;
				this->stack = stack.stack;
				stack.stack = nullptr;
				});
		}

		/// <summary>
		/// A constructor with initialization list
		/// (moves elements from the list, sets size as a 
		/// sum of number of elements plus standard number of elements)
		/// </summary>
		/// <param name="list">List of elements</param>
		BaseConcurencyDynamicStack(InitilizerList<T>&& list) : lock() {
			this->bufferSize = CurrentSettings().MemoryManager.StackReserveSize + list.size();
			this->stack = new T[this->bufferSize];

			if (list.size() != 0)
			{
				auto it = list.begin();
				SizeT i = list.size();

				this->count = list.size();

				for (SizeT i = 0; i < count; i++)
				{
					this->stack[i] = MoveRValue(*it); ++it;
				}
			}
			else
			{
				this->count = 0;
			}
		}

		~BaseConcurencyDynamicStack() {
			delete[] this->stack;
			this->stack = nullptr;
			this->count = this->bufferSize = 0;
		}

		/// <summary>
		/// Adds elements in stack (is stack is full
		/// then in extends on amount that is set in settings)
		/// </summary>
		/// <param name="item">A reference to element</param>
		void Push(const T& item) {
			return this->lock.ExclusiveLock([&]() mutable InlineLambda{
				if (this->count == this->bufferSize)
 {
ExtendBufferImpl(CurrentSettings().MemoryManager.StackReserveSize);
}
this->stack[this->count] = item;
++this->count;
				});
		}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"LValue" reference to queue</returns>
		BaseConcurencyDynamicStack& operator=(const BaseConcurencyDynamicStack& original) {
			this->~BaseConcurencyDynamicStack();
			this->BaseConcurencyDynamicStack::BaseConcurencyDynamicStack(original);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"RValue" reference to queue</returns>
		BaseConcurencyDynamicStack& operator=(BaseConcurencyDynamicStack&& original) {
			this->~BaseConcurencyDynamicStack();
			this->BaseConcurencyDynamicStack::BaseConcurencyDynamicStack(MoveRValue(original));
			return *this;
		}

		/// <summary>
		/// Adds elements in stack (is stack is full
		/// then in extends on amount that is set in settings)
		/// </summary>
		/// <param name="item">"RValue" reference to an element</param>
		void Push(T&& item) {
			return this->lock.ExclusiveLock([&, item = MoveRValue(item)]() mutable InlineLambda{
				if (this->count == this->bufferSize)
 {
ExtendBufferImpl(CurrentSettings().MemoryManager.StackReserveSize);
}
this->stack[this->count] = MoveRValue(item);
++this->count;
				});
		}

		/// <summary>
		/// Pops an elements from stack (if stack is empty, generates an exception)
		/// </summary>
		/// <returns>A popped elements from stack</returns>
		T Pop() {
			return this->lock.ExclusiveLock([&]() mutable InlineLambda{
				if (this->count == 0)
				Internals::Exceptions::ThrowEmptyStackException(SourcePosition);

				--this->count;
				return MoveRValue(this->stack[this->count]);
				});
		}

		/// <summary>
		/// Pops an element from stack
		/// </summary>
		/// <param name="item">A returned object if stack contains an element</param>
		/// <returns>A popped element from stack</returns>
		T Pop(T&& item) {
			return this->lock.ExclusiveLock([&]() mutable InlineLambda{
				if (this->count == 0)
					return MoveRValue(item);

				--this->count;
				return MoveRValue(this->stack[this->count]);
				});
		}

		/// <summary>
		/// Pops an element from stack
		/// </summary>
		/// <param name="item">A returned object if stack contains an element</param>
		/// <returns>A popped element from stack</returns>
		T Pop(const T& item) {
			return this->lock.ExclusiveLock([&]() mutable InlineLambda{
				if (this->count == 0)
					return MoveRValue(item);

				--this->count;
				return MoveRValue(this->stack[this->count]);
				});
		}

		/// <summary>
		/// Pops an element from stack
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">A lamda which returns an element if stack is not empty</param>
		/// <returns>A popped element from stack</returns>
		template<typename TLambda> requires Concepts::IsInvockableWithResult<TLambda, T>
		T Pop(TLambda&& lambda) {
			return this->lock.ExclusiveLock([&]() mutable InlineLambda{
				if (this->count == 0)
					return lambda();

				--this->count;
				return MoveRValue(this->stack[this->count]);
				});
		}

		/// <summary>
		/// Returns an upper element without popping from stack
		/// (if stack is empty, generates an exception)
		/// </summary>
		/// <returns>A constant reference to element</returns>
		const T& Top() const {
			return this->lock.SharedLock([&]() mutable -> const T & InlineLambda{
				if (this->count == 0)
				Internals::Exceptions::ThrowEmptyStackException(SourcePosition);

				return this->stack[this->count - 1];
				});
		}

		/// <summary>
		/// Returns an upper element without popping from stack
		/// (if stack is empty, generates an exception)
		/// </summary>
		/// <returns>A constant reference to element</returns>
		const T& Top() {
			return this->lock.SharedLock([&]() mutable -> const T & InlineLambda{
				if (this->count == 0)
				Internals::Exceptions::ThrowEmptyStackException(SourcePosition);

				return this->stack[this->count - 1];
				});
		}

		/// <summary>
		/// A stack extension on given number of elements
		/// </summary>
		/// <param name="AdditionalSize">A number on which is needed to extend a stack</param>
		ForceInline void ExtendBuffer(SizeT AdditionalSize) {
			this->lock.ExclusiveLock([&]() mutable InlineLambda{
				ExtendBufferImpl(AdditionalSize);
				});
		}

		/// <summary>
		/// Shrinking stack size to a curent number of elements
		/// </summary>
		void Shrink() {
			this->lock.ExclusiveLock([&]() mutable InlineLambda{
				T * newBuffer = new T[this->count];
				for (SizeT i = 0; i < this->count; i++)
				{
					newBuffer[i] = MoveRValue(this->stack[i]);
				}
				delete this->stack;
				this->stack = newBuffer;
				this->bufferSize = this->count;
				});
		}

		/// <summary>
		/// Optimized swap
		/// </summary>
		/// <param name="a">First element</param>
		/// <param name="b">Second element</param>
		template<typename U>
		friend void Cardinal::Swap(U& a, U& b);

		/// <summary>
		/// A number of elements in stack
		/// </summary>
		/// <returns>A current number of elements in stack</returns>
		ForceInline SizeT GetCount() const { return this->count; }

		/// <summary>
		/// A current number of elements in stack
		/// </summary>
		SizeT Property(GetCount) Count;

		/// <summary>
		/// Cloning a stack object
		/// </summary>
		/// <returns>A new stack object with copy of elements</returns>
		template<typename = void> requires (Concepts::IsClonable<T>)
			BaseConcurencyDynamicStack Clone() const {
			return this->lock.SharedLock([&]() InlineLambda{
				BaseConcurencyDynamicStack result;
				result.bufferSize = this->bufferSize;
				result.count = this->count;
				result.stack = new T[this->bufferSize];

				if constexpr (Concepts::IsPod<T>)
				{
					Memory::Copy(static_cast<const void*>(this->stack), static_cast<void*>(result.stack), this->count * sizeof(T));
				}
				else
				{
					for (SizeT i = 0; i < this->count; i++)
					{
						result.stack[i] = this->stack[i];
					}
				}
				return result;
				});
		}

		/// <summary>
		/// Cloning a stack object using custom lambda or functor
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">A function with which occurs cloning</param>
		/// <returns>A new stack object with copy of elements</returns>
		template<typename TLambda> requires (Concepts::IsInvockableWithResult<TLambda, T, const T&>)
			BaseConcurencyDynamicStack Clone(TLambda&& lambda) const {
			return this->lock.SharedLock([&]() InlineLambda{
				BaseConcurencyDynamicStack result;
				result.bufferSize = this->bufferSize;
				result.count = this->count;
				result.stack = new T[this->bufferSize];

				for (SizeT i = 0; i < this->count; i++)
				{
					result.stack[i] = lambda(this->stack[i]);
				}
				return result;
				});
		}

	private:
		void ExtendBufferImpl(SizeT AdditionalSize) {
			T* newBuffer = new T[this->bufferSize + AdditionalSize];

			for (SizeT i = 0; i < this->count; i++)
			{
				newBuffer[i] = MoveRValue(this->stack[i]);
			}

			delete this->stack;
			this->stack = newBuffer;
			this->bufferSize = this->bufferSize + AdditionalSize;
		}

	protected:
		template<template<typename, typename> typename TCollection, typename UHeap, typename U, Boolean TIsDynamic, Boolean TIsConcurencySuport, Boolean TIsReadonly, Boolean TIsCollectionCanReturnRef>
		friend class LINQ::ICollection;

		template<template<typename, typename> typename TCollection, typename UHeap, typename U, Boolean TIsConcurencySuport>
		friend class LINQ::IDynamicStack;

		template<template<typename UHeap, typename U> typename TCollection, typename UHeap, typename U, Boolean TIsDynamic, Boolean TIsConcurencySuport, Boolean TIsReadonly, Boolean TIsCollectionCanReturnRef>
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
		BaseConcurencyDynamicStack& ForEachImpl(TLambda&& lambda) {
			if constexpr (TypeTraits::IsInvockable<TLambda, T&> || TypeTraits::IsInvockable<TLambda, const T&>)
			{
				return this->lock.SharedLock([&]()->BaseConcurencyDynamicStack & InlineLambda{
					if (this->count == 0)
						return *this;

					for (SizeT i = 0; i < count; i++)
					{
						lambda(this->stack[i]);
					}

					return *this;
					});
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, T&, SizeT> || TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				return this->lock.SharedLock([&]()->BaseConcurencyDynamicStack & InlineLambda{
					if (this->count == 0)
						return *this;

					for (SizeT i = 0; i < count; i++)
					{
						lambda(this->stack[i], i);
					}

					return *this;
					});
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, T&> || TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, T&, SizeT> || TypeTraits::IsInvockable<TLambda, const T&, SizeT>);
				__assume(0);
			}
		}

		template<typename TLambda>
		BaseConcurencyDynamicStack& ForEachImpl(TLambda&& lambda, SizeT itemsCount) {
			if constexpr (TypeTraits::IsInvockable<TLambda, T&> || TypeTraits::IsInvockable<TLambda, const T&>)
			{
				return this->lock.SharedLock([&]()->BaseConcurencyDynamicStack & InlineLambda{
					if (this->count == 0)
						return *this;

					SizeT end = (itemsCount > this->Count) ? this->Count : itemsCount;

					for (SizeT i = 0; i < end; i++)
					{
						lambda(this->stack[i]);
					}

					return *this;
					});
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, T&, SizeT> || TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				return this->lock.SharedLock([&]()->BaseConcurencyDynamicStack & InlineLambda{
					if (this->count == 0)
						return *this;

					SizeT end = (itemsCount > this->Count) ? this->Count : itemsCount;

					for (SizeT i = 0; i < end; i++)
					{
						lambda(this->stack[i], i);
					}

					return *this;
					});
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, T&> || TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, T&, SizeT> || TypeTraits::IsInvockable<TLambda, const T&, SizeT>);
				__assume(0);
			}
		}

		template<typename TLambda>
		const BaseConcurencyDynamicStack& ForEachImpl(TLambda&& lambda) const {
			if constexpr (TypeTraits::IsInvockable<TLambda, const T&>)
			{
				return this->lock.SharedLock([&]() -> const BaseConcurencyDynamicStack & InlineLambda{
					if (this->count == 0)
						return *this;

					for (SizeT i = 0; i < count; i++)
					{
						lambda(this->stack[i]);
					}

					return *this;
					});
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				return this->lock.SharedLock([&]() -> const BaseConcurencyDynamicStack & InlineLambda{
					if (this->count == 0)
						return *this;

					for (SizeT i = 0; i < count; i++)
					{
						lambda(this->stack[i], i);
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
		const BaseConcurencyDynamicStack& ForEachImpl(TLambda&& lambda, SizeT itemsCount) const {
			if constexpr (TypeTraits::IsInvockable<TLambda, const T&>)
			{
				return this->lock.SharedLock([&]() -> const BaseConcurencyDynamicStack & InlineLambda{
					if (this->count == 0)
						return *this;

					SizeT end = (itemsCount > this->Count) ? this->Count : itemsCount;

					for (SizeT i = 0; i < end; i++)
					{
						lambda(this->stack[i]);
					}

					return *this;
					});
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				return this->lock.SharedLock([&]() -> const BaseConcurencyDynamicStack & InlineLambda{
					if (this->count == 0)
						return *this;

					SizeT end = (itemsCount > this->Count) ? this->Count : itemsCount;

					for (SizeT i = 0; i < end; i++)
					{
						lambda(this->stack[i], i);
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
			return MoveRValue(this->lock.SharedLock([&]() mutable InlineLambda{
				U result = value;

				if (this->count == 0)
					return result;

				for (SizeT i = 0; i < count; i++)
				{
					if constexpr (
						Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&> ||
						Concepts::IsInvockableWithResult<TLambda, U, U&, const T&>)
					{
						result = lambda(result, this->stack[i]);
					}
					else
					{
						lambda(result, this->stack[i]);
					}
				}

				return MoveRValue(result);
				}));
		}

		template<typename U, typename TLambda>
		U AggregateImpl(TLambda&& lambda, U&& value) const {
			return MoveRValue(this->lock.SharedLock([&, value = MoveRValue(value)]() mutable InlineLambda{
				if (this->count == 0)
					return value;

				for (SizeT i = 0; i < count; i++)
				{
					if constexpr (
						Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&> ||
						Concepts::IsInvockableWithResult<TLambda, U, U&, const T&>)
					{
						value = lambda(value, this->stack[i]);
					}
					else
					{
						lambda(value, this->stack[i]);
					}
				}

				return MoveRValue(value);
				}));
		}

		ForceInline bool AnyImpl() const { return this->count > 0; }

		template<typename TLambda>
		bool AnyImpl(TLambda&& lambda) const {
			return MoveRValue(this->lock.SharedLock([&]() InlineLambda{
				if (this->count == 0)
					return false;

				for (SizeT i = 0; i < count; i++)
				{
					if (lambda(this->stack[i]))
						return true;
				}

				return false;
				}));
		}

		template<typename TLambda>
		bool AllImpl(TLambda&& lambda) const {
			return MoveRValue(this->lock.SharedLock([&]() InlineLambda{
				if (this->count == 0)
					return false;

				for (SizeT i = 0; i < count; i++)
				{
					if (!lambda(this->stack[i]))
						return false;
				}

				return true;
				}));
		}

		template<typename TLambda>
		T& FirstImpl(TLambda&& lambda) {
			return this->lock.SharedLock([&]()->T & InlineLambda{
				if (this->count == 0)
					Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);

				for (SizeT i = 0; i < count; i++)
				{
					if (lambda(this->stack[i]))
						return static_cast<T&>(this->stack[i]);
				}

				Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
				});
		}

		template<typename TLambda>
		T& LastImpl(TLambda&& lambda) {
			return this->lock.SharedLock([&]()->T & InlineLambda{
				if (this->count == 0)
					Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);

				SizeT index = -1;

				for (SizeT i = 0; i < count; i++)
				{
					if (lambda(this->stack[i]))
						index = i;
				}

				if (index != -1)
					return static_cast<T&>(this->stack[index]);

				Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
				});
		}

		template<typename TLambda>
		const T& FirstImpl(TLambda&& lambda) const {
			return this->lock.SharedLock([&]() -> const T & InlineLambda{
				if (this->count == 0)
					Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);

				for (SizeT i = 0; i < count; i++)
				{
					if (lambda(this->stack[i]))
						return static_cast<T&>(this->stack[i]);
				}

				Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
				});
		}

		template<typename TLambda>
		const T& LastImpl(TLambda&& lambda) const {
			return this->lock.SharedLock([&]() -> const T & InlineLambda{
				if (this->count == 0)
					Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);

				SizeT index = -1;
				for (SizeT i = 0; i < count; i++)
				{
					if (lambda(this->stack[i]))
						index = i;
				}

				if (index != -1)
					return static_cast<T&>(this->stack[index]);

				Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
				});
		}

		template<typename TLambda>
		T FirstOrImpl(TLambda&& lambda, T&& item) const {
			return this->lock.SharedLock([&, item = MoveRValue(item)]() InlineLambda{
				if (count == 0)
					return MoveRValue(item);

				for (SizeT i = 0; i < count; i++)
				{
					if (lambda(this->stack[i]))
						return this->stack[i];
				}

				return MoveRValue(item);
				});
		}

		template<typename TLambda>
		T LastOrImpl(TLambda&& lambda, T&& item) const {
			return this->lock.SharedLock([&, item = MoveRValue(item)]() InlineLambda{
				if (count == 0)
					return MoveRValue(item);

				SizeT index = -1;

				for (SizeT i = 0; i < count; i++)
				{
					if (lambda(this->stack[i]))
						index = i;
				}

				if (index != -1)
					return this->stack[index];

				return MoveRValue(item);
				});
		}

		template<typename TLambda>
		T FirstOrImpl(TLambda&& lambda, const T& item) const {
			return this->lock.SharedLock([&]() InlineLambda{
				if (count == 0)
					return item;

				for (SizeT i = 0; i < count; i++)
				{
					if (lambda(this->stack[i]))
						return this->stack[i];
				}

				return item;
				});
		}

		template<typename TLambda>
		T LastOrImpl(TLambda&& lambda, const T& item) const {
			return this->lock.SharedLock([&]() InlineLambda{
				if (count == 0)
					return item;

				SizeT index = -1;

				for (SizeT i = 0; i < count; i++)
				{
					if (lambda(this->stack[i]))
						index = i;
				}

				if (index != -1)
					return this->stack[index];

				return item;
				});
		}

		template<typename TLambda, typename TGetterLambda>
		T FirstOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			return this->lock.SharedLock([&]() InlineLambda{
				if (count == 0)
					return otherLambda();

				for (SizeT i = 0; i < count; i++)
				{
					if (lambda(this->stack[i]))
						return this->stack[i];
				}

				return otherLambda();
				});
		}

		template<typename TLambda, typename TGetterLambda>
		T LastOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			return this->lock.SharedLock([&]() InlineLambda{
				if (count == 0)
					return otherLambda();

				SizeT index = -1;

				for (SizeT i = 0; i < count; i++)
				{
					if (lambda(this->stack[i]))
						index = i;
				}

				if (index != -1)
					return this->stack[index];

				return otherLambda();
				});
		}

		BaseConcurencyDynamicStack& ReverseImpl() {
			return this->lock.ExclusiveLock([&]()->BaseConcurencyDynamicStack & InlineLambda{
				for (SizeT i = 0; i < this->count / 2; i++)
 {
Swap(this->stack[i], this->stack[(this->count - 1) - i]);
}
return *this;
				});
		}

		ForceInline void UnsafeSet(const T& value, SizeT index) {
			this->stack[index] = value;
			this->count = index + 1;
		}

		ForceInline void UnsafeSet(T&& value, SizeT index) {
			this->stack[index] = MoveRValue(value);
			this->count = index + 1;
		}

		BaseConcurencyDynamicStack& RemoveAllImpl() {
			return this->lock.ExclusiveLock([&]()->BaseConcurencyDynamicStack & InlineLambda{
				delete[] this->stack; this->stack = nullptr;
				this->count = this->bufferSize = 0;
				return *this;
				});
		}

		template<typename TLambda>
		BaseConcurencyDynamicStack& RemoveAllImpl(TLambda&& lambda) {
			return this->lock.ExclusiveLock([&]()->BaseConcurencyDynamicStack & InlineLambda{
				for (SizeT i = 0; i < this->count; i++)
 {
lambda(MoveRValue(this->stack[i]));
}
delete[] this->stack; this->stack = nullptr;
this->count = this->bufferSize = 0;
return *this;
				});
		}

		template<typename TLambda>
		SizeT GetCountIfImpl(TLambda&& lambda) const {
			return this->lock.SharedLock([&]() InlineLambda{
				SizeT selectedCount = 0;

				for (SizeT i = 0; i < count; i++)
				{
					if (lambda(this->stack[i]))
						selectedCount++;
				}
				return selectedCount;
				});
		}
	};

	/// <summary>
	/// A stack with multithreading support
	/// </summary>
	template<typename T>
	using ConcurencyDynamicStack = BaseConcurencyDynamicStack<Cardinal::Memory::Heap, T>;

	/// <summary>
	/// A stack with multithreading support in protected heap
	/// </summary>
	template<typename T>
	using ConcurencySensitiveDynamicStack = BaseConcurencyDynamicStack<Cardinal::Memory::SensitiveDataHeap, T>;
}

export namespace Cardinal::TypeTraits::Collection
{
	template<typename THeap, typename T>
	struct IsDynamicStackT<Cardinal::Containers::BaseConcurencyDynamicStack<THeap, T>> : LogicTrue {};
}

export namespace Cardinal
{
	template<typename THeap, typename T>
	ForceInline void Swap(Containers::BaseConcurencyDynamicStack<THeap, T>& a, Containers::BaseConcurencyDynamicStack<THeap, T>& b) {
		a.lock.ExclusiveLock([&] () {
			b.lock.ExclusiveLock([&] () {
				Swap(a.count, b.count);
				Swap(a.bufferSize, b.bufferSize);
				Swap(a.stack, b.stack);
				});
			});
	}
}

#pragma warning(pop)