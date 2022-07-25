export module Cardinal.Core:Swap;

import :Types;
import :TypeTraits;

export namespace Cardinal
{
	/// <summary>
	/// Variables values swapping
	/// </summary>
	/// <typeparam name="T">Type of elements</typeparam>
	/// <param name="first">First value</param>
	/// <param name="second">Second value</param>
	/// <remarks>
	/// A classic swap through the third variable:
	/// An example: it is given the A and B variables, it is needed to swap their values
	/// The solution is doing a temporary variable C
	/// C = A; A = B; B = C;
	/// </remarks>
	template<typename T>
	__forceinline void Swap(T& first, T& second) {
		T tmp(MoveRValue(first));
		first = MoveRValue(second);
		second = MoveRValue(tmp);
	}

	/// <summary>
	/// Optimized variables values swapping "AChar"
	/// </summary>
	/// <param name="first">First value</param>
	/// <param name="second">Second value</param>
	/// <remarks>
	/// Optimized swapping occurs using the feature of bit XOR,
	/// which helps to swap values without third variable.
	/// Pseudocode: A = A ^ B; B = B ^ A; A = A ^ B;
	/// An example: the digits 5 and 10 are bit representation of 0101 and 1010:
	/// 0000 = 0101 ^ 1010; 0101 = 1010 ^ 0000; 1010 = 0101^0000; 
	/// </remarks>
	template<>
	__forceinline void Swap(AChar& first, AChar& second) { first ^= second; second ^= first; first ^= second; }

	/// <summary>
	/// Optimized variables values swapping "UChar"
	/// </summary>
	/// <param name="first">First value</param>
	/// <param name="second">Second value</param>
	/// <remarks>
	/// Optimized swapping occurs using the feature of bit XOR,
	/// which helps to swap values without third variable.
	/// Pseudocode: A = A ^ B; B = B ^ A; A = A ^ B;
	/// An example: the digits 5 and 10 are bit representation of 0101 and 1010:
	/// 0000 = 0101 ^ 1010; 0101 = 1010 ^ 0000; 1010 = 0101^0000; 
	/// </remarks>
	template<>
	__forceinline void Swap(UChar& first, UChar& second) { first ^= second; second ^= first; first ^= second; }

	/// <summary>
	/// Optimized variables values swapping "Byte"
	/// </summary>
	/// <param name="first">First value</param>
	/// <param name="second">Second value</param>
	/// <remarks>
	/// Optimized swapping occurs using the feature of bit XOR,
	/// which helps to swap values without third variable.
	/// Pseudocode: A = A ^ B; B = B ^ A; A = A ^ B;
	/// An example: the digits 5 and 10 are bit representation of 0101 and 1010:
	/// 0000 = 0101 ^ 1010; 0101 = 1010 ^ 0000; 1010 = 0101^0000; 
	/// </remarks>
	template<>
	__forceinline void Swap(Byte& first, Byte& second) { first ^= second; second ^= first; first ^= second; }

	/// <summary>
	/// Optimized variables values swapping "Int16"
	/// </summary>
	/// <param name="first">First value</param>
	/// <param name="second">Second value</param>
	/// <remarks>
	/// Optimized swapping occurs using the feature of bit XOR,
	/// which helps to swap values without third variable.
	/// Pseudocode: A = A ^ B; B = B ^ A; A = A ^ B;
	/// An example: the digits 5 and 10 are bit representation of 0101 and 1010:
	/// 0000 = 0101 ^ 1010; 0101 = 1010 ^ 0000; 1010 = 0101^0000; 
	/// </remarks>
	template<>
	__forceinline void Swap(Int16& first, Int16& second) { first ^= second; second ^= first; first ^= second; }

	/// <summary>
	/// Optimized variables values swapping "Int32"
	/// </summary>
	/// <param name="first">First value</param>
	/// <param name="second">Second value</param>
	/// <remarks>
	/// Optimized swapping occurs using the feature of bit XOR,
	/// which helps to swap values without third variable.
	/// Pseudocode: A = A ^ B; B = B ^ A; A = A ^ B;
	/// An example: the digits 5 and 10 are bit representation of 0101 and 1010:
	/// 0000 = 0101 ^ 1010; 0101 = 1010 ^ 0000; 1010 = 0101^0000; 
	/// </remarks>
	template<>
	__forceinline void Swap(Int32& first, Int32& second) { first ^= second; second ^= first; first ^= second; }

	/// <summary>
	/// Optimized variables values swapping "Int64"
	/// </summary>
	/// <param name="first">First value</param>
	/// <param name="second">Second value</param>
	/// <remarks>
	/// Optimized swapping occurs using the feature of bit XOR,
	/// which helps to swap values without third variable.
	/// Pseudocode: A = A ^ B; B = B ^ A; A = A ^ B;
	/// An example: the digits 5 and 10 are bit representation of 0101 and 1010:
	/// 0000 = 0101 ^ 1010; 0101 = 1010 ^ 0000; 1010 = 0101^0000; 
	/// </remarks>
	template<>
	__forceinline void Swap(Int64& first, Int64& second) { first ^= second; second ^= first; first ^= second; }

	/// <summary>
	/// Optimized variables values swapping "UInt16"
	/// </summary>
	/// <param name="first">First value</param>
	/// <param name="second">Second value</param>
	/// <remarks>
	/// Optimized swapping occurs using the feature of bit XOR,
	/// which helps to swap values without third variable.
	/// Pseudocode: A = A ^ B; B = B ^ A; A = A ^ B;
	/// An example: the digits 5 and 10 are bit representation of 0101 and 1010:
	/// 0000 = 0101 ^ 1010; 0101 = 1010 ^ 0000; 1010 = 0101^0000; 
	/// </remarks>
	template<>
	__forceinline void Swap(UInt16& first, UInt16& second) { first ^= second; second ^= first; first ^= second; }

	/// <summary>
	/// Optimized variables values swapping "UInt32"
	/// </summary>
	/// <param name="first">First value</param>
	/// <param name="second">Second value</param>
	/// <remarks>
	/// Optimized swapping occurs using the feature of bit XOR,
	/// which helps to swap values without third variable.
	/// Pseudocode: A = A ^ B; B = B ^ A; A = A ^ B;
	/// An example: the digits 5 and 10 are bit representation of 0101 and 1010:
	/// 0000 = 0101 ^ 1010; 0101 = 1010 ^ 0000; 1010 = 0101^0000; 
	/// </remarks>
	template<>
	__forceinline void Swap(UInt32& first, UInt32& second) { first ^= second; second ^= first; first ^= second; }

	/// <summary>
	/// Optimized variables values swapping "UInt32"
	/// </summary>
	/// <param name="first">First value</param>
	/// <param name="second">Second value</param>
	/// <remarks>
	/// Optimized swapping occurs using the feature of bit XOR,
	/// which helps to swap values without third variable.
	/// Pseudocode: A = A ^ B; B = B ^ A; A = A ^ B;
	/// An example: the digits 5 and 10 are bit representation of 0101 and 1010:
	/// 0000 = 0101 ^ 1010; 0101 = 1010 ^ 0000; 1010 = 0101^0000; 
	/// </remarks>
	template<>
	__forceinline void Swap(UInt64& first, UInt64& second) { first ^= second; second ^= first; first ^= second; }

	/// <summary>
	/// Optimized pointer adresses swapping
	/// </summary>
	/// <param name="first">First pointer</param>
	/// <param name="second">Second pointer</param>
	/// <remarks>
	/// Optimized swapping occurs using the feature in AMD64 architecture the pointer of everything is the UInt64 number.
	/// The swap itself occurs as follows:
	/// Transform a pointer into a "SizeT" number, then swap numbers
	/// </remarks>
	template<typename T>
	__forceinline void Swap(T*& first, T*& second) {
		Swap(reinterpret_cast<UInt64&>(first), reinterpret_cast<UInt64&>(second)); //-V2005 //-V2533
	}

	static_assert(sizeof(void*) == sizeof(SizeT), "SizeT size is not valid. Must be same as size of pointer");
	static_assert(reinterpret_cast<SizeT>(reinterpret_cast<void*>(0xF0F0F0F0F)) == static_cast<SizeT>(0xF0F0F0F0F), "Error. Can not aquire pointer address to SizeT");  //-V566

	/// <summary>
	/// Optimized variables values swapping "Float"
	/// </summary>
	/// <param name="first">First value</param>
	/// <param name="second">Second value</param>
	/// <remarks>
	/// Optimized swapping occurs using the feature of bit XOR and the feature of <c>union</c>.
	/// At first represent real integer numbers and then doing the XOR swap tithout third variable
	/// </remarks>
	template<>
	__forceinline void Swap(Float& first, Float& second) {
		static_assert(sizeof(Float) == sizeof(Int32), "Error. Float and Int32 must have same size");
		union
		{
			Int32* i;
			Float* f;
		} f1, f2;
		f1.f = &first;
		f2.f = &second;
		*f1.i ^= *f2.i;
		*f2.i ^= *f1.i;
		*f1.i ^= *f2.i;
	}

	/// <summary>
	/// Optimized variables values swapping "Double"
	/// </summary>
	/// <param name="first">First value</param>
	/// <param name="second">Second value</param>
	/// <remarks>
	/// Optimized swapping occurs using the feature of bit XOR and the feature of <c>union</c>.
	/// At first represent real integer numbers and then doing the XOR swap tithout third variable
	/// </remarks>
	template<>
	__forceinline void Swap(Double& first, Double& second) {
		static_assert(sizeof(Double) == sizeof(Int64), "Error. Double and Int64 must have same size");
		union
		{
			Int64* i;
			Double* f;
		} f1, f2;
		f1.f = &first;
		f2.f = &second;
		*f1.i ^= *f2.i;
		*f2.i ^= *f1.i;
		*f1.i ^= *f2.i;
	}
}