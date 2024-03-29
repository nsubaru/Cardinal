export module Cardinal.Math:Internal;

import :Base;

namespace Cardinal::Math::Internal
{
#pragma region Const
	constexpr Double E()
	{
		return 2.71828182845904523536l; //-V2517
	}
	constexpr Double Pi()
	{
		return 3.141592653589793238462643383279502884197169399;
	}
#pragma endregion

#pragma region Abs
	constexpr Int8 ConstexprAbs(Int8 Number)
	{
		return Number * ((Number < 0) * (-1i8) + (Number > 0)); //-V2517
	}
	static_assert((Int8)100 == ConstexprAbs((Int8)-100) && (Int8)100 == ConstexprAbs((Int8)100), L"The modulus of a number Int8 works incorrectly"); //-V2533

	constexpr Int16 ConstexprAbs(Int16 Number)
	{
		return Number * ((Number < 0) * (-1i16) + (Number > 0)); //-V2517
	}
	static_assert((Int16)100 == ConstexprAbs((Int16)-100) && (Int16)100 == ConstexprAbs((Int16)100), L"The modulus of a number Int16 works incorrectly"); //-V2533

	constexpr Int32 ConstexprAbs(Int32 Number)
	{
		return Number * ((Number < 0) * (-1) + (Number > 0));
	}
	static_assert(100 == ConstexprAbs(-100) && 100 == ConstexprAbs(100), L"The modulus of a number Int32 works incorrectly");

	constexpr Int64 ConstexprAbs(Int64 Number)
	{
		return Number * ((Number < 0) * (-1i64) + (Number > 0)); //-V2517
	}
	static_assert(100L == ConstexprAbs((Int64) -100L) && 100L == ConstexprAbs((Int64) 100L), L"The modulus of a number Int64 works incorrectly");

	constexpr Float ConstexprAbs(Float Number)
	{
		if (Number < 0)
			return -Number;
		return Number;
	}

	constexpr Double ConstexprAbs(Double Number)
	{
		if (Number < 0)
			return -Number;
		return Number;
	}
#pragma endregion

#pragma region Sign
	constexpr Boolean ConstexprSign(Float Number)
	{
		return Number < 0;
	}

	constexpr Boolean ConstexprSign(Double Number)
	{
		return Number < 0;
	}
#pragma endregion

#pragma region Factorial
	constexpr UInt64 ConstexprFactorial(UInt64 N)
	{
		if (N == 1 || N == 0)
			return 1;
		return N * ConstexprFactorial(N - 1);
	}
#pragma endregion

#pragma region FEqual
	constexpr bool FEqual(Double x, Double y)
	{
		return ConstexprAbs(x - y) <= Math::Epsilon<Double>();
	}
#pragma endregion

#pragma region Exp
	constexpr Double ConstexprExp(Double X, Double Res, Double N, Int64 i, Double t)
	{
		return FEqual(Res, Res + t / N) ?
			Res :
			ConstexprExp(X, Res + t / N, N * i, i + 1, t * X);
	}

	constexpr Double ConstexprExp(Double X)
	{
		return ConstexprExp(X, 1, 1, 2, X);
	}
#pragma endregion

#pragma region Ln
	constexpr Double ConstexprLnIter(Double x, Double y)
	{
		return y + 2.0 * (x - ConstexprExp(y, 1, 1, 2, y)) / (x + ConstexprExp(y, 1, 1, 2, y));
	}
	constexpr Double ConstexprLog(Double x, Double y)
	{
		return FEqual(y, ConstexprLnIter(x, y)) ? y : ConstexprLog(x, ConstexprLnIter(x, y));
	}
	// If x <= 1, we will multiply by e^5 repeatedly until x > 1
	constexpr Double ConstexprLnGt(Double x)
	{
		return x > 0.25 ? ConstexprLog(x, 0) :
			ConstexprLnGt(x * E() * E() * E() * E() * E()) - 5;
	}
	// If x >= 2e10, we will divide by e^5 repeatedly until x < 2e10
	constexpr Double ConstexprLnLT(Double x)
	{
		return x < 1024 ? ConstexprLog(x, 0) :
			ConstexprLnLT(x / (E() * E() * E() * E() * E())) + 5;
	}

	constexpr Double ConstexprLn(Double X)
	{
		return
			X >= 1024 ? ConstexprLnLT(X) :
			ConstexprLnGt(X);
	}

	static_assert(FEqual(ConstexprLn(E()), 1), L"ln not work properly");
#pragma endregion

#pragma region Other log
	constexpr Double ConstexprLog10(Double X)
	{
		return ConstexprLn(X) / ConstexprLn(10);
	}
	constexpr Double ConstexprLog2(Double X)
	{
		return ConstexprLn(X) / ConstexprLn(2);
	}
#pragma endregion

#pragma region Pow
	constexpr UInt64 ConstexprPow(UInt64 X, UInt8 Y, UInt64 Res = 1)
	{
		for (UInt8 i = 0; i < Y; i++)
		{
			Res *= X;
		}
		return Res;
	}
	constexpr Double ConstexprPow(Double X, Double Y)
	{
		return ConstexprExp(ConstexprLog(X, 0) * Y);
	}

	constexpr Double ConstexprPow(Double X, Int32 Y)
	{
		return (Y == 0) ? 1 :
			Y == 1 ? X :
			Y > 1 ? ((Y & 1) ? X * ConstexprPow(X, Y - 1) : ConstexprPow(X, Y / 2) * ConstexprPow(X, Y / 2)) :
			1 / ConstexprPow(X, -Y);
	}
#pragma endregion

#pragma region Sqrt
	constexpr Double ConstexprSqrt(Double X, Double Res = 0)
	{
		Res = X; //-V763
		for (int N = 1; N < 20; N++)
		{
			Res = 0.5 * (Res + X / Res);
		}
		return Res;
	}
#pragma endregion

#pragma region Sin
	constexpr Double ConstexprSin(Double X, Double Res = 0)
	{
		for (int N = 0; N < 20; N++)
		{
			Res += (ConstexprPow(-1.0, N) / ConstexprFactorial(2ull * N + 1)) * ConstexprPow(X, 2 * N + 1);
		}
		return Res;
	}
#pragma endregion

#pragma region Cos
	constexpr Double ConstexprCos(Double X, Double Res = 0)
	{
		for (int N = 0; N < 15; N++)
		{
			Res += (ConstexprPow(-1.0, N) / ConstexprFactorial(2ull * N)) * ConstexprPow(X, 2 * N);
		}
		return Res;
	}
#pragma endregion

#pragma region Tan
	constexpr Double ConstexprTan(Double X)
	{
		return ConstexprSin(X) / ConstexprCos(X);
	}
#pragma endregion

#pragma region CTan
	constexpr Double ConstexprCtan(Double X)
	{
		return ConstexprCos(X) / ConstexprSin(X);
	}
#pragma endregion

#pragma region ASin
	constexpr Double ConstexprASin(Double X, Double Res = 0)
	{
		for (int N = 0; N <= 20; N++)
		{
			Res += (ConstexprFactorial(2ull * N) / (ConstexprPow(4.0, N) * ConstexprPow(static_cast<Double>(ConstexprFactorial(N)), 2.0) * (2ull * N + 1))) * ConstexprPow(X, 2 * N + 1);
		}
		return Res;
	}
#pragma endregion

#pragma region ACos
	constexpr Double ConstexprACos(Double X)
	{
		return (Pi() / 2.0) - ConstexprASin(X);
	}
#pragma endregion

#pragma region ATan
	constexpr Double ConstexprATan(Double X, Double Res = 0) //-V2537
	{
		return ConstexprASin(1.0 / ConstexprSqrt(1 + X * X));
	}
#pragma endregion

#pragma region ACTan
	constexpr Double ConstexprACtan(Double X)
	{
		return (Pi() / 2) - ConstexprATan(X);
	}
#pragma endregion

#pragma region Runtime
	inline Float RuntimeAbs(Float Val)
	{
		*(((UInt32*) &Val)) &= 0x7fffffff;
		return Val;
	}

	inline Double RuntimeAbs(Double Val)
	{
		*(((UInt32*) &Val) + 1) &= 0x7fffffff;
		return Val;
	}

	Boolean RuntimeSign(Float val);

	Boolean RuntimeSign(Double val);

	Double RuntimePow(Double What, Double By);
	UInt64 RuntimePow(UInt64 What, UInt8 By);

	Double RuntimeLn(Double X);
	Double RuntimeLog10(Double X);
	Double RuntimeLog2(Double X);

	Double RuntimeExp(Double X);

	Double RuntimeSin(Double X);

	Double RuntimeCos(Double X);

	Double RuntimeTan(Double X);

	Double RuntimeCtan(Double X);

	Double RuntimeASin(Double X);

	Double RuntimeACos(Double X);

	Double RuntimeATan(Double X);

	Double RuntimeACtan(Double X);

	Double RuntimeSqrt(Double X);
#pragma endregion

	Float ModD(Float X, Float& intPart);

	Double ModD(Double X, Double& intPart);
}