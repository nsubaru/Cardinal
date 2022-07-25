export module Cardinal.Core:TypeTraits.Comparing;

import :TypeTraits.Base;
import :TypeTraits.Utils;
import :TypeTraits.LogicOperations;
import :TypeTraits.TypeConvertions;

export namespace Cardinal::TypeTraits
{
    template<typename T, typename U, typename = VoidT<>>
    struct HasEqualT
        : LogicFalse
    {};

    template<typename R, typename T, typename U, typename = VoidT<>>
    struct HasEqualRetT
        : LogicFalse
    {};

    template<typename T, typename U, typename = VoidT<>>
    struct HasNothrowEqualT
        : LogicFalse
    {};

    template<typename R, typename T, typename U, typename = VoidT<>>
    struct HasNothrowEqualRetT
        : LogicFalse
    {};

    template<typename T, typename U>
    struct HasEqualT<T, U, VoidT<decltype(DeclVal<T>() == DeclVal<U>())>>
        : LogicTrue
    {};

    template<typename R, typename T, typename U>
    struct HasEqualRetT<R, T, U, VoidT<decltype(DeclVal<T>() == DeclVal<U>())>>
        : IsConvertibleT<decltype(DeclVal<T>() == DeclVal<U>()), R>
    {};

    template<typename T, typename U>
    struct HasNothrowEqualT<T, U, VoidT<decltype(DeclVal<T>() == DeclVal<U>())>>
        : LogicConst<noexcept(DeclVal<T>() == DeclVal<U>())>
    {};

    template<typename R, typename T, typename U>
    struct HasNothrowEqualRetT<R, T, U, VoidT<decltype(DeclVal<T>() == DeclVal<U>())>>
        : LogicConst<(noexcept(DeclVal<T>() == DeclVal<U>()) && IsConvertible<decltype(DeclVal<T>() == DeclVal<U>()), R>)>
    {};

    // -------------------------------------------------------------------------------------------------------------------------------------------------------

    template<typename T, typename U, typename = VoidT<>>
    struct HasNotEqualT
        : LogicFalse
    {};

    template<typename R, typename T, typename U, typename = VoidT<>>
    struct HasNotEqualRetT
        : LogicFalse
    {};

    template<typename T, typename U, typename = VoidT<>>
    struct HasNothrowNotEqualT
        : LogicFalse
    {};

    template<typename R, typename T, typename U, typename = VoidT<>>
    struct HasNothrowNotEqualRetT
        : LogicFalse
    {};

    template<typename T, typename U>
    struct HasNotEqualT<T, U, VoidT<decltype(DeclVal<T>() != DeclVal<U>())>>
        : LogicTrue
    {};

    template<typename R, typename T, typename U>
    struct HasNotEqualRetT<R, T, U, VoidT<decltype(DeclVal<T>() != DeclVal<U>())>>
        : IsConvertibleT<decltype(DeclVal<T>() != DeclVal<U>()), R>
    {};

    template<typename T, typename U>
    struct HasNothrowNotEqualT<T, U, VoidT<decltype(DeclVal<T>() != DeclVal<U>())>>
        : LogicConst<noexcept(DeclVal<T>() != DeclVal<U>())>
    {};

    template<typename R, typename T, typename U>
    struct HasNothrowNotEqualRetT<R, T, U, VoidT<decltype(DeclVal<T>() != DeclVal<U>())>>
        : LogicConst<(noexcept(DeclVal<T>() != DeclVal<U>()) && IsConvertible<decltype(DeclVal<T>() != DeclVal<U>()), R>)>
    {};

    // -------------------------------------------------------------------------------------------------------------------------------------------------------

    template<typename T, typename U, typename = VoidT<>>
    struct HasLessT
        : LogicFalse
    {};

    template<typename R, typename T, typename U, typename = VoidT<>>
    struct HasLessRetT
        : LogicFalse
    {};

    template<typename T, typename U, typename = VoidT<>>
    struct HasNothrowLessT
        : LogicFalse
    {};

    template<typename R, typename T, typename U, typename = VoidT<>>
    struct HasNothrowLessRetT
        : LogicFalse
    {};

    template<typename T, typename U>
    struct HasLessT<T, U, VoidT<decltype(DeclVal<T>() < DeclVal<U>())>>
        : LogicTrue
    {};

    template<typename R, typename T, typename U>
    struct HasLessRetT<R, T, U, VoidT<decltype(DeclVal<T>() < DeclVal<U>())>>
        : IsConvertibleT<decltype(DeclVal<T>() < DeclVal<U>()), R>
    {};

    template<typename T, typename U>
    struct HasNothrowLessT<T, U, VoidT<decltype(DeclVal<T>() < DeclVal<U>())>>
        : LogicConst<noexcept(DeclVal<T>() < DeclVal<U>())>
    {};

    template<typename R, typename T, typename U>
    struct HasNothrowLessRetT<R, T, U, VoidT<decltype(DeclVal<T>() < DeclVal<U>())>>
        : LogicConst<(noexcept(DeclVal<T>() < DeclVal<U>()) && IsConvertible<decltype(DeclVal<T>() < DeclVal<U>()), R>)>
    {};

    // -------------------------------------------------------------------------------------------------------------------------------------------------------

    template<typename T, typename U, typename = VoidT<>>
    struct HasGreaterT
        : LogicFalse
    {};

    template<typename R, typename T, typename U, typename = VoidT<>>
    struct HasGreaterRetT
        : LogicFalse
    {};

    template<typename T, typename U, typename = VoidT<>>
    struct HasNothrowGreaterT
        : LogicFalse
    {};

    template<typename R, typename T, typename U, typename = VoidT<>>
    struct HasNothrowGreaterRetT
        : LogicFalse
    {};

    template<typename T, typename U>
    struct HasGreaterT<T, U, VoidT<decltype(DeclVal<T>() > DeclVal<U>())>>
        : LogicTrue
    {};

    template<typename R, typename T, typename U>
    struct HasGreaterRetT<R, T, U, VoidT<decltype(DeclVal<T>() > DeclVal<U>())>>
        : IsConvertibleT<decltype(DeclVal<T>() > DeclVal<U>()), R>
    {};

    template<typename T, typename U>
    struct HasNothrowGreaterT<T, U, VoidT<decltype(DeclVal<T>() > DeclVal<U>())>>
        : LogicConst<noexcept(DeclVal<T>() > DeclVal<U>())>
    {};

    template<typename R, typename T, typename U>
    struct HasNothrowGreaterRetT<R, T, U, VoidT<decltype(DeclVal<T>() > DeclVal<U>())>>
        : LogicConst<(noexcept(DeclVal<T>() > DeclVal<U>()) && IsConvertible<decltype(DeclVal<T>() > DeclVal<U>()), R>)>
    {};

    // -------------------------------------------------------------------------------------------------------------------------------------------------------

    template<typename T, typename U, typename = VoidT<>>
    struct HasLessEqualT
        : LogicFalse
    {};

    template<typename R, typename T, typename U, typename = VoidT<>>
    struct HasLessEqualRetT
        : LogicFalse
    {};

    template<typename T, typename U, typename = VoidT<>>
    struct HasNothrowLessEqualT
        : LogicFalse
    {};

    template<typename R, typename T, typename U, typename = VoidT<>>
    struct HasNothrowLessEqualRetT
        : LogicFalse
    {};

    template<typename T, typename U>
    struct HasLessEqualT<T, U, VoidT<decltype(DeclVal<T>() <= DeclVal<U>())>>
        : LogicTrue
    {};

    template<typename R, typename T, typename U>
    struct HasLessEqualRetT<R, T, U, VoidT<decltype(DeclVal<T>() <= DeclVal<U>())>>
        : IsConvertibleT<decltype(DeclVal<T>() <= DeclVal<U>()), R>
    {};

    template<typename T, typename U>
    struct HasNothrowLessEqualT<T, U, VoidT<decltype(DeclVal<T>() <= DeclVal<U>())>>
        : LogicConst<noexcept(DeclVal<T>() <= DeclVal<U>())>
    {};

    template<typename R, typename T, typename U>
    struct HasNothrowLessEqualRetT<R, T, U, VoidT<decltype(DeclVal<T>() <= DeclVal<U>())>>
        : LogicConst<(noexcept(DeclVal<T>() <= DeclVal<U>()) && IsConvertible<decltype(DeclVal<T>() <= DeclVal<U>()), R>)>
    {};

    // -------------------------------------------------------------------------------------------------------------------------------------------------------

    template<typename T, typename U, typename = VoidT<>>
    struct HasGreaterEqualT
        : LogicFalse
    {};

    template<typename R, typename T, typename U, typename = VoidT<>>
    struct HasGreaterEqualRetT
        : LogicFalse
    {};

    template<typename T, typename U, typename = VoidT<>>
    struct HasNothrowGreaterEqualT
        : LogicFalse
    {};

    template<typename R, typename T, typename U, typename = VoidT<>>
    struct HasNothrowGreaterEqualRetT
        : LogicFalse
    {};

    template<typename T, typename U>
    struct HasGreaterEqualT<T, U, VoidT<decltype(DeclVal<T>() >= DeclVal<U>())>>
        : LogicTrue
    {};

    template<typename R, typename T, typename U>
    struct HasGreaterEqualRetT<R, T, U, VoidT<decltype(DeclVal<T>() >= DeclVal<U>())>>
        : IsConvertibleT<decltype(DeclVal<T>() >= DeclVal<U>()), R>
    {};

    template<typename T, typename U>
    struct HasNothrowGreaterEqualT<T, U, VoidT<decltype(DeclVal<T>() >= DeclVal<U>())>>
        : LogicConst<noexcept(DeclVal<T>() >= DeclVal<U>())>
    {};

    template<typename R, typename T, typename U>
    struct HasNothrowGreaterEqualRetT<R, T, U, VoidT<decltype(DeclVal<T>() >= DeclVal<U>())>>
        : LogicConst<(noexcept(DeclVal<T>() >= DeclVal<U>()) && IsConvertible<decltype(DeclVal<T>() >= DeclVal<U>()), R>)>
    {};

    // -------------------------------------------------------------------------------------------------------------------------------------------------------

    template<typename T, typename U>
    inline constexpr auto HasEqualV = HasEqualT<T, U>::State;

    template<typename R, typename T, typename U>
    inline constexpr auto HasEqualRetV = HasEqualRetT<R, T, U>::State;

    template<typename T, typename U>
    inline constexpr auto HasNothrowEqualV = HasNothrowEqualT<T, U>::State;

    template<typename R, typename T, typename U>
    inline constexpr auto HasNothrowEqualRetV = HasNothrowEqualRetT<R, T, U>::State;

    // -------------------------------------------------------------------------------------------------------------------------------------------------------

    template<typename T, typename U>
    inline constexpr auto HasNotEqualV = HasNotEqualT<T, U>::State;

    template<typename R, typename T, typename U>
    inline constexpr auto HasNotEqualRetV = HasNotEqualRetT<R, T, U>::State;

    template<typename T, typename U>
    inline constexpr auto HasNothrowNotEqualV = HasNothrowNotEqualT<T, U>::State;

    template<typename R, typename T, typename U>
    inline constexpr auto HasNothrowNotEqualRetV = HasNothrowNotEqualRetT<R, T, U>::State;

    // -------------------------------------------------------------------------------------------------------------------------------------------------------

    template<typename T, typename U>
    inline constexpr auto HasLessv = HasLessT<T, U>::State;

    template<typename R, typename T, typename U>
    inline constexpr auto HasLessRetV = HasLessRetT<R, T, U>::State;

    template<typename T, typename U>
    inline constexpr auto HasNothrowLessTv = HasNothrowLessT<T, U>::State;

    template<typename R, typename T, typename U>
    inline constexpr auto HasNothrowLessRetV = HasNothrowLessRetT<R, T, U>::State;

    // -------------------------------------------------------------------------------------------------------------------------------------------------------

    template<typename T, typename U>
    inline constexpr auto HasGreaterTv = HasGreaterT<T, U>::State;

    template<typename R, typename T, typename U>
    inline constexpr auto HasGreaterRetV = HasGreaterRetT<R, T, U>::State;

    template<typename T, typename U>
    inline constexpr auto HasNothrowGreaterTv = HasNothrowGreaterT<T, U>::State;

    template<typename R, typename T, typename U>
    inline constexpr auto HasNothrowGreaterRetV = HasNothrowGreaterRetT<R, T, U>::State;

    // -------------------------------------------------------------------------------------------------------------------------------------------------------

    template<typename T, typename U>
    inline constexpr auto HasLessEqualTV = HasLessEqualT<T, U>::State;

    template<typename R, typename T, typename U>
    inline constexpr auto HasLessEqualRetV = HasLessEqualRetT<R, T, U>::State;

    template<typename T, typename U>
    inline constexpr auto HasNothrowLessEqualTV = HasNothrowLessEqualT<T, U>::State;

    template<typename R, typename T, typename U>
    inline constexpr auto HasNothrowLessEqualRetV = HasNothrowLessEqualRetT<R, T, U>::State;

    // -------------------------------------------------------------------------------------------------------------------------------------------------------

    template<typename T, typename U>
    inline constexpr auto HasGreaterEqualTV = HasGreaterEqualT<T, U>::State;

    template<typename R, typename T, typename U>
    inline constexpr auto HasGreaterEqualRetV = HasGreaterEqualRetT<R, T, U>::State;

    template<typename T, typename U>
    inline constexpr auto HasNothrowGreaterEqualTV = HasNothrowGreaterEqualT<T, U>::State;

    template<typename R, typename T, typename U>
    inline constexpr auto HasNothrowGreaterEqualRetV = HasNothrowGreaterEqualRetT<R, T, U>::State;


    template<typename T, typename U>
    struct IsComparable
        : LogicConst<
        And<
        HasEqualT<T, U>,
        HasNotEqualT<T, U>,
        HasLessT<T, U>,
        HasGreaterT<T, U>,
        HasLessEqualT<T, U>,
        HasGreaterEqualT<T, U>
        > // And
        > // LogicConst
    {}; // IsComparable

    template<typename R, typename T, typename U>
    struct IsComparableRet
        : LogicConst<
        And<
        HasEqualRetT<R, T, U>,
        HasNotEqualRetT<R, T, U>,
        HasLessRetT<R, T, U>,
        HasGreaterRetT<R, T, U>,
        HasLessEqualRetT<R, T, U>,
        HasGreaterEqualRetT<R, T, U>
        > // And
        > // LogicConst
    {}; // IsComparableRet

    template<typename T, typename U>
    struct IsNothrowComparable
        : LogicConst<
        And<
        HasNothrowEqualT<T, U>,
        HasNothrowNotEqualT<T, U>,
        HasNothrowLessT<T, U>,
        HasNothrowGreaterT<T, U>,
        HasNothrowLessEqualT<T, U>,
        HasNothrowGreaterEqualT<T, U>
        > // And
        > // LogicConst
    {}; // IsNothrowComparable

    template<typename R, typename T, typename U>
    struct IsNothrowComparableRet
        : LogicConst<
        And<
        HasNothrowEqualRetT<R, T, U>,
        HasNothrowNotEqualRetT<R, T, U>,
        HasNothrowLessRetT<R, T, U>,
        HasNothrowGreaterRetT<R, T, U>,
        HasNothrowLessEqualRetT<R, T, U>,
        HasNothrowGreaterEqualRetT<R, T, U>
        > // And
        > // LogicConst
    {}; // IsNothrowComparableRet

    template<typename T, typename U>
    inline constexpr auto IsComparableV = IsComparable<T, U>::State;

    template<typename R, typename T, typename U>
    inline constexpr auto IsComparableRetV = IsComparableRet<R, T, U>::State;

    template<typename T, typename U>
    inline constexpr auto IsNothrowComparableV = IsNothrowComparable<T, U>::State;

    template<typename R, typename T, typename U>
    inline constexpr auto IsNothrowComparableRetV = IsNothrowComparableRet<R, T, U>::State;
}