export module Cardinal.Containers.NonConcurency.FixedStack;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;
export import Cardinal.Containers.InitilizerList;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 4172)

export namespace Cardinal::Containers
{
	/// <summary>
	/// A stack with fixed maximal number of elements
	/// </summary>
	template<typename THeap, SizeT TSize, typename T>
		requires (TSize > 0)
	class BaseFixedStack final :
		public LINQ::IFixedStack<BaseFixedStack, THeap, T, TSize, true> {
	private:
		SizeT count;
		T stack[TSize] = {};

	public:
		~BaseFixedStack() {
			if constexpr (TypeTraits::IsSameV<THeap, Memory::SensitiveDataHeap> &&
				TypeTraits::IsPodV<T>)
			{
				Memory::SecureZeroMemory(stack);
			}
		}

	public:
		/// <summary>
		/// An empty constructor
		/// </summary>
		ForceInline BaseFixedStack() : count(0), stack() {}

		/// <summary>
		/// Copying constructor
		/// </summary>
		/// <param name="original">Original</param>
		BaseFixedStack(const BaseFixedStack& original) : count(original.count) {
			if constexpr (TypeTraits::IsPodV<T>)
			{
				Memory::Copy(original.stack, this->stack);
			}
			else
			{
				for (SizeT i = 0; i < count; i++)
				{
					this->stack[i] = original.stack[i];
				}
			}
		}

		/// <summary>
		/// Moving constructor
		/// </summary>
		/// <param name="original">Original</param>
		BaseFixedStack(BaseFixedStack&& original) : count(original.count) {
			if constexpr (TypeTraits::IsPodV<T>)
			{
				Memory::Copy(original.stack, this->stack);
			}
			else
			{
				for (SizeT i = 0; i < TSize; i++)
				{
					this->stack[i] = MoveRValue(original.stack[i]);
				}
			}
		}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"LValue" reference to array</returns>
		BaseFixedStack& operator=(const BaseFixedStack& original) {
			this->~BaseFixedStack();
			this->BaseFixedStack::BaseFixedStack(original);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"RValue" reference to array</returns>
		BaseFixedStack& operator=(BaseFixedStack&& original) {
			this->~BaseFixedStack();
			this->BaseFixedStack::BaseFixedStack(MoveRValue(original));
			return *this;
		}

		/// <summary>
		/// A constructor with initialization list 
		/// (if there is ore elements in list, the rest will be ignored)
		/// </summary>
		/// <param name="list">Initialization list</param>
		BaseFixedStack(InitilizerList<T>&& list) : count(0) {
			if (list.size() != 0)
			{
				SizeT ActuallyCopyied = (TSize < list.size()) ? TSize : list.size();
				auto it = list.begin();

				for (SizeT i = 0; i < ActuallyCopyied; i++)
				{
					stack[i] = MoveRValue(*it); ++it;
				}

				count = ActuallyCopyied;
			}
		}

		/// <summary>
		/// Adds elements in stack (if stack is full than generates an exception)
		/// </summary>
		/// <param name="item">A reference to element</param>
		ForceInline void Push(const T& item) {
			if (count == TSize)
				Internals::Exceptions::ThrowFullStackException(SourcePosition, TSize);

			stack[count] = item;
			++count;
		}

		/// <summary>
		/// Adds elements in stack (if stack is full than generates an exception)
		/// </summary>
		/// <param name="item">"RValue" reference to an element</param>
		ForceInline void Push(T&& item) {
			if (count == TSize)
				Internals::Exceptions::ThrowFullStackException(SourcePosition, TSize);

			stack[count] = MoveRValue(item);
			++count;
		}

		/// <summary>
		/// Popping an element from stack (if stack is empty than generates an exception)
		/// </summary>
		/// <returns>"RValue" reference to an element</returns>
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

			--count;
			return MoveRValue(stack[count]);
		}

		/// <summary>
		/// Gaining the top element in stack (if stack is empty than generates an exception) 
		/// </summary>
		/// <returns>A reference to a top element</returns>
		ForceInline const T& Top() {
			if (count == 0)
				Internals::Exceptions::ThrowEmptyStackException(SourcePosition);

			return stack[count - 1];
		}

		/// <summary>
		/// Gaining the top element in stack (if stack is empty than generates an exception)
		/// </summary>
		/// <returns>A reference to a top element</returns>
		ForceInline const T& Top() const {
			if (count == 0)
				Internals::Exceptions::ThrowEmptyStackException(SourcePosition);

			return stack[count - 1];
		}

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

		/// <summary>
		/// Stack cloning
		/// </summary>
		/// <returns>A stack with copy of elements</returns>
		template<typename = void> requires (Concepts::IsClonable<T>)
			BaseFixedStack Clone() const {
			BaseFixedStack result;

			if constexpr (TypeTraits::IsPodV<T>)
			{
				Memory::Copy(stack, result.stack);
			}
			else
			{
				for (SizeT i = 0; i < count; i++)
				{
					result.stack[i] = Clone(stack[i]);
				}
			}
			result.count = this->count;
			return result;
		}

		/// <summary>
		/// Stack cloning using custom lambda or functor
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">A function with which occurs cloning</param>
		/// <returns>A stack with copy of elements</returns>
		template<typename TLambda> requires (Concepts::IsInvockableWithResult<TLambda, T, const T&>)
			BaseFixedStack Clone(TLambda&& lambda) const {
			BaseFixedStack result;
			result.count = count;

			for (SizeT i = 0; i < count; i++)
			{
				result.stack[i] = lambda(stack[i]);
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
		BaseFixedStack& ForEachImpl(TLambda&& lambda) {
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
		const BaseFixedStack& ForEachImpl(TLambda&& lambda) const {
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

		template<typename U, typename TLambda>
		U AggregateImpl(TLambda&& lambda, const U& value) const {
			U result = value;
			U& data = result;

			if (count == 0)
				return result;

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
				return value;

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

			SizeT Index = Math::Max<SizeT>();

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(stack[i]))
					Index = i;
			}

			if (Index != Math::Max<SizeT>())
				return static_cast<T&>(stack[Index]);

			Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
		}

		template<typename TLambda>
		const T& FirstImpl(TLambda&& lambda) const {
			if (count == 0)
				Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(stack[i]))
					return static_cast<const T&>(stack[i]);
			}

			Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
		}

		template<typename TLambda>
		const T& LastImpl(TLambda&& lambda) const {
			if (count == 0)
				Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);

			SizeT Index = Math::Max<SizeT>();

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(stack[i]))
					Index = i;
			}

			if (Index != Math::Max<SizeT>())
				return static_cast<const T&>(stack[Index]);

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

			SizeT Index = Math::Max<SizeT>();

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(stack[i]))
					Index = i;
			}

			if (Index != Math::Max<SizeT>())
				return stack[Index];

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

			SizeT Index = Math::Max<SizeT>();

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(stack[i]))
					Index = i;
			}

			if (Index != Math::Max<SizeT>())
				return stack[Index];

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

			SizeT Index = Math::Max<SizeT>();

			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(stack[i]))
					Index = i;
			}

			if (Index != Math::Max<SizeT>())
				return stack[Index];

			return otherLambda();
		}

		BaseFixedStack& ReverseImpl() {
			for (SizeT i = 0; i < count / 2; i++)
			{
				Swap(stack[i], stack[(count - 1) - i]);
			}
			return *this;
		}

		ForceInline void UnsafeSet(T&& Value, SizeT Index) {
			stack[Index] = MoveRValue(Value);
			count = Index + 1;
		}

		ForceInline void UnsafeSet(const T& Value, SizeT Index) {
			stack[Index] = Value;
			count = Index + 1;
		}

		BaseFixedStack& RemoveAllImpl() {
			for (SizeT i = 0; i < count; i++)
			{
				stack[i].~T();
			}
			this->count = 0;
			return *this;
		}

		template<typename TLambda>
		BaseFixedStack& RemoveAllImpl(TLambda&& lambda) {
			for (SizeT i = 0; i < count; i++)
			{
				lambda(MoveRValue(stack[i])); stack[i].~T();
			}
			this->count = 0;
			return *this;
		}
	};

	/// <summary>
	/// A stack with fixed maximal number of elements in standard heap
	/// </summary>
	template<SizeT TSize, typename T>
	using FixedStack = BaseFixedStack<Cardinal::Memory::Heap, TSize, T>;

	/// <summary>
	/// A stack with fixed maximal number of elements in protected heap
	/// </summary>
	template<SizeT TSize, typename T>
	using SensitiveFixedStack = BaseFixedStack<Cardinal::Memory::SensitiveDataHeap, TSize, T>;
}

export namespace Cardinal::TypeTraits::Collection
{
	template<typename THeap, SizeT TSize, typename T>
	struct IsFixedStackT<Cardinal::Containers::BaseFixedStack<THeap, TSize, T>> : LogicTrue {};
}

export namespace Cardinal
{
	template<typename THeap, SizeT TSize, typename T>
	ForceInline void Swap(Containers::BaseFixedStack<THeap, TSize, T>& A, Containers::BaseFixedStack<THeap, TSize, T>& B) {
		Swap(A.stack, B.stack);
		Swap(A.count, B.count);
	}
}

#pragma warning(pop)