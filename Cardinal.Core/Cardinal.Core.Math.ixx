export module Cardinal.Core:Math;

import :Types;

/// <summary>
/// Math functions
/// </summary>
export namespace Cardinal::Core::Math
{
	/// <summary>
	/// Maximal type value
	/// </summary>
	template<typename T>
	constexpr T Max() { throw 0; };

	/// <summary>
	/// Maximal value of Int8
	/// </summary>
	template<>
	constexpr Int8 Max() { return 0x7Fi8; };

	/// <summary>
	/// Maximal value of Int16
	/// </summary>
	template<>
	constexpr Int16 Max() { return 0x7FFFi16; };

	/// <summary>
	/// Maximal value of Int32
	/// </summary>
	template<>
	constexpr Int32 Max() { return 0x7FFFFFFFi32; };

	/// <summary>
	/// Maximal value of Int64
	/// </summary>
	template<>
	constexpr Int64 Max() { return 0x7FFFFFFFFFFFFFFFi64; };

	/// <summary>
	/// Maximal value of Float
	/// </summary>
	template<>
	constexpr Float Max() { return 3.4E38f; };

	/// <summary>
	/// Maximal value of Double
	/// </summary>
	template<>
	constexpr Double Max() { return static_cast<Double>(1.7E308); };

	/// <summary>
	/// Maximal value of UInt8
	/// </summary>
	template<>
	constexpr UInt8 Max() { return 0xFFui8; };

	/// <summary>
	/// Maximal value of UInt16
	/// </summary>
	template<>
	constexpr UInt16 Max() { return 0xFFFFui16; };

	/// <summary>
	/// Maximal value of UInt32
	/// </summary>
	template<>
	constexpr UInt32 Max() { return 0xFFFFFFFFui32; };

	/// <summary>
	/// Maximal value of UInt64
	/// </summary>
	template<>
	constexpr UInt64 Max() { return 0xFFFFFFFFFFFFFFFFui64; };

	/// <summary>
	/// Maximal value of UChar
	/// </summary>
	template<>
	constexpr UChar Max() { return 0xFFFFui64; };

	/// <summary>
	/// Maximal value of AChar
	/// </summary>
	template<>
	constexpr AChar Max() { return 0xFFui64; };

	/// <summary>
	/// Minimal type value
	/// </summary>
	template<typename T>
	constexpr T Min();

	/// <summary>
	/// Minimal value of Int8
	/// </summary>
	template<>
	constexpr Int8 Min() { return 0x80i8; };

	/// <summary>
	/// Minimal value of Int16
	/// </summary>
	template<>
	constexpr Int16 Min() { return 0x8000i16; };

	/// <summary>
	/// Minimal value of Int32
	/// </summary>
	template<>
	constexpr Int32 Min() { return 0x80000000i32; };

	/// <summary>
	/// Minimal value of Int64
	/// </summary>
	template<>
	constexpr Int64 Min() { return 0x8000000000000000i64; };

	/// <summary>
	/// Minimal value of Float
	/// </summary>
	template<>
	constexpr Float Min() { return static_cast<Float>(3.4E-38); };

	/// <summary>
	/// Minimal value of Double
	/// </summary>
	template<>
	constexpr Double Min() { return 1.7E-308; };

	/// <summary>
	/// Minimal value of UInt8
	/// </summary>
	template<>
	constexpr UInt8 Min() { return 0x00ui8; };

	/// <summary>
	/// Minimal value of UInt16
	/// </summary>
	template<>
	constexpr UInt16 Min() { return 0x0000ui16; };

	/// <summary>
	/// Minimal value of UInt32
	/// </summary>
	template<>
	constexpr UInt32 Min() { return 0x00000000ui32; };

	/// <summary>
	/// Minimal value of UInt64
	/// </summary>
	template<>
	constexpr UInt64 Min() { return 0x0000000000000000ui64; };

	/// <summary>
	/// Minimal value of UChar
	/// </summary>
	template<>
	constexpr UChar Min() { return 0x0000ui64; };

	/// <summary>
	/// Minimal value of AChar
	/// </summary>
	template<>
	constexpr AChar Min() { return 0x00ui64; };

	/// <summary>
	/// Number of digits in numeric
	/// </summary>
	/// <typeparam name="T">Numeric type</typeparam>
	/// <param name="val">Numeric value</param>
	/// <returns>Count of digits</returns>
	template<typename T>
	constexpr UInt8 DigitsCount(T val) {
		// Variable for storing sign
		bool sign = val < 0;
		// Remove sign if they exist;
		if (sign)
			val *= -1;
		// Announcing the result changing
		UInt8 result = 0;
		// Decrease number of decimal categories in the cycle by division and calculate a number of categories until they exist
		do {
			val /= 10;
			result++;
		} while (val > 0);
		// Return value, if sign exist then include that
		return result;
	}

	/// <summary>
	/// Maximal number of digits in numeric type, includes sign if they available for type
	/// </summary>
	/// <typeparam name="T">Numeric type</typeparam>
	template<typename T>
	constexpr UInt8 MaxNumberCount() {
		return DigitsCount<T>(Math::Max<T>()) + (Math::Min<T>() != 0 ? 1 : 0);
	}

	/// <summary>
	/// Machine type epsilon(zero)
	/// </summary>
	/// <typeparam name="T">Real type</typeparam>
	template<typename T>
	constexpr T Epsilon();

	/// <summary>
	/// Machine epsilon(zero) for Float
	/// </summary>
	template<>
	constexpr Float Epsilon()
	{
		return 1.192092896e-07F;
	};

	/// <summary>
	/// Machine epsilon(zero) for Double
	/// </summary>
	template<>
	constexpr Double Epsilon()
	{
		return 2.2204460492503131e-016;
	};

	/// <value>
	/// Infinity
	/// </value>
	constexpr Double Infinity = 1e+300 * 1e+300;

	/// <summary>
	/// Undefined value
	/// </summary>
	constexpr Double NaN = Infinity * 0.0;

	/// <summary>
	/// Rounding the value up during the division
	/// </summary>
	/// <param name="What">Input number</param>
	/// <param name="By">Divider</param>
	/// <returns>Divided rounded upper value</returns>
	/// <remarks>
	/// This function is required when calculating the number of clusters that will be occupied by a file or data.
	/// For example: the cluster size has 512 bytes, the data size is 1000 bytes, the cluster can be either occupied or not.
	/// If the data does not completely occupy the cluster, it is still considered that it is occupied,
	/// respectively 1000 divided 512 will be equal to 1.95, but it is clear that the second cluster also will be occupied,
	/// so it is needed to round to the upper, to 2
	/// </remarks>
	__forceinline constexpr SizeT RoundToUpper(SizeT What, SizeT By) {
		return (What / By) * By != What
			?
			(What / By) + 1
			:
			What / By;
	}
}