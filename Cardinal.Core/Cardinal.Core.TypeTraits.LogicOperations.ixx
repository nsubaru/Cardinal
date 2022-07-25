export module Cardinal.Core:TypeTraits.LogicOperations;

import :TypeTraits.Base;

export namespace Cardinal::TypeTraits {
#pragma region Conditional
    /// <summary>
    /// Compile-time - logic statement: if "Cond == True", returns "IfTrue" type, else "IfFalse"
    /// </summary>
    template<Boolean TCond, typename TIfTrue, typename TIfFalse>
    struct ConditionT { using Result = TIfFalse; };

    /// <summary>
    /// Compile-time - logic statement: if "Cond == True", returns "IfTrue" type, else "IfFalse"
    /// </summary>
    template<typename TIfTrue, typename TIfFalse>
    struct ConditionT<true, TIfTrue, TIfFalse> { using Result = TIfTrue; };

    /// <summary>
    /// Compile-time - logic statement result
    /// </summary>
    template<Boolean TCond, typename TIfTrue, typename TIfFalse>
    using ConditionV = typename ConditionT<TCond, TIfTrue, TIfFalse>::Result;
#pragma endregion

#pragma region EnableIf
    /// <summary>
    /// Generates "SFINAE" error if "TB false" and returns type if "true"
    /// </summary>
    template<bool TB, class T = void>
    struct EnableIf {};

    /// <summary>
    /// Generates "SFINAE" error if "TB false" and returns type if "true"
    /// </summary>
    template<class T>
    struct EnableIf<true, T> { typedef T Type; };

    /// <summary>
    /// Generates "SFINAE" error if "TB false" and returns type if "true"
    /// </summary>
    template<typename TFalseT, class T = void>
    struct EnableIfT {};

    /// <summary>
    /// Generates "SFINAE" error if "TB false" and returns type if "true"
    /// </summary>
    template<class T>
    struct EnableIfT<LogicTrue, T> { typedef T Type; };

    /// <summary>
    /// Generates "SFINAE" error if "TB false" and returns type if "true"
    /// </summary>
    template<bool TB, class T>
    using EnableIfV = typename EnableIf<TB, T>::Type;

    /// <summary>
    /// Generates "SFINAE" error if "TB false" and returns type if "true"
    /// </summary>
    template<typename B, typename T>
    using EnableIfTV = typename EnableIfT<B, T>::Type;
#pragma endregion

#pragma region And
    /// <summary>
    /// Logical "and" realization for metaprogramming
    /// </summary>
    template <bool FirstValue, class TFirst,
        class... TRest>
    struct AndTImpl { // Handles false trait or last trait
        using Result = TFirst;
    };

    /// <summary>
    /// Logical "and" realization for metaprogramming
    /// </summary>
    template <class TTrue, class TNext, class... TRest>
    struct AndTImpl<true, TTrue, TNext, TRest...> { // The first trait is true, try the next one
        using Result = typename AndTImpl<TNext::State, TNext, TRest...>::Result;
    };

    /// <summary>
    /// Logical "and" realization for metaprogramming
    /// </summary>
    template <class... TTypes>
    struct AndT : LogicTrue { // If _Traits is empty, LogicTrue
    };

    /// <summary>
    /// Logical "and" realization for metaprogramming
    /// </summary>
    template <class TFirst, class... TRest>
    struct AndT<TFirst, TRest...>
        : LogicConstT<typename AndTImpl<TFirst::State, TFirst, TRest...>::Result> { // Otherwise, if any of TRest are false, the first false
        // trait Otherwise, the last trait in TRest
    };

    /// <summary>
    /// Logical "and" realization for metaprogramming
    /// </summary>
    template <class... TTypes>
    constexpr bool And = AndT<TTypes...>::State;
#pragma endregion

#pragma region Negation
    /// <summary>
    /// Logical negation realization for metaprogramming
    /// </summary>
    template<class TType>
    struct NegationT
        : LogicConst<!static_cast<bool>(TType::State)>
    {	// The negated result of TType
    };

    /// <summary>
    /// Logical negation realization for metaprogramming
    /// </summary>
    template<class TType>
    inline constexpr bool Negation = NegationT<TType>::State;
#pragma endregion

#pragma region Or
    /// <summary>
    /// Logical "or" realization for metaprogramming
    /// </summary>
    template<bool TFirstValue,
        class TFirst,
        class... TRest>
    struct Disjunction
    {	// Handle true trait or last trait
        using Type = TFirst;
    };

    template<class TFalse,
        class TNext,
        class... TRest>
    struct Disjunction<false, TFalse, TNext, TRest...>
    {	// First trait is false, try the next trait
        using Type = typename Disjunction<TNext::State, TNext, TRest...>::Type;
    };

    template<class... TTypes>
    struct OrT
        : LogicFalse
    {	// If TTypes is empty, falseTType
    };

    template<class TFirst,
        class... TRest>
    struct OrT<TFirst, TRest...>
        : Disjunction<TFirst::State, TFirst, TRest...>::Type
    {	// Otherwise, if any of _Traits are true, the first true trait
        // Otherwise, the last trait in _Traits
    };

    template<class... TTypes>
    inline constexpr bool Or = OrT<TTypes...>::State;
#pragma endregion

#pragma region Logic operations with constexpr variable 
    /// <summary>
    /// Returns logical "and" from variables
    /// </summary>
    template<typename ... TItems>
    constexpr Boolean AndV(TItems ... items) {
        return (... && items);
    }

    /// <summary>
    /// Returns logical "or" from variables
    /// </summary>
    template<typename ... TItems>
    constexpr Boolean OrV(TItems ... items) {
        return (... || items);
    }
#pragma endregion
}