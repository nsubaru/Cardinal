export module Cardinal.Math;

export import :Base;
export import :SafeInt;

import :Internal;

export namespace Cardinal::Math
{
	/// <summary>
	/// "Pi" value of the constant
	/// </summary>
	inline constexpr Double Pi = Internal::Pi();

	/// <summary>
	/// "E" value of the constant
	/// </summary>
	inline constexpr Double E = Internal::E();

#pragma region Abs
	/// <summary>
	/// Calculates the modulus of a number
	/// </summary>
	/// <param name="Val">Input number</param>
	/// <returns>The modulus of a number</returns>
	ForceInline constexpr Int8 Abs(Int8 Val) {
		return Internal::ConstexprAbs(Val);
	}

	/// <summary>
	/// Calculates the modulus of a number
	/// </summary>
	/// <param name="Val">Input number</param>
	/// <returns>The modulus of a number</returns>
	ForceInline constexpr Int16 Abs(Int16 Val) {
		return Internal::ConstexprAbs(Val);
	}

	/// <summary>
	/// Calculates the modulus of a number
	/// </summary>
	/// <param name="Val">Input number</param>
	/// <returns>The modulus of a number</returns>
	ForceInline constexpr Int32 Abs(Int32 Val) {
		return Internal::ConstexprAbs(Val);
	}

	/// <summary>
	/// Calculates the modulus of a number
	/// </summary>
	/// <param name="Val">Input number</param>
	/// <returns>The modulus of a number</returns>
	ForceInline constexpr Int64 Abs(Int64 Val) {
		return Internal::ConstexprAbs(Val);
	}

	/// <summary>
	/// Calculates the modulus of a number
	/// </summary>
	/// <param name="Val">Input number</param>
	/// <returns>The modulus of a number</returns>
	ForceInline constexpr Float Abs(Float Val) {
		if (TypeTraits::IsCompileTimeEvaluating())
			return Internal::ConstexprAbs(Val);
		else
			return Internal::RuntimeAbs(Val);
	}

	/// <summary>
	/// Calculates the modulus of a number
	/// </summary>
	/// <param name="Val">Input number</param>
	/// <returns>The modulus of a number</returns>
	ForceInline constexpr Double Abs(Double Val) {
		if (TypeTraits::IsCompileTimeEvaluating())
			return Internal::ConstexprAbs(Val);
		else
			return Internal::RuntimeAbs(Val);
	}
#pragma endregion

#pragma region Pow
	/// <summary>
	/// Bringing the number to the power of "x ^ y"
	/// </summary>
	/// <param name="X">A number which is brings to power</param>
	/// <param name="Y">The powered number</param>
	/// <returns>Result</returns>
	ForceInline constexpr UInt64 Pow(UInt64 X, UInt8 Y) {
		if (TypeTraits::IsCompileTimeEvaluating())
			return Internal::ConstexprPow(X, Y);
		else
			return Internal::RuntimePow(X, Y);
	}

	/// <summary>
	/// Bringing the number to the power of "x ^ y"
	/// </summary>
	/// <param name="X">A number which is brings to power</param>
	/// <param name="Y">The powered number</param>
	/// <returns>Result</returns>
	ForceInline constexpr Double Pow(Double X, Int32 Y) {
		if (TypeTraits::IsCompileTimeEvaluating())
			return Internal::ConstexprPow(X, Y);
		else
			return Internal::RuntimePow(X, Y);
	}

	/// <summary>
	/// Bringing the number to the power of "x ^ y"
	/// </summary>
	/// <param name="X">A number which is brings to power</param>
	/// <param name="Y">The powered number</param>
	/// <returns>Result</returns>
	ForceInline constexpr Double Pow(Double X, Double Y) {
		if (TypeTraits::IsCompileTimeEvaluating())
			return Internal::ConstexprPow(X, Y);
		else
			return Internal::RuntimePow(X, Y);
	}
#pragma endregion

#pragma region Log
	/// <summary>
	/// Calculates the natural logarithm of a number
	/// </summary>
	/// <param name="X">Input value</param>
	/// <returns>Natural logarithm</returns>
	ForceInline constexpr Double Ln(Double X) {
		if (TypeTraits::IsCompileTimeEvaluating())
			return Internal::ConstexprLn(X);
		else
			return Internal::RuntimeLn(X);
	}

	/// <summary>
	/// Calculates the natural logarithm of a number
	/// </summary>
	/// <param name="X">Input value</param>
	/// <returns>Natural logarithm</returns>
	ForceInline constexpr Double Lg(Double X) {
		return Ln(X);
	}

	/// <summary>
	/// Calculates the logarithm with base 2 of the number
	/// </summary>
	/// <param name="X">Input value</param>
	/// <returns>Logarithm with base 2</returns>
	ForceInline constexpr Double Log2(Double X) {
		if (TypeTraits::IsCompileTimeEvaluating())
			return Internal::ConstexprLog2(X);
		else
			return Internal::RuntimeLog2(X);
	}
#pragma endregion

#pragma region Sqrt
	/// <summary>
	/// Calculates the square root of a number
	/// </summary>
	/// <param name="X">Input value</param>
	/// <returns>Square root of a number</returns>
	ForceInline constexpr Double Sqrt(Double X) {
		if (TypeTraits::IsCompileTimeEvaluating())
			return Internal::ConstexprSqrt(X);
		else
			return Internal::RuntimeSqrt(X);
	}
#pragma endregion

#pragma region Sin
	/// <summary>
	/// Calculates the sine of a number
	/// </summary>
	/// <param name="X">Value in radians</param>
	/// <returns>Sine of a number</returns>
	ForceInline constexpr Double Sin(Double X) {
		if (TypeTraits::IsCompileTimeEvaluating())
			return Internal::ConstexprSin(X);
		else
			return Internal::RuntimeSin(X);
	}
#pragma endregion

#pragma region Cos
	/// <summary>
	/// Calculates the cosine of a number
	/// </summary>
	/// <param name="X">Value in radians</param>
	/// <returns>Cosine of a number</returns>
	ForceInline constexpr Double Cos(Double X) {
		if (TypeTraits::IsCompileTimeEvaluating())
			return Internal::ConstexprCos(X);
		else
			return Internal::RuntimeCos(X);
	}
#pragma endregion

#pragma region Tan
	/// <summary>
	/// Calculates the tangent of a number
	/// </summary>
	/// <param name="X">Value in radians</param>
	/// <returns>Tangent of a number</returns>
	ForceInline constexpr Double Tan(Double X) {
		if (TypeTraits::IsCompileTimeEvaluating())
			return Internal::ConstexprTan(X);
		else
			return Internal::RuntimeTan(X);
	}
#pragma endregion

#pragma region CTan
	/// <summary>
	/// Calculates the cotangent of a number
	/// </summary>
	/// <param name="X">Value in radians</param>
	/// <returns>Cotangent of a number</returns>
	ForceInline constexpr Double CTan(Double X) {
		if (TypeTraits::IsCompileTimeEvaluating())
			return Internal::ConstexprCtan(X);
		else
			return Internal::RuntimeCtan(X);
	}
#pragma endregion

#pragma region ASin
	/// <summary>
	/// Calculates the angle in radians by the sine of the angle
	/// </summary>
	/// <param name="X">Sine of the angle</param>
	/// <returns>Angle in radians</returns>
	ForceInline constexpr Double ASin(Double X) {
		if (TypeTraits::IsCompileTimeEvaluating())
			return Internal::ConstexprASin(X);
		else
			return Internal::RuntimeASin(X);
	}
#pragma endregion

#pragma region ACos
	/// <summary>
	/// Calculates the angle in radians by the cosine of the angle
	/// </summary>
	/// <param name="X">Cosine of the angle</param>
	/// <returns>Angle in radians</returns>
	ForceInline constexpr Double ACos(Double X) {
		if (TypeTraits::IsCompileTimeEvaluating())
			return Internal::ConstexprACos(X);
		else
			return Internal::RuntimeACos(X);
	}
#pragma endregion

#pragma region ATan
	/// <summary>
	/// Calculates the angle in radians by the tangent of the angle
	/// </summary>
	/// <param name="X">Tangent of the angle</param>
	/// <returns>Angle in radians</returns>
	ForceInline constexpr Double ATan(Double X) {
		if (TypeTraits::IsCompileTimeEvaluating())
			return Internal::ConstexprATan(X);
		else
			return Internal::RuntimeATan(X);
	}
#pragma endregion

#pragma region ACtan
	/// <summary>
	/// Calculates the angle in radians by the cotangent of the angle
	/// </summary>
	/// <param name="X">Cotangent of the angle</param>
	/// <returns>Angle in radians</returns>
	ForceInline constexpr Double ACtan(Double X) {
		if (TypeTraits::IsCompileTimeEvaluating())
			return Internal::ConstexprACtan(X);
		else
			return Internal::RuntimeACtan(X);
	}
#pragma endregion

#pragma region Sign
	/// <summary>
	/// Determines the sign of the number
	/// </summary>
	/// <param name="Val">Input number</param>
	/// <returns>"True" if positive</returns>
	ForceInline constexpr Boolean Sign(Int8 Val) {
		return ((Val) & ((Int8) 1 << 7)); //-V2533
	}

	/// <summary>
	/// Determines the sign of the number
	/// </summary>
	/// <param name="Val">Input number</param>
	/// <returns>"True" if positive</returns>
	ForceInline constexpr Boolean Sign(Int16 Val) {
		return ((Val) & ((Int16) 1 << 15)); //-V2533
	}

	/// <summary>
	/// Determines the sign of the number
	/// </summary>
	/// <param name="Val">Input number</param>
	/// <returns>"True" if positive</returns>
	ForceInline constexpr Boolean Sign(Int32 Val) {
		return ((Val) & ((Int32) 1 << 31)); //-V2533
	}

	/// <summary>
	/// Determines the sign of the number
	/// </summary>
	/// <param name="Val">Input number</param>
	/// <returns>"True" if positive</returns>
	ForceInline constexpr Boolean Sign(Int64 Val) {
		return ((Val) & ((Int64) 1 << 63)); //-V2533
	}

	/// <summary>
	/// Determines the sign of the number
	/// </summary>
	/// <param name="Val">Input number</param>
	/// <returns>"True" if positive</returns>
	ForceInline constexpr Boolean Sign(Float Val) {
		if (TypeTraits::IsCompileTimeEvaluating())
			return Internal::ConstexprSign(Val);
		else
			return Internal::RuntimeAbs(Val);
	}

	/// <summary>
	/// Determines the sign of the number
	/// </summary>
	/// <param name="Val">Input number</param>
	/// <returns>"True" if positive</returns>
	ForceInline constexpr Boolean Sign(Double Val) {
		if (TypeTraits::IsCompileTimeEvaluating())
			return Internal::ConstexprSign(Val);
		else
			return Internal::RuntimeSign(Val);
	}

	/// <summary>
	/// Returns the number by position in decimal view
	/// </summary>
	/// <param name="Number">Input number</param>
	/// <param name="Pos">Position from zero from right to left</param>
	/// <returns>Values for the corresponding position</returns>
	ForceInline constexpr UInt8 GetNumberByPos(const UInt64 Number, const UInt8 Pos) {
		return static_cast<UInt8>(
			Number % Pow(10ULL, static_cast<UInt8>(Pos + 1ui8)) / Pow(10ULL, static_cast<UInt8>(Pos + 1ui8)) //-V747 //-V2517
			);
	}
#pragma endregion

	/// <summary>
	/// Exhibitor from number: "e ^ x"
	/// </summary>
	/// <param name="X">Input number</param>
	/// <returns>Function result</returns>
	ForceInline constexpr Double Exp(Double X) {
		if (TypeTraits::IsCompileTimeEvaluating())
			return Internal::ConstexprExp(X);
		else
			return Internal::RuntimeExp(X);
	}

	/// <summary>
	/// Divides a number into an integer and a real part
	/// </summary>
	/// <param name="X">Input number</param>
	/// <param name="Y">The output integer part of number</param>
	/// <returns>Real part of number</returns>
	ForceInline Double ModD(Double X, Double& Y) {
		return Internal::ModD(X, Y);
	}

	/// <summary>
	/// Divides a number into an integer and a real part
	/// </summary>
	/// <param name="X">Input number</param>
	/// <param name="Y">The output integer part of number</param>
	/// <returns>Real part of number</returns>
	ForceInline Float ModD(Float X, Float& Y) {
		return Internal::ModD(X, Y);
	}

	/// <summary>
	/// Comparing the real numbers taking into account the error
	/// </summary>
	/// <param name="first">First number</param>
	/// <param name="second">Second number</param>
	/// <returns>"True" if the numbers taking into account the error are equal</returns>
	ForceInline constexpr bool Equals(Float first, Float second) {
		return Abs(first - second) <= Epsilon<Double>();
	}

	/// <summary>
	/// Comparing the real numbers taking into account the error
	/// </summary>
	/// <param name="first">First number</param>
	/// <param name="second">Second number</param>
	/// <returns>"True" if the numbers taking into account the error are equal</returns>
	ForceInline constexpr bool Equals(Double first, Double second) {
		return Abs(first - second) <= Epsilon<Double>();
	}
}