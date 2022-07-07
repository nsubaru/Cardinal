export module Cardinal.Core:TypeTraits.Pack;

import :TypeTraits.Base;
import :TypeTraits.LogicOperations;
import :TypeTraits.TypeConvertions;

export namespace Cardinal::Core::TypeTraits
{
    /// <summary>
    /// A types pack (special construction for parameters packets enumeration)
    /// </summary>
    template<typename ... T>
    struct PackItems {};

    /// <summary>
    /// A types pack (special construction for parameters packets enumeration)
    /// </summary>
    template<SizeT I, typename T>
    struct PackItem {
        static inline constexpr SizeT Index = I;
        using Type = T;
    };

    /// <summary>
    /// A types pack (special construction for parameters packets enumeration)
    /// </summary>
    template<SizeT I>
    struct PackItem<I, PackItems<>>;

    /// <summary>
    /// A types pack (special construction for parameters packets enumeration)
    /// </summary>
    template<typename T, typename ...TTypes>
    struct PackItem<0, PackItems<T, TTypes...>> {
        typedef T Type;
    };

    /// <summary>
    /// A types pack (special construction for parameters packets enumeration)
    /// </summary>
    template<SizeT I, typename T, typename ... TTypes>
    struct PackItem<I, PackItems<T, TTypes...>> {
        typedef typename PackItem<I - 1, TTypes...>::Type Type;
    };


    /// <summary>
    /// An indexer which generates a list of numbers
    /// </summary>
    template<SizeT ...>
    struct Indexator {};

    /// <summary>
    /// An indexer which generates a list of numbers
    /// </summary>
    template<SizeT TStart, typename TIndexatorT, SizeT TEnd>
    struct IndexerImpl;

    /// <summary>
    /// An indexer which generates a list of numbers
    /// </summary>
    template<SizeT TEnd, SizeT ... TI>
    struct IndexerImpl<TEnd, Indexator<TI...>, TEnd> {
        typedef Indexator<TI...> Type;
    };

    /// <summary>
    /// An indexer which generates a list of numbers
    /// </summary>
    template<SizeT TStart, SizeT ... I, SizeT TEnd>
    struct IndexerImpl<TStart, Indexator<I...>, TEnd> {
        typedef typename IndexerImpl<TStart + 1, Indexator<TStart, I...>, TEnd>::Type Type;
    };

    /// <summary>
    /// An indexer which generates a list of numbers
    /// </summary>
    template<SizeT TEnd, SizeT TStart = 0>
    struct Indexer {
        typedef typename IndexerImpl<TStart, Indexator<>, TEnd>::Type Type;
    };

    /// <summary>
    /// An indexer for a types pack
    /// </summary>
    template<typename TIndexator, typename ... TTypes>
    struct PackIndexator;

    /// <summary>
    /// An indexer for a types pack
    /// </summary>
    template<SizeT ...I, typename ...TTypes>
    struct PackIndexator<Indexator<I...>, TTypes...> : PackItem<I, TTypes>... {};

    /// <summary>
    /// Pack realization
    /// </summary>
    template<SizeT TCount, typename ... TTypes>
    struct PackImpl {
        typedef PackIndexator<typename Indexer<TCount>::Type, TTypes...> Items;
    };

    /// <summary>
    /// A types pack
    /// </summary>
    template<typename ... TTypes>
    struct Pack
    {
        /// <summary>
        /// A types cortege
        /// </summary>
        typedef typename PackImpl<sizeof...(TTypes), TTypes...>::Items Items;
        static inline constexpr SizeT Count = sizeof...(TTypes);

        /// <summary>
        /// If types in packs transforms from types cortege
        /// </summary>
        template<typename ...TArgs>
        struct IsConvertibleFromTypePackT :
            ConditionT<
            sizeof...(TTypes) == sizeof...(TArgs),
            LogicConst<AndT<IsConvertibleT<TTypes, TArgs>...>::State>,
            LogicFalse
            >::Result
        {};

        /// <summary>
        /// If types in packs transforms from types cortege
        /// </summary>
        template<typename ...TArgs>
        static inline constexpr Boolean IsConvertibleFromTypePackV = IsConvertibleFromTypePackT<TArgs...>::State;
    };
}