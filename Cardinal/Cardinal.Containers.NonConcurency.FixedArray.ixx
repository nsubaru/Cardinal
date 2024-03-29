export module Cardinal.Containers.NonConcurency.FixedArray;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;
export import Cardinal.Containers.InitilizerList;
export import Cardinal.Settings;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
#pragma warning(disable: 4172)
#pragma warning(disable: 6386)
#pragma warning(disable: 4996)

export namespace Cardinal::Containers
{
	/// <summary>
	/// An array with a fixed number of elements
	/// </summary>
	template<typename THeap, SizeT TSize, typename T>
		requires (TSize > 0)
	class BaseFixedArray final :
		public LINQ::IFixedArray<BaseFixedArray, THeap, T, TSize, false>,
		public Memory::Allocator<THeap> {
	public:
		using RawArray = const T(&)[TSize];

	private:
		T array[TSize] = {};

	public:
		~BaseFixedArray() {
			if constexpr (TypeTraits::IsSameV<THeap, Memory::SensitiveDataHeap> &&
				TypeTraits::IsPodV<T>)
			{
				Memory::SecureZeroMemory(array);
			}
		}

	private:
		template<typename TLambda>
		void QuickSort(T* A, SizeT p, SizeT r, TLambda& lambda) {
			if (p < r)
			{
				SizeT q = Partition(A, p, r, lambda);
				QuickSort(A, p, q, lambda);
				QuickSort(A, q + 1, r, lambda);
			}
		}

		template<typename TLambda>
		SizeT Partition(T* A, SizeT p, SizeT r, TLambda& lambda) {
			T x = A[p]; // pivot
			SizeT i = p;
			SizeT j = r;
			while (true)
			{
				while (lambda(A[i], x) && i != r)
				{
					i++;
				}

				while (!lambda(A[j], x) && j != p)
				{
					j--;
				}

				if (i < j)
				{
					Swap(A[i], A[j]);
				}
				else
				{
					return j;
				}
			}
		}

	public:
		ForceInline SizeT GetCount() const { return TSize; }
		SizeT Property(GetCount) Count;

	protected:
		template<template<typename, typename> typename TCollection, typename UHeap, typename U, Boolean TIsDynamic, Boolean TIsConcurencySuport, Boolean TIsReadonly, Boolean TIsCollectionCanReturnRef>
		friend class LINQ::ICollection;

		template<template<typename, SizeT, typename> typename TCollection, typename UHeap, typename U, SizeT USize, Boolean TIsConcurencySuport>
		friend class LINQ::IFixedCollection;

		template<template<typename, SizeT, typename> typename TCollection, typename UHeap, typename U, SizeT USize, Boolean TIsConcurencySuport>
		friend class LINQ::IFixedArray;

		template<template<typename UHeap, typename U> typename TCollection, typename UHeap, typename U, Boolean TIsDynamic, Boolean TIsConcurencySuport, Boolean TIsReadonly, Boolean TIsCollectionCanReturnRef>
		friend class LINQ::ICollectionBase;

		template<typename U>
		friend struct Cardinal::Serialization::SerializationHandler;

		template<template<typename, SizeT, typename> typename TCollection, typename UHeap, typename U, SizeT USize, Boolean TIsConcurencySuport>
		friend class LINQ::IFixedCollection;

	protected:
		template<typename TLambda>
		ForceInline BaseFixedArray& ForEachImpl(TLambda&& lambda) {
			if constexpr (TypeTraits::IsInvockable<TLambda, T&> || TypeTraits::IsInvockable<TLambda, const T&>)
			{
				for (SizeT i = 0; i < TSize; i++)
				{
					lambda(array[i]);
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, T&, SizeT> || TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				for (SizeT i = 0; i < TSize; i++)
				{
					lambda(array[i], i);
				}
				return *this;
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, T&> || TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, T&, SizeT> || TypeTraits::IsInvockable<TLambda, const T&, SizeT>);
				__assume(0);
			}
		}

		template<typename TLambda>
		ForceInline const BaseFixedArray& ForEachImpl(TLambda&& lambda) const {
			if constexpr (TypeTraits::IsInvockable<TLambda, const T&>)
			{
				for (SizeT i = 0; i < TSize; i++)
				{
					lambda(array[i]);
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				for (SizeT i = 0; i < TSize; i++)
				{
					lambda(array[i], i);
				}
				return *this;
			}
			else
			{
				static_assert(TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, const T&, SizeT>);
				__assume(0);
			}
		}

		template<typename U, typename TLambda>
		ForceInline T AggregateImpl(TLambda&& lambda, const U& value) const {
			U result = value;
			U& data = result;

			for (SizeT i = 0; i < TSize; i++)
			{
				if constexpr (
					Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&> ||
					Concepts::IsInvockableWithResult<TLambda, U, U&, const T&>)
				{
					data = lambda(data, array[i]);
				}
				else
				{
					lambda(data, array[i]);
				}
			}
			return MoveRValue(result);
		}

		template<typename U, typename TLambda>
		ForceInline U AggregateImpl(TLambda&& lambda, U&& value) const {
			for (SizeT i = 0; i < TSize; i++)
			{
				if constexpr (
					Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&> ||
					Concepts::IsInvockableWithResult<TLambda, U, U&, const T&>)
				{
					value = lambda(value, array[i]);
				}
				else
				{
					lambda(value, array[i]);
				}
			}
			return MoveRValue(value);
		}

		ForceInline bool AnyImpl() const { return TSize > 0; }

		template<typename TLambda>
		ForceInline bool AnyImpl(TLambda&& lambda) const {
			for (SizeT i = 0; i < TSize; i++)
			{
				if (lambda(array[i]))
					return true;
			}
			return false;
		}

		template<typename TLambda>
		ForceInline bool AllImpl(TLambda&& lambda) const {
			for (SizeT i = 0; i < TSize; i++)
			{
				if (!lambda(array[i]))
					return false;
			}
			return true;
		}

		ForceInline BaseFixedArray& SortByImpl(bool IsDescending = true) {
			if (IsDescending)
			{
				auto l = [] (const T& V1, const T& V2) { return V1 < V2; };
				QuickSort(array, 0, TSize - 1, l);
			}
			else
			{
				auto l = [] (const T& V1, const T& V2) { return V1 > V2; };
				QuickSort(array, 0, TSize - 1, l);
			}
			return *this;
		}

		template<typename TLambda>
		ForceInline BaseFixedArray& SortByLambdaImpl(TLambda&& lambda) {
			if constexpr (TSize == 0)
			{
				return *this;
			}
			else
			{
				QuickSort(array, 0, TSize - 1, lambda);
				return *this;
			}
		}

		template<typename TLambda>
		ForceInline T& FirstImpl(TLambda&& lambda) {
			for (SizeT i = 0; i < TSize; i++)
				if (lambda(array[i]))
					return static_cast<T&>(array[i]);

			Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
		}

		template<typename TLambda>
		ForceInline T& LastImpl(TLambda&& lambda) {
			SizeT Index = Math::Max<SizeT>();

			for (SizeT i = 0; i < TSize; i++)
				if (lambda(array[i]))
					Index = i;

			if (Index != Math::Max<SizeT>())
				return static_cast<T&>(array[Index]);

			Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
		}

		template<typename TLambda>
		ForceInline const T& FirstImpl(TLambda&& lambda) const {
			for (SizeT i = 0; i < TSize; i++)
				if (lambda(array[i]))
					return static_cast<const T&>(array[i]);

			Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
		}

		template<typename TLambda>
		ForceInline const T& LastImpl(TLambda&& lambda) const {
			SizeT Index = Math::Max<SizeT>();

			for (SizeT i = 0; i < TSize; i++)
				if (lambda(array[i]))
					Index = i;

			if (Index != Math::Max<SizeT>())
				return static_cast<const T&>(array[Index]);

			Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
		}

		template<typename TLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, T&& item) const {
			if (TSize == 0)
				return MoveRValue(item);

			for (SizeT i = 0; i < TSize; i++)
				if (lambda(array[i]))
					return array[i];

			return MoveRValue(item);
		}

		template<typename TLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, T&& item) const {
			if (TSize == 0)
				return MoveRValue(item);

			SizeT Index = Math::Max<SizeT>();

			for (SizeT i = 0; i < TSize; i++)
				if (lambda(array[i]))
					Index = i;

			if (Index != Math::Max<SizeT>())
				return array[Index];

			return MoveRValue(item);
		}

		template<typename TLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, const T& item) const {
			if (TSize == 0)
				return item;

			for (SizeT i = 0; i < TSize; i++)
				if (lambda(array[i]))
					return array[i];

			return item;
		}

		template<typename TLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, const T& item) const {
			if (TSize == 0)
				return item;

			SizeT Index = Math::Max<SizeT>();

			for (SizeT i = 0; i < TSize; i++)
				if (lambda(array[i]))
					Index = i;

			if (Index != Math::Max<SizeT>())
				return array[Index];

			return item;
		}

		template<typename TLambda, typename TGetterLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			if (TSize == 0)
				return otherLambda();

			for (SizeT i = 0; i < TSize; i++)
				if (lambda(array[i]))
					return array[i];

			return otherLambda();
		}

		template<typename TLambda, typename TGetterLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			if (TSize == 0)
				return otherLambda();

			SizeT Index = Math::Max<SizeT>();

			for (SizeT i = 0; i < TSize; i++)
				if (lambda(array[i]))
					Index = i;

			if (Index != Math::Max<SizeT>())
				return array[Index];

			return otherLambda();
		}

		ForceInline BaseFixedArray& ReverseImpl() {
			for (SizeT i = 0; i < TSize / 2; i++)
			{
				Swap(array[i], array[(TSize - 1) - i]);
			}
			return *this;
		}

		ForceInline void UnsafeSet(const T& Value, SizeT Index) {
			array[Index] = Value;
		}

		ForceInline void UnsafeSet(T&& Value, SizeT Index) {
			array[Index] = MoveRValue(Value);
		}

		ForceInline BaseFixedArray& RemoveAllImpl() {
			for (SizeT i = 0; i < TSize; i++)
			{
				array[i].~T();
			}
			return *this;
		}

		template<typename TLambda>
		ForceInline BaseFixedArray& RemoveAllImpl(TLambda&& lambda) {
			for (SizeT i = 0; i < TSize; i++)
			{
				lambda(MoveRValue(array[i]));
			}
			return *this;
		}

	public:
		/// <summary>
		/// An empty constructor that initializes an array with standard value
		/// </summary>
		ForceInline constexpr BaseFixedArray() : array() {}

		/// <summary>
		/// Copying constructor
		/// </summary>
		/// <param name="original">Original</param>
		constexpr BaseFixedArray(const BaseFixedArray& original) : array() {
			if (TypeTraits::IsCompileTimeEvaluating())
			{
				for (SizeT i = 0; i < TSize; i++)
				{
					array[i] = original.array[i];
				}
			}
			else if constexpr (!TypeTraits::IsPodV<T>)
			{
				for (SizeT i = 0; i < TSize; i++)
				{
					array[i] = original.array[i];
				}
			}
			else
			{
				Memory::Copy(original.array, this->array);
			}
		}

		/// <summary>
		/// Moving constructor
		/// </summary>
		/// <param name="original">Original</param>
		constexpr BaseFixedArray(BaseFixedArray&& original) : array() {
			if (TypeTraits::IsCompileTimeEvaluating())
			{
				for (SizeT i = 0; i < TSize; i++)
				{
					array[i] = MoveRValue(original.array[i]);
				}
			}
			else if constexpr (!TypeTraits::IsPodV<T>)
			{
				for (SizeT i = 0; i < TSize; i++)
				{
					this->array[i] = MoveRValue(original.array[i]);
				}
			}
			else
			{
				Memory::Copy(original.array, this->array);
			}
		}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"LValue" reference to array</returns>
		constexpr BaseFixedArray& operator=(const BaseFixedArray& original) {
			this->~BaseFixedArray();
			this->BaseFixedArray::BaseFixedArray(original);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"RValue" reference to array</returns>
		constexpr BaseFixedArray& operator=(BaseFixedArray&& original) {
			this->~BaseFixedArray();
			this->BaseFixedArray::BaseFixedArray(MoveRValue(original));
			return *this;
		}

		/// <summary>
		/// A constructor with initialization list (copies all elements in list. 
		/// If there are more then only those who fit in size of array, 
		/// if less then other initializes by standard values)
		/// </summary>
		/// <param name="list">List of elements</param>
		constexpr BaseFixedArray(InitilizerList<T>&& list) : array() {
			if (list.size() != 0)
			{
				const SizeT actuallyCopyied = (TSize < list.size()) ? TSize : list.size();
				auto it = list.begin();
				for (SizeT i = 0; i < actuallyCopyied; i++)
				{
					array[i] = MoveRValue(*it); ++it;
				}
			}
		}

		/// <summary>
		/// A copying fixed array constructor
		/// </summary>
		/// <param name="arr">A fixed array</param>
		ForceInline constexpr BaseFixedArray(const T(&arr)[TSize]) : array() {
			for (SizeT i = 0; i < TSize; i++)
			{
				array[i] = arr[i];
			}
		}

		/// <summary>
		/// A fixed array constructor which is copying from start index to an end
		/// </summary>
		/// <param name="arr">A fixed array</param>
		/// <param name="startPosition">A start index in fixed array</param>
		template<SizeT USize>
		constexpr BaseFixedArray(const T(&arr)[USize], SizeT startPosition) : array() {
			for (SizeT i = 0; i < ((TSize < USize) ? TSize : USize); i++)
			{
				array[i] = arr[i + startPosition];
			}
		}

	public:
		/// <summary>
		/// An access operator to array elements
		/// </summary>
		/// <param name="Index">Index of element</param>
		/// <returns>A references on elements</returns>
		ForceInline constexpr T& operator[](SizeT Index) {
			if (Index < TSize)
			{
				return array[Index];
			}
			else
			{
				Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, Index, TSize);
			}
		}

		/// <summary>
		/// A constant access operator to array elements
		/// </summary>
		/// <param name="index">Index of element</param>
		/// <returns>A constant references on elements</returns>
		ForceInline constexpr const T& operator[](SizeT index) const {
			if (index < TSize)
			{
				return array[index];
			}
			else
			{
				Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, index, TSize);
			}
		}

		/// <summary>
		/// Array equality operator, returns "true" if all elements are equal
		/// </summary>
		/// <param name="other">Right operand</param>
		/// <returns>"True" if all elements of two arrays are equal</returns>
		constexpr bool operator==(const BaseFixedArray& other) const {
			for (SizeT i = 0; i < TSize; i++)
			{
				if (other.array[i] != this->array[i])
					return false;
			}
			return true;
		}

		/// <summary>
		/// Array inequality operator, returns "false" if all elements are not equal
		/// </summary>
		/// <param name="other">Right operand</param>
		/// <returns>"False" if all elements of two arrays are not equal</returns>
		constexpr bool operator!=(const BaseFixedArray& other) const {
			for (SizeT i = 0; i < TSize; i++)
			{
				if (other.array[i] != this->array[i])
					return true;
			}
			return false;
		}

		/// <summary>
		/// Array equality operator, returns "true" if all elements are equal
		/// </summary>
		/// <param name="other">Right operand</param>
		/// <returns>"True" if all elements of two arrays are equal</returns>
		constexpr bool operator==(const RawArray& other) const {
			for (SizeT i = 0; i < TSize; i++)
			{
				if (other[i] != this->array[i])
					return false;
			}
			return true;
		}

		/// <summary>
		/// Array equality operator, returns "true" if all elements are equal
		/// </summary>
		/// <param name="left">Left operand</param>
		/// <param name="right">Right operand</param>
		/// <returns>"True" if all elements of two arrays are equal</returns>
		constexpr friend bool operator==(const RawArray& left, const BaseFixedArray& right) {
			for (SizeT i = 0; i < TSize; i++)
			{
				if (left[i] != right.array[i])
					return false;
			}
			return true;
		}

		/// <summary>
		/// Array inequality operator, returns "false" if all elements are not equal
		/// </summary>
		/// <param name="other">Right operand</param>
		/// <returns>"False" if all elements of two arrays are not equal</returns>
		constexpr bool operator!=(const RawArray& other) const {
			for (SizeT i = 0; i < TSize; i++)
			{
				if (other[i] != this->array[i])
					return true;
			}
			return false;
		}

		/// <summary>
		/// Array inequality operator, returns "false" if all elements are not equal
		/// </summary>
		/// <param name="left">Left operand</param>
		/// <param name="right">Right operand</param>
		/// <returns>"False" if all elements of two arrays are not equal</returns>
		constexpr friend bool operator!=(const RawArray& left, const BaseFixedArray& right) {
			for (SizeT i = 0; i < TSize; i++)
			{
				if (left[i] != right.array[i])
					return true;
			}
			return false;
		}

		/// <summary>
		/// Array object cloning
		/// </summary>
		/// <returns>A new object with copy of all elements</returns>
		template<typename = void> requires (Concepts::IsClonable<T>)
			constexpr BaseFixedArray Clone() const {
			BaseFixedArray result;

			if (TypeTraits::IsCompileTimeEvaluating())
			{
				for (SizeT i = 0; i < TSize; i++)
				{
					result.array[i] = array[i];
				}
			}
			else if constexpr (TypeTraits::IsPodV<T>)
			{
				Memory::Copy(array, result.array);
			}
			else
			{
				for (SizeT i = 0; i < TSize; i++)
				{
					result.array[i] = Clone(array[i]);
				}
			}

			return result;
		}

		/// <summary>
		/// Array object cloning using custom lambda or functor
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">A function with which occurs cloning</param>
		/// <returns>A new object with copy of all elements</returns>
		template<typename TLambda> requires (Concepts::IsInvockableWithResult<TLambda, T, const T&>)
			constexpr BaseFixedArray Clone(TLambda&& lambda) const {
			BaseFixedArray result;

			for (SizeT i = 0; i < TSize; i++)
			{
				result.array[i] = lambda(array[i]);
			}

			return result;
		}

		operator RawArray () const {
			return array;
		}

		template<typename U>
		friend void Cardinal::Swap(U& a, U& b);

	public:
		/// <summary>
		/// A two arrays with fixed size combining
		/// </summary>
		/// <typeparam name="THeap">A heap of arrays</typeparam>
		/// <typeparam name="T">Type of elements</typeparam>
		/// <param name="first">First array</param>
		/// <param name="second">Second array</param>
		/// <returns>Result array</returns>
		template<typename UHeap, typename U, SizeT TSize1, SizeT TSize2>
		friend BaseFixedArray<UHeap, TSize1 + TSize2, U> Combine(
			const BaseFixedArray<UHeap, TSize1, U>& first,
			const BaseFixedArray<UHeap, TSize2, U>& second
		);
	};

	/// <summary>
	/// A two arrays with fixed size combining
	/// </summary>
	/// <typeparam name="THeap">A heap of arrays</typeparam>
	/// <typeparam name="T">Type of elements</typeparam>
	/// <param name="first">First array</param>
	/// <param name="second">Second array</param>
	/// <returns>Result array</returns>
	template<typename THeap, typename T, SizeT TSize1, SizeT TSize2>
	BaseFixedArray<THeap, TSize1 + TSize2, T> Combine(
		const BaseFixedArray<THeap, TSize1, T>& first,
		const BaseFixedArray<THeap, TSize2, T>& second
	) {
		BaseFixedArray<THeap, TSize1 + TSize2, T> result;

		if constexpr (TypeTraits::IsPodV<T>)
		{
			Memory::Copy(first.array, result.array, TSize1 * sizeof(T));
			Memory::Copy(second.array, result.array + TSize1 * sizeof(T), TSize2 * sizeof(T));
		}
		else
		{
			for (SizeT i = 0; i < TSize1; i++)
			{
				result.array[i] = first.array[i];
			}
			for (SizeT i = 0; i < TSize2; i++)
			{
				result.array[TSize1 + i] = second.array[i];
			}
		}

		return result;
	}

	/// <summary>
	/// An array with a fixed number of elements
	/// </summary>
	template<SizeT TSize, typename T>
	using FixedArray = BaseFixedArray<Cardinal::Memory::Heap, TSize, T>;

	/// <summary>
	/// An array with a fixed number of elements in protected heap
	/// </summary>
	template<SizeT TSize, typename T>
	using SensitiveFixedArray = BaseFixedArray<Cardinal::Memory::SensitiveDataHeap, TSize, T>;
}

export namespace Cardinal::TypeTraits::Collection
{
	template<typename THeap, SizeT TSize, typename T>
	struct IsFixedArrayT<Cardinal::Containers::BaseFixedArray<THeap, TSize, T>> : LogicTrue {};
}

export namespace Cardinal
{
	template<typename THeap, SizeT TSize, typename T>
	ForceInline void Swap(Containers::BaseFixedArray<THeap, TSize, T>& A, Containers::BaseFixedArray<THeap, TSize, T>& B) {
		Swap(A.array, B.array);
	}
}

#pragma warning(pop)