export module Cardinal.Core:TypeTraits.TypeClasification;

import :TypeTraits.Base;
import :TypeTraits.NumericConverter;
import :TypeTraits.LogicOperations;

export namespace Cardinal::Core::TypeTraits
{
    /// <summary>
    /// Returns "true" if type is "POD"
    /// </summary>
    template<typename T>
    struct IsPodT : LogicConst<__is_pod(T)> {};

    /// <summary>
    /// Returns "true" if type is "POD"
    /// </summary>
    template<typename T>
    constexpr bool IsPodV = __is_pod(T);

    /// <summary>
    /// Returns "true" if type is integer
    /// </summary>
    template<typename T>
    constexpr bool IsIntegerNumber =
        TypeTraits::IsSigned<T> ||
        TypeTraits::IsUnsigned<T>;

    /// <summary>
    /// Returns "true" if type is real
    /// </summary>
    template<typename T>
    constexpr bool IsFloatPointNumber =
        TypeTraits::IsSameT<T, Float>::State ||
        TypeTraits::IsSameT<T, Double>::State;

    /// <summary>
    /// Returns "true" if type is numeric
    /// </summary>
    template<typename T>
    constexpr bool IsNumeric =
        IsIntegerNumber<T> ||
        IsFloatPointNumber<T>;

#pragma warning(push)
    //VS bug char and wchar_t trait as equal
#pragma warning(disable: 6287)
    /// <summary>
    /// Returns if type is symbolic
    /// </summary>
    template<typename T>
    constexpr bool IsCharacter =
        TypeTraits::IsSameT<T, AChar>() ||
        TypeTraits::IsSameT<T, UChar>() ||
        TypeTraits::IsSameT<T, U8Char>() ||
        TypeTraits::IsSameT<T, U16Char>() ||
        TypeTraits::IsSameT<T, U32Char>();
#pragma warning(pop)

    /// <summary>
    /// Returns if type is symbolic
    /// </summary>
    template<typename T>
    constexpr bool IsBoolean = TypeTraits::IsSameT<T, bool>();

    /// <summary>
    /// Returns "true" if type is "enum"
    /// </summary>
    template<typename T>
    struct IsEnumT : LogicConst<__is_enum(T)> {};

    /// <summary>
    /// Returns "true" if type is "enum"
    /// </summary>
    template<typename T>
    constexpr bool IsEnumV = __is_enum(T);

    /// <summary>
    /// Returns "true" if type is "enum"
    /// </summary>
    template<typename T>
    struct EnumUnderlyingTypeT {
        using Type = __underlying_type(T);
    };

    /// <summary>
    /// Returns numeric type "enum"
    /// </summary>
    template<typename T>
    using EnumUnderlyingTypeV = __underlying_type(T);

    /// <summary>
    /// Returns "true" if type does not contain static fields
    /// </summary>
    template <class TTy>
    struct IsEmptyT : LogicConst<__is_empty(TTy)> {};

    /// <summary>
    /// Returns "true" if type does not contain static fields
    /// </summary>
    template <class TTy>
    inline constexpr bool IsEmptyV = __is_empty(TTy);

    /// <summary>
    /// Returns "true" if class is marked with the modifier "final"
    /// </summary>
    template <class TTy>
    struct IsFinalT : LogicConst<__is_final(TTy)> {};

    /// <summary>
    /// Returns "true" if class is marked with the modifier "final"
    /// </summary>
    template <class TTy>
    constexpr bool IsFinalV = __is_final(TTy);

    /// <summary>
    /// Returns "true" if its a class or a structure
    /// </summary>
    template<typename T>
    constexpr bool IsClassOrStructV = __is_class(T);
}