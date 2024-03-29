export module Cardinal.Containers.Concurency.FixedStack;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.Containers.LINQ;
export import Cardinal.Containers.TypeTraits;
export import Cardinal.Threading.SRWLock;
export import Cardinal.Containers.InitilizerList;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
#pragma warning(disable: 4172)
#pragma warning(disable: 4996)


export namespace Cardinal::Containers
{
    /// <summary>
    /// A stack with fixed maximal number of elements and multithreading support
    /// </summary>
    template<typename THeap, SizeT TSize, typename T>
        requires (TSize > 0)
    class BaseConcurencyFixedStack :
        public LINQ::IFixedStack<BaseConcurencyFixedStack, THeap, T, TSize, true> {
    private:
        RefCountT count;
        Threading::SRWLock mutable lock;
        T stack[TSize];

    public:
        ~BaseConcurencyFixedStack() {
            if constexpr (TypeTraits::IsSameV<THeap, Memory::SensitiveDataHeap> &&
                TypeTraits::IsPodV<T>)
            {
                Memory::SecureZeroMemory(stack);
            }
        };

    public:
        /// <summary>
        /// An empty constructor
        /// </summary>
        ForceInline BaseConcurencyFixedStack() : count(0), lock(), stack() {}

        /// <summary>
        /// A constructor with initialization list
        /// (if stack has more elements than maximal size, the rest will be ignored)
        /// </summary>
        /// <param name="list">Initialization list</param>
        BaseConcurencyFixedStack(InitilizerList<T>&& list) : count(0), lock(), stack() {
            if (list.size() != 0)
            {
                SizeT actuallyCopyied = (TSize < list.size()) ? TSize : list.size();
                auto it = list.begin();

                for (SizeT i = 0; i < actuallyCopyied; i++)
                {
                    this->stack[i] = MoveRValue(*it); ++it;
                }

                this->count = actuallyCopyied;
            }
        }

        /// <summary>
        /// Copying constructor from the original stack
        /// </summary>
        /// <param name="stack">Original stack</param>
        BaseConcurencyFixedStack(const BaseConcurencyFixedStack& original) : lock() {
            original.lock.SharedLock([&] () {
                this->count = original.count;

                if constexpr (TypeTraits::IsPodV<T>)
                {
                    Memory::Copy(original.stack, this->stack);
                }
                else
                {
                    for (SizeT i = 0; i < TSize; i++)
                    {
                        this->stack[i] = original.stack[i];
                    }
                }
                });
        }

        /// <summary>
        /// Moving constructor from the original stack
        /// </summary>
        /// <param name="stack">Original stack</param>
        BaseConcurencyFixedStack(BaseConcurencyFixedStack&& original) : lock() {
            original.lock.SharedLock([&] () {
                this->count = original.count;
                original.count = 0;

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
                });
        }

        /// <summary>
        /// Copying operator
        /// </summary>
        /// <param name="original">Original</param>
        /// <returns>"LValue" reference to array</returns>
        BaseConcurencyFixedStack& operator=(const BaseConcurencyFixedStack& original) {
            this->~BaseConcurencyFixedStack();
            this->BaseConcurencyFixedStack::BaseConcurencyFixedStack(original);
            return *this;
        }

        /// <summary>
        /// Moving operator
        /// </summary>
        /// <param name="original">Original</param>
        /// <returns>"RValue" reference to array</returns>
        BaseConcurencyFixedStack& operator=(BaseConcurencyFixedStack&& original) {
            this->~BaseConcurencyFixedStack();
            this->BaseConcurencyFixedStack::BaseConcurencyFixedStack(MoveRValue(original));
            return *this;
        }

        /// <summary>
        /// Adds elements in stack (is stack is full then generates an exception)
        /// </summary>
        /// <param name="item">A reference to element</param>
        void Push(const T& item) {
            this->lock.ExclusiveLock([&]() mutable InlineLambda{
                if (this->count == TSize)
                Internals::Exceptions::ThrowFullStackException(SourcePosition, TSize);

                this->stack[this->count] = item;
                ++this->count;
                });
        }

        /// <summary>
        /// Adds elements in stack (is stack is full then generates an exception)
        /// </summary>
        /// <param name="item">"RValue" reference to an element</param>
        void Push(T&& item) {
            this->lock.ExclusiveLock([&]() mutable InlineLambda{
                if (this->count == TSize)
                Internals::Exceptions::ThrowFullStackException(SourcePosition, TSize);

                this->stack[this->count] = MoveRValue(item);
                ++this->count;
                });
        }

        /// <summary>
        /// Popping the element from stack (if stack is empty then generates an exception)
        /// </summary>
        /// <returns>"RValue" reference to an element</returns>
        T Pop() {
            return MoveRValue(this->lock.ExclusiveLock([&]() mutable InlineLambda{
                if (this->count == 0)
                Internals::Exceptions::ThrowEmptyStackException(SourcePosition);

                --this->count;
                return MoveRValue(this->stack[this->count]);
                }));
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
        /// Gaining top element from stack (if stack is full the generates an exception)
        /// </summary>
        /// <returns>A reference to a top alement</returns>
        const T& Top() {
            return this->lock.ExclusiveLock([&]() mutable -> const T & InlineLambda{
                if (this->count == 0)
                Internals::Exceptions::ThrowEmptyStackException(SourcePosition);

                return this->stack[this->count - 1];
                });
        }

        /// <summary>
        /// Gaining top element from stack (if stack is empty then generates an exception)
        /// </summary>
        /// <returns>A reference to a top alement</returns>
        const T& Top() const {
            return this->lock.ExclusiveLock([&]() mutable -> const T & InlineLambda{
                if (this->count == 0)
                Internals::Exceptions::ThrowEmptyStackException(SourcePosition);

                return this->stack[this->count - 1];
                });
        }

        template<typename U>
        friend void Cardinal::Swap(U& A, U& B);

        /// <summary>
        /// A number of elements in stack
        /// </summary>
        /// <returns>A current number of elements in stack</returns>
        ForceInline  SizeT GetCount() const { return this->count; }

        /// <summary>
        /// A current number of elements in stack
        /// </summary>
        SizeT Property(GetCount) Count;

        /// <summary>
        /// Stack cloning
        /// </summary>
        /// <returns>A stack with copy of all elements</returns>
        template<typename = void> requires (Concepts::IsClonable<T>)
            BaseConcurencyFixedStack Clone() const {
            return this->lock.SharedLock([&]() InlineLambda{
                BaseConcurencyFixedStack result;
                result.count = this->count;

                if constexpr (TypeTraits::IsPodV<T>)
                {
                    Memory::Copy(this->stack, result.stack);
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
        /// Stack cloning using custom lambda or functor
        /// </summary>
        /// <typeparam name="TLambda">Lambda type</typeparam>
        /// <param name="lambda">A function with which occurs cloning</param>
        /// <returns>A stack with copy of all elements</returns>
        template<typename TLambda> requires (Concepts::IsInvockableWithResult<TLambda, T, const T&>)
            BaseConcurencyFixedStack Clone(TLambda&& lambda) const {
            return this->lock.SharedLock([&]() InlineLambda{
                BaseConcurencyFixedStack result;
                result.count = this->count;

                for (SizeT i = 0; i < this->count; i++)
                {
                    result.stack[i] = lambda(this->stack[i]);
                }

                return result;
                });
        }

    protected:
        template<template<typename, typename> typename TCollection, typename UHeap, typename U, Boolean TIsDynamic, Boolean TIsConcurencySuport, Boolean TIsReadonly, Boolean TIsCollectionCanReturnRef>
        friend class LINQ::ICollection;

        template<template<typename, SizeT, typename> typename TCollection, typename UHeap, typename U, SizeT USize, Boolean TIsConcurencySuport>
        friend class LINQ::IFixedCollection;

        template<template<typename, SizeT, typename> typename TCollection, typename UHeap, typename U, SizeT USize, Boolean TIsConcurencySuport>
        friend class LINQ::IFixedStack;

        template<template<typename UHeap, typename U> typename TCollection, typename UHeap, typename U, Boolean TIsDynamic, Boolean TIsConcurencySuport, Boolean TIsReadonly, Boolean TIsCollectionCanReturnRef>
        friend class LINQ::ICollectionBase;

        template<typename U>
        friend struct Cardinal::Serialization::SerializationHandler;

    protected:
        template<typename Lambda>
        BaseConcurencyFixedStack& ForEachImpl(Lambda&& lambda) {
            if constexpr (TypeTraits::IsInvockable<Lambda, T&> ||
                TypeTraits::IsInvockable<Lambda, const T&>)
            {
                return this->lock.SharedLock([&]()->BaseConcurencyFixedStack & InlineLambda{
                    if (this->count == 0)
                        return *this;

                    for (SizeT i = this->count; i-- > 0;)
                    {
                        lambda(this->stack[i]);
                    }

                    return *this;
                    });
            }
            else if constexpr (TypeTraits::IsInvockable<Lambda, T&, SizeT> ||
                TypeTraits::IsInvockable<Lambda, const T&, SizeT>)
            {
                return this->lock.SharedLock([&]()->BaseConcurencyFixedStack & InlineLambda{
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
                static_assert(TypeTraits::IsInvockable<Lambda, T&> ||
                    TypeTraits::IsInvockable<Lambda, const T&> ||
                    TypeTraits::IsInvockable<Lambda, T&, SizeT> ||
                    TypeTraits::IsInvockable<Lambda, const T&, SizeT>);
                __assume(0);
            }
        }

        template<typename Lambda>
        const BaseConcurencyFixedStack& ForEachImpl(Lambda&& lambda) const {
            if constexpr (TypeTraits::IsInvockable<Lambda, const T&>)
            {
                return this->lock.SharedLock([&]() -> const BaseConcurencyFixedStack & InlineLambda{
                    if (this->count == 0)
                        return *this;

                    for (SizeT i = 0; i < count; i++)
                    {
                        lambda(this->stack[i]);
                    }

                    return *this;
                    });
            }
            else if constexpr (TypeTraits::IsInvockable<Lambda, const T&, SizeT>)
            {
                return this->lock.SharedLock([&]() -> const BaseConcurencyFixedStack & InlineLambda{
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
                static_assert(TypeTraits::IsInvockable<Lambda, const T&> ||
                    TypeTraits::IsInvockable<Lambda, const T&, SizeT>);
                __assume(0);
            }
        }

        template<typename U, typename TLambda>
        U AggregateImpl(TLambda&& lambda, const U& value) const {
            return MoveRValue(this->lock.SharedLock([&]() mutable InlineLambda{
                U result = value;
                U & data = result;

                if (this->count == 0)
                    return result;

                for (SizeT i = 0; i < count; i++)
                {
                    if constexpr (
                        Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&> ||
                        Concepts::IsInvockableWithResult<TLambda, U, U&, const T&>)
                    {
                        data = lambda(data, this->stack[i]);
                    }
                    else
                    {
                        lambda(data, this->stack[i]);
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

        template<typename Lambda>
        bool AnyImpl(Lambda&& lambda) const {
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

        template<typename Lambda>
        bool AllImpl(Lambda&& lambda) const {
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

        template<typename Lambda>
        T& FirstImpl(Lambda&& lambda) {
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

        template<typename Lambda>
        T& LastImpl(Lambda&& lambda) {
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

        template<typename Lambda>
        const T& FirstImpl(Lambda&& lambda) const {
            return this->lock.SharedLock([&]() -> const T & InlineLambda{
                if (this->count == 0)
                    Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);

                for (SizeT i = 0; i < count; i++)
                {
                    if (lambda(this->stack[i]))
                        return static_cast<const T&>(this->stack[i]);
                }

                Internals::Exceptions::ThrowNullPointException(SourcePosition, 0, 0);
                });
        }

        template<typename Lambda>
        const T& LastImpl(Lambda&& lambda) const {
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
                    return static_cast<const T&>(this->stack[index]);

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

        BaseConcurencyFixedStack& ReverseImpl() {
            return this->lock.ExclusiveLock([&]()->BaseConcurencyFixedStack & InlineLambda{
                for (SizeT i = 0; i < this->count / 2; i++)
 {
Cardinal::Swap(this->stack[i], this->stack[(this->count - 1) - i]);
}
return *this;
                });
        }

        ForceInline void UnsafeSet(T&& value, SizeT index) {
            this->stack[index] = MoveRValue(value);
            this->count = index + 1;
        }

        ForceInline void UnsafeSet(const T& value, SizeT index) {
            this->stack[index] = MoveRValue(value);
            this->count = index + 1;
        }

        BaseConcurencyFixedStack& RemoveAllImpl() {
            return this->lock.ExclusiveLock([&]()->BaseConcurencyFixedStack & InlineLambda{
                for (SizeT i = 0; i < this->count; i++)
 {
this->stack[i].~T();
}
this->count = 0;
return *this;
                });
        }

        template<typename Lambda>
        BaseConcurencyFixedStack& RemoveAllImpl(Lambda&& lambda) {
            return this->lock.ExclusiveLock([&]()->BaseConcurencyFixedStack & InlineLambda{
                for (SizeT i = 0; i < this->count; i++)
 {
lambda(MoveRValue(this->stack[i])); this->stack[i].~T();
}
this->count = 0;
return *this;
                });
        }
    };

    /// <summary>
    /// A stack with fixed maximal number of elements and multithreading support in ordinary heap
    /// </summary>
    template<SizeT TSize, typename T>
    using ConcurencyFixedStack = BaseConcurencyFixedStack<Cardinal::Memory::Heap, TSize, T>;

    /// <summary>
    /// A stack with fixed maximal number of elements and multithreading support in protected heap
    /// </summary>
    template<SizeT TSize, typename T>
    using ConcurencySensitiveFixedStack = BaseConcurencyFixedStack<Cardinal::Memory::SensitiveDataHeap, TSize, T>;
}

export namespace Cardinal::TypeTraits::Collection
{
    template<typename THeap, SizeT TSize, typename T>
    struct IsFixedStackT<Cardinal::Containers::BaseConcurencyFixedStack<THeap, TSize, T>> : LogicTrue {};
}

export namespace Cardinal
{
    template<typename THeap, SizeT TSize, typename T>
    ForceInline void Swap(Containers::BaseConcurencyFixedStack<THeap, TSize, T>& a, Containers::BaseConcurencyFixedStack<THeap, TSize, T>& b) {
        a.lock.ExclusiveLock([&] () {
            b.lock.ExclusiveLock([&] () {
                Swap(a.count, b.count);
                for (SizeT i = 0; i < TSize; i++)
                {
                    Swap(a.stack[i], b.stack[i]);
                }
                });
            });
    }
}
#pragma warning(pop)