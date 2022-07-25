export module Cardinal.Core:TypeTraits.Utils;

import :TypeTraits.Base;
import :TypeTraits.Reference;

export namespace Cardinal::TypeTraits
{
	/// <summary>
	/// Creating virtual variable (is needed for pattern matching by types)
	/// </summary>
	template<class TTy>
	AddRvalueReferenceV<TTy> Declval() noexcept { return {}; }
}

export namespace Cardinal
{
	/// <summary>
	/// Object moving function (rvalue transitioning)
	/// </summary>
	/// <typeparam name="T">Object type(without rvalue)</typeparam>
	/// <param name="arg">"lvalue" reference to "rvalue" object</param>
	/// <returns>"rvalue" reference to an object</returns>
	template <typename T>
	__forceinline constexpr TypeTraits::RemoveRefV<T>&& MoveRValue(T&& arg) {
		return static_cast<TypeTraits::RemoveRefV<T>&&>(arg);
	}

	/// <summary>
	/// Object moving function (rvalue transitioning)
	/// </summary>
	/// <typeparam name="T">Object type(without rvalue)</typeparam>
	/// <param name="arg">"lvalue" reference to "rvalue" object</param>
	/// <returns>"rvalue" reference to an object</returns>
	template <typename T>
	__forceinline constexpr const TypeTraits::RemoveRefV<T>&& MoveRValue(const T&& arg) {
		return static_cast<const TypeTraits::RemoveRefV<T>&&>(arg);
	}

	/// <summary>
	/// Perfect transition
	/// </summary>
	/// <typeparam name="TType">Object type without "rvalue"</typeparam>
	/// <param name="arg">"lvalue" reference to a "rvalue" object</param>
	/// <returns>"rvalue" reference to an object</returns>
	template<typename TType>
	[[nodiscard]]
	__forceinline constexpr TType&& ForwardReference(TypeTraits::RemoveRefV<TType>& arg) noexcept {
		// Forward an lvalue as either an lvalue or an rvalue
		return (static_cast<TType&&>(arg));
	}

	/// <summary>
	/// Perfect transition
	/// </summary>
	/// <typeparam name="TType">Object type without "rvalue"</typeparam>
	/// <param name="arg">"lvalue" reference to a "rvalue" object</param>
	/// <returns>"rvalue" reference to an object</returns>
	template<typename TType>
	[[nodiscard]]
	__forceinline constexpr TType&& ForwardReference(TypeTraits::RemoveRefV<TType>&& arg) noexcept {
		// forward an rvalue as an rvalue
		static_assert(!TypeTraits::IsLValueReferenceV<TType>(), "Bad forward call");
		return (static_cast<TType&&>(arg));
	}
}