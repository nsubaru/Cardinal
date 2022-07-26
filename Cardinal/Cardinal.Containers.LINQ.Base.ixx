export module Cardinal.Containers.LINQ:Base;

export import Cardinal.Core;

import Cardinal.Exceptions.Internals;

export namespace Cardinal::Serialization
{
	/// <summary>
    /// Serializer core
    /// </summary>
    /// <typeparam name="T">A type from which the serializer works</typeparam>
	template<typename T>
	struct SerializationHandler;
}

export namespace Cardinal::Concepts
{
    /// <summary>
    /// General concept for container items copy functor without passing item index
    /// </summary>
    template<typename TLambda, typename T>
    concept CopyFunctionWithoutIdx = requires(TLambda&& lambda, const T & item) { {lambda(item)}->Concepts::NotVoid; };

    /// <summary>
    /// General concept for container items copy functor with passing item index
    /// </summary>
    template<typename TLambda, typename T>
    concept CopyFunctionWithIdx = requires(TLambda&& lambda, const T & item, SizeT idx) { {lambda(item, idx)}->Concepts::NotVoid; };

    /// <summary>
    /// General concept for container items copy functor
    /// </summary>
    template<typename TLambda, typename T>
    concept CopyFunction = CopyFunctionWithoutIdx<TLambda, T> || CopyFunctionWithIdx<TLambda, T>;

    /// <summary>
    /// General concept for container items move functor without passing item index
    /// </summary>
    template<typename TLambda, typename T>
    concept MoveFunctionWithoutIdx = requires(TLambda&& lambda, T&& item) { {lambda(MoveRValue(item))}->Concepts::NotVoid; };

    /// <summary>
    /// General concept for container items move functor with passing item index
    /// </summary>
    template<typename TLambda, typename T>
    concept MoveFunctionWithIdx = requires(TLambda && lambda, T&& item, SizeT idx) { {lambda(MoveRValue(item), idx) }->Concepts::NotVoid; };

    /// <summary>
    /// General concept for container items move functor
    /// </summary>
    template<typename TLambda, typename T>
    concept MoveFunction = MoveFunctionWithoutIdx<TLambda, T> || MoveFunctionWithIdx<TLambda, T>;

    /// <summary>
    /// General concept for container items predicative functor without passing item index
    /// </summary>
    template<typename TLambda, typename T>
    concept ConditionFunctionWithoutIdx = requires(TLambda&& lambda, const T& item) { {lambda(item) }->Concepts::IsSame<Boolean>; };

    /// <summary>
    /// General concept for container items predicative functor with passing item index
    /// </summary>
    template<typename TLambda, typename T>
    concept ConditionFunctionWithIdx = requires(TLambda && lambda, const T & item, SizeT idx) { {lambda(item, idx) }->Concepts::IsSame<Boolean>; };

    /// <summary>
    /// General concept for container items predicative functor
    /// </summary>
    template<typename TLambda, typename T>
    concept ConditionFunction = ConditionFunctionWithoutIdx<T, TLambda> || ConditionFunctionWithIdx<T, TLambda>;
}

export namespace Cardinal::TypeTraits
{
    /// <summary>
    /// Container items copy functor return type
    /// </summary>
    /// <typeparam name="TLambda">Functor</typeparam>
    /// <typeparam name="T">Item type</typeparam>
    template<typename T, Concepts::CopyFunction<T> TLambda>
    using CopyFunctionRet = TypeTraits::ConditionV<
        Concepts::CopyFunctionWithIdx<TLambda, T>,
        TypeTraits::InvockableResult<TLambda, const T&, SizeT>,
        TypeTraits::InvockableResult<TLambda, const T&>
    >;

    /// <summary>
    /// Container items move functor return type
    /// </summary>
    /// <typeparam name="TLambda">Functor</typeparam>
    /// <typeparam name="T">Item type</typeparam>
    template<typename T, Concepts::MoveFunction<T> TLambda>
    using MoveFunctionRet = TypeTraits::ConditionV<
        Concepts::MoveFunctionWithIdx<TLambda, T>,
        TypeTraits::InvockableResult<TLambda, T&&, SizeT>,
        TypeTraits::InvockableResult<TLambda, T&&>
    >;

    /// <summary>
    /// Helper template variable for conditional copy optimization
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template<typename T, Concepts::ConditionFunction<T> TConditionLambda, Concepts::CopyFunction<T> TCopyLambda>
    constexpr bool IsIndexNeedForPerformeConditionalCopy = Concepts::CopyFunctionWithIdx<TCopyLambda, T> || Concepts::ConditionFunctionWithIdx<TConditionLambda, T>;

    /// <summary>
    /// Helper template variable for conditional copy optimization
    /// </summary>
    /// <typeparam name="T"></typeparam>
    template<typename T, Concepts::ConditionFunction<T> TConditionLambda, Concepts::MoveFunction<T> TCopyLambda>
    constexpr bool IsIndexNeedForPerformeConditionalMove = Concepts::MoveFunctionWithIdx<TCopyLambda, T> || Concepts::ConditionFunctionWithIdx<TConditionLambda, T>;
}

/// <summary>
/// "API LINQ" namespace
/// </summary>
export namespace Cardinal::Containers::LINQ
{
    /// <summary>
    /// Gives "CTTI" the information for collections
    /// </summary>
    /// <typeparam name="TCollection">Collection type</typeparam>
    /// <typeparam name="THeap">Collection heap</typeparam>
	template<
		typename TCollection,
		typename THeap,
		typename T,
		Boolean TIsDynamicT,
		Boolean TIsResizeableT,
		Boolean TIsConcurencySupportT,
        Boolean TIsReadonly = false,
        Boolean TIsCollectionCanReturnRef = true
	>
    VirtualStruct TLinq {
        /// <summary>
        /// Collection type
        /// </summary>
        using CollectionType = TCollection;
               
        /// <summary>
        /// Heap type
        /// </summary>
		using HeapType = THeap;
                
        /// <summary>
        /// Element type
        /// </summary>
		using DataType = T;
                
        /// <summary>
        /// "True" if collection is dynamic
        /// </summary>
		static constexpr Boolean IsDynamic = TIsDynamicT;
                
        /// <summary>
        /// "True" if collection allows the number of elements change
        /// </summary>
		static constexpr Boolean IsResizeable = TIsResizeableT;

        /// <summary>
        /// "True" if collection support only read operations,
        /// those collections don't support serialization
        /// </summary>
        static constexpr Boolean IsReadonly = TIsReadonly;

        /// <summary>
        /// "False" notify that collection item generate and can not be returned from methods as reference
        /// </summary>
        static constexpr Boolean IsCollectionCanReturnRef = TIsCollectionCanReturnRef;
	};

    /// <summary>
    /// A "LINQ" base class for containers
    /// </summary>
	VirtualClass ILinqBase {};
            
    /// <summary>
    /// A generic collections base class
    /// </summary>
	VirtualClass ICollectionGenericBase {};
           
    /// <summary>
    /// A fixed collections base class
    /// </summary>
	VirtualClass IFixedCollectionGenericBase {};
           
    /// <summary>
    /// An associative collections base class
    /// </summary>
    VirtualClass IAssociableCollectionGenericBase {};
            
    /// <summary>
    /// A string collections base class
    /// </summary>
    VirtualClass IStringCollectionGenericBase {};

    /// <summary>
    /// A pair collections base class
    /// </summary>
    VirtualClass IPairCollectionGenericBase {};

#pragma region LINQ for one typearg Collection
    /// <summary>
    /// Generic class of generic collections
    /// </summary>
    /// <typeparam name="TCollection">Collection type</typeparam>
    /// <typeparam name="THeap">Collection heap</typeparam>
    /// <typeparam name="T">Type of elements</typeparam>
    /// <typeparam name="TIsDynamic">Indicates whether the collection is dynamically changes its size</typeparam>
    /// <typeparam name="TIsConcurencySuport">Indicates whether the collection supports multithreading</typeparam>
	template<
        template<typename THeap, typename T> typename TCollection,
		typename THeap,
		typename T,
		Boolean TIsDynamic,
		Boolean TIsConcurencySuport,
        Boolean TIsReadonly = false,
        Boolean TIsCollectionCanReturnRef = true
	>
    VirtualClass ICollectionBase :
		public ILinqBase,
		public ICollectionGenericBase,
		public TLinq<TCollection<THeap, T>, THeap, T, TIsDynamic, true, TIsConcurencySuport>
	{};
#pragma endregion

#pragma region LINQ for fixed size Collection
    /// <summary>
    /// A "CTTI" metadata base class for fixed collections
    /// </summary>
    /// <typeparam name="TCollection">Collection type</typeparam>
    /// <typeparam name="THeap">Collection heap</typeparam>
    /// <typeparam name="T">Type of elements</typeparam>
    /// <typeparam name="TSize">A number of elements</typeparam>
    /// <typeparam name="TIsConcurencySuport">If supports multithreding</typeparam>
	template<typename TCollection, typename THeap, typename T, SizeT TSize, Boolean TIsConcurencySuport>
    VirtualStruct TFixedLinq : public TLinq<TCollection, THeap, T, false, false, TIsConcurencySuport> {
        /// <summary>
        /// A number of elements
        /// </summary>
		static constexpr SizeT FixedSize = TSize;
	};

    /// <summary>
    /// A base class for fixed collections
    /// </summary>
    /// <typeparam name="TCollection">Fixed collection type</typeparam>
    /// <typeparam name="THeap">Heap type</typeparam>
    /// <typeparam name="T">Type of elements</typeparam>
    /// <typeparam name="TSize">A number of elements</typeparam>
    /// <typeparam name="TIsConcurencySuport">If supports multithreding</typeparam>
	template<
        template<typename, SizeT, typename> typename TCollection,
		typename THeap,
		typename T,
		SizeT TSize,
		Boolean TIsConcurencySuport
	>
	VirtualClass IFixedCollectionBase :
		public ILinqBase,
		public IFixedCollectionGenericBase,
		public TFixedLinq<TCollection<THeap, TSize, T>, THeap, T, TSize, TIsConcurencySuport>
	{

	public:
        /// <summary>
        /// Copy elements to a new generic collection
        /// </summary>
        /// <typeparam name="TNewCollection">New generic collection</typeparam>
        /// <returns>New collection with copy of elements</returns>
        template<typename TNewCollection> requires (
                TypeTraits::IsBasedOnV<ICollectionGenericBase, TNewCollection>&&
                TypeTraits::IsSameV<
                    typename TNewCollection::DataType,
                    T> &&
                !TNewCollection::IsReadonly
            )
		TNewCollection CopyTo() {
			if constexpr (!TNewCollection::IsDynamic) {
                decltype(auto) thisCollectionRef = static_cast<const TCollection<THeap, TSize, T>&>(*this);
				TNewCollection instance(thisCollectionRef.Count);
                thisCollectionRef.ForEach([&] (const T& value, SizeT index) InlineLambda
                {
                    if constexpr (TypeTraits::IsMemberFunctionPointerV<decltype(&T::Clone)>)
                    {
                        instance.UnsafeSet(value.Clone(), index);
                    }
                    else
                    {
                        instance.UnsafeSet(T(value), index);
                    }
                }
                    );
				return instance;
			}
			else {
                decltype(auto) thisCollectionRef = static_cast<const TCollection<THeap, TSize, T>&>(*this);
                TNewCollection instance{};
                thisCollectionRef.ForEach([&] (const T& value) InlineLambda
                {
                    if constexpr (TypeTraits::IsMemberFunctionPointerV<decltype(&T::Clone)>)
                    {
                        instance.UnsafeAdd(value.Clone());
                    }
                    else
                    {
                        instance.UnsafeAdd(T(value));
                    }
                });
				return instance;
			}
		}

        /// <summary>
        /// Copy elements to a new generic collection
        /// </summary>
        /// <typeparam name="TNewCollection">New generic collection</typeparam>
        /// <returns>New collection with copy of elements</returns>
        template<typename TNewCollection> requires (
                TypeTraits::IsBasedOnV<ICollectionGenericBase, TNewCollection>&&
                TypeTraits::IsSameV<
                    typename TNewCollection::DataType,
                    T> &&
                !TNewCollection::IsReadonly
        )
        TNewCollection CopyTo() const
        {
			if constexpr (!TNewCollection::IsDynamic) {
                decltype(auto) thisCollectionRef = static_cast<const TCollection<THeap, TSize, T>&>(*this);
				TNewCollection instance(thisCollectionRef.Count);
				thisCollectionRef.ForEach([&](const T& value, SizeT index) InlineLambda {
                    if constexpr (TypeTraits::IsMemberFunctionPointerV<decltype(&T::Clone)>)
                    {
                        instance.UnsafeSet(value.Clone(), index);
                    }
                    else
                    {
                        instance.UnsafeSet(T(value), index);
                    }
				});
				return instance;
			}
			else {
                decltype(auto) thisCollectionRef = static_cast<const TCollection<THeap, TSize, T>&>(*this);
                TNewCollection instance{};
				thisCollectionRef.ForEach([&](const T& value) InlineLambda {
                    if constexpr (TypeTraits::IsMemberFunctionPointerV<decltype(&T::Clone)>)
                    {
                        instance.UnsafeAdd(value.Clone());
                    }
                    else
                    {
                        instance.UnsafeAdd(T(value));
                    }
				});
				return instance;
			}
		}

        /// <summary>
        /// Moves elements in a new generic collection (elements in old collection becomes invalid)
        /// </summary>
        /// <typeparam name="TNewCollection">New generic collection type</typeparam>
        /// <returns>New generic collection with elements from old collection</returns>
		template<template<typename U> typename TNewCollection> 
        requires (Concepts::IsBasedOn<ICollectionGenericBase, TNewCollection<T>> && !TNewCollection<T>::IsReadonly)
        TNewCollection<T> MoveTo() {
			if constexpr (!TNewCollection<T>::IsDynamic) {
				decltype(auto) thisCollectionRef = static_cast<TCollection<THeap, TSize, T>&>(*this);
				TNewCollection<T> instance(thisCollectionRef.Count);
                auto index = 0Sz;
				thisCollectionRef.RemoveAll([&](T&& value) InlineLambda {
                    instance.UnsafeSet(MoveRValue(value), index); index++;
				});
				return instance;
			}
			else {
                decltype(auto) thisCollectionRef = static_cast<TCollection<THeap, TSize, T>&>(*this);
				TNewCollection<T> instance;
				thisCollectionRef.RemoveAll([&](T&& value) InlineLambda {
					instance.UnsafeAdd(MoveRValue(value));
				});
				return instance;
			}
		}
	};
#pragma endregion

#pragma region LINQ for associable Collection
    /// <summary>
    /// A "CTTI" metadata base class for associative collections
    /// </summary>
    /// <typeparam name="TCollection">Collection type</typeparam>
    /// <typeparam name="THeap">Heap type</typeparam>
    /// <typeparam name="TKey">Key type</typeparam>
    /// <typeparam name="TValue">Value type</typeparam>
    /// <typeparam name="TPair">A key-value pair type</typeparam>
    /// <typeparam name="TIsConcurencySuport">If supports multithreding</typeparam>
	template<typename TCollection, typename THeap, typename TKey, typename TValue, typename TPair, Boolean TIsConcurencySuport>
    // ReSharper disable once CppInconsistentNaming
    VirtualStruct TAssociableLinq : public TLinq<TCollection, THeap, TPair, true, true, TIsConcurencySuport> {
        // ReSharper restore CppInconsistentNaming
                    
        /// <summary>
        /// Key type
        /// </summary>
		using KeyType = TKey;
               
        /// <summary>
        /// Value type
        /// </summary>
		using ValueType = TValue;
	};

    /// <summary>
    /// An associable collections base type
    /// </summary>
    /// <typeparam name="TCollection">Associative collection type</typeparam>
    /// <typeparam name="THeap">Heap type</typeparam>
    /// <typeparam name="TKey">Key type</typeparam>
    /// <typeparam name="TValue">Value type</typeparam>
    /// <typeparam name="TPair">A key-value pair type</typeparam>
    /// <typeparam name="TIsConcurencySuport">If supports multithreding</typeparam>
	template<
        template<typename THeap, typename TKey, typename TValue> typename TCollection,
		typename THeap,
		typename TKey,
		typename TValue,
		typename TPair,
		bool TIsConcurencySuportT
	>
	VirtualClass IAssociableCollectionBase :
		public ILinqBase,
		public IAssociableCollectionGenericBase,
		public TAssociableLinq<TCollection<THeap, TKey, TValue>, THeap, TKey, TValue, TPair, true>
	{
	};
#pragma endregion

#pragma region LINQ for Pair
    /// <summary>
    /// A "CTTI" metadata base class for key-value pair
    /// </summary>
    /// <typeparam name="TPair">Pair type</typeparam>
    /// <typeparam name="THeap">Heap type</typeparam>
    /// <typeparam name="TKey">Key type</typeparam>
    /// <typeparam name="TValue">Value type</typeparam>
    /// <typeparam name="TIsConcurencySuport">If supports multithreding</typeparam>
	template<typename TPair, typename THeap, typename TKey, typename TValue, Boolean TIsConcurencySuport>
    VirtualStruct TPairLinq : 
        public TLinq<TPair, THeap, TPair, false, false, TIsConcurencySuport>, 
        public IPairCollectionGenericBase 
    {
        /// <summary>
        /// Key type
        /// </summary>
        using KeyType = TKey;
                
        /// <summary>
        /// Value type
        /// </summary>
		using ValueType = TValue;
	};
#pragma endregion

#pragma region LINQ for String
    /// <summary>
    /// A "CTTI" metadata base class for string values
    /// </summary>
    /// <typeparam name="TString">String type</typeparam>
    /// <typeparam name="THeap">Heap type</typeparam>
    /// <typeparam name="TChar">Symbol type</typeparam>
    /// <typeparam name="TIsConcurencySuport">If supports multithreding</typeparam>
	template<typename TString, typename THeap, typename TChar, Boolean TIsConcurencySuport>
    VirtualStruct TStringLinq : public TLinq<TString, THeap, TChar, false, true, TIsConcurencySuport> {
        /// <summary>
        /// Symbol type
        /// </summary>
        using CharacterType = TChar;
	};

    /// <summary>
    /// String value generic type
    /// </summary>
    /// <typeparam name="TString">String value type</typeparam>
    /// <typeparam name="THeap">Heap type</typeparam>
    /// <typeparam name="TChar">Symbol type</typeparam>
	template<template<typename THeap> typename TString, typename THeap, typename TChar, Boolean TIsConcurencySuport>
	VirtualClass IStringCollectionBaseLinq : 
        public TStringLinq<TString<THeap>, THeap, TChar, TIsConcurencySuport>, 
        public IStringCollectionGenericBase {
	};
#pragma endregion

#pragma region LINQ Single item containers interfaces
    /// <summary>
    /// An interface for "smart" container identification
    /// </summary>
    VirtualClass ISmartPtrGenericBase {};

    /// <summary>
    /// An interface for "lazy" container identification
    /// </summary>
    VirtualClass ILazyGenericBase {};
#pragma endregion
}