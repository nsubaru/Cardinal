export module Cardinal.Containers.NonConcurency.DynamicStack;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;
export import Cardinal.Containers.InitilizerList;
export import Cardinal.Settings;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
#pragma warning(disable: 4172)

export namespace Cardinal::Containers
{
	/// <summary>
	/// A stack of elements
	/// </summary>
	template<typename THeap, typename T>
	class BaseDynamicStack :
		public LINQ::IDynamicStack<BaseDynamicStack, THeap, T, false> {
	private:
		SizeT count;
		SizeT bufferSize;
		T* Restrict stack;

	public:
		~BaseDynamicStack() {
			delete[] stack;
			count = bufferSize = 0;
			stack = nullptr;
		}

	public:
		/// <summary>
		/// An empty constructor that initializes a stack with standard maximal number of elements
		/// </summary>
		ForceInline BaseDynamicStack() :
			count(0),
			bufferSize(CurrentSettings().MemoryManager.StackReserveSize),
			stack(new T[CurrentSettings().MemoryManager.StackReserveSize]) {
		}

		/// <summary>
		/// A constructor with buffer size
		/// </summary>
		/// <param name="bufferSize">Buffer size</param>
		ForceInline BaseDynamicStack(SizeT bufferSize) :
			count(0),
			stack(new T[bufferSize]),
			bufferSize(bufferSize) {
		}

		/// <summary>
		/// Copying constructor
		/// </summary>
		/// <param name="original">Original</param>
		BaseDynamicStack(const BaseDynamicStack& original) :
			count(original.count),
			bufferSize(original.bufferSize),
			stack(nullptr) {
			this->stack = new T[original.bufferSize];

			if constexpr (!TypeTraits::IsPodV<T>)
			{
				for (SizeT i = 0; i < count; i++)
				{
					this->stack[i] = original.stack[i];
				}
			}
			else
			{
				Memory::Copy(original.stack, this->stack, sizeof(T) * original.count);
			}

			this->count = original.count;
		}

		/// <summary>
		/// Moving constructor
		/// </summary>
		/// <param name="original">Original</param>
		BaseDynamicStack(BaseDynamicStack&& original) {
			this->count = MoveRValue(original.count);
			this->stack = MoveRValue(original.stack);
			this->bufferSize = MoveRValue(original.bufferSize);

			original.count = 0;
			original.stack = nullptr;
			original.bufferSize = 0;
		}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"LValue" reference to array</returns>
		BaseDynamicStack& operator=(const BaseDynamicStack& original) {
			this->~BaseDynamicStack();
			this->BaseDynamicStack::BaseDynamicStack(original);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"RValue" reference to array</returns>
		BaseDynamicStack& operator=(BaseDynamicStack&& original) {
			this->~BaseDynamicStack();
			this->BaseDynamicStack::BaseDynamicStack(MoveRValue(original));
			return *this;
		}

		/// <summary>
		/// A constructor with initialization list 
		/// (moves elements from the list, sets size as a 
		/// sum of number of elements plus standard number of elements)
		/// </summary>
		/// <param name="list">List of elements</param>
		BaseDynamicStack(InitilizerList<T>&& list) :
			count(0),
			bufferSize(0),
			stack(nullptr) {
			if (list.size() != 0)
			{
				count = list.size();
				bufferSize = count + CurrentSettings().MemoryManager.StackReserveSize;
				this->stack = new T[bufferSize];

				auto it = list.begin();

				for (SizeT i = 0; i < count; i++)
				{
					stack[i] = MoveRValue(*it); ++it;
				}
			}
			else
			{
				count = 0;
				bufferSize = count + CurrentSettings().MemoryManager.StackReserveSize;
				this->stack = new T[bufferSize];
			}
		}

		/// <summary>
		/// Adds elements in stack (is stack is full
		/// then in extends on amount that is set in settings)
		/// </summary>
		/// <param name="item">A reference to element</param>
		ForceInline void Push(const T& item) {
			if (count == bufferSize)
			{
				ExtendBufferImpl(CurrentSettings().MemoryManager.StackReserveSize);
			}

			stack[count] = item;
			++count;
		}

		/// <summary>
		/// Adds elements in stack (is stack is full
		/// then in extends on amount that is set in settings)
		/// </summary>
		/// <param name="item">"RValue" reference to an element</param>
		ForceInline void Push(T&& item) {
			if (count == bufferSize)
			{
				ExtendBufferImpl(CurrentSettings().MemoryManager.StackReserveSize);
			}

			stack[count] = MoveRValue(item);
			++count;
		}

		/// <summary>
		/// Pops an element from stack (if stack is empty, generates an exception)
		/// </summary>
		/// <returns>A popped element from stack</returns>
		ForceInline T Pop() {
			if (count == 0)
				Internals::Exceptions::ThrowEmptyStackException(SourcePosition);

			--count;
			return MoveRValue(stack[count]);
		}

		/// <summary>
		/// Pops an element from stack
		/// </summary>
		/// <param name="item">A returned object if stack contains an element</param>
		/// <returns>A popped element from stack</returns>
		ForceInline T Pop(T&& item) {
			if (count == 0)
				return MoveRValue(item);

			--count;
			return MoveRValue(stack[count]);
		}

		/// <summary>
		/// Pops an element from stack
		/// </summary>
		/// <param name="item">A returned object if stack contains an element</param>
		/// <returns>A popped element from stack</returns>
		ForceInline T Pop(const T& item) {
			if (count == 0)
				return item;

			--count;
			return MoveRValue(stack[count]);
		}

		/// <summary>
		/// Pops an element from stack
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">A lamda which returns an element if stack is not empty</param>
		/// <returns>A popped element from stack</returns>
		template<typename TLambda> requires Concepts::IsInvockableWithResult<TLambda, T>
		ForceInline T Pop(TLambda&& lambda) {
			if (count == 0)
				return lambda();

			return MoveRValue(stack[count]);
		}

		/// <summary>
		/// Returns an upper element without popping from stack
		/// (if stack is empty, generates an exception)
		/// </summary>
		/// <returns>A constant reference to element</returns>
		ForceInline const T& Top() const {
			if (count == 0)
				Internals::Exceptions::ThrowEmptyStackException(SourcePosition);
			
			return stack[count - 1];
		}

		/// <summary>
		/// Returns an upper element without popping from stack
		/// (if stack is empty, generates an exception)
		/// </summary>
		/// <returns>A constant reference to element</returns>
		ForceInline const T& Top() {
			if (count == 0)
				Internals::Exceptions::ThrowEmptyStackException(SourcePosition);

			return stack[count - 1];
		}

		/// <summary>
		/// A stack extension on given number of elements
		/// </summary>
		/// <param name="AdditionalSize">A number to which the stack is need to extend</param>
		ForceInline void ExtendBuffer(SizeT AdditionalSize) {
			ExtendBufferImpl(AdditionalSize);
		}

		/// <summary>
		/// Shrinking stack size to a current number of elements
		/// </summary>
		void Shrink() {
			T* newBuffer = new T[count];
			for (SizeT i = 0; i < count; i++)
			{
				newBuffer[i] = MoveRValue(stack[i]);
			}

			delete stack;
			stack = newBuffer;
			bufferSize = count;
		}

		/// <summary>
		/// Cloning a stack object
		/// </summary>
		/// <returns>A new stack object with copy of elements</returns>
		template<typename = void> requires (Concepts::IsClonable<T>)
			BaseDynamicStack Clone() const {
			BaseDynamicStack result;
			result.bufferSize = this->bufferSize;
			result.count = this->count;
			result.stack = new T[this->bufferSize];

			if constexpr (Concepts::IsPod<T>)
			{
				Memory::Copy(this->stack, result.stack, sizeof(T) * this->count);
			}
			else
			{
				for (SizeT i = 0; i < count; i++)
				{
					result.stack[i] = stack[i];
				}
			}

			return result;
		}

		/// <summary>
		/// Cloning a stack object using custom lambda or functor
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">A function with which occurs cloning</param>
		/// <returns>A new stack object with copy of elements</returns>
		template<typename TLambda> requires (Concepts::IsInvockableWithResult<TLambda, T, const T&>)
			BaseDynamicStack Clone(TLambda&& lambda) const {
			BaseDynamicStack result;
			result.bufferSize = bufferSize;
			result.count = count;
			result.stack = new T[bufferSize];
			for (SizeT i = 0; i < count; i++)
			{
				result.stack[i] = lambda(stack[i]);
			}
			return result;
		}

		/// <summary>Optimized swap</summary>
		/// <param name = 'A'>First element</param>
		/// <param name = 'B'>Second element</param>
		template<typename U>
		friend void Cardinal::Swap(U& A, U& B);

		/// <summary>
		/// A number of elements in stack
		/// </summary>
		/// <returns>A current number of elements in stack</returns>
		ForceInline SizeT GetCount() const { return count; }

		/// <summary>
		/// A current number of elements in stack
		/// </summary>
		SizeT Property(GetCount) Count;

	private:
		void ExtendBufferImpl(SizeT AdditionalSize) {
			T* newBuffer = nullptr;
			try
			{
				T* newBuffer = new T[bufferSize + AdditionalSize];

				for (SizeT i = 0; i < count; i++)
				{
					newBuffer[i] = MoveRValue(stack[i]);
				}

				Swap(stack, newBuffer);
				delete newBuffer; newBuffer = nullptr;
				bufferSize = bufferSize + AdditionalSize;
			}
			catch (...)
			{
				delete newBuffer;
				Unsafe(Internals::Exceptions::__ReThrow());
			}
		}

	protected:
		template<template<typename, typename> typename TCollection, typename THeap, typename T, Boolean TIsDynamic, Boolean TIsConcurencySuport, Boolean TIsReadonly, Boolean TIsCollectionCanReturnRef>
		friend class LINQ::ICollection;

		template<template<typename, typename> typename TCollection, typename THeap, typename T, Boolean TIsConcurencySuport>
		friend class LINQ::IDynamicStack;

		template<template<typename THeap, typename T> typename TCollection, typename THeap, typename T, Boolean TIsDynamic, Boolean TIsConcurencySuport, Boolean TIsReadonly, Boolean TIsCollectionCanReturnRef>
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
		BaseDynamicStack& ForEachImpl(TLambda&& lambda) {
			if constexpr (TypeTraits::IsInvockable<TLambda, T&> || TypeTraits::IsInvockable<TLambda, const T&>)
			{
				if (count == 0)
					return *this;

				for (SizeT i = 0; i < count; i++)
				{
					lambda(stack[i]);
				}

				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, T&, SizeT> || TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				if (count == 0)
					return *this;

				for (SizeT i = 0; i < count; i++)
				{
					lambda(stack[i], i);
				}

				return *this;
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, T&> || TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, T&, SizeT> || TypeTraits::IsInvockable<TLambda, const T&, SizeT>);
			}
		}

		template<typename TLambda>
		BaseDynamicStack& ForEachImpl(TLambda&& lambda, SizeT itemsCount) {
			if constexpr (TypeTraits::IsInvockable<TLambda, T&> || TypeTraits::IsInvockable<TLambda, const T&>)
			{
				if (count == 0)
					return *this;

				SizeT end = (itemsCount > count) ? count : itemsCount;

				for (SizeT i = 0; i < end; i++)
				{
					lambda(stack[i]);
				}

				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, T&, SizeT> || TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				if (count == 0)
					return *this;

				SizeT end = (itemsCount > count) ? count : itemsCount;

				for (SizeT i = 0; i < end; i++)
				{
					lambda(stack[i], i);
				}

				return *this;
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, T&> || TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, T&, SizeT> || TypeTraits::IsInvockable<TLambda, const T&, SizeT>);
			}
		}

		template<typename TLambda>
		const BaseDynamicStack& ForEachImpl(TLambda&& lambda) const {
			if constexpr (TypeTraits::IsInvockable<TLambda, const T&>)
			{
				if (count == 0)
					return *this;

				for (SizeT i = 0; i < count; i++)
				{
					lambda(stack[i]);
				}

				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				if (count == 0)
					return *this;

				for (SizeT i = 0; i < count; i++)
				{
					lambda(stack[i], i);
				}

				return *this;
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, const T&, SizeT>);
			}
		}

		template<typename TLambda>
		const BaseDynamicStack& ForEachImpl(TLambda&& lambda, SizeT itemsCount) const {
			if constexpr (TypeTraits::IsInvockable<TLambda, const T&>)
			{
				if (count == 0)
					return *this;

				SizeT end = (itemsCount > count) ? count : itemsCount;

				for (SizeT i = 0; i < end; i++)
				{
					lambda(stack[i]);
				}

				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				if (count == 0)
					return *this;

				SizeT end = (itemsCount > count) ? count : itemsCount;

				for (SizeT i = 0; i < end; i++)
				{
					lambda(stack[i], i);
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
				return MoveRValue(result);

			for (SizeT i = 0; i < count; i++)
			{
				if constexpr (
					Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&> ||
					Concepts::IsInvockableWithResult<TLambda, U, U&, const T&>)
				{
					data = lambda(data, stack[i]);
				}
				else
				{
					lambda(data, stack[i]);
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
					value = lambda(value, stack[i]);
				}
				else
				{
					lambda(value, stack[i]);
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
				if (lambda(stack[i]))
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
				if (!lambda(stack[i]))
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
				if (lambda(stack[i]))
					return static_cast<T&>(stack[i]);
			}

			Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
		}

		template<typename TLambda>
		T& LastImpl(TLambda&& lambda) {
			if (count == 0)
				Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);

			SizeT index = -1;

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(stack[i]))
					index = i;
			}

			if (index != -1)
				return static_cast<T&>(stack[index]);

			Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
		}

		template<typename TLambda>
		const T& FirstImpl(TLambda&& lambda) const {
			if (count == 0)
				Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(stack[i]))
					return static_cast<T&>(stack[i]);
			}

			Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
		}

		template<typename TLambda>
		const T& LastImpl(TLambda&& lambda) const {
			if (count == 0)
				Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);

			SizeT index = -1;
			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(stack[i]))
					index = i;
			}

			if (index != -1)
				return static_cast<T&>(stack[index]);

			Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
		}

		template<typename TLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, T&& item) const {
			if (count == 0)
				return MoveRValue(item);

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(stack[i]))
					return stack[i];
			}

			return MoveRValue(item);
		}

		template<typename TLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, T&& item) const {
			if (count == 0)
				return MoveRValue(item);

			SizeT index = -1;

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(stack[i]))
					index = i;
			}

			if (index != -1)
				return stack[index];

			return MoveRValue(item);
		}

		template<typename TLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, const T& item) const {
			if (count == 0)
				return item;

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(stack[i]))
					return stack[i];
			}

			return item;
		}

		template<typename TLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, const T& item) const {
			if (count == 0)
				return item;

			SizeT index = -1;
			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(stack[i]))
					index = i;
			}

			if (index != -1)
				return stack[index];

			return item;
		}

		template<typename TLambda, typename TGetterLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			if (count == 0)
				return otherLambda();

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(stack[i]))
					return stack[i];
			}

			return otherLambda();
		}

		template<typename TLambda, typename TGetterLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			if (count == 0)
				return otherLambda();

			SizeT index = -1;
			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(stack[i]))
					index = i;
			}

			if (index != -1)
				return stack[index];

			return otherLambda();
		}

		ForceInline BaseDynamicStack& ReverseImpl() {
			for (SizeT i = 0; i < count / 2; i++)
			{
				Swap(stack[i], stack[(count - 1) - i]);
			}
			return *this;
		}

		ForceInline void UnsafeSet(T&& Value, SizeT index) {
			stack[index] = MoveRValue(Value);
			count = index + 1;
		}

		ForceInline void UnsafeSet(const T& Value, SizeT index) {
			stack[index] = Value;
			count = index + 1;
		}

		ForceInline BaseDynamicStack& RemoveAllImpl() {
			delete[] stack; stack = nullptr;
			count = bufferSize = 0;
			this->count = 0;
			return *this;
		}

		template<typename TLambda>
		BaseDynamicStack& RemoveAllImpl(TLambda&& lambda) {
			for (SizeT i = 0; i < count; i++)
			{
				lambda(MoveRValue(stack[i]));
			}

			delete[] stack; stack = nullptr;
			count = bufferSize = 0;
			this->count = 0;
			return *this;
		}

		template<typename TLambda>
		SizeT GetCountIfImpl(TLambda&& lambda) const {
			SizeT selectedCount = 0;

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(stack[i]))
					selectedCount++;
			}

			return selectedCount;
		}
	};

	/// <summary>
	/// A stack of elements
	/// </summary>
	template<typename T>
	using DynamicStack = BaseDynamicStack<Cardinal::Memory::Heap, T>;

	/// <summary>
	/// A stack of elements in protected heap
	/// </summary>
	template<typename T>
	using SensitiveDynamicStack = BaseDynamicStack<Cardinal::Memory::SensitiveDataHeap, T>;
}

export namespace Cardinal::TypeTraits::Collection
{
	template<typename THeap, typename T>
	struct IsDynamicStackT<Cardinal::Containers::BaseDynamicStack<THeap, T>> : LogicTrue {};
}

export namespace Cardinal
{
	template<typename THeap, typename T>
	ForceInline void Swap(Containers::BaseDynamicStack<THeap, T>& A, Containers::BaseDynamicStack<THeap, T>& B) {
		Swap(A.count, B.count);
		Swap(A.bufferSize, B.bufferSize);
		Swap(A.stack, B.stack);
	}
}

#pragma warning(pop)