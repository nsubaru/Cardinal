export module Cardinal.Containers.Concurency.FixedArray;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;
export import Cardinal.Threading.SRWLock;
export import Cardinal.Containers.InitilizerList;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
#pragma warning(disable: 4172)
#pragma warning(disable: 6386)
#pragma warning(disable: 4996)

export namespace Cardinal::Containers
{
	/// <summary>
	/// An array with fixed number of elements and multithreading support
	/// </summary>
	template<typename THeap, SizeT TSize, typename T>
		requires (TSize > 0)
	class BaseConcurencyFixedArray final :
		public LINQ::IFixedArray<BaseConcurencyFixedArray, THeap, T, TSize, true>,
		public Memory::Allocator<THeap> {
	public:
		using RawArray = T(&)[TSize];

	private:
		Threading::SRWLock mutable lock;
		T array[TSize] = {};

	public:
		~BaseConcurencyFixedArray() {
			if constexpr (TypeTraits::IsSameV<THeap, Memory::SensitiveDataHeap> &&
				TypeTraits::IsPodV<T>)
			{
				Memory::SecureZeroMemory(array);
			}
		};

	private:
		template<typename TLambda>
		void QuickSort(T* a, SizeT p, SizeT r, TLambda& lambda) {
			if (p < r)
			{
				SizeT q = Partition(a, p, r, lambda);
				QuickSort(a, p, q, lambda);
				QuickSort(a, q + 1, r, lambda);
			}
		}

		template<typename TLambda>
		SizeT Partition(T* a, SizeT p, SizeT r, TLambda& lambda) {
			T x = a[p]; // pivot
			auto i = p;
			auto j = r;
			while (true)
			{
				while (lambda(a[i], x) && i != r)
				{
					i++;
				}

				while (!lambda(a[j], x) && j != p)
				{
					j--;
				}
				if (i < j)
				{
					Swap(a[i], a[j]);
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

	protected:
		template<typename TLambda>
		BaseConcurencyFixedArray& ForEachImpl(TLambda&& lambda) {
			if constexpr (TypeTraits::IsInvockable<TLambda, T&> || TypeTraits::IsInvockable<TLambda, const T&>)
			{
				return this->lock.SharedLock([&]()->BaseConcurencyFixedArray & InlineLambda{
					for (SizeT i = 0; i < TSize; i++)
					{
						lambda(this->array[i]);
					}
					return *this;
					});
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, T&, SizeT> || TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				return this->lock.SharedLock([&]()->BaseConcurencyFixedArray & InlineLambda{
					for (SizeT i = 0; i < TSize; i++)
					{
						lambda(this->array[i], i);
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
		const BaseConcurencyFixedArray& ForEachImpl(TLambda&& lambda) const {
			if constexpr (TypeTraits::IsInvockable<TLambda, const T&>)
			{
				return this->lock.SharedLock([&]() -> const BaseConcurencyFixedArray & InlineLambda{
					for (SizeT i = 0; i < TSize; i++)
					{
						lambda(this->array[i]);
					}
					return *this;
					});
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				return this->lock.SharedLock([&]() -> const BaseConcurencyFixedArray & InlineLambda{
					for (SizeT i = 0; i < TSize; i++)
					{
						lambda(this->array[i], i);
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
				U & data = result;
				for (SizeT i = 0; i < TSize; i++)
				{
					if constexpr (
						Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&> ||
						Concepts::IsInvockableWithResult<TLambda, U, U&, const T&>)
					{
						data = lambda(data, this->array[i]);
					}
					else
					{
						lambda(data, this->array[i]);
					}
				}
				return MoveRValue(result);
				}));
		}

		template<typename U, typename TLambda>
		U AggregateImpl(TLambda&& lambda, U&& value) const {
			return MoveRValue(this->lock.SharedLock([&, value = MoveRValue(value)]() mutable InlineLambda{
				for (SizeT i = 0; i < TSize; i++)
				{
					if constexpr (
						Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&> ||
						Concepts::IsInvockableWithResult<TLambda, U, U&, const T&>)
					{
						value = lambda(value, this->array[i]);
					}
					else
					{
						lambda(value, this->array[i]);
					}
				}
				return MoveRValue(value);
				}));
		}

		ForceInline bool AnyImpl() const { return TSize > 0; }

		template<typename TLambda>
		bool AnyImpl(TLambda&& lambda) const {
			return MoveRValue(this->lock.SharedLock([&]() InlineLambda{
				for (SizeT i = 0; i < TSize; i++)
				{
					if (lambda(this->array[i]))
						return true;
				}
				return false;
				}));
		}

		template<typename TLambda>
		bool AllImpl(TLambda&& lambda) const {
			return MoveRValue(this->lock.SharedLock([&]() InlineLambda{
				for (SizeT i = 0; i < TSize; i++)
				{
					if (!lambda(this->array[i]))
						return false;
				}
				return true;
				}));
		}

		BaseConcurencyFixedArray& SortByImpl(bool IsDescending = true) {
			return this->lock.ExclusiveLock([&]()->BaseConcurencyFixedArray & InlineLambda{
				if (IsDescending)
 {
auto lambda = [] (const T& v1, const T& v2) { return v1 < v2; };
QuickSort(this->array, 0, TSize - 1, lambda);
}
else
{
auto lambda = [] (const T& v1, const T& v2) { return v1 > v2; };
QuickSort(this->array, 0, TSize - 1, lambda);
}
return *this;
				});
		}

		template<typename TLambda>
		BaseConcurencyFixedArray& SortByLambdaImpl(TLambda&& lambda) {
			if constexpr (TSize == 0)
			{
				return *this;
			}
			else
			{
				return this->lock.ExclusiveLock([&]()->BaseConcurencyFixedArray & InlineLambda
					{
						QuickSort(this->array, 0, TSize - 1, lambda);
						return *this;
					});
			}
		}

		template<typename TLambda>
		T& FirstImpl(TLambda&& lambda) {
			return this->lock.SharedLock([&]()->T & InlineLambda{
				for (SizeT i = 0; i < TSize; i++)
					if (lambda(this->array[i]))
						return static_cast<T&>(this->array[i]);

				Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
				});
		}

		template<typename TLambda>
		T& LastImpl(TLambda&& lambda) {
			return this->lock.SharedLock([&]()->T & InlineLambda{
				SizeT Index = -1;
				for (SizeT i = 0; i < TSize; i++)
					if (lambda(this->array[i]))
						Index = i;

				if (Index != -1)
					return static_cast<T&>(this->array[Index]);

				Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
				});
		}

		template<typename TLambda>
		const T& FirstImpl(TLambda&& lambda) const {
			return this->lock.SharedLock([&]() -> const T & InlineLambda{
				for (SizeT i = 0; i < TSize; i++)
					if (lambda(this->array[i]))
						return static_cast<const T&>(this->array[i]);

				Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
				});
		}

		template<typename TLambda>
		const T& LastImpl(TLambda&& lambda) const {
			return this->lock.SharedLock([&]() -> const T & InlineLambda{
				SizeT Index = -1;
				for (SizeT i = 0; i < TSize; i++)
					if (lambda(this->array[i]))
						Index = i;

				if (Index != -1)
					return static_cast<const T&>(this->array[Index]);

				Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
				});
		}

		template<typename TLambda>
		T FirstOrImpl(TLambda&& lambda, T&& item) const {
			return this->lock.SharedLock([&, item = MoveRValue(item)]() InlineLambda{
				if (TSize == 0)
					return MoveRValue(item);

				for (SizeT i = 0; i < TSize; i++)
					if (lambda(this->array[i]))
						return this->array[i];

				return MoveRValue(item);
				});
		}

		template<typename TLambda>
		T LastOrImpl(TLambda&& lambda, T&& item) const {
			return this->lock.SharedLock([&, item = MoveRValue(item)]() InlineLambda{
				if (TSize == 0)
					return MoveRValue(item);

				SizeT Index = -1;
				for (SizeT i = 0; i < TSize; i++)
					if (lambda(this->array[i]))
						Index = i;

				if (Index != -1)
					return this->array[Index];

				return MoveRValue(item);
				});
		}

		template<typename TLambda>
		T FirstOrImpl(TLambda&& lambda, const T& item) const {
			return this->lock.SharedLock([&]() InlineLambda{
				if (TSize == 0)
					return item;

			for (SizeT i = 0; i < TSize; i++)
				if (lambda(this->array[i]))
					return this->array[i];

				return item;
				});
		}

		template<typename TLambda>
		T LastOrImpl(TLambda&& lambda, const T& item) const {
			return this->lock.SharedLock([&]() InlineLambda{
				if (TSize == 0)
					return item;

			SizeT Index = -1;
			for (SizeT i = 0; i < TSize; i++)
				if (lambda(this->array[i]))
					Index = i;

			if (Index != -1)
				return this->array[Index];

				return item;
				});
		}

		template<typename TLambda, typename TGetterLambda>
		T FirstOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			return this->lock.SharedLock([&]() InlineLambda{
				if (TSize == 0)
					return otherLambda();

				for (SizeT i = 0; i < TSize; i++)
					if (lambda(this->array[i]))
						return this->array[i];

				return otherLambda();
				});
		}

		template<typename TLambda, typename TGetterLambda>
		T LastOrImpl(TLambda&& lambda, TLambda&& otherLambda) const {
			return this->lock.SharedLock([&]() InlineLambda{
				if (TSize == 0)
					return otherLambda();

			SizeT Index = -1;
			for (SizeT i = 0; i < TSize; i++)
				if (lambda(this->array[i]))
					Index = i;

			if (Index != -1)
				return this->array[Index];

				return otherLambda();
				});
		}

		BaseConcurencyFixedArray& ReverseImpl() {
			return this->lock.ExclusiveLock([&]()->BaseConcurencyFixedArray & InlineLambda{
				for (SizeT i = 0; i < TSize / 2; i++)
 {
Cardinal::Swap(static_cast<T&>(this->array[i]), static_cast<T&>(this->array[(TSize - 1) - i]));
}
return *this;
				});
		}

		ForceInline void UnsafeSet(const T& Value, SizeT Index) {
			this->array[Index] = Value;
		}

		ForceInline void UnsafeSet(T&& Value, SizeT Index) {
			this->array[Index] = MoveRValue(Value);
		}

		BaseConcurencyFixedArray& RemoveAllImpl() {
			return this->lock.ExclusiveLock([&]()->BaseConcurencyFixedArray & InlineLambda{
				for (SizeT i = 0; i < TSize; i++)
 {
this->array[i].~T();
}
this->count = 0;
return *this;
				});
		}

		template<typename TLambda>
		BaseConcurencyFixedArray& RemoveAllImpl(TLambda&& lambda) {
			return this->lock.ExclusiveLock([&]()->BaseConcurencyFixedArray & InlineLambda{
				for (SizeT i = 0; i < TSize; i++)
 {
lambda(MoveRValue(this->array[i]));
}

return *this;
				});
		}

	public:
		/// <summary>
		/// An empty constructor (initializes an array with standard value)
		/// </summary>
		ForceInline constexpr BaseConcurencyFixedArray() : lock() {}

		/// <summary>
		/// A constructor with initialization list (copies all elements in list. 
		/// If there are more then only those who fit in size of array, 
		/// if less then other initializes by standard values)
		/// </summary>
		/// <param name="list">List of elements</param>
		constexpr BaseConcurencyFixedArray(InitilizerList<T>&& list) : lock() {
			if (list.size() != 0)
			{
				const SizeT actuallyCopyied = (TSize < list.size()) ? TSize : list.size();
				auto it = list.begin();
				for (SizeT i = 0; i < actuallyCopyied; i++)
				{
					this->array[i] = MoveRValue(*it); ++it;
				}
			}
		}

		/// <summary>
		/// A copying fixed array constructor
		/// </summary>
		/// <param name="arr">A fixed array</param>
		constexpr BaseConcurencyFixedArray(const T(&arr)[TSize]) : lock() {
			if (TypeTraits::IsCompileTimeEvaluating())
			{
				for (SizeT i = 0; i < TSize; i++)
				{
					array[i] = arr[i];
				}
			}
			else if constexpr (!TypeTraits::IsPodV<T>)
			{
				for (SizeT i = 0; i < TSize; i++)
				{
					array[i] = arr[i];
				}
			}
			else
			{
				Memory::Copy(arr, this->array);
			}
		}

		/// <summary>
		/// A fixed array constructor which is copying from start index to an end
		/// </summary>
		/// <param name="arr">A fixed array</param>
		/// <param name="startPosition">A start index in fixed array</param>
		template<SizeT USize>
		constexpr BaseConcurencyFixedArray(const T(&arr)[USize], SizeT startPosition) : lock() {
			for (SizeT i = 0; i < ((TSize < USize) ? TSize : USize); i++)
			{
				this->array[i] = arr[i + startPosition];
			}
		}

		/// <summary>
		/// Copying constructor from original array
		/// </summary>
		/// <param name="original">Original array</param>
		constexpr BaseConcurencyFixedArray(const BaseConcurencyFixedArray& original) : lock() {
			original.lock.SharedLock([&] () {
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
				});
		}

		/// <summary>
		/// Moving constructor from original array
		/// </summary>
		/// <param name="original">Original array</param>
		constexpr BaseConcurencyFixedArray(BaseConcurencyFixedArray&& original) : lock() {
			original.lock.ExclusiveLock([&] () {
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
						array[i] = MoveRValue(original.array[i]);
					}
				}
				else
				{
					Memory::Copy(original.array, this->array);
				}
				});
		}

	public:
		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"LValue" reference to array</returns>
		constexpr BaseConcurencyFixedArray& operator=(const BaseConcurencyFixedArray& original) {
			this->~BaseConcurencyFixedArray();
			this->BaseConcurencyFixedArray::BaseConcurencyFixedArray(original);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"RValue" reference to array</returns>
		constexpr BaseConcurencyFixedArray& operator=(BaseConcurencyFixedArray&& original) {
			this->~BaseConcurencyFixedArray();
			this->BaseConcurencyFixedArray::BaseConcurencyFixedArray(MoveRValue(original));
			return *this;
		}

		/// <summary>
		/// An access operator to array elements
		/// </summary>
		/// <param name="Index">Index of element</param>
		/// <returns>A references on elements</returns>
		ForceInline constexpr T& operator[](SizeT Index) {
			return this->lock.SharedLock([&]()->T & InlineLambda{
				if (Index < TSize)
				{
					return this->array[Index];
				}
				else
				{
					Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, Index, TSize);
				}
				});
		}

		/// <summary>
		/// A constant access operator to array elements
		/// </summary>
		/// <param name="Index">Index of element</param>
		/// <returns>A constant references on elements</returns>
		ForceInline constexpr const T& operator[](SizeT Index) const {
			return this->lock.SharedLock([&]() -> const T & InlineLambda{
				if (Index < TSize)
				{
					return this->array[Index];
				}
				else
				{
					Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, Index, TSize);
				}
				});
		}

		/// <summary>
		/// Array equality operator, returns "true" if all elements are equal
		/// </summary>
		/// <param name="other">Right operand</param>
		/// <returns>"True" if all elements of two arrays are equal</returns>
		constexpr bool operator==(const BaseConcurencyFixedArray& other) const {
			return this->lock.SharedLock([&]() -> const T & InlineLambda{
				for (SizeT i = 0; i < TSize; i++)
				{
					if (other.array[i] != this->array[i])
						return false;
				}
				return true;
				});
		}

		/// <summary>
		/// Array inequality operator, returns "false" if all elements are not equal
		/// </summary>
		/// <param name="other">Right operand</param>
		/// <returns>"False" if all elements of two arrays are not equal</returns>
		constexpr bool operator!=(const BaseConcurencyFixedArray& other) const {
			return this->lock.SharedLock([&]() -> const T & InlineLambda{
				for (SizeT i = 0; i < TSize; i++)
				{
					if (other.array[i] != this->array[i])
						return true;
				}
				return false;
				});
		}

		/// <summary>
		/// Array equality operator, returns "true" if all elements are equal
		/// </summary>
		/// <param name="other">Right operand</param>
		/// <returns>"True" if all elements of two arrays are equal</returns>
		constexpr bool operator==(const RawArray& other) const {
			return this->lock.SharedLock([&]() -> const T & InlineLambda{
				for (SizeT i = 0; i < TSize; i++)
				{
					if (other[i] != this->array[i])
						return false;
				}
				return true;
				});
		}

		/// <summary>
		/// Array equality operator, returns "true" if all elements are equal
		/// </summary>
		/// <param name="left">Left operand</param>
		/// <param name="right">Right operand</param>
		/// <returns>"True" if all elements of two arrays are equal</returns>
		constexpr friend bool operator==(const RawArray& left, const BaseConcurencyFixedArray& right) {
			return this->lock.SharedLock([&]() -> const T & InlineLambda{
				for (SizeT i = 0; i < TSize; i++)
				{
					if (left[i] != right.array[i])
						return false;
				}
				return true;
				});
		}

		/// <summary>
		/// Array inequality operator, returns "false" if all elements are not equal
		/// </summary>
		/// <param name="other">Right operand</param>
		/// <returns>"False" if all elements of two arrays are not equal</returns>
		constexpr bool operator!=(const RawArray& other) const {
			return this->lock.SharedLock([&]() -> const T & InlineLambda{
				for (SizeT i = 0; i < TSize; i++)
				{
					if (other[i] != this->array[i])
						return true;
				}
				return false;
				});
		}

		/// <summary>
		/// Array inequality operator, returns "false" if all elements are not equal
		/// </summary>
		/// <param name="left">Left operand</param>
		/// <param name="right">Right operand</param>
		/// <returns>"False" if all elements of two arrays are not equal</returns>
		constexpr friend bool operator!=(const RawArray& left, const BaseConcurencyFixedArray& right) {
			return this->lock.SharedLock([&]() -> const T & InlineLambda{
				for (SizeT i = 0; i < TSize; i++)
				{
					if (left[i] != right.array[i])
						return true;
				}
				return false;
				});
		}

		/// <summary>
		/// Array object cloning
		/// </summary>
		/// <returns>A new object with copy of all elements</returns>
		template<typename = void> requires (Concepts::IsClonable<T>)
			constexpr BaseConcurencyFixedArray Clone() const {
			return this->lock.SharedLock([&]() InlineLambda{
				BaseConcurencyFixedArray result;

				if (TypeTraits::IsCompileTimeEvaluating())
				{
					for (SizeT i = 0; i < TSize; i++)
					{
						result.array[i] = this->array[i];
					}
				}
				else if constexpr (TypeTraits::IsPodV<T>)
 {
Memory::Copy(this->array, result.array);
}
else
{
for (SizeT i = 0; i < TSize; i++)
{
	result.array[i] = Clone(this->array[i]);
}
}
return result;
				});
		}

		/// <summary>
		/// Array object cloning using custom lambda or functor
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">A function with which occurs cloning</param>
		/// <returns>A new object with copy of all elements</returns>
		template<typename TLambda> requires (Concepts::IsInvockableWithResult<TLambda, T, const T&>)
			constexpr BaseConcurencyFixedArray Clone(TLambda&& lambda) const {
			return this->lock.SharedLock([&]() InlineLambda{
				BaseConcurencyFixedArray result;

				for (SizeT i = 0; i < TSize; i++)
				{
					result.array[i] = lambda(this->array[i]);
				}

				return result;
				});
		}

		template<typename U>
		friend void Cardinal::Swap(U& A, U& B);

	public:
		operator const RawArray() const {
			return const_cast<const RawArray>(this->array);
		}
	};

	/// <summary>
	/// An array with fixed number of elements and multithreading support
	/// </summary>
	template<SizeT TSize, typename T>
	using ConcurencyFixedArray = BaseConcurencyFixedArray<Cardinal::Memory::Heap, TSize, T>;

	/// <summary>
	/// An array with fixed number of elements and multithreading support in protected heap
	/// </summary>
	template<SizeT TSize, typename T>
	using ConcurencySensitiveFixedArray = BaseConcurencyFixedArray<Cardinal::Memory::SensitiveDataHeap, TSize, T>;
}

export namespace Cardinal::TypeTraits::Collection
{
	template<typename THeap, SizeT TSize, typename T>
	struct IsFixedArrayT<Cardinal::Containers::BaseConcurencyFixedArray<THeap, TSize, T>> : LogicTrue {};
}

export namespace Cardinal
{
	template<typename THeap, SizeT TSize, typename T>
	ForceInline void Swap(Containers::BaseConcurencyFixedArray<THeap, TSize, T>& a, Containers::BaseConcurencyFixedArray<THeap, TSize, T>& b) {
		a.lock.ExclusiveLock([&] () {
			b.lock.ExclusiveLock([&] () {
				for (SizeT i = 0; i < TSize; i++)
				{
					Swap(a.array[i], b.array[i]);
				}
				});
			});
	}
}

#pragma warning(pop)