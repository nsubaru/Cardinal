export module Cardinal.String:Convertors;

import :Base;

import Cardinal.Core.Internals;
import Cardinal.Exceptions.Internals;
import Cardinal.Time.Internals;

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 4251)
namespace Cardinal
{
    class IntegerOverflowException;
}

namespace Cardinal::Details
{
	template<typename THeap, template<typename THeap> typename TString>
    VirtualClass TConvertorsStringPart {
    private:
		using TBaseString = TString<THeap>;
	public:
#pragma region Converting to string

#pragma region Converting int types to string

#pragma region Convertinh signed int types to string
        /// <summary>
        /// Convert an integer of type "Int8" to a string
        /// </summary>
        /// <returns>Returns the string representation of number</returns>
        static TBaseString toString(Int8 val) {
            if (val == 0) return TBaseString(L"0");
            Boolean sign = Math::Sign(val);
            Char buf[5];
            SizeT i = 0;
            if (sign) Math::Abs(val);
            do {
                buf[i] = L'0' + (val % 10);
                i++;
                val /= 10;
            } while (val != 0);
            if (sign) {
                buf[i] = L'-';
                i++;
            }
            buf[i] = L'\0';
            i++;
            TBaseString str;
            str.CurrentSize = i;
            str.ReservedSize = str.CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
            str.str = (CharStr)TBaseString::operator new[](sizeof(Char)* str.ReservedSize); //-V2005
            Memory::Copy(buf, str.str, str.CurrentSize * sizeof(Char));
            Cardinal::Reverse(str.str, str.CurrentSize - 1);
            return str;
        }

        /// <summary>
        /// Convert an integer of type "Int16" to a string
        /// </summary>
        /// <returns>Returns the string representation of number</returns>
        static TBaseString toString(Int16 val) {
            if (val == 0)
                return TBaseString(L"0");

            Boolean sign = Math::Sign(val);
            Char buf[10];
            if (sign)
            {
                val = Math::Abs(val);
            }

            SizeT i = 0;
            do {
                buf[i] = L'0' + (val % 10);
                i++;
                val /= 10;
            } while (val != 0);

            if (sign) {
                buf[i] = L'-';
                i++;
            }

            buf[i] = L'\0';
            i++;
            TBaseString str;
            str.CurrentSize = i;
            str.ReservedSize = str.CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
            str.str = (CharStr)TBaseString::operator new[](sizeof(Char)* str.ReservedSize); //-V2005
            Memory::Copy(buf, str.str, str.CurrentSize * sizeof(Char));
            Cardinal::Reverse(str.str, str.CurrentSize - 1);
            return str;
        }

        /// <summary>
        /// Convert an integer of type "Int32" to a string
        /// </summary>
        /// <returns>Returns the string representation of number</returns>
        static TBaseString toString(Int32 val) {
            if (val == 0) 
                return TBaseString(L"0");

            Boolean sign = Math::Sign(val);
            Char buf[20];
            SizeT i = 0;
            if (sign)
            {
                val = Math::Abs(val);
            }

            do {
                buf[i] = L'0' + (val % 10);
                i++;
                val /= 10;
            } while (val != 0);

            if (sign) {
                buf[i] = L'-';
                i++;
            }

            buf[i] = L'\0';
            i++;
            TBaseString str;
            str.CurrentSize = i;
            str.ReservedSize = str.CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
            str.str = (CharStr)TBaseString::operator new[](sizeof(Char)* str.ReservedSize); //-V2005
            Memory::Copy(buf, str.str, str.CurrentSize * sizeof(Char));
            Cardinal::Reverse(str.str, str.CurrentSize - 1);
            return str;
        }

        /// <summary>
        /// Convert an integer of type "Int64" to a string
        /// </summary>
        /// <returns>Returns the string representation of number</returns>
        static TBaseString toString(Int64 val) {
            if (val == 0) 
                return TBaseString(L"0");

            Boolean sign = Math::Sign(val);
            Char buf[30];
            SizeT i = 0;
            if (sign)
            {
                val = Math::Abs(val);
            }

            do {
                buf[i] = L'0' + (val % 10);
                i++;
                val /= 10;
            } while (val != 0);

            if (sign) {
                buf[i] = L'-';
                i++;
            }

            buf[i] = L'\0';
            i++;
            TBaseString str;
            str.CurrentSize = i;
            str.ReservedSize = str.CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
            str.str = (CharStr)TBaseString::operator new[](sizeof(Char)* str.ReservedSize); //-V2005
            Memory::Copy(buf, str.str, str.CurrentSize * sizeof(Char));
            Cardinal::Reverse(str.str, str.CurrentSize - 1);
            return str;
        }
#pragma endregion

#pragma region Convert unsigned int types to string
        /// <summary>
        /// Convert an integer of type "UInt8" to a string
        /// </summary>
        /// <returns>Returns the string representation of number</returns>
        static TBaseString toString(UInt8 val) {
            if (val == 0) 
                return TBaseString(L"0");
            Char buf[5];
            SizeT i = 0;

            do {
                buf[i] = L'0' + (val % 10);
                i++;
                val /= 10;
            } while (val != 0);

            buf[i] = L'\0';
            i++;
            TBaseString str;
            str.CurrentSize = i;
            str.ReservedSize = str.CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
            str.str = (CharStr)TBaseString::operator new[](sizeof(Char)* str.ReservedSize); //-V2005
            Memory::Copy(buf, str.str, str.CurrentSize * sizeof(Char));
            Cardinal::Reverse(str.str, str.CurrentSize - 1);
            return str;
        }

        /// <summary>
        /// Convert an integer of type "UInt16" to a string
        /// </summary>
        /// <returns>Returns the string representation of number</returns>
        static TBaseString toString(UInt16 val) {
            if (val == 0) 
                return TBaseString(L"0");

            Char buf[10];
            SizeT i = 0;

            do {
                buf[i] = L'0' + (val % 10);
                i++;
                val /= 10;
            } while (val != 0);

            buf[i] = L'\0';
            i++;
            TBaseString str;
            str.CurrentSize = i;
            str.ReservedSize = str.CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
            str.str = (CharStr)TBaseString::operator new[](sizeof(Char)* str.ReservedSize); //-V2005
            Memory::Copy(buf, str.str, str.CurrentSize * sizeof(Char));
            Cardinal::Reverse(str.str, str.CurrentSize - 1);
            return str;
        }

        /// <summary>
        /// Convert an integer of type "UInt32" to a string
        /// </summary>
        /// <returns>Returns the string representation of number</returns>
        static TBaseString toString(UInt32 val) {
            if (val == 0) return TBaseString(L"0");
            Char buf[15];
            SizeT i = 0;

            do {
                buf[i] = L'0' + (val % 10);
                i++;
                val /= 10;
            } while (val != 0);

            buf[i] = L'\0';
            i++;
            TBaseString str;
            str.CurrentSize = i;
            str.ReservedSize = str.CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
            str.str = (CharStr)TBaseString::operator new[](sizeof(Char)* str.ReservedSize); //-V2005
            Memory::Copy(buf, str.str, str.CurrentSize * sizeof(Char));
            Cardinal::Reverse(str.str, str.CurrentSize - 1);
            return str;
        }

        /// <summary>
        /// Convert an integer of type "UInt64" to a string
        /// </summary>
        /// <returns>Returns the string representation of number</returns>
        static TBaseString toString(UInt64 val) {
            if (val == 0) 
                return TBaseString(L"0");

            Char buf[30];
            SizeT i = 0;

            do {
                buf[i] = L'0' + (val % 10);
                i++;
                val /= 10;
            } while (val != 0);

            buf[i] = L'\0';
            i++;
            TBaseString str;
            str.CurrentSize = i;
            str.ReservedSize = str.CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
            str.str = (CharStr)TBaseString::operator new[](sizeof(Char)* str.ReservedSize); //-V2005
            Memory::Copy(buf, str.str, str.CurrentSize * sizeof(Char));
            Cardinal::Reverse(str.str, str.CurrentSize - 1);
            return str;
        }
#pragma endregion
#pragma endregion
        /// <summary>
        /// Convert a logical value to a string, returns "<c>"True"</c>" or "<c>"False"</c>"
        /// </summary>
        /// <params name = "val">The logical value</params>
        /// <returns>"<c>"True"</c>" if "<c>val == true</c>", else "<c>"False"</c>"</returns>
        ForceInline static TBaseString toString(Boolean val) {
            if (val) return TBaseString(L"true"); else return TBaseString(L"false");
        }

        /// <summary>
        /// Convert a logical value to a string, returns the contents of the string "<c>IfTrue</c>" or "<c>IfFalse</c>"
        /// </summary>
        /// <params name = "val">The logical value</params>
        /// <params name = "IfTrue">A string that returns if "<c>val == true</c>"</params>
        /// <params name = "IfFalse">A string that returns if "<c>val == false</c>"</params>
        /// <returns>"<c>IfTrue</c>" or "<c>IfFalse</c>" depending on the value "<c>val</c>"</returns>
        ForceInline static TBaseString toString(Boolean val, const TBaseString& IfTrue, const TBaseString& IfFalse) {
            if (val) return IfTrue; else return IfFalse;
        }
#pragma endregion

#pragma region Converting string to types

#pragma region Converting float typed to string
        /// <summary>
        /// Convert a real number with single precision to a string with a certain number of decimal places
        /// </summary>
        /// <param name = "val">The number to convert</param>
        /// <param name = "precision">The number of decimal places</param>
        /// <returns>String representation of number</returns>
        static TBaseString toString(Float val, UInt8 precision = 3) {
            using namespace Math;
                    
            if (val != val) return L"NaN";

            if (Math::Equals(val, static_cast<Float>(Infinity))) return L"Infinity"; //-V550
            if (Math::Equals(val, static_cast<Float>(-Infinity))) return L"-Infinity"; //-V550
            if (Math::Equals(val, 0.0f)) return L"0"; //-V550

            if (precision > 10) precision = 10;

            Boolean sign = Math::Sign(val);
            if (sign) val *= -1.0;
            Float IntPart = 0;
            Float DoublePart = Math::ModD(val, IntPart);
            DoublePart *= Math::Pow(10, precision);
            Char buf[50] = { 0 };
            SizeT size = 0;
            while (DoublePart >= 1) {
                buf[size] = L'0' + static_cast<Int64>(DoublePart) % 10; //-V2005 //-V2003
                size++;
                DoublePart /= 10;
            }
            if(size != precision) for (size; size < precision; size++) buf[size] = L'0';
            buf[size] = L'.';
            size++;
            if (Math::Equals(IntPart, 0.0F)) { //-V550
                buf[size] = L'0';
                size++;
            }
            else
                while (IntPart >= 1) {
                    buf[size] = L'0' + static_cast<Int64>(IntPart) % 10; //-V2005 //-V2003
                    size++;
                    IntPart /= 10;
                }
            if (sign) {
                buf[size] = '-';
                size++;
            }
            buf[size] = L'\0';
            Cardinal::Reverse(buf, size);
            size++;
            TBaseString str;
            str.CurrentSize = size;
            str.ReservedSize = str.CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
            str.str = (CharStr)TBaseString::operator new[](sizeof(Char)* str.ReservedSize); //-V2005
            Memory::Copy(buf, str.str, (str.CurrentSize) * sizeof(Char));
            return str;
        }

        /// <summary>
        /// Convert a real number with double precision to a string with a certain number of decimal places
        /// </summary>
        /// <param name = "val">The number to convert</param>
        /// <param name = "precision">The number of decimal places</param>
        /// <returns>String representation of number</returns>
        static TBaseString toString(Double val, UInt8 precision = 5) {
            using namespace Math;
                    
            if (val != val) return L"NaN";

            if (Math::Equals(val, Infinity)) return L"Infinity"; //-V550
            if (Math::Equals(val, -Infinity)) return L"-Infinity"; //-V550
            if (Math::Equals(val, 0.0)) return L"0"; //-V550

            if (precision > 10) precision = 10;
            Boolean sign = Math::Sign(val);
            if (sign) val *= -1.0;
            Double IntPart = 0;
            Double DoublePart = Math::ModD(val, IntPart);
            DoublePart *= Math::Pow(10, precision);
            Char buf[50] = { 0 };
            SizeT size = 0;
            while (DoublePart >= 1) {
                buf[size] = L'0' + static_cast<Int64>(DoublePart) % 10; //-V2005 //-V2003
                size++;
                DoublePart /= 10;
            }
            if(size != precision) for (size; size < precision; size++) buf[size] = L'0';
            buf[size] = L'.';
            size++;
            if (Math::Equals(IntPart, 0.0)) { //-V550
                buf[size] = L'0';
                size++;
            }
            else
                while (IntPart >= 1) {
                    buf[size] = L'0' + static_cast<Int64>(IntPart) % 10; //-V2005 //-V2003
                    size++;
                    IntPart /= 10;
                }
            if (sign) {
                buf[size] = '-';
                size++;
            }
            buf[size] = L'\0';
            Cardinal::Reverse(buf, size);
            size++;
            TBaseString str;
            str.CurrentSize = size;
            str.ReservedSize = str.CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
            str.str = (CharStr)TBaseString::operator new[](sizeof(Char)* str.ReservedSize); //-V2005
            Memory::Copy(buf, str.str, (str.CurrentSize) * sizeof(Char));
            return str;
        }
#pragma endregion

#pragma region Converting to int types
#pragma region Converting to unsigned int types
        /// <summary>
        /// Convert a string to an integer
        /// </summary>
        /// <typeparam name = "T">The number type</typeparam>
        /// <returns>Numerical representation</returns>
        /// <exception cref="Rem::Exceptions::NullPointException">If string is empty</exception>
        /// <exception cref="Rem::Exceptions::IntegerOverflowException">If the number is too large</exception>
        /// <exception cref="Rem::Exceptions::InvalidFormatException">If there is a non-numeric character in the string</exception>
        template<typename T = Cardinal::Int32>
        T toInt() const {
            const TBaseString* This = static_cast<const TBaseString*>(this);

            if (This->CurrentSize == 0 || This->Lenght == 0)
                Internals::Exceptions::ThrowNullPointException(SourcePosition, reinterpret_cast<SizeT>(This), 0);
                    
            if constexpr (TypeTraits::IsSameV<TypeTraits::Unsigned<T>, T>) {
                if (This->Lenght > Math::MaxNumberCount<T>())
                    Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);
                        
                Math::SafeInt<T> ret;
                        
                for (SizeT i = 0; i < This->Lenght; i++) {
                    if (!(L'0' <= This->str[i] && This->str[i] <= L'9'))
                        Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);
                    ret += static_cast<UInt8>(This->str[i] - L'0');
                    if (i < This->CurrentSize - 2)
                        ret *= 10;
                }
                return ret.Value;
            }
            else {
                Boolean sign(This->str[0] == '-');
                        
                if (This->Lenght > Math::MaxNumberCount<T>() + (sign ? 1 : 0))
                    Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);
                        
                SizeT i = 0;
                if (This->str[0] == L'-' || This->str[0] == L'+')
                    i = 1;
                Math::SafeInt<T> ret = 0;
                        
                for (i; i < This->Lenght; i++) {
                    if (!(L'0' <= This->str[i] && This->str[i] <= L'9'))
                        Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);
                    ret += This->str[i] - L'0';
                    if (i < This->CurrentSize - 2)
                        ret *= 10;
                }
                return Math::ConvertUnsignedToSigned<T>(ret.Value) * ((sign) ? -1 : 1);
            }
        }
#pragma endregion
#pragma endregion

#pragma region Converting to float point types
        /// <summary>
        /// Convert text to a real number "Float" (may be preceded by a '+' or '-' character)
        /// </summary>
        /// <returns>Numerical representation</returns>
        /// <exception cref="Rem::Exceptions::NullPointException">If string is empty</exception>
        /// <exception cref="Rem::Exceptions::IntegerOverflowException">If the number of digits is greater than I can fit the number "Float"</exception>
        /// <exception cref="Rem::Exceptions::InvalidFormatException">If the line is different from numbers and single dot characters</exception>
        Float toFloat() const {
            const TBaseString* This = static_cast<const TBaseString*>(this);

            if (This->CurrentSize == 0)
                Internals::Exceptions::ThrowNullPointException(SourcePosition, reinterpret_cast<SizeT>(This), 0);

            if (This->CurrentSize > 18)
                Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);

            Boolean sign(This->str[0] == '-');
            SizeT i = 0;
            if (This->str[0] == L'-' || This->str[0] == L'+')
                i = 1;

            Float int_part = 0.0, double_part = 0.0;
            for (i; i < This->Lenght && This->str[i] != L'.'; i++) {
                if (!(L'0' <= This->str[i] && This->str[i] <= L'9'))
                    Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);
                int_part += This->str[i] - L'0';
                int_part *= 10;
            }

            int_part /= 10;

            if (i != This->CurrentSize) {
                for (SizeT j = This->Lenght - 1; j > i; j--)
                {
                    if (!(L'0' <= This->str[j] && This->str[j] <= L'9'))
                        Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);
                    double_part += This->str[j] - L'0';
                    double_part /= 10;
                }
                int_part += double_part;
            }

            return int_part * ((sign) ? -1.0f : 1.0f);
        }

        /// <summary>
        /// Convert text to a real number "Double" (may be preceded by a '+' or '-' character)
        /// </summary>
        /// <returns>Numerical representation</returns>
        /// <exception cref="Rem::Exceptions::NullPointException">If string is empty</exception>
        /// <exception cref="Rem::Exceptions::IntegerOverflowException">If the number of digits is greater than I can fit the number "Double"</exception>
        /// <exception cref="Rem::Exceptions::InvalidFormatException">If the line is different from numbers and single dot characters</exception>
        Double toDouble() const {
            const TBaseString* This = static_cast<const TBaseString*>(this);

            if (This->CurrentSize == 0)
                Internals::Exceptions::ThrowNullPointException(SourcePosition, reinterpret_cast<SizeT>(This), 0);

            if (This->CurrentSize > 18)
                Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);

            Boolean sign(This->str[0] == '-');
            SizeT i = 0;
            if (This->str[0] == L'-' || This->str[0] == L'+')
                i = 1;

            Double int_part = 0.0, double_part = 0.0;
            for (i; i < This->Lenght && This->str[i] != L'.'; i++) {
                if (!(L'0' <= This->str[i] && This->str[i] <= L'9'))
                    Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);
                int_part += This->str[i] - L'0';
                int_part *= 10;
            }

            int_part /= 10;
            if (i != This->CurrentSize) {
                for (SizeT j = This->Lenght - 1; j > i; j--)
                {
                    if (!(L'0' <= This->str[j] && This->str[j] <= L'9'))
                        Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);
                    double_part += This->str[j] - L'0';
                    double_part /= 10;
                }
                int_part += double_part;
            }
            return int_part * ((sign) ? -1.0 : 1.0);
        }

        /// <summary>
        /// Convert a string to a logical value,
        /// if 'true' or 'True', returns "true",
        /// if 'false' or 'False', returns "false",
        /// else generates "InvalidArgumentException" exception
        /// </summary>
        /// <exception cref="Rem::Exceptions::InvalidArgumentException">If the string cannot be converted to a logical value</exception>
        /// <returns>The logical value</returns>
        ForceInline Boolean toBoolean() const {
            const TBaseString& This = static_cast<const TBaseString&>(*this);

            if (This == L"true" || This == L"True")
                return true;
            else if (This == L"false" || This == L"False")
                return false;
            else
                Internals::Exceptions::ThrowInvalidArgumentException(SourcePosition, L"this", L"incorrect string value");
        }
#pragma endregion
        /// <summary>
        /// Returns the string representation of "UUID"
        /// </summary>
        /// <returns>String representation of "UUID" in the form  "{????????-????-????-????????????????}"</returns>
        ForceInline static TBaseString toString(const UUID& id) {
            constexpr SizeT uuidStrSz = (sizeof(UUID::UUIDStringWildCard) / sizeof(Char));

            Char data[uuidStrSz] = { 0 };
            Internals::ConvertUuidToCharStr(id, data);

            return TBaseString(data, uuidStrSz);
        }

        /// <summary>
        /// Convert date and time to string representation
        /// </summary>
        /// <typeparam name="date">Date object</typeparam>
        ForceInline static TBaseString toString(Cardinal::Time::Date date) {
            Char buffer[24] = { 0 };
            Internals::ConvertDateToStringImpl(date, buffer);
            return buffer;
        }

        /// <summary>
        /// Convert date and time to string representation
        /// </summary>
        /// <param name="time">Date and time object</param>
        ForceInline static TBaseString toString(Cardinal::Time::DateTime time) {
            Char buffer[24] = { 0 };
            Internals::ConvertDateTimeToStringImpl(time, buffer);
            return buffer;
        }

        /// <summary>
        /// Convert time and date to a string representation
        /// </summary>
        /// <param name="dateSpan">The object of the date segment</param>
        ForceInline static TBaseString toString(Cardinal::Time::DateSpan dateSpan) {
            Char buffer[24] = { 0 };
            Internals::ConvertTimeSpanToStringImpl(dateSpan, buffer);
            return buffer;
        }

        /// <summary>
        /// Convert time and date to a string representation
        /// </summary>
        /// <param name="time">The object of the time segment</param>
        ForceInline static TBaseString toString(Cardinal::Time::TimeSpan time) {
            Char buffer[22] = { 0 };
            auto value = static_cast<Time::DateSpan>(time);
            Internals::ConvertTimeSpanToStringImpl(value, buffer);
            return buffer;
        }

        /// <summary>
        /// Convert time and date to a string representation
        /// </summary>
        /// <param name="time">Time range object</param>
        static TBaseString toString(Cardinal::Time::TimeRange time) {
            Char buffer[(23 * 2) + 3] = { 0 };
            Internals::ConvertDateTimeToStringImpl(time.StartTime, buffer);
            buffer[23] = L' ';
            buffer[24] = L'-';
            buffer[25] = L' ';
            const auto value = static_cast<Time::DateTime>(time.StartTime.Ticks + time.Duration.Ticks);
            Internals::ConvertDateTimeToStringImpl(value, buffer + 26);
            return buffer;
        }

        /// <summary>
        /// Converts a description of "CxxRtti" to a type description
        /// </summary>
        /// <param name="typeInfo">"CxxRtti" to a type description</param>
        /// <returns>Coded type name</returns>
        ForceInline static TBaseString toString(const TypeInfo& typeInfo) {
            return TBaseString(const_cast<AChar*>(typeInfo.GetCxxName()));
        }

        /// <summary>
        /// Convert binary text to string variable
        /// </summary>
        /// <params name = "array">Array of letters</params>
        /// <returns>Array text line</returns>
        InternalApiCall static TBaseString toString(const Containers::BaseArray<THeap, Byte>& array) {
            TBaseString str;
            str.CurrentSize = array.Count / sizeof(Char);
            str.ReservedSize = str.CurrentSize + CurrentSettings().MemoryManager.StringReserveSize;
            str.str = (CharStr)TBaseString::operator new[](str.ReservedSize * sizeof(Char));//new Char[str.CurrentSize + 1];
            Memory::Copy(array.GetRaw(), str.str, array.Count);
            str.str[str.CurrentSize] = L'\0';
            return str;
        }

        /// <summary>
        /// Convert enumeration to string representation
        /// </summary>
        /// <params name="enumValue">Numerical value of enumeration</typeparam>
        template<typename T> requires TypeTraits::IsEnumV<T>
        ForceInline static TBaseString toString(T enumValue) {
            return TBaseString::toString((TypeTraits::EnumUnderlyingTypeV<T>)enumValue);
        }

        /// <summary>
        /// Convert a text representation to an enumeration value
        /// </summary>
        /// <typeparam name="T">The enumeration type</typeparam>
        /// <returns>The enumeration value</returns>
        template<typename T> requires TypeTraits::IsEnumV<T>
        ForceInline T toEnum() const {
            return (T) this->toInt<typename TypeTraits::EnumUnderlyingTypeV<T>>();
        }

        /// <summary>
        /// Convert dimension type to string
        /// </summary>
        /// <params name = "dataSize">Value</params>
        /// <params name = "formatSize">If "true", then formatting is used, if not, it is converted as the number of bytes</params>
        /// <returns>String representation</returns>
        ForceInline static TBaseString toString(Memory::DataSizeT dataSize, bool formatSize = false) {
            if (!formatSize) {
                return TBaseString::toString(dataSize.Bytes);
            }
            else {
                SizeT data = dataSize.Bytes;

                if (data < Memory::DataSizeT::Kilobyte) {
                    auto result = TBaseString::toString(dataSize.Bytes);
                    result += L" b";
                    return result;
                }
                else if (data < Memory::DataSizeT::Megabyte) {
                    auto result = TBaseString::toString(dataSize.Bytes / (Memory::DataSizeT::Kilobyte * 1.0), 3);
                    result += L" Kb";
                    return result;
                }
                else if (data < Memory::DataSizeT::Gigabyte) {
                    auto result = TBaseString::toString(dataSize.Bytes / (Memory::DataSizeT::Megabyte * 1.0), 3);
                    result += L" Mb";
                    return result;
                }
                else if (data < Memory::DataSizeT::Terabyte) {
                    auto result = TBaseString::toString(dataSize.Bytes / (Memory::DataSizeT::Gigabyte * 1.0), 3);
                    result += L" Gb";
                    return result;
                }
                else if (data < Memory::DataSizeT::Petabyte) {
                    auto result = TBaseString::toString(dataSize.Bytes / (Memory::DataSizeT::Terabyte * 1.0), 3);
                    result += L" Tb";
                    return result;
                }
                else if (data < Memory::DataSizeT::Exabyte) {
                    auto result = TBaseString::toString(dataSize.Bytes / (Memory::DataSizeT::Petabyte * 1.0), 3);
                    result += L" Pb";
                    return result;
                }
                else {
                    auto result = TBaseString::toString(dataSize.Bytes / (Memory::DataSizeT::Exabyte * 1.0), 3);
                    result += L" Eb";
                    return result;
                }
            }
        }

        /// <summary>
        /// Convert from string to binary "UUID" representation
        /// </summary>
        /// <returns>"UUID"</returns>
        UUID toUUID() const {
            const TBaseString* This = static_cast<const TBaseString*>(this);

            if (This->str == nullptr || This->CurrentSize == 0)
                Internals::Exceptions::ThrowNullPointException(SourcePosition, reinterpret_cast<SizeT>(This), 0);
            UUID res;
            if (!Internals::ConvertCharStrToUuid(This->str, res)) {
                Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);
            }
            return res;
        }

        /// <summary>
        /// Convert a string to a dimensional type
        /// </summary>
        /// <returns>DataSizeT object</returns>
        Memory::DataSizeT ToDataSize() {
            const TBaseString* This = static_cast<const TBaseString*>(this);

            if (This->CurrentSize == 0)
                Internals::Exceptions::ThrowNullPointException(SourcePosition, reinterpret_cast<SizeT>(This), 0);
                    
            Math::SafeInt<SizeT> ret;
            SizeT intPart = 0, doublePart = 0;
            SizeT i = 0;

            for (i; CStr::IsNumber(This->str[i]); i++)
            {
                intPart += This->str[i] - L'0';
                intPart *= 10;
            }
            intPart /= 10;

            if (This->str[i] == L'.')
            {
                for (i++; CStr::IsNumber(This->str[i]); i++)
                {
                    doublePart += This->str[i] - L'0';
                    doublePart *= 10;
                }
                doublePart /= 10;
            }
                    
            if (!This->str[i] == L'\0')
            {
                for (i; This->str[i] == L' '; i++);
            }

            switch (This->str[i])
            {
                case '\0' :
                case 'b' :
                case 'B' :
                    return Memory::DataSizeT(intPart);

                case 'k' :
                case 'K' :
                    if (This->str[i + 1] != L'B' && This->str[i + 1] != L'b')
                    {
                        Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);
                    }
                    else
                    {
                        double d = (double) doublePart;
                        while (d > 0) d /= 10;
                        d += intPart;
                        d *= Memory::DataSizeT::Kilobyte;
                        return Memory::DataSizeT(static_cast<SizeT>(d));
                    }
                case 'm' :
                case 'M' :
                    if (This->str[i + 1] != L'B' && This->str[i + 1] != L'b')
                    {
                        Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);
                    }
                    else
                    {
                        double d = (double) doublePart;
                        while (d > 0) d /= 10;
                        d += intPart;
                        d *= Memory::DataSizeT::Megabyte;
                        return Memory::DataSizeT(static_cast<SizeT>(d));
                    }
                case 'g' :
                case 'G' :
                    if (This->str[i + 1] != L'B' && This->str[i + 1] != L'b')
                    {
                        Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);
                    }
                    else
                    {
                        double d = (double) doublePart;
                        while (d > 0) d /= 10;
                        d += intPart;
                        d *= Memory::DataSizeT::Gigabyte;
                        return Memory::DataSizeT(static_cast<SizeT>(d));
                    }
                case 't' :
                case 'T' :
                    if (This->str[i + 1] != L'B' && This->str[i + 1] != L'b')
                    {
                        Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);
                    }
                    else
                    {
                        double d = (double) doublePart;
                        while (d > 0) d /= 10;
                        d += intPart;
                        d *= Memory::DataSizeT::Terabyte;
                        return Memory::DataSizeT(static_cast<SizeT>(d));
                    }
                case 'p' :
                case 'P' :
                    if (This->str[i + 1] != L'B' && This->str[i + 1] != L'b')
                    {
                        Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);
                    }
                    else
                    {
                        double d = (double) doublePart;
                        while (d > 0) d /= 10;
                        d += intPart;
                        d *= Memory::DataSizeT::Petabyte;
                        return Memory::DataSizeT(static_cast<SizeT>(d));
                    }
                case 'e' :
                case 'E' :
                    if (This->str[i + 1] != L'B' && This->str[i + 1] != L'b')
                    {
                        Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);
                    }
                    else
                    {
                        double d = (double) doublePart;
                        while (d > 0) d /= 10;
                        d += intPart;
                        d *= Memory::DataSizeT::Exabyte;
                        return Memory::DataSizeT(static_cast<SizeT>(d));
                    }
                default:
                    Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);
                break;
            }
        }

#pragma endregion
	};
}

#pragma warning(pop)