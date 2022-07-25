export module Cardinal.Core:TypeTraits.TypeConvertions;

import :TypeTraits.Base;
import :TypeTraits.Utils;

export namespace Cardinal::TypeTraits
{
    /// <summary>
    /// Checks if an object can be constructed with given arguments types
    /// </summary>
    template<class TTy,
        class... TArgs>
    struct IsConstructibleT
        : LogicConst<__is_constructible(TTy, TArgs...)>
    {	// Determine whether TTy(_Args...) is constructible
    };

    /// <summary>
    /// Checks if an object can be constructed with given arguments types
    /// </summary>
    template<class TTy,
        class... TArgs>
    inline constexpr bool IsConstructible = __is_constructible(TTy, TArgs...);

    /// <summary>
    /// Checks if one type can be transformed into another
    /// </summary>
    template<class TFrom,
        class TTo>
    struct IsConvertibleT
        : LogicConst<__is_convertible_to(TFrom, TTo)>
    {	// Determine whether _From is convertible to _To
    };

    /// <summary>
    /// Checks if one type can be transformed into another
    /// </summary>
    template<class TFrom,
        class TTo>
    inline constexpr bool IsConvertible = __is_convertible_to(TFrom, TTo);

    /// <summary>
    /// Checks if an object can be constructed with given arguments types and without generating exceptions
    /// </summary>
    template<class TTy,
        class... _Args>
    struct IsNothrowConstructibleT
        : LogicConst<__is_nothrow_constructible(TTy, _Args...)>
    {	// Determine whether TTy(_Args...) is nothrow constructible
    };

    /// <summary>
    /// Checks if an object can be constructed with given arguments types and without generating exceptions
    /// </summary>
    template<class TTy,
        class... _Args>
    inline constexpr bool IsNothrowConstructible = IsNothrowConstructibleT<TTy, _Args...>::State;

    /// <summary>
    /// Checks if value of first type can be assigned by another value
    /// </summary>
    template<class TTo,
        class TFrom>
    struct IsAssignableT
        : LogicConst<__is_assignable(TTo, TFrom)>
    {	// Determine whether _From can be assigned to _To
    };

    /// <summary>
    /// Checks if value of first type can be assigned by another value
    /// </summary>
    template<class TTo,
        class TFrom>
    inline constexpr bool IsAssignable = IsAssignableT<TTo, TFrom>::State;

    /// <summary>
    /// Checks if value of first type can be assigned by another value without generating exceptions
    /// </summary>
    template<class TTo,
        class TFrom>
    struct IsNothrowAssignableT
        : LogicConst<__is_nothrow_assignable(TTo, TFrom)>
    {	// Determine whether _From can be assigned to _To, nothrow
    };

    /// <summary>
    /// Checks if value of first type can be assigned by another value without generating exceptions
    /// </summary>
    template<class TTo,
        class TFrom>
    inline constexpr bool IsNothrowAssignable = __is_nothrow_assignable(TTo, TFrom);

    /// <summary>
    /// Checks if type has a constructor without arguments
    /// </summary>
    template<class TTy>
    struct IsDefaultConstructibleT
        : LogicConst<__is_constructible(TTy)>
    {	// Determine whether TTy has a default constructor
    };

    /// <summary>
    /// Checks if type has a constructor without arguments
    /// </summary>
    template<class TTy>
    inline constexpr bool IsDefaultConstructible = __is_constructible(TTy);

    /// <summary>
    /// Checks if type has an implicit constructor without arguments
    /// </summary>
    template<class TTy,
        class = void>
    struct IsImplicitlyDefaultConstructibleT
        : LogicFalse
    {	// Determine whether TTy is implicitly default constructible
    };

    namespace Details
    {
        /// <summary>
        /// Utility function for checking implicit object generation
        /// </summary>
        template<class TTy>
        void ImplicitlyDefaultConstruct(const TTy&) = delete;
    }

    /// <summary>
    /// Checks if type has an implicit constructor without arguments
    /// </summary>
    template<class TTy>
    struct IsImplicitlyDefaultConstructibleT < TTy, VoidT<decltype(Details::ImplicitlyDefaultConstruct<TTy>({})) >>
        : LogicTrue
    {	// Determine whether TTy is implicitly default constructible
    };

    /// <summary>
    /// Checks if type has an implicit constructor without arguments without generation exceptions
    /// </summary>
    template<class TTy>
    struct IsNothrowDefaultConstructibleT
        : LogicConst<__is_nothrow_constructible(TTy)>
    {	// Determine whether TTy has a nothrow default constructor
    };

    /// <summary>
    /// Checks if type has an implicit constructor without arguments without generation exceptions
    /// </summary>
    template<class TTy>
    inline constexpr bool IsNothrowDefaultConstructible = __is_nothrow_constructible(TTy);

    /// <summary>
    /// Checks if type has a move operator
    /// </summary>
    template<class TTy>
    struct IsMoveAssignableT
        : LogicConst<__is_assignable(AddLvalueReferenceV<TTy>, TTy)>
    {	// Determine whether TTy has a move assignment operator
    };

    template<class TTy>
    inline constexpr bool IsMoveAssignable = __is_assignable(AddLvalueReferenceV<TTy>, TTy);

    /// <summary>
    /// Checks if type has a move operator without generation exceptions
    /// </summary>
    template<class TTy>
    struct IsNothrowMoveAssignableT
        : LogicConst<__is_nothrow_assignable(AddLvalueReferenceV<TTy>, TTy)>
    {	// Determine whether TTy has a nothrow move assignment operator
    };

    /// <summary>
    /// Checks if type has a move operator without generation exceptions
    /// </summary>
    template<class TTy>
    inline constexpr bool IsNothrowMoveAssignable = __is_nothrow_assignable(AddLvalueReferenceV<TTy>, TTy);

    /// <summary>
    /// Checks if type has a utility move function
    /// </summary>
    template<class TTy1,
        class TTy2,
        class = void>
    struct SwappableWithHelper
        : LogicFalse
    {	// swap(declval<TTy1>(), declval<TTy2>()) is not valid
    };

    /// <summary>
    /// Checks if type has a utility move function
    /// </summary>
    template<class TTy1,
        class TTy2>
    struct SwappableWithHelper<TTy1, TTy2, VoidT<decltype(Swap(Declval<TTy1>(), Declval<TTy2>()))>>
        : LogicTrue
    {	// swap(declval<TTy1>(), declval<TTy2>()) is valid
    };

    /// <summary>
    /// Checks if two types can be swapped
    /// </summary>
    template<class TTy1,
        class TTy2>
    struct IsSwappableWith
        : LogicConst<
        SwappableWithHelper<TTy1, TTy2>::State&&
        SwappableWithHelper<TTy2, TTy1>::State>
    {	// Determine if expressions with type and value category TTy1 and TTy2
        // can be swapped (and vice versa)
    };

    /// <summary>
    /// Checks if type has a swap operation
    /// </summary>
    template<class TTy>
    struct IsSwappable
        : LogicConst<IsSwappableWith<
        AddLvalueReferenceV<TTy>,
        AddLvalueReferenceV<TTy>>::State>
    {	// Determine if TTy lvalues satisfy is_swappable_with
    };

    /// <summary>
    /// Returns "true" if type of variable can be assigned by copying
    /// </summary>
    template <class TTy>
    struct IsCopyAssignableT
        : LogicConst<__is_assignable(AddLvalueReferenceV<TTy>, AddLvalueReferenceV<const TTy>)> {
    };

    /// <summary>
    /// Returns "true" if type of variable can be assigned by copying
    /// </summary>
    template <class TTy>
    inline constexpr bool IsCopyAssignableV = __is_assignable(
        AddLvalueReferenceV<TTy>, AddLvalueReferenceV<const TTy>);

    /// <summary>
    /// True if type is abstract (have pure virtual method)
    /// </summary>
    /// <typeparam name="T">Type</typeparam>
    template<typename T>
    constexpr bool IsAbstract = __is_abstract(T);

    /// <summary>
    /// True if type all methods are abstract
    /// </summary>
    /// <typeparam name="T">Type</typeparam>
    template<typename T>
    constexpr bool IsInterface = __is_interface_class(T);

    /// <summary>
    /// True, if type have one or more virtual methods
    /// </summary>
    /// <typeparam name="T">Type</typeparam>
    template<typename T>
    constexpr bool IsHaveVirtualMethods = __is_polymorphic(T);

    /// <summary>
    /// True, if type final, this mean that no other type can inherit from that one
    /// </summary>
    /// <typeparam name="T">Type</typeparam>
    template<typename T>
    constexpr bool IsFinal = __is_sealed(T);

    /// <summary>
    /// True, if type is union, this mean that all fields use same adress
    /// </summary>
    /// <typeparam name="T">Type</typeparam>
    template<typename T>
    constexpr bool IsUnion = __is_union(T);

    /// <summary>
    /// True, if constructor without argument of type is autogen
    /// </summary>
    /// <typeparam name="T">Type</typeparam>
    template<typename T>
    constexpr bool HasTriviaCtor = __has_trivial_constructor(T);

    /// <summary>
    /// True, if copy constructor of type is autogen
    /// </summary>
    /// <typeparam name="T">Type</typeparam>
    template<typename T>
    constexpr bool HasTriviaCopyCtor = __has_trivial_copy(T);

    /// <summary>
    /// True, if destructor of type is autogen
    /// </summary>
    /// <typeparam name="T">Type</typeparam>
    template<typename T>
    constexpr bool HasTriviaDtor = __has_trivial_destructor(T);

    /// <summary>
    /// True, if destructor of type is not autogen
    /// </summary>
    /// <typeparam name="T">Type</typeparam>
    template<typename T>
    constexpr bool HasUserDtor = __has_user_destructor(T);

    /// <summary>
    /// True, if destructor of type is virtual is required for inheritance
    /// </summary>
    /// <typeparam name="T">Type</typeparam>
    template<typename T>
    constexpr bool HasVirtualDtor = __has_virtual_destructor(T);
}