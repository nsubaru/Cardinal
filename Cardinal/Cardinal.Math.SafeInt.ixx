module;

#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

export module Cardinal.Math:SafeInt;

import :Base;
import Cardinal.Exceptions.Internals;

extern "C" {
	unsigned __int64 _umul128(
		unsigned __int64 Multiplier,
		unsigned __int64 Multiplicand,
		unsigned __int64* HighProduct
	);
}

export namespace Cardinal::Math
{
#pragma region SafeInt
	/// <summary>
	/// A class of numbers with safe operations
	/// </summary>
	/// <param name = "T">A base type (can be only "unsigned int" with arbitary bit rate)</param>
	template<typename T> requires TypeTraits::IsSigned<T> || TypeTraits::IsUnsigned<T>
	class SafeInt final {
	private:
		/// <summary>
		/// The number type
		/// </summary>
		using Int = TypeTraits::Unsigned<T>;
		
		/// <summary>
		/// Value
		/// </summary>
		Int value;
	
	public:
		/// <summary>
		/// Standard constructor
		/// </summary>
		constexpr SafeInt() : value(0) {}

		/// <summary>
		/// A constructor which receives a value
		/// </summary>
		constexpr SafeInt(Int value) : value(value) {}

		/// <summary>
		/// Two numbers addition
		/// </summary>
		/// <param name = "second">Left operand</param>
		/// <exception cref="IntegerOverflowException">If overflow was occured</exception>
		/// <returns>Operation result</returns>
		constexpr ForceInline SafeInt operator+(SafeInt second) {
			if (static_cast<Int>(this->value + second.value) >= this->value)
				return static_cast<Int>(this->value + second.value);
			else
				Cardinal::Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);
		}

		/// <summary>
		/// Two numbers substraction
		/// </summary>
		/// <param name = "second">Left operand</param>
		/// <exception cref="IntegerOverflowException">If overflow was occured</exception>
		/// <returns>Operation result</returns>
		constexpr ForceInline SafeInt operator-(SafeInt second) {
			if (this->value >= second.value)
				return static_cast<Int>(this->value - second.value);
			else
				Cardinal::Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);
		}

#pragma warning(push)
#pragma warning(disable : 4164)
		/// <summary>
		/// Two numbers multiplication
		/// </summary>
		/// <param name = "second">Left operand</param>
		/// <exception cref="IntegerOverflowException">If overflow was occured</exception>
		/// <returns>Operation result</returns>
#pragma intrinsic(_umul128)
		constexpr ForceInline SafeInt operator*(SafeInt second) {
			if constexpr (TypeTraits::IsSameV<Int, UInt8>)
			{
				UInt16 res = static_cast<UInt16>(this->value) * second.value;
				if (res > Math::Max<Int>())
					Cardinal::Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);
				else
					return static_cast<Int>(res);
			}
			else if constexpr (TypeTraits::IsSameV<Int, UInt16>)
			{
				UInt32 res = static_cast<UInt32>(this->value) * second.value;
				if (res > Math::Max<Int>())
					Cardinal::Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);
				else
					return static_cast<Int>(res);
			}
			else if constexpr (TypeTraits::IsSameV<Int, UInt32>)
			{
				UInt64 res = static_cast<UInt64>(this->value) * second.value;
				if (res > Math::Max<Int>())
					Cardinal::Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);
				else
					return static_cast<Int>(res);
			}
			else if constexpr (TypeTraits::IsSameV<Int, UInt64>)
			{
				UInt64 res[2];
				res[0] = _umul128(this->value, second.value, &res[1]);
				UInt32 res = this->value * second.value;
				if (res[1] > 0)
					Cardinal::Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);
				else
					return res;
			}
			// Unreacheable code
			Cardinal::Internals::Exceptions::ThrowInternalError(SourcePosition);
		}
#pragma warning(pop)

		/// <summary>
		/// Two numbers division
		/// </summary>
		/// <param name = "second">Left operand</param>
		/// <exception cref="IntegerOverflowException">If overflow was occured</exception>
		/// <returns>Operation result</returns>
		constexpr ForceInline SafeInt operator/(SafeInt second) {
			if (second.value == 0)
				Cardinal::Internals::Exceptions::ThrowDivideByZero(SourcePosition);
			else
				return this->value / second.value;
		}

		/// <summary>
		/// Division on given number
		/// </summary>
		/// <param name = "second">Left operand</param>
		/// <exception cref="IntegerOverflowException">If overflow was occured</exception>
		/// <returns>Operation result</returns>
		constexpr ForceInline SafeInt operator/=(SafeInt second) {
			if (second.value == 0)
				Cardinal::Internals::Exceptions::ThrowDivideByZero(SourcePosition);
			else
				return this->value /= second.value;
		}

		/// <summary>
		/// Addition to a given number
		/// </summary>
		/// <param name = "second">Left operand</param>
		/// <exception cref="IntegerOverflowException">If overflow was occured</exception>
		/// <returns>Operation result</returns>
		constexpr ForceInline SafeInt operator+=(SafeInt second) {
			if (static_cast<Int>(this->value + second.value) >= this->value)
				return this->value = static_cast<Int>(this->value + second.value);
			else
				Cardinal::Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);
		}

		/// <summary>
		/// Substraction from a current number
		/// </summary>
		/// <param name = "second">Left operand</param>
		/// <exception cref="IntegerOverflowException">If overflow was occured</exception>
		/// <returns>Operation result</returns>
		constexpr ForceInline SafeInt operator-=(SafeInt second) {
			if (this->value >= second.value)
				return this->value = static_cast<Int>(this->value - second.value);
			else
				Cardinal::Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);
		}

#pragma warning(push)
#pragma warning(disable : 4164)
		// We check overflow so this warning not needed
#pragma warning(disable : 26451)
		/// <summary>
		/// Multiplication to a current number
		/// </summary>
		/// <param name = "second">Left operand</param>
		/// <exception cref="IntegerOverflowException">If overflow was occured</exception>
		/// <returns>Operation result</returns>
		constexpr ForceInline SafeInt operator*=(SafeInt second) {
			if constexpr (TypeTraits::IsSameV<Int, UInt8>)
			{
				UInt16 res = this->value * second.value;
				if (res > Math::Max<Int>())
					Cardinal::Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);
				else
					return this->value = static_cast<Int>(res);
			}
			else if constexpr (TypeTraits::IsSameV<Int, UInt16>)
			{
				UInt32 res = this->value * second.value;
				if (res > Math::Max<Int>())
					Cardinal::Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);
				else
					return this->value = static_cast<Int>(res);
			}
			else if constexpr (TypeTraits::IsSameV<Int, UInt32>)
			{
				UInt64 res = static_cast<UInt64>(this->value) * static_cast<UInt64>(second.value);
				if (res > Math::Max<Int>())
					Cardinal::Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);
				else
					return this->value = static_cast<Int>(res);
			}
			else if constexpr (TypeTraits::IsSameV<Int, UInt64>)
			{
				UInt64 res;
				UInt64 highPart = 0;
				res = _umul128(this->value, second.value, &highPart);
				if (highPart > 0)
					Cardinal::Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);
				else
					return this->value = res;
			}
			// Unreacheable code
			Cardinal::Internals::Exceptions::ThrowInternalError(SourcePosition);
		}

#pragma warning(pop)
		/// <summary>
		/// Universal comparing operator
		/// </summary>
		/// <param name="second">Second number</param>
		/// <returns>Comparing result</returns>
		constexpr ForceInline auto operator<=>(SafeInt second) {
			return this->value <=> second.value;
		}
		/// <summary>
		/// Returns the value of number
		/// </summary>
		/// <returns>Value of a number</returns>
		constexpr ForceInline Int GetValue() { return value; }
		/// <summary>
		/// Value of a number
		/// </summary>
		Int Property(GetValue) Value;
	};

	/// <summary>
	/// Safe "UInt8"
	/// </summary>
	namespace { using SafeInt8 = SafeInt<UInt8>; }
	
	/// <summary>
	/// Safe "UInt16"
	/// </summary>
	namespace { using SafeInt16 = SafeInt<UInt16>; }
	
	/// <summary>
	/// Safe "UInt32"
	/// </summary>
	namespace { using SafeInt32 = SafeInt<UInt32>; }
	
	/// <summary>
	/// Safe "UInt64"
	/// </summary>
	namespace { using SafeInt64 = SafeInt<UInt64>; }
#pragma endregion

#pragma region ConvertUnsignedToSigned
	/// <summary>
	/// Transforms numbers in signed type with overflow checking
	/// </summary>
	template<typename T>
	TypeTraits::Signed<T> ConvertUnsignedToSigned(TypeTraits::Unsigned<T> From) {
		if (From > static_cast<TypeTraits::Unsigned<T>>(Max<TypeTraits::Signed<T>>()))
			Cardinal::Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);
		return static_cast<TypeTraits::Signed<T>>(From);
	}
#pragma endregion
}