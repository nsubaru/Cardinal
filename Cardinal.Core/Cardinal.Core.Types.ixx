export module Cardinal.Core:Types;

export namespace std
{
	enum class align_val_t : unsigned __int64 {};
}

/// <summary>
/// VCRT namespace (avoid usage if possible)
/// </summary>
export namespace CRT = std;

export namespace Cardinal
{
	/// <summary>
	/// 8-bit unsigned numeric variable
	/// </summary>
	using UInt8 = unsigned __int8;

	/// <summary>
	/// 16-bit unsigned numeric variable
	/// </summary>
	using UInt16 = unsigned __int16;

	/// <summary>
	/// 32-bit unsigned numeric variable
	/// </summary>
	using UInt32 = unsigned __int32;

	/// <summary>
	/// 64-bit unsigned numeric variable
	/// </summary>
	using UInt64 = unsigned __int64;

	/// <summary>
	/// 8-bit signed numeric variable
	/// </summary>
	using Int8 = signed __int8;

	/// <summary>
	/// 16-bit signed numeric variable
	/// </summary>
	using Int16 = signed __int16;

	/// <summary>
	/// 32-bit signed numeric variable
	/// </summary>
	using Int32 = signed __int32;

	/// <summary>
	/// 64-bit signed numeric variable
	/// </summary>
	using Int64 = signed __int64;

	/// <summary>
	/// 128-bit signed number
	/// </summary>
	union __declspec(intrin_type) __declspec(align(16)) Int128 { Int64 fields[2]; };

	/// <summary>
	/// 128-bit unsigned number
	/// </summary>
	union __declspec(intrin_type) __declspec(align(16)) UInt128 { UInt64 fields[2]; };

	/// <summary>
	/// 32-bit real number
	/// </summary>
	using Float = float;

	/// <summary>
	/// 64-bit real number
	/// </summary>
	using Double = double;

	/// <summary>
	/// One byte
	/// </summary>
	using Byte = unsigned char;

	/// <summary>
	/// Array of bytes
	/// </summary>
	using ByteArray = Byte*;

	/// <summary>
	/// Size type
	/// </summary>
	using SizeT = size_t;

	static_assert(sizeof(char16_t) == sizeof(wchar_t), "Invalid wchar_t size. wchar_t must have same size as char16_t");

	/// <summary>
	/// A symbol in UTF-16LE coding
	/// </summary>
	using Char = wchar_t; using UChar = wchar_t;

	/// <summary>
	/// A symbol in ASCII coding
	/// </summary>
	using AChar = char;

	/// <summary>
	/// A symbol in UTF-8 coding
	/// </summary>
	using U8Char = char8_t;

	/// <summary>
	/// A symbol in UTF-16 coding
	/// </summary>
	using U16Char = char16_t;

	/// <summary>
	/// A symbol in UTF-32 coding
	/// </summary>
	using U32Char = char32_t;

	/// <summary>
	/// Pointer to an array of symbols in UTF-16LE coding
	/// </summary>
	using CharStr = Char*; using UCharStr = UChar*;

	/// <summary>
	/// Pointer to an array of symbols in ASCII coding
	/// </summary>
	using ACharStr = AChar*;

	/// <summary>
	/// Pointer to a static array of symbols in ASCII coding
	/// </summary>
	using ConstACharStr = const AChar* const;

	/// <summary>
	/// Pointer to a static array of symbols in UTF-16LE coding
	/// </summary>
	using ConstCharStr = const Char* const; using UConstCharStr = UChar*;

	/// <summary>
	/// A reference to static line of UTF-16 symbols
	/// </summary>
	template<SizeT StrSize>
	using StaticTextRef = const wchar_t(&)[StrSize];

	/// <summary>
	/// A reference to static line of ASCII symbols
	/// </summary>
	template<SizeT StrSize>
	using StaticATextRef = const char(&)[StrSize];

	/// <summary>
	/// Logic type
	/// </summary>
	using Boolean = bool;

	/// <summary>
	/// A type for identification
	/// </summary>
	using ID = UInt64;

	/// <summary>
	/// A null type for pointers
	/// </summary>
	using NullptrT = decltype(nullptr);

	/// <summary>
	/// Nullpointer
	/// </summary>
	constexpr NullptrT NullPtr = nullptr;

	/// <summary>
	/// Untyped memory block
	/// </summary>
	using RawMemBlk = void*;

	/// <summary>
	/// Descriptor (unknown type)
	/// </summary>
	using Handle = void*;

	/// <summary>
	/// A marker type which points that it is need to allign structure/class by certain border
	/// </summary>
	using AllignRequirements = CRT::align_val_t;

	/// <summary>
	/// Realization details
	/// </summary>
	using ImplSpecificT = void*;
}

#pragma warning(push)
#pragma warning(disable: 4455)
#pragma warning(disable: 4114)

/// <summary>
/// A line literal for UTF-16LE coding
/// </summary>
/// <param name="text">A pointer on a string constant</param>
/// <param name="size">Number of elenemts in string array</param>
/// <returns>CharStr constant</returns>
__forceinline constexpr Cardinal::CharStr operator "" U(const wchar_t* const text, const Cardinal::SizeT size)
{
	using namespace Cardinal;
	return const_cast<const CharStr>(text);
}

/// <summary>
/// A SizeT literal for integer
/// </summary>
/// <param name="size">Integer value</param>
/// <returns>SizeT value</returns>
__forceinline constexpr Cardinal::SizeT operator "" Sz(const unsigned long long size) {
	using namespace Cardinal;
	return static_cast<Cardinal::SizeT>(size);
}

#pragma warning(pop)