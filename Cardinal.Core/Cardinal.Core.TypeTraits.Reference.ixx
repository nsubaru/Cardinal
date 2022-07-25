export module Cardinal.Core:TypeTraits.Reference;

import :TypeTraits.Base;

export namespace Cardinal::TypeTraits
{
#pragma region Check Ref modificators
	/// <summary>
    /// Returns "true" if type is "LValue"
    /// </summary>
    template <class>
    constexpr bool IsLValueReferenceV = false;

    /// <summary>
    /// Returns "true" if type is "LValue"
    /// </summary>
    template <class TTy>
    constexpr bool IsLValueReferenceV<TTy&> = true;

    /// <summary>
    /// Returns "true" if type is "LValue"
    /// </summary>
    template <class TTy>
    struct IsLValueReferenceT : LogicConst<IsLValueReferenceV<TTy>> {};

    /// <summary>
    /// Returns "true" if type is "RValue"
    /// </summary>
    template <class>
    constexpr bool IsRValueReferenceV = false;

    /// <summary>
    /// Returns "true" if type is "RValue"
    /// </summary>
    template <class TTy>
    constexpr bool IsRValueReferenceV<TTy&&> = true;

    /// <summary>
    /// Returns "true" if type is "RValue"
    /// </summary>
    template <class TTy>
    struct IsRValueReferenceT : LogicConst<IsRValueReferenceV<TTy>> {};

    /// <summary>
    /// Returns "true" if type is a reference
    /// </summary>
    template <class>
    constexpr bool IsReferenceV = false;

    /// <summary>
    /// Returns "true" if type is a reference
    /// </summary>
    template <class TTy>
    constexpr bool IsReferenceV<TTy&> = true;

    /// <summary>
    /// Returns "true" if type is a reference
    /// </summary>
    template <class TTy>
    constexpr bool IsReferenceV<TTy&&> = true;

    /// <summary>
    /// Returns "true" if type is a reference
    /// </summary>
    template <class TTy>
    struct IsReferenceT : LogicConst<IsReferenceV<TTy>> {};
#pragma endregion

#pragma region Remove Ref
	/// <summary>
    /// Removes reference from type
    /// </summary>
	template<typename T>
	struct RemoveRefT { using Type = T; };

	/// <summary>
    /// Removes reference from type
    /// </summary>
	template<typename T>
	struct RemoveRefT<T&> { using Type = T; };

	/// <summary>
    /// Removes "RValue" reference from type
    /// </summary>
	template<typename T>
	struct RemoveRefT<T&&> { using Type = T; };

	/// <summary>
    /// A type withoun reference
    /// </summary>
	template<typename T>
	using RemoveRefV = typename RemoveRefT<T>::Type;
#pragma endregion

#pragma region Add Ref
	/// <summary>
	/// Adds "LValue" and "RValue" references to type
	/// </summary>
	template<class TType,
		class = void>
		struct AddReference
	{	// Adding reference
		using Lvalue = TType;
		using Rvalue = TType;
	};

    /// <summary>
    /// Adds "LValue" and "RValue" references to type
    /// </summary>
	template<class TType>
	struct AddReference<TType, VoidT<TType&>>
	{	// Adding reference
		using Lvalue = TType&;
		using Rvalue = TType&&;
	};

    /// <summary>
    /// Adds "RValue" reference
    /// </summary>
	template<class TType>
	using AddRvalueReferenceV = typename AddReference<TType>::Rvalue;

    /// <summary>
    /// Adds "LValue" reference
    /// </summary>
	template<class TType>
	using AddLvalueReferenceV = typename AddReference<TType>::Lvalue;

    /// <summary>
    /// Grants defenition for type during template instantiation
    /// </summary>
	template<class TType>
	AddRvalueReferenceV<TType> DeclVal() noexcept { return {}; };
#pragma endregion
}