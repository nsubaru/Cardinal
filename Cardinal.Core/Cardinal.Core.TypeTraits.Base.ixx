export module Cardinal.Core:TypeTraits.Base;

export import :Types;

/// <summary>
/// Compile-time verifications namespace
/// </summary>
export namespace Cardinal::Core::TypeTraits 
{
    /// <summary>
    /// Void abstract type
    /// </summary>
    template<class... TTypes>
	using VoidT = void;

	/// <summary>
	/// Logic constant template, used for compile-time logic statements
	/// </summary>
	template<bool Value>
	struct LogicConst
	{
        /// <summary>
        /// Cast value to boolean
        /// </summary>
        __forceinline constexpr operator bool() const { return Value; }

        /// <summary>
        /// Value of entity
        /// </summary>
        static constexpr bool State = Value;
	};

    /// <summary>
    /// Logic "True" for compile-time verifications
    /// </summary>
    using LogicTrue = LogicConst<true>;

    /// <summary>
    /// Logic "False" for compile-time verifications
    /// </summary>
    using LogicFalse = LogicConst<false>;

    /// <summary>
    /// Logic statement, if type T == U is compile-time executable
    /// </summary>
    template<typename T, typename U>
    struct IsSameT : LogicFalse {};

    /// <summary>
    /// Logic statement, if type T == U is compile-time executable
    /// </summary>
    template<typename T>
    struct IsSameT<T, T> : LogicTrue {};

    /// <summary>
    /// Returns true is types are the same
    /// </summary>
    template<typename T, typename U>
    constexpr bool IsSameV = IsSameT<T, U>::State;

	/// <summary>
	/// Logic constant template, used for compile-time logic statements
	/// </summary>
    template<typename TValue> requires requires { {TValue::State}; } 
	struct LogicConstT
	{
		/// <summary>
		/// Cast value to boolean
		/// </summary>
		__forceinline constexpr operator bool() const { return TValue::State; }

		/// <summary>
		/// Value of entity
		/// </summary>
		static constexpr bool State = TValue::State;
	};

    static_assert(LogicConstT<LogicConst<true>>::State);

    /// <summary>
    /// A logical judgment, whether type T is the result of a logical judgment
    /// </summary>
    template<typename T>
    struct IsLogicConstT : LogicConst<false> {};

    /// <summary>
    /// Logic statement, if type is LogicConst
    /// </summary>
    template<bool Value>
    struct IsLogicConstT<LogicConst<Value>> : LogicConst<true> {};

    /// <summary>
    /// Logic statement, if type is LogicConst of some type (for recursive validation)
    /// </summary>
    template<typename T>
    struct IsLogicConstT<LogicConstT<T>> : LogicConst<true> {};

    /// <summary>
    /// Logic statement, if type is LogicConst of some type
    /// </summary>
    template<typename T>
    constexpr bool IsLogicConstV = IsLogicConstT<T>::State;

    /// <summary>
    /// Compile-time - logic statement: is class "Derived" inherits class "Base" - returns "True", else returns "False"
    /// </summary>
    template<typename TBase, typename TDerived>
    struct IsBasedOn :LogicConst<__is_base_of(TBase, TDerived)> {};

    /// <summary>
    /// Compile-time - logic statement: is class "Derived" inherits class "Base" - returns "True", else returns "False"
    /// </summary>
    template<typename TBase, typename TDerived>
    constexpr Boolean IsBasedOnV = __is_base_of(TBase, TDerived);

    /// <summary>
    /// Returns true only if executes during compile-time
    /// </summary>
    [[nodiscard]] constexpr bool IsCompileTimeEvaluating() noexcept
    {
        return __builtin_is_constant_evaluated();
    }

    /// <summary>
    /// Returns if type is "void"
    /// </summary>
    template<class TTy>
    struct IsVoidT : LogicFalse {};

    /// <summary>
    /// Returns if type is "void"
    /// </summary>
    template<>
    struct IsVoidT<void> : LogicTrue {};

    /// <summary>
    /// Returns if type is "void"
    /// </summary>
    template<>
    struct IsVoidT<const void> : LogicTrue {};

    // Returns if type is "void"
    template<>
    struct IsVoidT<volatile void> : LogicTrue {};

    /// <summary>
    /// Returns if type is "void"
    /// </summary>
    template<>
    struct IsVoidT<const volatile void> : LogicTrue {};

    /// <summary>
    /// Returns if type is "void"
    /// </summary>
    template<class TTy>
    inline constexpr bool IsVoidV = IsVoidT<TTy>::State;

    /// <summary>
    /// Nesessary for type definition during template instantiation
    /// </summary>
    template <class TTy>
    struct IdentityT {
        using Type = TTy;
    };

    /// <summary>
    /// Nesessary for type definition during template instantiation
    /// </summary>
    template <class TTy>
    using IdentitV = typename IdentityT<TTy>::Type;

	/// <summary>
    /// Structure for tagging functions and methods by type tag
	/// </summary>
	template<typename T>
	struct __declspec(empty_bases) TypeTag {
		/// <summary>
		/// Tagging type
		/// </summary>
		using Type = T;
	};
}