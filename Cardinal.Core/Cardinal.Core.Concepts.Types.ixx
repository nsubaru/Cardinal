export module Cardinal.Core:Concepts.Types;

import :Concepts.Base;

export namespace Cardinal::Core::Concepts
{
	/// <summary>
	/// Checking if "T" is plain odd data
	/// </summary>
	template<typename T>
	concept IsPod = TypeTraits::IsPodV<T>;

	/// <summary>
	/// Checking if "T" is enumeration
	/// </summary>
	template<typename T>
	concept IsEnum = TypeTraits::IsEnumV<T>;

	/// <summary>
	/// Checking if "T" is integer numeric type 
	/// </summary>
	template<typename T>
	concept IsInteger = TypeTraits::IsSigned<T> || TypeTraits::IsUnsigned<T>;

	/// <summary>
	/// Checking if "T" is float point numeric type (float or double)
	/// </summary>
	template<typename T>
	concept IsFloat = TypeTraits::IsFloatPointNumber<T>;

	/// <summary>
	/// Checking if "T" is numeric type
	/// </summary>
	template<typename T>
	concept IsNumeric = TypeTraits::IsNumeric<T>;

	/// <summary>
	/// Checking if "T" is void type
	/// </summary>
	template<typename T>
	concept Void = TypeTraits::IsVoidV<T>;

	/// <summary>
	/// Checking if "T" is not void type
	/// </summary>
	template<typename T>
	concept NotVoid = !TypeTraits::IsVoidV<T>;

	/// <summary>
	/// Checking if "T" is signed numeric type
	/// </summary>
	template<typename T>
	concept SignedInteger = TypeTraits::IsSigned<T>;

	/// <summary>
	/// Checking if "T" is unsigned numeric type
	/// </summary>
	template<typename T>
	concept UnsignedInteger = TypeTraits::IsUnsigned<T>;

	/// <summary>
	/// Checking if "T" is integer numeric type
	/// </summary>
	template<typename T>
	concept Integer = TypeTraits::IsIntegerNumber<T>;

	/// <summary>
	/// Checking if "T" is float point numeric type
	/// </summary>
	template<typename T>
	concept FloatPointNumber = TypeTraits::IsFloatPointNumber<T>;

	/// <summary>
	/// Checking if "T" is boolean type
	/// </summary>
	template<typename T>
	concept IsLogicMetaType = TypeTraits::IsLogicConstV<T>;

	/// <summary>
	/// Checking if "T" is boolean type or have field T::State with value true
	/// </summary>
	template<typename T>
	concept LogicConstConcept = IsLogicMetaType<T> && T::State;

	/// <summary>
	/// Checking if "T" is numeric type
	/// </summary>
	template<typename T>
	concept Numeric = TypeTraits::IsNumeric<T>;
}