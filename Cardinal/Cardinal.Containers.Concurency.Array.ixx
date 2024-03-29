export module Cardinal.Containers.Concurency.Array;

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
#pragma warning(disable: 4172)

export namespace Cardinal::Containers
{
    /// <summary>
    /// An array which supports multithreading based on "SRWLock"
    /// </summary>
    template<typename THeap, typename T>
    class BaseConcurencyArray final :
        public Memory::Allocator<THeap>,
        public LINQ::IArray<BaseConcurencyArray, THeap, T, true> {
    private:
        SizeT count;
        T* Restrict array;
        Threading::SRWLock mutable lock;

    private:
        template<typename Lambda>
        void QuickSort(T* a, SizeT p, SizeT r, Lambda& lambda) {
            if (p < r)
            {
                SizeT q = Partition(a, p, r, lambda);
                QuickSort(a, p, q, lambda);
                QuickSort(a, q + 1, r, lambda);
            }
        }

        template<typename Lambda>
        SizeT Partition(T* a, SizeT p, SizeT r, Lambda& lambda) {
            T x = a[p];
            SizeT i = p;
            SizeT j = r;
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
                    Cardinal::Swap(a[i], a[j]);
                }
                else
                {
                    return j;
                }
            }
        }

    protected:
        template<template<typename, typename> typename, typename, typename, Boolean, Boolean, Boolean, Boolean>
        friend class LINQ::ICollection;

        template<template<typename, typename> typename, typename, typename, Boolean>
        friend class LINQ::IArray;

        template<template<typename, SizeT, typename> typename TCollection, typename UHeap, typename U, SizeT TSize, Boolean TIsConcurencySuport>
        friend class LINQ::IFixedCollectionBase;

        template<template<typename UHeap, typename U>typename TList, typename UHeap, typename U, bool TIsDynamic, bool TIsConcurencySuport>
        friend class LINQ::IOrderable;

        template<template<typename, SizeT, typename> typename TCollection, typename THeap, typename T, SizeT TSize, Boolean TIsConcurencySuport>
        friend class LINQ::IFixedCollection;

        template<typename U>
        friend struct Serialization::SerializationHandler;

    protected:
        template<typename TLambda>
        BaseConcurencyArray& ForEachImpl(TLambda&& lambda) {
            if constexpr (TypeTraits::IsInvockable<TLambda, T&> ||
                TypeTraits::IsInvockable<TLambda, const T&>)
            {
                return lock.SharedLock([&]()->BaseConcurencyArray & InlineLambda{
                    for (SizeT i = 0; i < count; i++)
                    {
                        lambda(array[i]);
                    }
                    return *this;
                    });
            }
            else if constexpr (TypeTraits::IsInvockable<TLambda, T&, SizeT> ||
                TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
            {
                return lock.SharedLock([&]()->BaseConcurencyArray & InlineLambda{
                    for (SizeT i = 0; i < count; i++)
                    {
                        lambda(array[i], i);
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
        const BaseConcurencyArray& ForEachImpl(TLambda&& lambda) const {
            if constexpr (TypeTraits::IsInvockable<TLambda, const T&>)
            {
                return this->lock.SharedLock([&]() -> const BaseConcurencyArray & InlineLambda{
                    for (SizeT i = 0; i < count; i++)
                    {
                        lambda(array[i]);
                    }
                    return *this;
                    });
            }
            else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
            {
                return this->lock.SharedLock([&]() -> const BaseConcurencyArray & InlineLambda{
                    for (SizeT i = 0; i < count; i++)
                    {
                        lambda(array[i], i);
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
        BaseConcurencyArray& ForEachImpl(TLambda&& lambda, SizeT reqCount) {
            if (reqCount > this->Count)
                reqCount = this->Count;

            if constexpr (TypeTraits::IsInvockable<TLambda, T&> ||
                TypeTraits::IsInvockable<TLambda, const T&>)
            {
                return lock.SharedLock([&] () -> BaseConcurencyArray& {
                    for (SizeT i = 0; i < reqCount; i++)
                    {
                        lambda(array[i]);
                    }
                    return *this;
                    });
            }
            else if constexpr (TypeTraits::IsInvockable<TLambda, T&, SizeT> ||
                TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
            {
                return lock.SharedLock([&]()->BaseConcurencyArray & InlineLambda{
                    for (SizeT i = 0; i < reqCount; i++)
                    {
                        lambda(array[i], i);
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
        const BaseConcurencyArray& ForEachImpl(TLambda&& lambda, SizeT reqCount) const {
            if (reqCount > this->Count)
                reqCount = this->Count;

            if constexpr (TypeTraits::IsInvockable<TLambda, const T&>)
            {
                return this->lock.SharedLock([&]() -> const BaseConcurencyArray & InlineLambda{
                    for (SizeT i = 0; i < reqCount; i++)
                    {
                        lambda(array[i]);
                    }
                    return *this;
                    });
            }
            else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
            {
                return this->lock.SharedLock([&]() -> const BaseConcurencyArray & InlineLambda{
                    for (SizeT i = 0; i < reqCount; i++)
                    {
                        lambda(array[i], i);
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
        BaseConcurencyArray& ForEachImpl(TLambda&& lambda, SizeT startIndex, SizeT reqCount) {
            if (startIndex > this->Count)
                Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, startIndex, this->Count);

            if (reqCount == Math::Max<SizeT>())
                reqCount = this->count - startIndex;

            if (startIndex + reqCount > this->Count)
                reqCount = this->Count - startIndex;

            if constexpr (TypeTraits::IsInvockable<TLambda, T&> ||
                TypeTraits::IsInvockable<TLambda, const T&>)
            {
                return lock.SharedLock([&]()->BaseConcurencyArray & InlineLambda{
                    for (SizeT i = 0; i < reqCount; i++)
                    {
                        lambda(array[startIndex + i]);
                    }
                    return *this;
                    });
            }
            else if constexpr (TypeTraits::IsInvockable<TLambda, T&, SizeT> ||
                TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
            {
                return lock.SharedLock([&]()->BaseConcurencyArray & InlineLambda{
                    for (SizeT i = 0; i < reqCount; i++)
                    {
                        lambda(array[startIndex + i], startIndex + i);
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
        const BaseConcurencyArray& ForEachImpl(TLambda&& lambda, SizeT startIndex, SizeT reqCount) const {
            if (startIndex > this->Count)
                Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, startIndex, this->Count);

            if (reqCount == Math::Max<SizeT>())
                reqCount = this->count - startIndex;

            if (startIndex + reqCount > this->Count)
                reqCount = this->Count - startIndex;

            if constexpr (TypeTraits::IsInvockable<TLambda, const T&>)
            {
                return this->lock.SharedLock([&]() -> const BaseConcurencyArray & InlineLambda{
                    for (SizeT i = 0; i < reqCount; i++)
                    {
                        lambda(array[startIndex + i]);
                    }
                    return *this;
                    });
            }
            else if constexpr (TypeTraits::IsInvockable<TLambda, const T&, SizeT>)
            {
                return this->lock.SharedLock([&]() -> const BaseConcurencyArray & InlineLambda{
                    for (SizeT i = 0; i < reqCount; i++)
                    {
                        lambda(array[startIndex + i], startIndex + i);
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

    protected:
        template<typename U, typename TLambda>
        U AggregateImpl(TLambda&& lambda, const U& Value) const {
            return MoveRValue(this->lock.SharedLock([&]() InlineLambda{
                U result = Value;
                for (SizeT i = 0; i < count; i++)
                {
                    if constexpr (Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&> ||
                        Concepts::IsInvockableWithResult<TLambda, U, U&, const T&>)
                    {
                        result = lambda(result, array[i]);
                    }
                    else
                    {
                        lambda(result, array[i]);
                    }
                }
                return MoveRValue(result);
                }));
        }

        template<typename U, typename TLambda>
        U AggregateImpl(TLambda&& lambda, U&& result) const {
            return MoveRValue(this->lock.SharedLock([&]() InlineLambda{
                for (SizeT i = 0; i < count; i++)
                {
                    if constexpr (
                        Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&> ||
                        Concepts::IsInvockableWithResult<TLambda, U, U&, const T&>)
                    {
                        result = lambda(result, array[i]);
                    }
                    else
                    {
                        lambda(result, array[i]);
                    }
                }
                return MoveRValue(result);
                }));
        }

        ForceInline bool AnyImpl() const { return count > 0; }

        template<typename TLambda>
        bool AnyImpl(TLambda&& lambda) const {
            return MoveRValue(this->lock.SharedLock([&]() mutable InlineLambda{
                for (SizeT i = 0; i < count; i++)
                {
                    if (lambda(array[i]))
                        return true;
                }
                return false;
                }));
        }

        template<typename TLambda>
        bool AllImpl(TLambda&& lambda) const {
            return MoveRValue(this->lock.SharedLock([&]() InlineLambda{
                for (SizeT i = 0; i < count; i++)
                {
                    if (!lambda(array[i]))
                        return false;
                }
                return true;
                }));
        }

        BaseConcurencyArray& SortByImpl(bool IsDescending = true) {
            return lock.ExclusiveLock([&] () -> BaseConcurencyArray& {
                if (IsDescending)
                {
                    auto l = [](const T& V1, const T& V2) InlineLambda{ return V1 < V2; };
                    QuickSort(array, 0, count - 1, l);
                }
                else
                {
                    auto l = [](const T& V1, const T& V2) InlineLambda{ return V1 > V2; };
                    QuickSort(array, 0, count - 1, l);
                }
                return *this;
                });
        }

        template<typename TLambda>
        BaseConcurencyArray& SortByLambdaImpl(TLambda&& lambda) {
            return lock.ExclusiveLock([&]()->BaseConcurencyArray & InlineLambda
                {
                    if (count == 0)
                        return *this;

                    QuickSort(array, 0, count - 1, lambda);
                    return *this;
                });
        }

        template<typename TLambda>
        T& FirstImpl(TLambda&& lambda) {
            return lock.SharedLock([&]()->T & InlineLambda{
                for (SizeT i = 0; i < count; i++)
                    if (lambda(array[i]))
                        return static_cast<T&>(array[i]);

                Internals::Exceptions::ThrowNotFound(SourcePosition);
                });
        }

        template<typename TLambda>
        T& LastImpl(TLambda&& lambda) {
            return lock.SharedLock([&]()->T & InlineLambda{
                SizeT Index = -1;
                for (SizeT i = 0; i < count; i++)
                    if (lambda(array[i]))
                        Index = i;

                if (Index != -1)
                    return static_cast<T&>(array[Index]);

                Internals::Exceptions::ThrowNotFound(SourcePosition);
                });
        }

        template<typename TLambda>
        const T& FirstImpl(TLambda&& lambda) const {
            return lock.SharedLock([&]() -> const T & InlineLambda{
                for (SizeT i = 0; i < count; i++)
                    if (lambda(array[i]))
                        return static_cast<T&>(array[i]);

                Internals::Exceptions::ThrowNotFound(SourcePosition);
                });
        }

        template<typename TLambda>
        const T& LastImpl(TLambda&& lambda) const {
            return lock.SharedLock([&]() -> const T & InlineLambda{
                SizeT Index = -1;
                for (SizeT i = 0; i < count; i++)
                    if (lambda(array[i]))
                        Index = i;

                if (Index != -1)
                    return static_cast<T&>(array[Index]);

                Internals::Exceptions::ThrowNotFound(SourcePosition);
                });
        }

        template<typename TLambda>
        T FirstOrImpl(TLambda&& lambda, T&& item) const {
            return lock.SharedLock([&, item = MoveRValue(item)]() InlineLambda{
                if (count == 0)
                    return MoveRValue(item);

                for (SizeT i = 0; i < count; i++)
                    if (lambda(array[i]))
                        return array[i];

                return MoveRValue(item);
                });
        }

        template<typename TLambda>
        T LastOrImpl(TLambda&& lambda, T&& item) const {
            return lock.SharedLock([&, item = MoveRValue(item)]() InlineLambda{
                if (count == 0)
                    return MoveRValue(item);

                SizeT Index = -1;
                for (SizeT i = 0; i < count; i++)
                    if (lambda(array[i]))
                        Index = i;

                if (Index != -1)
                    return array[Index];

                return MoveRValue(item);
                });
        }

        template<typename TLambda>
        T FirstOrImpl(TLambda&& lambda, const T& item) const {
            return lock.SharedLock([&]() InlineLambda{
                if (count == 0)
                    return item;

                for (SizeT i = 0; i < count; i++)
                    if (lambda(array[i]))
                        return array[i];

                return item;
                });
        }

        template<typename TLambda>
        T LastOrImpl(TLambda&& lambda, const T& item) const {
            return lock.SharedLock([&]() InlineLambda{
                if (count == 0)
                    return item;

                SizeT Index = -1;
                for (SizeT i = 0; i < count; i++)
                    if (lambda(array[i]))
                        Index = i;

                if (Index != -1)
                    return array[Index];

                return item;
                });
        }

        template<typename TLambda, typename TGetterLambda>
        T FirstOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
            return lock.SharedLock([&]() InlineLambda{
                if (count == 0)
                    return otherLambda();

                for (SizeT i = 0; i < count; i++)
                    if (lambda(array[i]))
                        return array[i];

                return otherLambda();
                });
        }

        template<typename TLambda, typename TGetterLambda>
        T LastOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
            return lock.SharedLock([&]() InlineLambda{
                if (count == 0)
                    return otherLambda();

                SizeT Index = -1;
                for (SizeT i = 0; i < count; i++)
                    if (lambda(array[i]))
                        Index = i;

                if (Index != -1)
                    return array[Index];

                return otherLambda();
                });
        }

        BaseConcurencyArray& ReverseImpl() {
            return lock.ExclusiveLock([&]()->BaseConcurencyArray & InlineLambda{
                for (SizeT i = 0; i < count / 2; i++)
 {
Cardinal::Swap<T>(array[i], array[(count - 1) - i]);
}
return *this;
                });
        }

        ForceInline void UnsafeSet(const T& Value, SizeT Index) {
            array[Index] = Value;
        }

        ForceInline void UnsafeSet(T&& Value, SizeT Index) {
            array[Index] = MoveRValue(Value);
        }

        ForceInline BaseConcurencyArray& RemoveAllImpl() {
            return lock.ExclusiveLock([&]()->BaseConcurencyArray & InlineLambda{
                delete[] this->array;
                this->array = nullptr;
                this->count = 0;
                return *this;
                });
        }

        template<typename TLambda>
        BaseConcurencyArray& RemoveAllImpl(TLambda&& lambda) {
            return lock.ExclusiveLock([&]()->BaseConcurencyArray & InlineLambda{
                for (SizeT i = 0; i < count; i++)
 {
lambda(MoveRValue(array[i]));
}

delete[] this->array;
this->array = nullptr;
this->count = 0;
return *this;
                });
        }

        template<typename TLambda>
        SizeT GetCountIfImpl(TLambda&& lambda) const {
            return lock.SharedLock([&]() InlineLambda{
                SizeT selectedCount = 0;
                for (SizeT i = 0; i < count; i++)
                {
                    if (lambda(array[i]))
                        selectedCount++;
                }
                return selectedCount;
                });
        }

    public:
        /// <summary>
        /// Zero length array constructor
        /// </summary>
        ForceInline BaseConcurencyArray() : count(0), array(nullptr), lock() {}

        /// <summary>
        /// List of elements constructor
        /// </summary>
        /// <param name="list">List of elements</param>
        BaseConcurencyArray(InitilizerList<T>&& list) : count(0), array(nullptr), lock() {
            if (list.size() != 0)
            {
                count = list.size();
                this->array = new T[count];
                auto it = list.begin();
                for (SizeT i = 0; i < count; i++)
                {
                    this->array[i] = MoveRValue(*it); ++it;
                }
            }
        }

        /// <summary>
        /// Copying constructor
        /// </summary>
        /// <param name="array">Existing array</param>
        BaseConcurencyArray(const BaseConcurencyArray& array) : count(0), array(nullptr), lock() {
            array.lock.SharedLock([&] () {
                this->array = new T[array.count];
                this->count = array.count;
                if constexpr (TypeTraits::IsPodV<T>)
                {
                    Memory::Copy(array.array, this->array, sizeof(T) * this->count);
                }
                else
                {
                    for (SizeT i = 0; i < this->count; i++)
                    {
                        this->array[i] = array.array[i];
                    }
                }
                });
        }

        /// <summary>
        /// Moving constructor
        /// </summary>
        /// <param name="array">Existing array</param>
        BaseConcurencyArray(BaseConcurencyArray&& array) : count(0), array(nullptr), lock() {
            array.lock.ExclusiveLock([&] () mutable {
                this->count = array.count;
                array.count = 0;
                this->array = array.array;
                array.array = nullptr;
                });
        }

        /// <summary>
        /// Number of elements constructor
        /// </summary>
        /// <param name="startCount">Starting number of elements</param>
        ForceInline BaseConcurencyArray(SizeT startCount) : count(0), array(nullptr), lock() {
            if (startCount != 0)
            {
                array = new T[startCount];
                count = startCount;
            }
        }

        /// <summary>
        /// Internal constructor with C-array and number of elements
        /// </summary>
        /// <param name="array">A pointer to an array of elements</param>
        /// <param name="startCount">Number of elements</param>
        InternalApiCall BaseConcurencyArray(const T*& array, SizeT startCount) : count(0), array(nullptr), lock() {
            if (startCount != 0)
            {
                this->array = new T[startCount];
                count = startCount;
                if constexpr (TypeTraits::IsPodV<T>)
                {
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
        /// Internal constructor with "RValue", reference to C-array and number of elements
        /// </summary>
        /// <param name="array">"RValue" reference to C-array</param>
        /// <param name="startCount">Number of elements</param>
        ForceInline InternalApiCall BaseConcurencyArray(T*&& array, SizeT startCount) : count(startCount), array(MoveRValue(array)), lock() {}

        /// <summary>
        /// Copying operator
        /// </summary>
        /// <param name="original">Original</param>
        /// <returns>"LValue" reference to array</returns>
        BaseConcurencyArray& operator=(const BaseConcurencyArray& original) {
            this->~BaseConcurencyArray();
            this->BaseConcurencyArray::BaseConcurencyArray(original);
            return *this;
        }

        /// <summary>
        /// Moving operator
        /// </summary>
        /// <param name="original">Original</param>
        /// <returns>"RValue" reference to array</returns>
        BaseConcurencyArray& operator=(BaseConcurencyArray&& original) {
            this->~BaseConcurencyArray();
            this->BaseConcurencyArray::BaseConcurencyArray(MoveRValue(original));
            return *this;
        }

        /// <summary>
        /// A raw pointer recieving operator to elements of array
        /// </summary>
        ForceInline InternalApiCall explicit operator const T* const() const { return array; }

    public:
        /// <summary>
        /// An access operator to array elements
        /// </summary>
        /// <param name="index">Element number</param>
        /// <returns>A reference to element</returns>
        ForceInline T& operator[](SizeT index)& {
            return lock.SharedLock([&]()->T & InlineLambda{
                if (index < count)
                {
                    return array[index];
                }
                else
                {
                    Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, index, count);
                }
                });
        }

        /// <summary>
        /// A constant access operator to array elements
        /// </summary>
        /// <param name="index">Element number</param>
        /// <returns>A constant reference to element</returns>
        ForceInline const T& operator[](SizeT index) const& {
            return lock.SharedLock([&]() -> const T & InlineLambda{
                if (index < count)
                {
                    return array[index];
                }
                else
                {
                    Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, index, count);
                }
                });
        }

        /// <summary>
        /// An access operator to array elements
        /// </summary>
        /// <param name="index">Element number</param>
        /// <returns>A reference to element</returns>
        ForceInline T operator[](SizeT index)&& {
            return lock.SharedLock([&] ()->T InlineLambda {
                if (index < count)
                {
                    return MoveRValue(array[index]);
                }
                else
                {
                    Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, index, count);
                }
                });
        }

        /// <summary>
        /// Resize
        /// </summary>
        /// <param name="newSize">New array size</param>
        /// <returns>A reference to a current object</returns>
        BaseConcurencyArray& Resize(SizeT newSize) {
            return lock.ExclusiveLock([&]()->BaseConcurencyArray & InlineLambda{
                if (array == nullptr || count == 0)
                {
                    array = new T[newSize];
                    count = newSize;
                }
                else
 {
if (newSize == 0)
{
delete[] array;
count = 0;
}
else
{
T* buf = new T[newSize];
const SizeT copySize = (newSize > count) ? count : newSize;

if constexpr (TypeTraits::IsPodV<T>)
{
Memory::Copy(array, buf, copySize * sizeof(T));
}
else
{
for (SizeT i = 0; i < copySize; i++)
{
    buf[i] = MoveRValue(array[i]);
}
}
count = newSize;
delete[] array;
array = buf;
}
}
return *this;
                });
        }

        /// <summary>
        /// Array cloning
        /// </summary>
        /// <returns>Returns new array with copies of all elements</returns>
        template<typename = void> requires (Concepts::IsClonable<T>)
            BaseConcurencyArray Clone() const {
            return lock.SharedLock([&] () -> BaseConcurencyArray InlineLambda {
                if constexpr (TypeTraits::IsPodV<T>)
                {
                    return BaseConcurencyArray((const T*&) (array), count);
                }
                else
                {
                    T* array = nullptr;

                    try
                    {
                        array = new T[count];

                        for (SizeT i = 0; i < count; ++i)
                        {
                            array[i] = Clone(this->array[i]);
                        }

                        return BaseConcurencyArray(MoveRValue(array), count);
                    }
                    catch (...)
                    {
                        delete[] array;
                        Internals::Exceptions::__ReThrow();
                    }
                }
                });
        }

        /// <summary>
        /// Array cloning using custom lambda or fuctor
        /// </summary>
        /// <typeparam name="TLambda">Lambda type</typeparam>
        /// <param name="lambda">A function with which occurs cloning</param>
        /// <returns>Returns new array with copies of all elements</returns>
        template<typename TLambda> requires (Concepts::IsInvockableWithResult<TLambda, T, const T&>)
            BaseConcurencyArray Clone(TLambda&& lambda) const {
            return lock.SharedLock([&] () -> BaseConcurencyArray InlineLambda {
                T* newArray = nullptr;
                try
                {
                    newArray = new T[count];
                    for (auto i = 0ui64; i < count; i++)
                    {
                        newArray[i] = lambda(array[i]);
                    }

                    return BaseConcurencyArray(MoveRValue(newArray), count);
                }
                catch (...)
                {
                    delete[] newArray;
                    Internals::Exceptions::__ReThrow();
                }
                });
        }

        /// <summary>
        /// Optimized swap
        /// </summary>
        /// <typeparam name="U">Type of elements</typeparam>
        /// <param name="a">First element</param>
        /// <param name="b">Second element</param>
        template<typename U>
        friend void Cardinal::Swap(U& a, U& b);

    public:
        /// <summary>
        /// Returns current number of elements
        /// </summary>
        ForceInline SizeT GetCount() const { return count; }

        /// <summary>
        /// A number of elements
        /// </summary>
        SizeT Property(GetCount) Count;

        /// <summary>
        /// Returns raw pointer to elements of array
        /// </summary>
        ForceInline const T* const GetRaw() const { return array; }

    public:
        /// <summary>
        /// Array equality operator
        /// </summary>
        /// <param name="other">Second operand</param>
        /// <returns>"True" if "other" is a reference to "this" or the number of elements are equal and elements are also equal</returns>
        bool operator==(const BaseConcurencyArray& other) const requires Concepts::HaveEqual<T, T> {
            if (other.array == this->array)
                return true;

            if (other.count != this->count)
                return false;

            return this->lock.SharedLock([&] () {
                return other.lock.SharedLock([&] () {
                    for (SizeT i = 0; i < this->count; i++)
                    {
                        if (!(other.array[i] == this->array[i]))
                            return false;
                    }

                    return true;
                    });
                });
        }

        /// <summary>
        /// Array inequality operator
        /// </summary>
        /// <param name="other">Second operand</param>
        /// <returns>"False" if "other" is a reference to "this" or the number of elements are equal and elements are also equal</returns>
        bool operator!=(const BaseConcurencyArray& other) const requires Concepts::HaveEqual<T, T> {
            return !(*this == other);
        }

    public:
        ~BaseConcurencyArray() {
            delete[] array;
            array = nullptr;
        }
    };

    /// <summary>
    /// An array with multithreding support
    /// </summary>
    template<typename T>
    using ConcurencyArray = BaseConcurencyArray<Cardinal::Memory::Heap, T>;

    /// <summary>
    /// An array with multithreding support in protected heap
    /// </summary>
    template<typename T>
    using ConcurencySensitiveArray = BaseConcurencyArray<Cardinal::Memory::SensitiveDataHeap, T>;
}

export namespace Cardinal::TypeTraits::Collection
{
    template<typename THeap, typename T>
    struct IsArrayT<Cardinal::Containers::BaseConcurencyArray<THeap, T>> : LogicTrue {};
}

export namespace Cardinal
{
    template<typename THeap, typename T>
    ForceInline void Swap(Containers::BaseConcurencyArray<THeap, T>& a, Containers::BaseConcurencyArray<THeap, T> b) {
        a.lock.ExclusiveLock([&] () {
            b.lock.ExclusiveLock([&] () {
                Swap(a.count, b.count);
                Swap(a.array, b.array);
                Swap(a.lock, b.lock);
                });
            });
    }
}

#pragma warning(pop)