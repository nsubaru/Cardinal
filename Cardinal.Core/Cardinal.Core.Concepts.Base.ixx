export module Cardinal.Core:Concepts.Base;

export import :Types;
export import :TypeTraits;

export namespace Cardinal::Core::Concepts
{
	/// <summary>
	/// Checking if "U" type inherits by "T" type
	/// </summary>
	/// <typeparam name="TBase">Base class(interface)</typeparam>
	/// <typeparam name="TDerived">Inherited class(realization)</typeparam>
	template<typename TBase, typename TDerived>
	concept IsBasedOn = TypeTraits::IsBasedOnV<TBase, TDerived>;

	/// <summary>
	/// Checking if "T" and "U" types are the same
	/// </summary>
	template<typename T, typename U>
	concept IsSame = TypeTraits::IsSameV<T, U>;

	/// <summary>
	/// Checking if "T" can convert to "U"
	/// </summary>
	template<typename T, typename U>
	concept IsConvertible = TypeTraits::IsConvertible<T, U>;

	/// <summary>
	/// Checking if "T" have ctor with given "TArgs..." type pack
	/// </summary>
	template<typename T, typename ... TArgs>
	concept IsConstructibleWith = TypeTraits::IsConstructible<T, TArgs...>;

	/// <summary>
	/// Checking if "T" have ctor without arguments
	/// </summary>
	template<typename T>
	concept IsDefaultConstructible = TypeTraits::IsConstructible<T>;
}