export module Cardinal.CTTI:TypeTraits.Collections;

export import :Base;
export import :TypeTraits.HeapMetaData;

export namespace Cardinal::CTTI::TypeTraits
{
    template<typename THeap, typename TCollection, typename TItemType, bool TIsConcurencySupport, EType TType, bool TIsConst, bool TIsVolatile, bool TIsSeriizable = true>
    struct BaseICollection :
        BaseHeapCTTITypeData<TCollection, THeap, TType, TIsSeriizable, TIsConst, TIsVolatile> {
        using ItemType = TItemType;
        using ItemTypeMetaData = CTTITypeData<TItemType>;
        constexpr static Boolean IsConcurencySupport = TIsConcurencySupport;
    };

    // Pair
    template<typename THeap, typename TKey, typename TValue, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::BasePair<THeap, TKey, TValue>, TIsConst, TIsVolatile> :
        BaseHeapCTTITypeData<
        Containers::BasePair<THeap, TKey, TValue>,
        THeap,
        EType::Pair,
        CTTITypeData<TKey>::IsSerializable&& CTTITypeData<TValue>::IsSerializable,
        TIsConst,
        TIsVolatile> {
        using KeyType = TKey;
        using ValueType = TValue;

        using KeyTypeMetaData = CTTITypeData<TKey>;
        using ValueTypeMetaData = CTTITypeData<TValue>;
    };

    // Lazy
    template<typename THeap, typename T, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<BaseLazy<THeap, T>, TIsConst, TIsVolatile> :
        BaseHeapCTTITypeData<BaseLazy<THeap, T>, THeap, EType::Box, CTTITypeData<T>::IsSerializable, TIsConst, TIsVolatile> {
        using ValueType = T;

        using ValueTypeMetaData = CTTITypeData<T>;
    };

    // SmartPtr
    template<typename THeap, typename T, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<BaseSmartPtr<THeap, T>, TIsConst, TIsVolatile> :
        BaseHeapCTTITypeData<BaseSmartPtr<THeap, T>, THeap, EType::Box, CTTITypeData<T>::IsSerializable, TIsConst, TIsVolatile> {
        using ValueType = T;

        using ValueTypeMetaData = CTTITypeData<T>;
    };

#pragma region Collections
    // Base implementation
    template<typename THeap, typename TCollection, typename TItemType, bool TIsConcurencySupport, bool TIsConst, bool TIsVolatile, bool TIsIndexable>
    struct BaseLINQCollection :
        BaseICollection<THeap, TCollection, TItemType, TIsConcurencySupport, EType::Collection, TIsConst, TIsVolatile, CTTITypeData<TItemType>::IsSerializable> {
        static constexpr Boolean IsIndexable = TIsIndexable;
    };

    // Array
    template<typename THeap, typename T, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::BaseArray<THeap, T>, TIsConst, TIsVolatile> :
        BaseLINQCollection<THeap, Containers::BaseArray<THeap, T>, T, false, TIsConst, TIsVolatile, true> {
    };

    template<typename THeap, typename T, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::BaseConcurencyArray<THeap, T>, TIsConst, TIsVolatile> :
        BaseLINQCollection<THeap, Containers::BaseConcurencyArray<THeap, T>, T, true, TIsConst, TIsVolatile, true> {
    };

    // DynamicQueue

    template<typename THeap, typename T, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::BaseDynamicQueue<THeap, T>, TIsConst, TIsVolatile> :
        BaseLINQCollection<THeap, Containers::BaseDynamicQueue<THeap, T>, T, false, TIsConst, TIsVolatile, true> {
    };

    template<typename THeap, typename T, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::BaseConcurencyDynamicQueue<THeap, T>, TIsConst, TIsVolatile> :
        BaseLINQCollection<THeap, Containers::BaseConcurencyDynamicQueue<THeap, T>, T, true, TIsConst, TIsVolatile, true> {
    };

    // DynamicStack
    template<typename THeap, typename T, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::BaseDynamicStack<THeap, T>, TIsConst, TIsVolatile> :
        BaseLINQCollection<THeap, Containers::BaseDynamicStack<THeap, T>, T, false, TIsConst, TIsVolatile, true> {
    };

    template<typename THeap, typename T, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::BaseConcurencyDynamicStack<THeap, T>, TIsConst, TIsVolatile> :
        BaseLINQCollection<THeap, Containers::BaseConcurencyDynamicStack<THeap, T>, T, true, TIsConst, TIsVolatile, true> {
    };

    // List
    template<typename THeap, typename T, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::BaseList<THeap, T>, TIsConst, TIsVolatile> :
        BaseLINQCollection<THeap, Containers::BaseList<THeap, T>, T, false, TIsConst, TIsVolatile, false> {
    };

    template<typename THeap, typename T, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::BaseConcurencyList<THeap, T>, TIsConst, TIsVolatile> :
        BaseLINQCollection<THeap, Containers::BaseConcurencyList<THeap, T>, T, true, TIsConst, TIsVolatile, false> {
    };
#pragma endregion

#pragma region Fixed collection
    // Base implementation
    template<typename THeap, typename TCollection, SizeT TSize, typename TItemType, bool TIsConcurencySupport, bool TIsConst, bool TIsVolatile, bool TIsFixedItemCount>
    struct BaseLINQFixedCollection :
        BaseICollection<
        THeap,
        TCollection,
        TItemType,
        TIsConcurencySupport,
        EType::FixedCollection,
        TIsConst,
        TIsVolatile,
        CTTITypeData<TItemType>::IsSerializable> {
        static constexpr auto ItemCount = TSize;
        static constexpr Boolean IsFixedItemCount = TIsFixedItemCount;
    };

    // Fixed Array
    template<typename THeap, typename T, SizeT TSize, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::BaseFixedArray<THeap, TSize, T>, TIsConst, TIsVolatile> :
        BaseLINQFixedCollection<THeap, Containers::BaseFixedArray<THeap, TSize, T>, TSize, T, false, TIsConst, TIsVolatile, true> {
    };

    template<typename THeap, typename T, SizeT TSize, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::BaseConcurencyFixedArray<THeap, TSize, T>, TIsConst, TIsVolatile> :
        BaseLINQFixedCollection<THeap, Containers::BaseConcurencyFixedArray<THeap, TSize, T>, TSize, T, true, TIsConst, TIsVolatile, true> {
    };

    // Fixed Queue
    template<typename THeap, typename T, SizeT TSize, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::BaseFixedQueue<THeap, TSize, T>, TIsConst, TIsVolatile> :
        BaseLINQFixedCollection<THeap, Containers::BaseFixedQueue<THeap, TSize, T>, TSize, T, false, TIsConst, TIsVolatile, false> {
    };

    template<typename THeap, typename T, SizeT TSize, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::BaseConcurencyFixedQueue<THeap, TSize, T>, TIsConst, TIsVolatile> :
        BaseLINQFixedCollection<THeap, Containers::BaseConcurencyFixedQueue<THeap, TSize, T>, TSize, T, true, TIsConst, TIsVolatile, false> {
    };

    // Fixed Stack
    template<typename THeap, typename T, SizeT TSize, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::BaseFixedStack<THeap, TSize, T>, TIsConst, TIsVolatile> :
        BaseLINQFixedCollection<THeap, Containers::BaseFixedStack<THeap, TSize, T>, TSize, T, false, TIsConst, TIsVolatile, false> {
    };

    template<typename THeap, typename T, SizeT TSize, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::BaseConcurencyFixedStack<THeap, TSize, T>, TIsConst, TIsVolatile> :
        BaseLINQFixedCollection<THeap, Containers::BaseConcurencyFixedStack<THeap, TSize, T>, TSize, T, true, TIsConst, TIsVolatile, false> {
    };
#pragma endregion

#pragma region Associable Collection
    // Base implementation
    template<typename THeap, typename TCollection, typename TPair, typename TKey, typename TValue, bool TIsConcurencySupport, bool TIsConst, bool TIsVolatile>
    struct BaseLINQAssociableCollection :
        BaseICollection<
        THeap,
        TCollection,
        TPair, TIsConcurencySupport,
        EType::AssociableCollection,
        TIsConst,
        TIsVolatile,
        CTTITypeData<TPair>::IsSerializable&& CTTITypeData<TKey>::IsSerializable&& CTTITypeData<TValue>::IsSerializable> {
        using PairType = TPair;
        using KeyType = TKey;
        using ValueType = TValue;

        using PairTypeMetaData = CTTITypeData<TPair>;
        using KeyTypeMetaData = CTTITypeData<TKey>;
        using ValueTypeMetaData = CTTITypeData<TValue>;
    };

    // Dictionary
    template<typename THeap, typename TKey, typename TValue, Containers::TCharacterComparator CharacterCompatator, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::BaseDictionary<THeap, TKey, TValue, CharacterCompatator>, TIsConst, TIsVolatile> :
        BaseLINQAssociableCollection<
        THeap,
        Containers::BaseDictionary<THeap, TKey, TValue, CharacterCompatator>,
        typename Containers::BaseDictionary<THeap, TKey, TValue, CharacterCompatator>::DataType,
        TKey,
        TValue,
        false,
        TIsConst,
        TIsVolatile> {
        static constexpr bool IsCaseSensitiveCharacterComparing = CharacterCompatator == Cardinal::Containers::DefaultCharacterComparator;
        static constexpr bool IsCaseInsensitiveCharacterComparing = CharacterCompatator == Cardinal::Containers::CaseInsesitiveCharacterComparator;
    };

    template<typename THeap, typename TKey, typename TValue, Containers::TCharacterComparator CharacterCompatator, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::BaseConcurencyDictionary<THeap, TKey, TValue, CharacterCompatator>, TIsConst, TIsVolatile> :
        BaseLINQAssociableCollection<
        THeap,
        Containers::BaseConcurencyDictionary<THeap, TKey, TValue, CharacterCompatator>,
        typename Containers::BaseConcurencyDictionary<THeap, TKey, TValue, CharacterCompatator>::DataType,
        TKey,
        TValue,
        true,
        TIsConst,
        TIsVolatile> {
        static constexpr bool IsCaseSensitiveCharacterComparing = CharacterCompatator == Cardinal::Containers::DefaultCharacterComparator;
        static constexpr bool IsCaseInsensitiveCharacterComparing = CharacterCompatator == Cardinal::Containers::CaseInsesitiveCharacterComparator;
    };

    // Map
    template<typename THeap, typename TKey, typename TValue, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::BaseMap<THeap, TKey, TValue>, TIsConst, TIsVolatile> :
        BaseLINQAssociableCollection<
        THeap,
        Containers::BaseMap<THeap, TKey, TValue>,
        typename Containers::BaseMap<THeap, TKey, TValue>::DataType,
        TKey,
        TValue,
        false,
        TIsConst,
        TIsVolatile> {
    };

    template<typename THeap, typename TKey, typename TValue, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::BaseConcurencyMap<THeap, TKey, TValue>, TIsConst, TIsVolatile> :
        BaseLINQAssociableCollection<
        THeap,
        Containers::BaseConcurencyMap<THeap, TKey, TValue>,
        typename Containers::BaseConcurencyMap<THeap, TKey, TValue>::DataType,
        TKey,
        TValue,
        true,
        TIsConst,
        TIsVolatile> {
    };
#pragma endregion

#pragma region Special Collection
    // BitMapT
    template<typename THeap, typename T, SizeT TSize, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::BaseBitMapT<THeap, TSize, T>, TIsConst, TIsVolatile>
        : BaseLINQFixedCollection<
        THeap,
        Containers::BaseBitMapT<THeap, TSize, T>,
        Containers::BaseBitMapT<THeap, TSize, T>::Lenght,
        T,
        false,
        TIsConst,
        TIsVolatile,
        true> {
        static constexpr SizeT FullBytesCount = Containers::BaseBitMapT<THeap, TSize, T>::FullClustersCount;
        static constexpr SizeT LastByteBitsCount = Containers::BaseBitMapT<THeap, TSize, T>::LastClusterBitsCount;
        static constexpr SizeT BitCountInFragment = Containers::BaseBitMapT<THeap, TSize, T>::BitCountInFragment;
    };

    // Tuple
    template<typename ... Types, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::Tuple<Types...>, TIsConst, TIsVolatile>
        : BaseCTTITypeDataImpl<
        Containers::Tuple<Types...>,
        EType::Tuple,
        Cardinal::TypeTraits::OrV(CTTITypeData<Types>::IsSerializable...),
        TIsConst,
        TIsVolatile
        > {
    };

    // Empty Tuple specialization
    template<bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Containers::Tuple<>, TIsConst, TIsVolatile>
        : BaseCTTITypeDataImpl<
        Containers::Tuple<>,
        EType::Tuple,
        true,
        TIsConst,
        TIsVolatile
        > {
    };
#pragma endregion
#pragma region Functionality
    // Optional
    template<typename T, bool TIsConst, bool TIsVolatile>
    struct CTTITypeDataImpl<Functionality::Optional<T>, TIsConst, TIsVolatile> :
        BaseCTTITypeDataImpl<Functionality::Optional<T>, EType::Box, CTTITypeData<T>::IsSerializable, TIsConst, TIsVolatile> {
        using ValueType = T;

        using ValueTypeMetaData = CTTITypeData<T>;
    };
#pragma endregion
}