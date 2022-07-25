export module Cardinal.Core:EnumFlags;

import :Types;
import :BitOperations;
import :Math;
import :TypeTraits;
import :Concepts;

namespace Cardinal
{
	/// <summary>
	/// A bit fields class based on enumerations
	/// </summary>
	/// <typeparam name="T">Enumeration type</typeparam>
	template<typename T, typename TValueType = typename TypeTraits::EnumUnderlyingTypeV<T>> requires Concepts::IsEnum<T>
	struct EnumFlags {
	public:
		using ValueType = typename TypeTraits::EnumUnderlyingTypeV<T>;

	public:
		ValueType Value;

	private:
		constexpr EnumFlags(ValueType value) : Value(value) {}

	public:
		/// <summary>
		/// Empty constructor
		/// </summary>
		constexpr EnumFlags() : Value(0) {}

		/// <summary>
		/// Empty constructor
		/// </summary>
		/// <param name="value">Bit index</param>
		constexpr EnumFlags(T value) : Value(0) {
			Bits::BitSet(this->Value, static_cast<ValueType>(value));
		}

	public:
		/// <summary>
		/// Bit addition operator
		/// </summary>
		/// <param name="item">Set of bits</param>
		/// <returns>New value</returns>
		constexpr EnumFlags operator|(EnumFlags item) const {
			return EnumFlags(this->Value | item.Value);
		}

		/// <summary>
		/// Bit addition operator
		/// </summary>
		/// <param name="item">Bit index</param>
		/// <returns>New value</returns>
		constexpr EnumFlags operator|(T item) const {
			auto newValue = this->Value;
			Bits::BitSet(newValue, static_cast<ValueType>(item));
			return EnumFlags(newValue);
		}

	public:
		/// <summary>
		/// Checking if enum values is enabled operator
		/// </summary>
		/// <param name="item">Set of bits</param>
		/// <returns>New value</returns>
		constexpr bool operator&(EnumFlags item) const {
			return this->Value == item.Value;
		}

		/// <summary>
		/// Checking if enum value is enabled operator
		/// </summary>
		/// <param name="item">Bit index</param>
		/// <returns>New value</returns>
		constexpr bool operator&(T item) const {
			return Bits::BitCheck(this->Value, static_cast<ValueType>(item));
		}

	public:
		/// <summary>
		/// Addition operator to existing field
		/// </summary>
		/// <param name="item">Bit index</param>
		/// <returns>New value</returns>
		constexpr EnumFlags& operator|=(T item) {
			Bits::BitSet(this->Value, static_cast<ValueType>(item));
			return *this;
		}

		/// <summary>
		/// Addition operator to existing field
		/// </summary>
		/// <param name="item">Set of bits</param>
		/// <returns>New value</returns>
		constexpr EnumFlags& operator|=(EnumFlags item) {
			this->value |= item.Value;
			return *this;
		}

	public:
		/// <summary>
		/// Returns a set of values object
		/// </summary>
		/// <returns>Set of values</returns>
		constexpr ValueType GetValue() const { return this->Value; }

	public:
		/// <summary>
		/// Equality operator between two enum flags
		/// </summary>
		/// <param name="flags">Right operand</param>
		/// <returns>Logic result of equality</returns>
		constexpr bool operator==(EnumFlags flags) {
			return this->Value == flags.Value;
		}

		/// <summary>
		/// Inequality operator between two enum flags
		/// </summary>
		/// <param name="flags">Right operand</param>
		/// <returns>Logic result of inequality</returns>
		constexpr bool operator!=(EnumFlags flags) {
			return this->Value != flags.Value;
		}

		/// <summary>
		/// 3-way comparing operator between EnumFlags
		/// </summary>
		/// <param name="other">Second operand</param>
		/// <returns>Relative order between items</returns>
		constexpr auto operator<=>(const EnumFlags& other) const {
			return this->Value <=> other.Value;
		}

	public:
		constexpr friend EnumFlags operator|(T item, EnumFlags rights);
	};

	/// <summary>
	/// Bit addition operator
	/// </summary>
	/// <typeparam name="T">Enumeration type</typeparam>
	/// <param name="item">Bit index</param>
	/// <param name="rights">Set of bits</param>
	/// <returns>New value</returns>
	template<typename T> requires Concepts::IsEnum<T>
	inline constexpr EnumFlags<T> operator|(T item, EnumFlags<T> rights) {
		auto newValue = rights.Value;
		Bits::BitSet(newValue, static_cast<typename EnumFlags<T>::ValueType>(item));
		return EnumFlags(newValue);
	}

	/// <summary>
	/// Checking if enum value is enabled operator
	/// </summary>
	/// <typeparam name="T">Enumeration type</typeparam>
	/// <param name="item1">Left enum variable</param>
	/// <param name="item2">Right enum flags variable</param>
	/// <returns>true if value enabled</returns>
	template<typename T> requires Concepts::IsEnum<T>
	inline constexpr bool operator&(T item, EnumFlags<T> right) {
		return Bits::BitCheck(right.Value, static_cast<typename EnumFlags<T>::ValueType>(item));
	}

	/// <summary>
	/// Bit combining operator
	/// </summary>
	/// <typeparam name="T">Enumeration type</typeparam>
	/// <param name="item1">Left enum variable</param>
	/// <param name="item2">Right enum variable</param>
	/// <returns>Combined enum flag</returns>
	template<typename U> requires Concepts::IsEnum<U>
	constexpr EnumFlags<U> operator|(U item1, U item2)
	{
		return (EnumFlags<U>() | item1) | item2;
	}

	namespace TypeTraits
	{
		template<typename T>
		struct IsEnumFlagsT : LogicFalse {};

		template<typename T, typename TValue>
		struct IsEnumFlagsT<EnumFlags<T, TValue>> : LogicTrue {};

		template<typename T>
		constexpr Boolean IsEnumFlagsV = IsEnumFlagsT<T>::State;
	}
}