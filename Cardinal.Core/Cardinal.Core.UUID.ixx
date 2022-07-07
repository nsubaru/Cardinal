module;
#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

export module Cardinal.Core:UUID;

import :Types;
import :Math;
import :Comparing;

export namespace Cardinal::Core
{
	/// <summary>
	/// A unique identifier class "UUID" compatible with "GUID" which is used in Windows kernel
	/// </summary>
	class UUID {
	private:
		/// <summary>
		/// Object value
		/// </summary>
		union {
			GUID uuid;
			UInt128 uint128;
		};

	public:
		/// <summary>
		/// UUID mask for string formatting
		/// </summary>
		static constexpr Char UUIDStringWildCard[38] = L"{????????-????-????-????????????????}";

		/// <summary>
		/// Null UUID
		/// </summary>
		/// <returns>Returns null UUID</returns>
		constexpr __forceinline static UUID GetZeroUUID() { return UUID(); };

		/// <summary>
		/// Maximal possible UUID
		/// </summary>
		/// <returns>Returns maximal possible UUID</returns>
		constexpr __forceinline static UUID GetMaxUUID() { return UUID(GUID{ 0xffffffff,0xffff,0xffff,{0xff,0xff,0xff,0xff,0xff} }); };//-V112 //-V2523

		/// <summary>
		/// An empty constructor which generates null identifier
		/// </summary>
		/// <returns>Null identifier</returns>
		constexpr __forceinline UUID() : uuid({ 0,0,0, {0,0,0,0,0,0,0,0} }) { }

		/// <summary>
		/// GUID constructor
		/// </summary>
		/// <param name="uuid">GUID value</param>
		/// <returns>Identifier</returns>
		constexpr __forceinline UUID(const GUID& uuid) : uuid(uuid) {}

		/// <summary>
		/// Implicit transformation into "GUID" which is needed for "ntdll" fuctions for compatibility with "Nt" and "Rtl"
		/// </summary>
		/// <returns>GUID with the object value</returns>
		constexpr __forceinline operator const GUID& () const { return this->uuid; }

		/// <summary>
		/// Postfix decrement
		/// </summary>
		/// <returns>New value</returns>
		constexpr __forceinline UUID& operator--() {
			if (uuid.Data4[7] == 0)
				if (uuid.Data4[6] == 0)
					if (uuid.Data4[5] == 0)
						if (uuid.Data4[4] == 0)
							if (uuid.Data4[3] == 0)
								if (uuid.Data4[2] == 0)
									if (uuid.Data4[1] == 0)
										if (uuid.Data4[0] == 0)
											if (uuid.Data3 == 0)
												if (uuid.Data2 == 0)
													if (uuid.Data1 != 0) uuid.Data1 -= 1;
													else return *this;
												else uuid.Data2 -= 1;
											else uuid.Data3 -= 1;
										else uuid.Data4[0] -= 1;
									else uuid.Data4[1] -= 1;
								else uuid.Data4[2] -= 1;
							else uuid.Data4[3] -= 1;
						else uuid.Data4[4] -= 1;
					else uuid.Data4[5] -= 1;
				else uuid.Data4[6] -= 1;
			else uuid.Data4[7] -= 1;
			return *this;
		}

		/// <summary>
		/// Postfix increment
		/// </summary>
		/// <returns>New value</returns>
		constexpr __forceinline UUID& operator++() {
			if (uuid.Data4[7] == Math::Max<UInt8>())
				if (uuid.Data4[6] == Math::Max<UInt8>())
					if (uuid.Data4[5] == Math::Max<UInt8>())
						if (uuid.Data4[4] == Math::Max<UInt8>())
							if (uuid.Data4[3] == Math::Max<UInt8>())
								if (uuid.Data4[2] == Math::Max<UInt8>())
									if (uuid.Data4[1] == Math::Max<UInt8>())
										if (uuid.Data4[0] == Math::Max<UInt8>())
											if (uuid.Data3 == Math::Max<UInt16>())
												if (uuid.Data2 == Math::Max<UInt16>())
													if (uuid.Data1 != Math::Max<UInt32>()) uuid.Data1 += 1;
													else return *this;
												else uuid.Data2 += 1;
											else uuid.Data3 += 1;
										else uuid.Data4[0] += 1;
									else uuid.Data4[1] += 1;
								else uuid.Data4[2] += 1;
							else uuid.Data4[3] += 1;
						else uuid.Data4[4] += 1;
					else uuid.Data4[5] += 1;
				else uuid.Data4[6] += 1;
			else uuid.Data4[7] += 1;
			return *this;
		}

		/// <summary>
		/// Prefix decrement
		/// </summary>
		/// <returns>Returns old value</returns>
		constexpr __forceinline UUID operator--(int) {
			const UUID uuidCp = this->uuid;
			--* this;
			return uuidCp;
		}

		/// <summary>
		/// Prefix increment
		/// </summary>
		/// <returns>Returns old value</returns>
		constexpr __forceinline UUID operator++(int) {
			const UUID uuidCp = this->uuid;
			++* this;
			return uuidCp;
		}

		/// <summary>
		/// Universal comparing
		/// </summary>
		/// <returns>Relativity of objects</returns>
		constexpr auto operator<=>(const UUID& right) const {
			const auto result = this->uint128.fields[0] <=> right.uint128.fields[0];

			if (result == nullptr)
			{
				return this->uint128.fields[1] <=> right.uint128.fields[1];
			}

			return result;
		}

		/// <summary>
		/// Equality operator for UUID
		/// </summary>
		/// <param name="right">Right opera</param>
		/// <returns>Equality result</returns>
		constexpr bool operator==(const UUID& right) const {
			return (*this <=> right) == 0;
		}

		/// <summary>
		/// Nonequality operator for UUID
		/// </summary>
		/// <param name="right">Right opera</param>
		/// <returns>Nonequality result</returns>
		constexpr bool operator!=(const UUID& right) const {
			return (*this <=> right) != 0;
		}
	};
}