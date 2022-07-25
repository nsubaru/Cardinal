export module Cardinal.Core:TypeTraits.Function;

import :TypeTraits.Base;

export namespace Cardinal::TypeTraits 
{
    /// <summary>
    /// Compile-time check if this type is a function
    /// </summary>
    template<typename>
    struct IsFunction : LogicFalse { };

    /// <summary>
    /// Compile-time check if this type is a function
    /// </summary>
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...)> : LogicTrue {};

    /// <summary>
    /// Compile-time check if this type is a function
    /// </summary>
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...)> : LogicTrue {};

    /// <summary>
    /// Compile-time check if this type is a function
    /// </summary>
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) const> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) volatile> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) const volatile> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) const> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) volatile> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) const volatile> : LogicTrue {};

    /// <summary>
    /// Specialization for function types that have ref-qualifiers
    /// </summary>
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...)&> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) const&> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) volatile&> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) const volatile&> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...)&> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) const&> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) volatile&> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) const volatile&> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...)&& > : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) const&&> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) volatile&&> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) const volatile&&> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...)&& > : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) const&&> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) volatile&&> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) const volatile&&> : LogicTrue {};

    /// <summary>
    /// Specializations for noexcept versions of all the above (C++17 and later)
    /// </summary>
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) const noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) volatile noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) const volatile noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) const noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) volatile noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) const volatile noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) & noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) const& noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) volatile& noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) const volatile& noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) & noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) const& noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) volatile& noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) const volatile& noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) && noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) const&& noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) volatile&& noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs...) const volatile&& noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) && noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) const&& noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) volatile&& noexcept> : LogicTrue {};
    template<typename TRet, typename... TArgs>
    struct IsFunction<TRet(TArgs..., ...) const volatile&& noexcept> : LogicTrue {};

    template<typename T>
    constexpr bool IsFunctionV = IsFunction<T>::State;
}