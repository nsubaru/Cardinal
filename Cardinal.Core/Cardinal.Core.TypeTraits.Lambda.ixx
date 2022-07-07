export module Cardinal.Core:TypeTraits.Lambda;

import :TypeTraits.Base;
import :TypeTraits.Pack;
import :TypeTraits.LogicOperations;

namespace Cardinal::Core::TypeTraits::DetailsLambda
{
    /// <summary>
    /// Defining lambda types
    /// </summary>
    /// <typeparam name="TRet">Lambda return type</typeparam>
    /// <typeparam name="TCls">Lambda class type</typeparam>
    /// <typeparam name="TIsMutable">Is lambda "mutable"</typeparam>
    /// <typeparam name="TLambdaArgs">Lambda arguments</typeparam>
    template<class TRet, class TCls, class TIsMutable, class... TLambdaArgs>
    struct Lambda
    {
        /// <summary>
        /// Is labmda "mutable"
        /// </summary>
        using IsMutableT = TIsMutable;

        /// <summary>
        /// Is labmda "mutable"
        /// </summary>
        static constexpr auto IsMutable = TIsMutable::State;

        /// <summary>
        /// Number of arguments
        /// </summary>
        static constexpr auto Arity = sizeof...(TLambdaArgs);

        /// <summary>
        /// Lambda return type
        /// </summary>
        using ReturnType = TypeTraits::ConditionV<TypeTraits::IsVoidV<TRet>, TypeTraits::VoidT<void>, TRet>;

        /// <summary>
        /// Lambda arguments packets
        /// </summary>
        using Args = TypeTraits::Pack<TLambdaArgs...>;

        /// <summary>
        /// "LogicTrue" if lambda result can be transformed into "TExpectedTRet" type
        /// </summary>
        template<typename TExpectedTRet>
        using IsTRetTypeConvertibleTo = typename
            TypeTraits::ConditionT<
            TypeTraits::IsVoidV<ReturnType>,
            TypeTraits::IsVoidT<TExpectedTRet>,
            TypeTraits::IsConvertibleT<ReturnType, TExpectedTRet>
            >::Result;

        /// <summary>
        /// If possible to call lambda with the following set of types
        /// </summary>
        template<typename ... TParams>
        using CanCallWith = typename TypeTraits::Pack<TLambdaArgs...>::template IsConvertibleFromTypePackT<TParams...>;

        /// <summary>
        /// If possible to transform the argument and the return type
        /// </summary>
        template<typename TExpected, typename ... TParams>
        using IsConvertible = TypeTraits::AndT<IsTRetTypeConvertibleTo<TExpected>, CanCallWith<TParams...>>;

        /// <summary>
        /// Lambda function sign
        /// </summary>
        using LambdaFunctionSign = TRet(*)(TLambdaArgs...);
    };
}

export namespace Cardinal::Core::TypeTraits
{
    /// <summary>
    /// Defining lambda types
    /// </summary>
    /// <typeparam name="TLd">Lambda type</typeparam>
    template<class TLd>
    struct LambdaType
        : LambdaType<decltype(&TLd::operator())>
    {};

    /// <summary>
    /// Defining lambda types
    /// </summary>
    /// <typeparam name="TRet">Lambda return type</typeparam>
    /// <typeparam name="TCls">Lambda class type</typeparam>
    /// <typeparam name="TArgs">Lambda arguments</typeparam>
    template<class TRet, class TCls, class... TArgs>
    struct LambdaType<TRet(TCls::*)(TArgs...)>
        : DetailsLambda::Lambda<TRet, TCls, LogicTrue, TArgs...>
    {};

    /// <summary>
    /// Defining lambda types
    /// </summary>
    /// <typeparam name="TRet">Lambda return type</typeparam>
    /// <typeparam name="TCls">Lambda class type</typeparam>
    /// <typeparam name="TArgs">Lambda arguments</typeparam>
    template<class TRet, class TCls, class... TArgs>
    struct LambdaType<TRet(TCls::*)(TArgs...) const>
        : DetailsLambda::Lambda<TRet, TCls, LogicFalse, TArgs...>
    {};

    /// <summary>
    /// If lambda return type and arguments matches
    /// </summary>
    /// <typeparam name="TLd">Lambda type</typeparam>
    /// <typeparam name="TRet">Expected return type</typeparam>
    /// <typeparam name="TParams">Expected arguments types</typeparam>
    template<class TLd, typename TRet, typename ... TParams>
    struct IsLambdaConvertibleT
        :LogicConstT<
        AndT<
        IsSameT<typename LambdaType<TLd>::ReturnType, TRet>,
        IsSameT<typename LambdaType<TLd>::Args, Pack<TParams...>>
        >
        > {};

    /// <summary>
    /// If lambda return type and arguments matches 
    /// </summary>
    /// <typeparam name="TLd">Lambda type</typeparam>
    /// <typeparam name="TRet">Expected return type</typeparam>
    /// <typeparam name="TParams">Expected arguments types</typeparam>
    template<class TLd, typename TRet, typename ... TParams>
    constexpr bool IsLambdaConvertibleV = IsLambdaConvertibleT<TLd, TRet, TParams...>::State;

    /// <summary>
    /// Checks if lambda has capture list
    /// </summary>
    template<typename T>
    static constexpr bool IsLambdaHaveCapture = !(__is_convertible_to(T, typename LambdaType<T>::LambdaFunctionSign));
}