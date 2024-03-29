export module Cardinal.StringView:Convertors;

export import :Base;

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
	template<typename THeap, template<typename THeap> typename TStringView>
    VirtualClass TConvertorsStringViewPart {
    private:
		using TBaseStringView = TStringView<THeap>;
		
    public:
#pragma region Converting string to types

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
            const TBaseStringView* This = static_cast<const TBaseStringView*>(this);

            if (This->ActualViewLen == 0)
                Internals::Exceptions::ThrowNullPointException(SourcePosition, reinterpret_cast<SizeT>(This), 0);
                    
            if constexpr (TypeTraits::IsSameV<TypeTraits::Unsigned<T>, T>) {
                if (This->ActualViewLen > Math::MaxNumberCount<T>())
                    Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);
                        
                Math::SafeInt<T> ret;
                        
                for (SizeT i = 0; i < This->ActualViewLen; i++) {
                    if (!(L'0' <= This->refStr->str[This->offset + i] && This->refStr->str[This->offset + i] <= L'9'))
                        Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);

                    ret += static_cast<UInt8>(This->refStr->str[This->offset + i] - L'0');

                    ret *= 10;
                }

                ret /= 10;

                return ret.Value;
            }
            else {
                Boolean sign(This->refStr->str[This->offset] == '-');
                        
                if (This->ActualViewLen > Math::MaxNumberCount<T>() + (sign ? 1 : 0))
                    Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);
                        
                SizeT i = 0;
                if (This->refStr->str[This->offset] == L'-' || This->refStr->str[This->offset] == L'+')
                    i = 1;

                Math::SafeInt<T> ret = 0;
                        
                for (i; i < This->ActualViewLen; i++) {
                    if (!(L'0' <= This->refStr->str[This->offset + i] && This->refStr->str[This->offset + i] <= L'9'))
                        Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);

                    ret += This->refStr->str[This->offset + i] - L'0';
                    ret *= 10;
                }

                ret /= 10;

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
        /// <exception cref="Rem::Exceptions::IntegerOverflowException">If the number of digits is greater than can fit the number "Float"</exception>
        /// <exception cref="Rem::Exceptions::InvalidFormatException">If the line is different from numbers and single dot characters</exception>
        Float toFloat() const {
            const TBaseStringView* This = static_cast<const TBaseStringView*>(this);

            if (This->ActualViewLen == 0)
                Internals::Exceptions::ThrowNullPointException(SourcePosition, reinterpret_cast<SizeT>(This), 0);

            if (This->ActualViewLen > 18)
                Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);

            Boolean sign(This->refStr->str[This->offset] == '-');
            SizeT i = 0;
            if (This->refStr->str[This->offset] == L'-' || This->refStr->str[This->offset] == L'+')
                i = 1;

            Float int_part = 0.0, double_part = 0.0;
            for (i; i < This->ActualViewLen && This->refStr->str[This->offset + i] != L'.'; i++) {
                if (!(L'0' <= This->refStr->str[This->offset + i] && This->refStr->str[This->offset + i] <= L'9'))
                    Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);

                int_part += This->refStr->str[This->offset + i] - L'0';
                int_part *= 10;
            }

            int_part /= 10;

            if (i != This->ActualViewLen) {
                for (SizeT j = This->ActualViewLen - 1; j > i; j--)
                {
                    if (!(L'0' <= This->refStr->str[This->offset + j] && This->refStr->str[This->offset + j] <= L'9'))
                        Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);

                    double_part += This->refStr->str[This->offset + j] - L'0';
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
        /// <exception cref="Rem::Exceptions::IntegerOverflowException">If the number of digits is greater than can fit the number "Double"</exception>
        /// <exception cref="Rem::Exceptions::InvalidFormatException">If the line is different from numbers and single dot characters</exception>
        Double toDouble() const {
            const TBaseStringView* This = static_cast<const TBaseStringView*>(this);

            if (This->ActualViewLen == 0)
                Internals::Exceptions::ThrowNullPointException(SourcePosition, reinterpret_cast<SizeT>(This), 0);

            if (This->ActualViewLen > 18)
                Internals::Exceptions::ThrowIntegerOverflowException(SourcePosition);

            Boolean sign(This->refStr->str[This->offset] == '-');
            SizeT i = 0;
            if (This->refStr->str[This->offset] == L'-' || This->refStr->str[This->offset] == L'+')
                i = 1;

            Double int_part = 0.0, double_part = 0.0;
            for (i; i < This->ActualViewLen && This->refStr->str[This->offset + i] != L'.'; i++) {
                if (!(L'0' <= This->refStr->str[This->offset + i] && This->refStr->str[This->offset + i] <= L'9'))
                    Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);
                int_part += This->refStr->str[This->offset + i] - L'0';
                int_part *= 10;
            }

            int_part /= 10;
            if (i != This->ActualViewLen) {
                for (SizeT j = This->ActualViewLen - 1; j > i; j--)
                {
                    if (!(L'0' <= This->refStr->str[This->offset + j] && This->refStr->str[This->offset + j] <= L'9'))
                        Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);
                    double_part += This->refStr->str[This->offset + j] - L'0';
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
            const TBaseStringView& This = static_cast<const TBaseStringView&>(*this);

            if (This == L"true" || This == L"True" || This == L"1")
                return true;
            else if (This == L"false" || This == L"False" || This == L"0")
                return false;
            else
                Internals::Exceptions::ThrowInvalidArgumentException(SourcePosition, L"this", L"incorrect string value");
        }
#pragma endregion
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
        /// Convert from string to binary "UUID" representation
        /// </summary>
        /// <returns>"UUID"</returns>
        UUID toUUID() const {
            const TBaseStringView* This = static_cast<const TBaseStringView*>(this);

            if (This->refStr->str == nullptr || This->ActualViewLen == 0)
                Internals::Exceptions::ThrowNullPointException(SourcePosition, reinterpret_cast<SizeT>(This), 0);
            UUID res;
            if (!Internals::ConvertCharStrToUuid(This->refStr->str + This->offset, res)) {
                Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);
            }
            return res;
        }

        /// <summary>
        /// Convert a string to a dimensional type
        /// </summary>
        /// <returns>DataSizeT object</returns>
        Memory::DataSizeT ToDataSize() {
            const TBaseStringView* This = static_cast<const TBaseStringView*>(this);

            if (This->ActualViewLen == 0)
                Internals::Exceptions::ThrowNullPointException(SourcePosition, reinterpret_cast<SizeT>(This), 0);
                    
            Math::SafeInt<SizeT> ret;
            SizeT intPart = 0, doublePart = 0;
            SizeT i = 0;

            for (i; CStr::IsNumber(This->refStr->str[This->offset + i]); i++)
            {
                intPart += This->refStr->str[This->offset + i] - L'0';
                intPart *= 10;
            }
            intPart /= 10;

            if (This->refStr->str[This->offset + i] == L'.')
            {
                for (i++; CStr::IsNumber(This->refStr->str[This->offset + i]); i++)
                {
                    doublePart += This->refStr->str[This->offset + i] - L'0';
                    doublePart *= 10;
                }
                doublePart /= 10;
            }

            for (i; i < This->len; i++)
            {
                if (This->refStr->str[This->offset + i] != L' ') break;
            }

            if(i != This->len) 
            {
                switch (This->refStr->str[This->offset + i])
                {
                    case '\0' :
                    case 'b' :
                    case 'B' :
                        return Memory::DataSizeT(intPart);

                    case 'k' :
                    case 'K' :
                        if (This->refStr->str[This->offset + i + 1] != L'B' && This->refStr->str[This->offset + i + 1] != L'b')
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
                        if (This->refStr->str[This->offset + i + 1] != L'B' && This->refStr->str[This->offset + i + 1] != L'b')
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
                        if (This->refStr->str[This->offset + i + 1] != L'B' && This->refStr->str[This->offset + i + 1] != L'b')
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
                        if (This->refStr->str[This->offset + i + 1] != L'B' && This->refStr->str[This->offset + i + 1] != L'b')
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
                        if (This->refStr->str[This->offset + i + 1] != L'B' && This->refStr->str[This->offset + i + 1] != L'b')
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
                        if (This->refStr->str[This->offset + i + 1] != L'B' && This->refStr->str[This->offset + i + 1] != L'b')
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
            else
            {
                return Memory::DataSizeT(intPart);
            }
        }
#pragma endregion
	};
}

#pragma warning(pop)