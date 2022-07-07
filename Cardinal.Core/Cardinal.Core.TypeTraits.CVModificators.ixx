export module Cardinal.Core:TypeTraits.CVModificators;

import :TypeTraits.Base;
import :TypeTraits.Reference;

export namespace Cardinal::Core::TypeTraits
{
#pragma region Remove CV
    /// <summary>
    /// Returns type without "const" modifier
    /// </summary>
    template<typename T>
    struct RemoveConstT
    {
        using Type = T;
    };

    /// <summary>
    /// Returns type without "const" modifier
    /// </summary>
    template<typename T>
    struct RemoveConstT<const T>
    {
        using Type = T;
    };

    /// <summary>
    /// Returns type without "const" modifier
    /// </summary>
    template<typename T>
    using RemoveConst = typename RemoveConstT<T>::Type;

    /// <summary>
    /// Returns type without "volatile" modifier
    /// </summary>
    template<typename T>
    struct RemoveVolatileT
    {
        using Type = T;
    };

    /// <summary>
    /// Returns type without "volatile" modifier
    /// </summary>
    template<typename T>
    struct RemoveVolatileT<volatile T>
    {
        using Type = T;
    };

    /// <summary>
    /// Returns type without "volatile" modifier
    /// </summary>
    template<typename T>
    using RemoveVolatile = typename RemoveVolatileT<T>::Type;

    /// <summary>
    /// Returns type without "cv" modifiers
    /// </summary>
    template< class T >
    struct RemoveCvT
    {
        typedef typename RemoveVolatileT<typename RemoveConstT<T>::Type>::Type Type;
    };

    /// <summary>
    /// Returns type without "cv" modifiers
    /// </summary>
    template<typename T>
    using RemoveCv = typename RemoveCvT<T>::Type;

    /// <summary>
    /// Returns type without "cv" modifiers
    /// </summary>
    template<typename T>
    using RemoveCvRefV = RemoveCv<RemoveRefV<T>>;
#pragma endregion

#pragma region Add CV
    /// <summary>
    /// Adds "const" modifier to type
    /// </summary>
    template <class TTy>
    struct AddConstT {
        using Type = const TTy;
    };

    /// <summary>
    /// Adds "const" modifier to type
    /// </summary>
    template <class TTy>
    using AddConstV = typename AddConstT<TTy>::Type;

    /// <summary>
    /// Adds "volatile" modifier to type
    /// </summary>
    template <class TTy>
    struct AddVolatileT {
        using Type = volatile TTy;
    };

    /// <summary>
    /// Adds "volatile" modifier to type
    /// </summary>
    template <class TTy>
    using AddVolatileV = typename AddVolatileT<TTy>::Type;

    /// <summary>
    /// Adds "cv" modifiers to type
    /// </summary>
    template <class TTy>
    struct AddCvT {
        using Type = const volatile TTy;
    };

    /// <summary>
    /// Adds "cv" modifiers to type
    /// </summary>
    template <class TTy>
    using AddCvV = typename AddCvT<TTy>::Type;
#pragma endregion

#pragma region Check CV
    /// <summary>
    /// Returns if type has "const" modifier
    /// </summary>
    template<typename T>
    struct IsConstT : LogicFalse
    {};

    /// <summary>
    /// Returns if type has "const" modifier
    /// </summary>
    template<typename T>
    struct IsConstT<const T> : LogicTrue
    {};

    /// <summary>
    /// Returns if type has "const" modifier
    /// </summary>
    template<typename T>
    constexpr Boolean IsConstV = IsConstT<T>::State;

    /// <summary>
    /// Returns if type has "volatile" modifier
    /// </summary>
    template<typename T>
    struct IsVolatileT : LogicFalse
    {};

    /// <summary>
    /// Returns if type has "volatile" modifier
    /// </summary>
    template<typename T>
    struct IsVolatileT<volatile T> : LogicTrue
    {};

    /// <summary>
    /// Returns if type has "volatile" modifier
    /// </summary>
    template<typename T>
    constexpr Boolean IsVolatileV = IsVolatileT<T>::State;
#pragma endregion
}