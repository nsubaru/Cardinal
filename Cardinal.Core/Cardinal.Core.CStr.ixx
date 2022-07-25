module;
#include "Cardinal.Core.Definitions.h"

export module Cardinal.Core:CStr;

import :Types;
import :Math;
import :Internals;

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

/// <summary>
/// Functions namespace for working with C-like string variables
/// </summary>
namespace Cardinal::CStr
{
    /// <summary>
    /// "Compile-time" function for calculating the number of symbols in symbolic array
    /// </summary>
    /// <param name="cStr">String value</param>
    /// <returns>Number of elements in string array</returns>
    template<SizeT N>
    ForceInline constexpr SizeT Size(const StaticTextRef<N>& cStr) {
        for (SizeT i = 0; i < N; i++)
        {
            if (cStr[i] == '\0')
                return i;
        }

        return Math::Max<SizeT>();
    }

    /// <summary>
    /// "Compile-time" function for calculating the number of symbols in symbolic array
    /// </summary>
    /// <param name="cStr">String value</param>
    /// <returns>Number of elements in string array</returns>
    template<SizeT N>
    ForceInline constexpr SizeT Size(const StaticATextRef<N>& cStr) {
        for (SizeT i = 0; i < N; i++)
        {
            if (cStr[i] == '\0')
                return i;
        }

        return Math::Max<SizeT>();
    }

    /// <summary>
    /// "Compile-time" function for calculating the number of symbols in symbolic array. 
    /// It is not recommended for use because the array boundary check is missing
    /// </summary>
    /// <param name="cStr">String value</param>
    /// <returns>Number of elements in string array</returns>
    DeprecatedCall(L"No guarantees of going out of bounds") ForceInline constexpr SizeT Size(const CharStr cStr) {
        SizeT i = 0;
        while (cStr[i] != '\0')
        {
            i++;
        }
        return i;
    }

    /// <summary>
    /// "Compile-time" function for calculating the number of symbols in symbolic array
    /// It is not recommended for use because the array boundary check is missing
    /// </summary>
    /// <param name="cStr">String value</param>
    /// <returns>Number of elements in string array</returns>
    DeprecatedCall(L"No guarantees of going out of bounds") ForceInline constexpr SizeT Size(const Char* cStr) {
        SizeT i = 0;
        while (cStr[i] != '\0')
        {
            i++;
        }
        return i;
    }

    /// <summary>
    /// "Compile-time" function for calculating the number of ASCII symbols in symbolic array
    /// It is not recommended for use because the array boundary check is missing
    /// </summary>
    /// <param name="cStr">String value</param>
    /// <returns>Number of elements in string array</returns>
    DeprecatedCall(L"No guarantees of going out of bounds") ForceInline constexpr SizeT Size(const AChar* cStr) {
        SizeT i = 0;
        while (cStr[i] != '\0')
        {
            i++;
        }
        return i;
    }

    /// <summary>
    /// C-like strings copying function. It is not recommended for use
    /// because the array boundary check is missing
    /// </summary>
    /// <param name="source">String value from which in copying</param>
    /// <param name="dest">String value in which in copying</param>
    DeprecatedCall(L"No guarantees of going out of bounds") ForceInline void Copy(CharStr source, CharStr dest) {
        while ((*dest++ = *source++)); //-V2507
    }

    /// <summary>
    /// C-like strings copying function. It is not recommended for use
    /// because the array boundary check is missing
    /// </summary>
    /// <param name="source">String value from which in copying</param>
    /// <param name="dest">String value in which in copying</param>
    DeprecatedCall(L"No guarantees of going out of bounds") ForceInline void Copy(const Char* source, CharStr dest) {
        while ((*dest++ = *source++)); //-V2507
    }

    /// <summary>
    /// C-like strings copying function. It is not recommended for use
    /// because the array boundary check is missing
    /// </summary>
    /// <param name="source">String value from which in copying</param>
    /// <param name="dest">String value in which in copying</param>
    DeprecatedCall(L"No guarantees of going out of bounds") ForceInline void Copy(const AChar* source, ACharStr dest) {
        while ((*dest++ = *source++));
    }

    /// <summary>
    /// "Compile-time" function for comparing UTF16-LE C-like strings
    /// </summary>
    /// <param name="str1">First string variable</param>
    /// <param name="str2">Second string variable</param>
    /// <returns>True if all the symbols are identical</returns>
    ForceInline constexpr Boolean Compare(const CharStr str1, const CharStr str2) {
        const auto str1Size = Size(str1);
        const auto str2Size = Size(str2);

        if (str1Size != str2Size) return false;

        for (SizeT i = 0; i < str1Size; i++)
            if (str1[i] != str2[i]) return false;

        return true;
    }

    /// <summary>
    /// "Compile-time" function for comparing UTF16-LE C-like strings
    /// </summary>
    /// <param name="str1">First string variable</param>
    /// <param name="str2">Second string variable</param>
    /// <returns>True if all the symbols are identical</returns>
    ForceInline constexpr Boolean Compare(const Char* str1, const Char* str2) {
        const auto str1Size = Size(str1);
        const auto str2Size = Size(str2);

        if (str1Size != str2Size) return false;

        for (SizeT i = 0; i < str1Size; i++)
            if (str1[i] != str2[i]) return false;

        return true;
    }

    /// <summary>
    /// "Compile-time" function for comparing ASCII C-like strings
    /// </summary>
    /// <param name="str1">First string variable</param>
    /// <param name="str2">Second string variable</param>
    /// <returns>True if all the symbols are identical</returns>
    ForceInline constexpr Boolean Compare(const ACharStr str1, const ACharStr str2) { //-V2558
        const auto str1Size = Size(str1);
        const auto str2Size = Size(str2);

        if (str1Size != str2Size) return false;

        for (SizeT i = 0; i < str1Size; i++)
            if (str1[i] != str2[i]) return false;

        return true;
    }

    /// <summary>
    /// "Compile-time" function for comparing UTF16-LE C-like strings
    /// </summary>
    /// <param name="str1">First string variable</param>
    /// <param name="str2">Second string variable</param>
    /// <returns>True if all the symbols are identical</returns>
    ForceInline constexpr Boolean Compare(const AChar* str1, const AChar* str2) {
        const auto str1Size = Size(const_cast<ACharStr>(str1)); 
        const auto str2Size = Size(const_cast<ACharStr>(str2)); 

        if (str1Size != str2Size) return false;

        for (SizeT i = 0; i < str1Size; i++)
            if (str1[i] != str2[i]) return false;

        return true;
    }

    /// <summary>
    /// Transforms the symbol into upper case
    /// </summary>
    /// <param name="symbol">Symbol which is need to transform</param>
    /// <returns>Transformed symbol</returns>
    ForceInline Char ToUpcase(const Char symbol) {
        return Internals::UpcaseCharImp(symbol);
    }

    /// <summary>
    /// Transforms the symbol into lower case
    /// </summary>
    /// <param name="symbol">Symbol which is need to transform</param>
    /// <returns>Transformed symbol</returns>
    ForceInline Char ToDowncase(const Char symbol) {
        return Internals::DowncaseCharImp(symbol);
    }

    /// <summary>
    /// Return true if character in number
    /// </summary>
    /// <param name="symbol">Character</param>
    /// <returns>true if character is number</returns>
    ForceInline constexpr bool IsNumber(const Char symbol) {
        return L'0' <= symbol && symbol <= L'9';
    }

    /// <summary>
    /// Return true is character is whitespace(' ', '\n', '\r' and etc.)
    /// </summary>
    /// <param name="symbol">Character</param>
    /// <returns>true if character is whitespace</returns>
    ForceInline constexpr bool IsWhitespace(const Char symbol) {
        switch (symbol)
        {
        case L' ':
        case L'\r':
        case L'\n':
        case L'\t':
        case L'\0':
            return true;
        default:
            return false;
        }
    }
}

#pragma warning(pop)