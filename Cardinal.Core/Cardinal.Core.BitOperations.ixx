export module Cardinal.Core:BitOperations;

import :Types;

export namespace Cardinal
{
	/// <summary>
	/// Bit operations
	/// </summary>
	class Bits {
	public:
		/// <summary>
		/// Set 1 in a byte "b" with "bitIndex" index and with "bigEndian" order
		/// </summary>
		/// <param name="b">Byte in which is needed to set bit</param>
		/// <param name="bitIndex">Bit number started from 0</param>
		template<typename TInt = Byte>
		static constexpr __forceinline void BitSet(TInt& b, const SizeT bitIndex) {
			b |= static_cast<TInt>(1) << bitIndex;
		}

		/// <summary>
		/// Set 0 in a byte "b" with "bitIndex" index and with "bigEndian" order
		/// </summary>
		/// <param name="b">Byte in which is needed to set bit</param>
		/// <param name="bitIndex">Bit number started from 0</param>
		template<typename TInt = Byte>
		static constexpr __forceinline void BitReset(TInt& b, const SizeT bitIndex) {
			b &= ~(static_cast<TInt>(1) << bitIndex);
		}

		/// <summary>
		/// Bit checking in a byte "b" with "bitIndex" index and with "bigEndian" order
		/// </summary>
		/// <param name="b">Byte in which is needed to set bit</param>
		/// <param name="bitIndex">Bit number started from 0</param>
		/// <returns>True if 1, and false if 0</returns>
		template<typename TInt = Byte>
		static constexpr __forceinline Boolean BitCheck(const TInt& b, const SizeT bitIndex) { //-V669
			return b & (static_cast<TInt>(1) << bitIndex);
		}

		/// <summary>
		/// Extracts "count" number of bits from position "position" and returns its values
		/// </summary>
		/// <param name="value">Value</param>
		/// <param name="position">First bit position</param>
		/// <param name="count">Number of bits</param>
		template<typename TInt = Byte>
		static constexpr __forceinline TInt ExtractBitsFromInt(const TInt& value, const TInt& position, const TInt& count) {
			TInt mask = (static_cast<TInt>(1) << count) - static_cast<TInt>(1);
			return ((value >> position) & mask) << position;
		}

		/// <summary>
		/// Inverses a bit on "position" position and returns new value
		/// </summary>
		/// <param name="value">Value before inversing</param>
		/// <param name="position">An inversed bit position</param>
		template<typename TInt = Byte>
		static constexpr __forceinline TInt FlipBit(const TInt& value, const TInt& position) {
			return value ^ static_cast<TInt>(1) << position;
		}

	public:
		Bits() = delete;
		Bits(const Bits&) = delete;
		Bits(Bits&&) = delete;
		Bits& operator=(const Bits&) = delete;
		Bits& operator=(Bits&&) = delete;
	};
}