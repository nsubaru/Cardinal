export module Cardinal.Containers.NonConcurency.Array;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;
export import Cardinal.Containers.InitilizerList;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
#pragma warning(disable: 4172)
#pragma warning(disable: 6386)
#pragma warning(disable: 4996)

export namespace Cardinal::Containers
{
	/// <summary>
	/// An array
	/// </summary>
	template<typename THeap, typename T>
	class BaseArray final :
		public Memory::Allocator<THeap>,
		public LINQ::IArray<BaseArray, THeap, T, false> {
	private:
		SizeT count;
		T* Restrict array;
		
	private:
		template<typename Lambda>
		void QuickSort(T* A, SizeT p, SizeT r, Lambda& lambda) {
			if (p < r) {
				SizeT q = Partition(A, p, r, lambda);
				QuickSort(A, p, q, lambda);
				QuickSort(A, q + 1, r, lambda);
			}
		}

		template<typename Lambda>
		SizeT Partition(T* A, SizeT p, SizeT r, Lambda& lambda) {
			T x = A[p];
			SizeT i = p;
			SizeT j = r;

			while (true) {
				while (lambda(A[i], x) && i != r) {
					i++;
				}

				while (!lambda(A[j], x) && j != p) {
					j--;
				}
				if (i < j) {
					Swap(A[i], A[j]);
				}
				else {
					return j;
				}
			}
		}

	protected:
		template<template<typename, typename> typename, typename, typename, Boolean, Boolean, Boolean, Boolean>
		friend class LINQ::ICollection;

		template<template<typename, typename> typename, typename, typename, Boolean>
		friend class LINQ::IArray;

		template<template<typename, SizeT, typename> typename TCollection, typename THeap, typename T, SizeT TSize, Boolean TIsConcurencySuport>
		friend class LINQ::IFixedCollectionBase;

		template<template<typename THeap, typename T>typename TList, typename THeap, typename T, bool TIsDynamic, bool TIsConcurencySuport>
		friend class LINQ::IOrderable;

		template<typename U>
		friend struct Cardinal::Serialization::SerializationHandler;

		template<template<typename THeap, typename TKey, typename TValue> typename TCollection, typename THeap, typename TKey, typename TValue, typename TPair, bool IsConcurencySuportT>
		friend class LINQ::IAssociableCollection;

		template<template<typename, SizeT, typename> typename TCollection, typename THeap, typename T, SizeT TSize, Boolean TIsConcurencySuport>
		friend class LINQ::IFixedCollection;

	protected:
		template<typename TLambda>
		ForceInline BaseArray& ForEachImpl(TLambda&& lambda) {
			if constexpr (TypeTraits::IsInvockable<TLambda, T&> ||
							TypeTraits::IsInvockable<TLambda, const T&>)
			{
				for (SizeT i = 0; i < count; i++)
				{
					lambda(array[i]);
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, T&, SizeT> ||
								TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				for (SizeT i = 0; i < count; i++)
				{
					lambda(array[i], i);
				}
				return *this;
			}
			else
			{
				static_assert(
					TypeTraits::IsInvockable<TLambda, T&> ||
					TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, T&, SizeT> ||
					TypeTraits::IsInvockable<TLambda, const T&, SizeT>
					);
			}
		}

		template<typename TLambda>
		ForceInline const BaseArray& ForEachImpl(TLambda&& lambda) const {
			if constexpr (TypeTraits::IsInvockable<TLambda, const T&>) 
			{
				for (SizeT i = 0; i < count; i++)
				{
					lambda(array[i]);
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT>) 
			{
				for (SizeT i = 0; i < count; i++)
				{
					lambda(array[i], i);
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
		ForceInline BaseArray& ForEachImpl(TLambda&& lambda, SizeT reqCount) {
			if (reqCount > this->Count)
				reqCount = this->Count;

			if constexpr (TypeTraits::IsInvockable<TLambda, T&> ||
							TypeTraits::IsInvockable<TLambda, const T&>)
			{
				for (SizeT i = 0; i < reqCount; i++)
				{
					lambda(array[i]);
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, T&, SizeT> ||
								TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
			{
				for (SizeT i = 0; i < reqCount; i++)
				{
					lambda(array[i], i);
				}
				return *this;
			}
			else
			{
				static_assert(
					TypeTraits::IsInvockable<TLambda, T&> ||
					TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, T&, SizeT> ||
					TypeTraits::IsInvockable<TLambda, const T&, SizeT>
					);
			}
		}

		template<typename TLambda>
		ForceInline const BaseArray& ForEachImpl(TLambda&& lambda, SizeT reqCount) const {
			if (reqCount > this->Count)
				reqCount = this->Count;

			if constexpr (TypeTraits::IsInvockable<TLambda, const T&>) 
			{
				for (SizeT i = 0; i < reqCount; i++)
				{
					lambda(array[i]);
				}
				return *this;
			}
			else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT>) 
			{
				for (SizeT i = 0; i < reqCount; i++)
				{
					lambda(array[i], i);
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
		ForceInline BaseArray& ForEachImpl(TLambda&& lambda, SizeT startIndex, SizeT reqCount) {
            if (startIndex > this->count)
                Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, startIndex, this->count);

			if (reqCount == Math::Max<SizeT>())
				reqCount = this->count - startIndex;

            if (startIndex + reqCount > this->count)
				reqCount = this->count - startIndex;

            if constexpr (TypeTraits::IsInvockable<TLambda, T&> ||
							TypeTraits::IsInvockable<TLambda, const T&>) 
			{
                for (SizeT i = 0; i < reqCount; i++)
                {
                    lambda(array[startIndex + i]);
                }
                return *this;
            }
            else if constexpr (TypeTraits::IsInvockable<TLambda, T&, SizeT> ||
								TypeTraits::IsInvockable<TLambda, const T&, SizeT>) 
			{
                for (SizeT i = 0; i < reqCount; i++)
                {
                    lambda(array[startIndex + i], startIndex + i);
                }
                return *this;
            }
            else
			{
				static_assert(
					TypeTraits::IsInvockable<TLambda, T&> ||
					TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, T&, SizeT> ||
					TypeTraits::IsInvockable<TLambda, const T&, SizeT>
					);
            }
        }

        template<typename TLambda>
		ForceInline const BaseArray& ForEachImpl(TLambda&& lambda, SizeT startIndex, SizeT reqCount) const {
            if (startIndex > this->count)
                Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, startIndex, this->count);

			if (reqCount == Math::Max<SizeT>())
				reqCount = this->count - startIndex;

            if (startIndex + reqCount > this->count)
				reqCount = this->count - startIndex;

            if constexpr (TypeTraits::IsInvockable<TLambda, const T&>) 
			{
                for (SizeT i = 0; i < reqCount; i++)
                {
                    lambda(array[startIndex + i]);
                }
                return *this;
            }
            else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT>) 
			{
                for (SizeT i = 0; i < reqCount; i++)
                {
                    lambda(array[startIndex + i], startIndex + i);
                }
                return *this;
            }
            else {
				static_assert(TypeTraits::IsInvockable<TLambda, const T&> ||
					TypeTraits::IsInvockable<TLambda, const T&, SizeT>);
            }
        }
		
	protected:
		template<typename U, typename TLambda>
		ForceInline U AggregateImpl(TLambda&& lambda, const U& value) const {
			U result = value;
			U& data = result;
			for (SizeT i = 0; i < count; i++)
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
			for (SizeT i = 0; i < count; i++)
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

		ForceInline bool AnyImpl() const { return count > 0; }

		template<typename TLambda>
		ForceInline bool AnyImpl(TLambda&& lambda) const {
			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(array[i]))
					return true;
			}
			return false;
		}

		template<typename TLambda>
		ForceInline bool AllImpl(TLambda&& lambda) const {
			for (SizeT i = 0; i < count; i++)
			{
				if (!lambda(array[i]))
					return false;
			}
			return true;
		}

		ForceInline BaseArray& SortByImpl(bool IsDescending = true) {
			if (IsDescending) 
			{
				auto l = [](const T& V1, const T& V2) { return V1 < V2; };
				QuickSort(array, 0, count - 1, l);
			}
			else 
			{
				auto l = [](const T& V1, const T& V2) { return V1 > V2; };
				QuickSort(array, 0, count - 1, l);
			}
			return *this;
		}

		template<typename TLambda>
		ForceInline BaseArray& SortByLambdaImpl(TLambda&& lambda) {
			if (count == 0)
				return *this;

			QuickSort(array, 0, count - 1, lambda);
			return *this;
		}

		template<typename TLambda>
		ForceInline T& FirstImpl(TLambda&& lambda) {
			for (SizeT i = 0; i < count; i++)
				if (lambda(array[i]))
					return static_cast<T&>(array[i]);

			Internals::Exceptions::ThrowNotFound(SourcePosition);
		}

		template<typename TLambda>
		ForceInline T& LastImpl(TLambda&& lambda) {
			SizeT Index = -1;
			for (SizeT i = 0; i < count; i++)
				if (lambda(array[i]))
					Index = i;

			if (Index != -1)
				return static_cast<T&>(array[Index]);

			Internals::Exceptions::ThrowNotFound(SourcePosition);
		}

		template<typename TLambda>
		ForceInline const T& FirstImpl(TLambda&& lambda) const {
			for (SizeT i = 0; i < count; i++)
				if (lambda(array[i]))
					return static_cast<T&>(array[i]);

			Internals::Exceptions::ThrowNotFound(SourcePosition);
		}

		template<typename TLambda>
		ForceInline const T& LastImpl(TLambda&& lambda) const {
			SizeT Index = -1;
			for (SizeT i = 0; i < count; i++)
				if (lambda(array[i]))
					Index = i;

			if (Index != -1)
				return static_cast<T&>(array[Index]);

			Internals::Exceptions::ThrowNotFound(SourcePosition);
		}

		template<typename TLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, T&& item) const {
			if (count == 0)
				return MoveRValue(item);

			for (SizeT i = 0; i < count; i++)
				if (lambda(array[i]))
					return array[i];

			return MoveRValue(item);
		}

		template<typename TLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, T&& item) const {
			if (count == 0)
				return MoveRValue(item);

			SizeT Index = -1;
			for (SizeT i = 0; i < count; i++)
				if (lambda(array[i]))
					Index = i;

			if (Index != -1)
				return array[Index];

			return MoveRValue(item);
		}

		template<typename TLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, const T& item) const {
			if (count == 0)
				return item;

			for (SizeT i = 0; i < count; i++)
				if (lambda(array[i]))
					return array[i];

			return item;
		}

		template<typename TLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, const T& item) const {
			if (count == 0)
				return item;

			SizeT Index = -1;
			for (SizeT i = 0; i < count; i++)
				if (lambda(array[i]))
					Index = i;

			if (Index != -1)
				return array[Index];

			return item;
		}

		template<typename TLambda, typename TGetterLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			if (count == 0)
				return otherLambda();

			for (SizeT i = 0; i < count; i++)
				if (lambda(array[i]))
					return array[i];

			return otherLambda();
		}

		template<typename TLambda, typename TGetterLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			if (count == 0)
				return otherLambda();

			SizeT Index = -1;
			for (SizeT i = 0; i < count; i++)
				if (lambda(array[i]))
					Index = i;

			if (Index != -1)
				return array[Index];

			return otherLambda();
		}

		ForceInline BaseArray& ReverseImpl() {
			for (SizeT i = 0; i < count / 2; i++) {
				Swap(array[i], array[(count - 1) - i]);
			}
			return *this;
		}

		ForceInline void UnsafeSet(const T& Value, SizeT Index) {
			array[Index] = Value;
		}

		ForceInline void UnsafeSet(T&& Value, SizeT Index) {
			array[Index] = MoveRValue(Value);
		}

		ForceInline BaseArray& RemoveAllImpl() {
			delete[] this->array;
			this->array = nullptr;
			this->count = 0;
			return *this;
		}

		template<typename TLambda>
		ForceInline BaseArray& RemoveAllImpl(TLambda&& lambda) {
			if constexpr (Concepts::IsInvockable<TLambda, T&&>)
			{
				for (SizeT i = 0; i < count; i++)
				{
					lambda(MoveRValue(array[i]));
				}
				delete[] this->array;
				this->array = nullptr;
				this->count = 0;
				return *this;
			}
			else
			{
				for (SizeT i = 0; i < count; i++)
				{
					lambda(MoveRValue(array[i]), i);
				}
				delete[] this->array;
				this->array = nullptr;
				this->count = 0;
				return *this;
			}
		}

		template<typename TLambda>
		SizeT GetCountIfImpl(TLambda&& lambda) const {
			SizeT selectedCount = 0;
			for (SizeT i = 0; i < count; i++)
			{
				if (lambda(array[i]))
					selectedCount++;
			}
			return selectedCount;
		}
		
	public:
		/// <summary>
        /// Zero length array constructor
        /// </summary>
		ForceInline BaseArray() : count(0), array(nullptr) {}

		/// <summary>
        /// List of elements constructor
        /// </summary>
        /// <param name="list">List of elements</param>
		BaseArray(InitilizerList<T>&& list) : count(list.size()), array(nullptr) {
			if (list.size() != 0) {
				this->array = new T[list.size()];
				auto it = list.begin();
				for (SizeT i = 0; i < count; i++)
				{
					array[i] = MoveRValue(*it); it++;
				}
			}
		}

		/// <summary>
		/// Copying constructor
		/// </summary>
		/// <param name="original">Original</param>
		BaseArray(const BaseArray& original) : count(original.count), array(nullptr) {
			if (count != 0)
			{
				this->array = new T[count];
				if constexpr (!TypeTraits::IsPodV<T>)
				{
					for (SizeT i = 0; i < count; i++)
					{
						array[i] = original.array[i];
					}
				}
				else
				{
					Memory::Copy(original.array, this->array, sizeof(T) * count);
				}
			}
		}

		/// <summary>
		/// Moving constructor
		/// </summary>
		/// <param name="original">Original</param>
		BaseArray(BaseArray&& original) : 
			array(MoveRValue(original.array)), 
			count(MoveRValue(original.count)) 
		{
			original.array = nullptr;
			original.count = 0;
		}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"LValue" reference to array</returns>
		BaseArray& operator=(const BaseArray& original) {
			this->~BaseArray();
			this->BaseArray::BaseArray(original);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="original">Original</param>
		/// <returns>"RValue" reference to array</returns>
		BaseArray& operator=(BaseArray&& original) {
			this->~BaseArray();
			this->BaseArray::BaseArray(MoveRValue(original));
			return *this;
		}

		/// <summary>
        /// Number of elements constructor
        /// </summary>
        /// <param name="startCount">Container size</param>
		ForceInline BaseArray(SizeT startCount) : count(0), array(nullptr) {
			if (startCount != 0) {
				array = new T[startCount];
				count = startCount;
			}
		}

		/// <summary>
        /// The internal constructor with C-array and number of elements
        /// </summary>
        /// <param name="array">A pointer to an array of elements</param>
        /// <param name="startCount">Number of elements</param>
		InternalApiCall BaseArray(const T*& array, SizeT startCount) : count(0), array(nullptr) {
			if (startCount != 0) {
				this->array = new T[startCount];
				count = startCount;

				if constexpr (TypeTraits::IsPodV<T>) {
					Memory::Copy(array, this->array, startCount * sizeof(T));
				}
				else
				{
					for (SizeT i = 0; i < startCount; i++)
					{
						this->array[i] = array[i];
					}
				}
			}
		}

		/// <summary>
        /// The internal constructor with "RValue" reference to C-array and number of elements
        /// </summary>
        /// <param name="array">"RValue" reference to C-array</param>
        /// <param name="startCount">Number of elements</param>
		InternalApiCall BaseArray(T*&& array, SizeT startCount) : 
			count(startCount), 
			array(MoveRValue(array)) 
		{ 
			array = nullptr; 
		}

		/// <summary>
        /// The raw reference gaining operator on elements of array
        /// </summary>
        /// <returns>A reference to elements of array</returns>
		ForceInline InternalApiCall operator const T* const() const { return array; }
		
	public:
		/// <summary>
        /// An access operator to array elements
        /// </summary>
        /// <param name="Index">Element number</param>
        /// <returns>A reference to element</returns>
		ForceInline T& operator[](SizeT Index) & {
			if (Index < count)
            {
                return array[Index];
            }
            else
            {
                Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, Index, count);
            }
		}

		/// <summary>
        /// A constant access operator to array elements
        /// </summary>
        /// <param name="Index">Element number</param>
        /// <returns>A constant reference to element</returns>
		ForceInline const T& operator[](SizeT Index) const & {
			if (Index < count)
            {
                return array[Index];
            }
            else
            {
                Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, Index, count);
            }
		}

		/// <summary>
		/// A constant access operator to array elements
		/// </summary>
		/// <param name="Index">Element number</param>
		/// <returns>A constant reference to element</returns>
		ForceInline T&& operator[](SizeT Index) && {
			if (Index < count)
			{
				return MoveRValue(array[Index]);
			}
			else
			{
				Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, Index, count);
			}
		}
		
		/// <summary>
        /// Resize
        /// </summary>
        /// <param name="NewSize">New array size</param>
        /// <returns>A reference to a current object</returns>
		BaseArray& Resize(SizeT NewSize) {
			if (this->array == nullptr || this->count == 0)
			{
				this->array = new T[NewSize];
				this->count = NewSize;
			}
			else 
			{
				if (NewSize == 0) {
					delete[] array;
					count = 0;
				}
				else 
				{
					T* buf = new T[NewSize];
					SizeT CopySize = (NewSize > count) ? count : NewSize;
					if constexpr (TypeTraits::IsPodV<T>) {
						Memory::Copy(array, buf, CopySize * sizeof(T));
					}
					else 
					{
						for (SizeT i = 0; i < CopySize; i++)
						{
							buf[i] = MoveRValue(array[i]);
						}
					}
					count = NewSize;
					delete[] array;
					array = buf;
				}
			}
			return *this;
		}

		/// <summary>
        /// Array cloning
        /// </summary>
        /// <returns>Returns new array with copies of all elements</returns>
        template<typename = void> requires (Concepts::IsClonable<T>) 
        BaseArray Clone() const {
			if constexpr (TypeTraits::IsPodV<T>)
            {
                return BaseArray((const T*&)(array), count);
            }
            else
            {
                auto newArray = BaseArray((const T*&)(this->array), count);
                for (SizeT i = 0; i < count; i++)
                {
					newArray.array[i] = this->array[i].Clone();
                }
                return newArray;
            }
        }

		/// <summary>
		/// Array cloning using custom lambda or functor
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">A function with which occurs cloning</param>
		/// <returns>A lambda or functor which clones all elements</returns>
		template<typename TLambda> requires (Concepts::IsInvockableWithResult<TLambda, T, const T&>)
        BaseArray Clone(TLambda&& lambda) const {
            T* newArray = nullptr;
            try
            {
				newArray = new T[count];
                for (SizeT i = 0ui64; i < count; i++)
                {
					newArray[i] = lambda(newArray[i]);
                }

                return BaseArray(MoveRValue(newArray), count);
            }
            catch (...)
            {
                delete[] newArray;
				Internals::Exceptions::__ReThrow();
            }
        }

		/// <summary>
		/// Array equality operator
		/// </summary>
		/// <param name="other">Second operand</param>
		/// <returns>"True" if "other" is a reference to "this" or the number of elements are equal and elements are also equal</returns>
		bool operator==(const BaseArray& other) const requires Cardinal::Concepts::HaveEqual<T, T> {
			if (other.array == this->array)
				return true;

			if (other.count != this->count)
				return false;

			for (SizeT i = 0; i < this->count; i++)
			{
				if (!(other.array[i] == this->array[i]))
					return false;
			}

			return true;
		}

		/// <summary>
		/// Array inequality operator
		/// </summary>
		/// <param name="other">Second operand</param>
		/// <returns>"False" if "other" is a reference to "this" or the number of elements are equal and elements are also equal</returns>
		bool operator!=(const BaseArray& other) const requires Cardinal::Concepts::HaveEqual<T, T> {
			return !(*this == other);
		}

		/// Optimized swap
		/// </summary>
		/// <param name = 'A'>First element</param>
		/// <param name = 'B'>Second element</param>
		template<typename U>
		friend void Cardinal::Swap(U& A, U& B);
		
	public:
		/// <summary>
        /// Returns current number of elements
        /// </summary>
        /// <returns>A current number of elements</returns>
		ForceInline SizeT GetCount() const { return count; }

		/// <summary>
        /// A number of elements
        /// </summary>
		SizeT Property(GetCount) Count;

		/// <summary>
        /// Returns raw pointer to elements of array
        /// </summary>
        /// <returns>Raw pointer on the elements of array</returns>
		ForceInline const T* const GetRaw() const { return array; }
		
	public:
		~BaseArray() {
			delete[] array;
			array = nullptr;
			count = 0;
		}
	};

	/// <summary>
    /// An array in standard heap
    /// </summary>
	template<typename T>
	using Array = BaseArray<Cardinal::Memory::Heap, T>;

	// <summary>
    /// An array in protected heap
    /// </summary>
	template<typename T>
	using SensitiveArray = BaseArray<Cardinal::Memory::SensitiveDataHeap, T>;

	/// <summary>
	/// An array of bytes in standard heap
	/// </summary>
	using Blob = BaseArray<Cardinal::Memory::Heap, Byte>;

	/// <summary>
	/// An array in protected heap
	/// </summary>
	using SensitiveBlob = BaseArray<Cardinal::Memory::SensitiveDataHeap, Byte>;

	template<typename T> requires Concepts::IsPod<T>
	Blob AsBlob(const T& value) {
		union {
			const Byte* bytes[sizeof(T)];
			const T* valueRef;
		};

		valueRef = &value;

		return Blob((const Byte*&)(bytes), sizeof(T));
	}
}

export namespace Cardinal::TypeTraits::Collection
{
	template<typename THeap, typename T>
	struct IsArrayT<Cardinal::Containers::BaseArray<THeap, T>> : LogicTrue {};
}

export namespace Cardinal
{
	template<typename THeap, typename T>
	ForceInline void Swap(Containers::BaseArray<THeap, T>& A, Containers::BaseArray<THeap, T> B) {
		Swap(A.This, B.This);
	}
}

#pragma warning(pop)