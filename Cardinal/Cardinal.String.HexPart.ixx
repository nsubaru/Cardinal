export module Cardinal.String:HexPart;

import :Base;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 4251)

namespace Cardinal::Details
{
	template<typename THeap, template<typename THeap> typename TString>
    VirtualClass THexStringPart {
    private:
		using TBaseString = TString<THeap>;
   
    public:
        /// <summary>
        /// Array of symbols for hexadecimal letters
        /// </summary>
        static constexpr const auto HexNumbers = L"0123456789abcdef";
	
    public:
		/// <summary>
        /// Convert a string of hexadecimal symbols to a byte array
        /// </summary>
        /// <param name = "stringData">Text representation of hexadecimal symbols</param>
        /// <returns>Array of bytes</returns>
        /// <exception cref="Rem::Exceptions::NullPointException">If string is empty</exception>
        /// <exception cref="Rem::Exceptions::InvalidFormatException">If the string contains invalid symbols or its size is not a multiple of 2</exception>
        static Containers::BaseArray<THeap, Byte> hexToBinary(const TBaseString& stringData) {
            if (stringData.Lenght == 0)
                Internals::Exceptions::ThrowNullPointException(SourcePosition, reinterpret_cast<SizeT>(&stringData), 0);
            if (stringData.Lenght % 2)
                Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);

            SizeT Size = stringData.Lenght / 2;
            SizeT i1 = 0, i2 = 0;
            Containers::BaseArray<THeap, Byte> bytes(Size);
            // Byte <-> 2 hex digits
            union {
                //1 Byte (8 bits)
                Byte* binary;
                // 2 Hex digits (4x2 bits)
                struct {
                    // Second hex (4 bits)
                    Byte bit1 : 4;
                    // First hex (4 bits)
                    Byte bit0 : 4;
                } *Hex;
            } conv;
            for (i1; i1 < Size; i1++) {
                conv.binary = &bytes[i1];
                switch (stringData[i2]) {
                case L'0':
                    conv.Hex->bit0 = 0;
                    break;
                case L'1':
                    conv.Hex->bit0 = 0x1;
                    break;
                case L'2':
                    conv.Hex->bit0 = 0x2;
                    break;
                case L'3':
                    conv.Hex->bit0 = 0x3;
                    break;
                case L'4':
                    conv.Hex->bit0 = 0x4;
                    break;
                case L'5':
                    conv.Hex->bit0 = 0x5;
                    break;
                case L'6':
                    conv.Hex->bit0 = 0x6;
                    break;
                case L'7':
                    conv.Hex->bit0 = 0x7;
                    break;
                case L'8':
                    conv.Hex->bit0 = 0x8;
                    break;
                case L'9':
                    conv.Hex->bit0 = 0x9;
                    break;
                case L'A':
                case L'a':
                    conv.Hex->bit0 = 0xa;
                    break;
                case L'B':
                case L'b':
                    conv.Hex->bit0 = 0xb;
                    break;
                case L'C':
                case L'c':
                    conv.Hex->bit0 = 0xc;
                    break;
                case L'D':
                case L'd':
                    conv.Hex->bit0 = 0xd;
                    break;
                case L'E':
                case L'e':
                    conv.Hex->bit0 = 0xe;
                    break;
                case L'F':
                case L'f':
                    conv.Hex->bit0 = 0xf;
                    break;
                default:
                    Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);
                }
                i2++;
                switch (stringData[i2]) {
                case L'0':
                    conv.Hex->bit1 = 0;
                    break;
                case L'1':
                    conv.Hex->bit1 = 0x1;
                    break;
                case L'2':
                    conv.Hex->bit1 = 0x2;
                    break;
                case L'3':
                    conv.Hex->bit1 = 0x3;
                    break;
                case L'4':
                    conv.Hex->bit1 = 0x4;
                    break;
                case L'5':
                    conv.Hex->bit1 = 0x5;
                    break;
                case L'6':
                    conv.Hex->bit1 = 0x6;
                    break;
                case L'7':
                    conv.Hex->bit1 = 0x7;
                    break;
                case L'8':
                    conv.Hex->bit1 = 0x8;
                    break;
                case L'9':
                    conv.Hex->bit1 = 0x9;
                    break;
                case L'A':
                case L'a':
                    conv.Hex->bit1 = 0xa;
                    break;
                case L'B':
                case L'b':
                    conv.Hex->bit1 = 0xb;
                    break;
                case L'C':
                case L'c':
                    conv.Hex->bit1 = 0xc;
                    break;
                case L'D':
                case L'd':
                    conv.Hex->bit1 = 0xd;
                    break;
                case L'E':
                case L'e':
                    conv.Hex->bit1 = 0xe;
                    break;
                case L'F':
                case L'f':
                    conv.Hex->bit1 = 0xf;
                    break;
                default: Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);
                }
                i2++;
            }
            return bytes;
        }

        /// <summary>
        /// Convert a number, structure, class, or pointer to a "hex string"
        /// </summary>
        /// <param name = "Data">Data that converts to a hexadecimal representation</param>
        template<typename T> requires Concepts::IsPod<T>
        ForceInline static TBaseString toHexString(const T& Data) {
            return binaryToHex(Containers::Array<Byte>(reinterpret_cast<const Byte*>(&Data), sizeof(T)));
        }

        /// <summary>
        /// Convert a byte array to a 16-bit string (0-9a-f)
        /// </summary>
        /// <param name = "binaryData">Array of bytes</param>
        /// <returns>String representation of binary data</returns>
        template<typename UHeap>
        static TBaseString binaryToHex(Containers::BaseArray<UHeap, Byte>&& binaryData) {
            return binaryToHex(static_cast<const Containers::BaseArray<UHeap, Byte>>(binaryData));
        }

        /// <summary>
        /// Convert a byte array to a 16-bit string (0-9a-f)
        /// </summary>
        /// <param name = "binaryData">Array of bytes</param>
        /// <returns>String representation of binary data</returns>
        template<typename UHeap>
        static TBaseString binaryToHex(const Containers::BaseArray<UHeap, Byte>& binaryData) {
            // Byte <-> 2 hex digits
            union {
                // 1 Byte (8 bits)
                const Byte* binary;
                // 2 Hex digits (4x2 bits)
                const struct {
                    // Second hex (4 bits)
                    Byte LowPart : 4;
                    // First hex (4 bits)
                    Byte HighPart : 4;
                } *Hex;
            } conv = { 0 };

            if (binaryData.Count % 2 != 0)
                Internals::Exceptions::ThrowInvalidFormatException(SourcePosition);

            auto str = TBaseString::CreateFilledString(binaryData.Count * 2);

            SizeT i1 = 0, i2 = 0;
            for (i1; i1 < binaryData.Count; i1++) {
                conv.binary = reinterpret_cast<const Byte*>(&binaryData[i1]);

                str.str[i2] = HexNumbers[conv.Hex->HighPart];
                i2++;
                str.str[i2] = HexNumbers[conv.Hex->LowPart];
                i2++;
            }

            return str;
        }

        /// <summary>
        /// Hexadecimal representation of "UInt64"
        /// </summary>
        /// <param name = "Value">Input value</param>
        /// <returns>String representation of binary data</returns>
        static TBaseString UInt64asHex(UInt64 Value) {
            union ConvertT {
                UInt64 Variable;
                struct {
                    Byte LowPart : 4;
                    Byte HighPart : 4;
                }
                Code[sizeof(UInt64)];
            } converter{ .Variable = Value };

            TBaseString str(L"0x0000000000000000");

            for (int i = 0; i < sizeof(Value) * 2; i += 2) {
                str.str[i + 3] = HexNumbers[converter.Code[(sizeof(Value) - 1) - (i / 2)].LowPart];
                str.str[i + 2] = HexNumbers[converter.Code[(sizeof(Value) - 1) - (i / 2)].HighPart];
            }

            return str;
        }

        /// <summary>
        /// Hexadecimal representation of "UInt64"
        /// </summary>
        /// <param name = "Value">Input value</param>
        /// <returns>String representation of binary data</returns>
        static TBaseString Int32asHex(Int32 Value) {
            union ConvertT {
                Int32 Variable;
                struct {
                    Byte LowPart : 4;
                    Byte HighPart : 4;
                }
                Code[sizeof(Int32)];
            } converter{ .Variable = Value };

            TBaseString str(L"0x00000000");

            for (int i = 0; i < sizeof(Value) * 2; i += 2) {
                str.str[i + 3] = HexNumbers[converter.Code[(sizeof(Value) - 1) - (i / 2)].LowPart];
                str.str[i + 2] = HexNumbers[converter.Code[(sizeof(Value) - 1) - (i / 2)].HighPart];
            }

            return str;
        }
	};
}

#pragma warning(pop)