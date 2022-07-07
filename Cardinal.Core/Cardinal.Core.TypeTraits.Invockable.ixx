export module Cardinal.Core:TypeTraits.Invockable;

import :TypeTraits.Base;
import :TypeTraits.Utils;
import :TypeTraits.CVModificators;
import :TypeTraits.Function;
import :TypeTraits.TypeConvertions;

namespace Cardinal::Core::TypeTraits::Details
{
    template <class TF, class... TArgs>
    inline auto Invoke(TF&& f, TArgs&& ... args) ->
        decltype(ForwardReference<TF>(f)(ForwardReference<TArgs>(args)...)) {
        return ForwardReference<TF>(f)(ForwardReference<TArgs>(args)...);
    }

    template <class TBase, class T, class TDerived>
    inline auto Invoke(T TBase::* pmd, TDerived&& ref) ->
        decltype(ForwardReference<TDerived>(ref).*pmd) {
        return ForwardReference<TDerived>(ref).*pmd;
    }

    template <class PMD, class TPointer>
    inline auto Invoke(PMD&& pmd, TPointer&& ptr) ->
        decltype((*ForwardReference<TPointer>(ptr)).*ForwardReference<PMD>(pmd)) {
        return (*ForwardReference<TPointer>(ptr)).*ForwardReference<PMD>(pmd);
    }

    template <class TBase, class T, class TDerived, class... TArgs>
    inline auto Invoke(T TBase::* pmf, TDerived&& ref, TArgs&& ... args) ->
        decltype((ForwardReference<TDerived>(ref).*pmf)(ForwardReference<TArgs>(args)...)) {
        return (ForwardReference<TDerived>(ref).*pmf)(ForwardReference<TArgs>(args)...);
    }

    template <class PMF, class TPointer, class... TArgs>
    inline auto Invoke(PMF&& pmf, TPointer&& ptr, TArgs&& ... args) ->
        decltype(((*ForwardReference<TPointer>(ptr)).*ForwardReference<PMF>(pmf))(ForwardReference<TArgs>(args)...)) {
        return ((*ForwardReference<TPointer>(ptr)).*ForwardReference<PMF>(pmf))(ForwardReference<TArgs>(args)...);
    }

    template<typename Void, typename Callable, typename ... TTypes>
    struct InvokeTraits {
        using Invockable = LogicFalse;
        using Result = VoidT<>;
    };

    template<typename Callable, typename ... TTypes>
    struct InvokeTraits<VoidT<decltype(Invoke(DeclVal<Callable>(), DeclVal<TTypes>()...))>, Callable, TTypes...> {
        using Invockable = LogicTrue;
        using Result = decltype(Invoke(DeclVal<Callable>(), DeclVal<TTypes>()...));
    };
}

export namespace Cardinal::Core::TypeTraits
{
    /// <summary>
    /// Additional structure which indicates if this type is a class method or if method with this name exists in class
    /// </summary>
    template<typename T>
    struct IsMemberFunctionPointerHelper : LogicFalse {};

    /// <summary>
    /// Additional structure which indicates if this type is a class method or if method with this name exists in class
    /// </summary>
    template<typename TRet, typename TClass, typename ... TArgs>
    struct IsMemberFunctionPointerHelper<TRet(TClass::*)(TArgs...)> : IsFunction<TRet> {};

    /// <summary>
    /// Indicates whether this type is a pointer on a class method or if method with this name exists in class
    /// </summary>
    template<typename T>
    struct IsMemberFunctionPointerT : IsMemberFunctionPointerHelper<RemoveCv<T>> {};

    /// <summary>
    /// Indicates whether this type is a pointer on a class method or if method with this name exists in class
    /// </summary>
    /// <typeparam name="T">Pointer to a method</typeparam>
    template<typename T>
    constexpr bool IsMemberFunctionPointerV = IsMemberFunctionPointerT<T>::State;

    /// <summary>
    /// Type without reference
    /// </summary>
    template <typename, typename = void>
    struct ResultOfT {
        using Type = VoidT<>;
    };

    /// <summary>
    /// Type without reference
    /// </summary>
    template <typename TF, class...TArgs>
    struct ResultOfT<TF(TArgs...),
        decltype(void(Details::Invoke(TypeTraits::DeclVal<TF>(), TypeTraits::DeclVal<TArgs>()...)))> {
        using Type = decltype(Details::Invoke(TypeTraits::DeclVal<TF>(), TypeTraits::DeclVal<TArgs>()...));
    };

    /// <summary>
    /// Type without reference
    /// </summary>
    template<typename T>
    using ResultOfV = typename ResultOfT<T>::Type;

    /// <summary>
    /// Checks if "TCallable" type can be called with "TArgs" arguments list
    /// </summary>
    template<typename TCallable, typename ... TArgs>
    constexpr Boolean IsInvockable = Details::InvokeTraits<void, TCallable, TArgs...>::Invockable::State;

    /// <summary>
    /// Return type during "TCallable" call with "TArgs" arguments list
    /// </summary>
    template<typename TCallable, typename ... TArgs>
    using InvockableResult = typename Details::InvokeTraits<void, TCallable, TArgs...>::Result;

    /// <summary>
    /// Checks if "TCallable" type can be called with "TArgs" arguments list and "TRet" result
    /// </summary>
    template<typename TCallable, typename TRet, typename ... TArgs>
    constexpr Boolean IsInvockableWithResult = IsInvockable<TCallable, TArgs...> &&
        IsConvertible<TypeTraits::InvockableResult<TCallable, TArgs...>, TRet>;
}