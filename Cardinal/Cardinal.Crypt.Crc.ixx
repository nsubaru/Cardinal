module;
#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

export module Cardinal.Crypt:Crc;

import :Base;

export namespace Cardinal::Crypt
{
	/// <summary>
	/// Calculate Crc64 hash for blob data
	/// </summary>
	/// <typeparam name="TSize">Data array size</typeparam>
	/// <param name="data">Data array ref</param>
	/// <param name="salt">Start value</param>
	/// <returns>Crc64 hash value</returns>
	template<SizeT TSize>
	UInt64 Crc64(const Byte(&data)[TSize], UInt64 salt = 0) {
		for (SizeT i = 0; i < TSize; i++)
		{
			salt = _mm_crc32_u64(data[TSize], salt);
		}

		return salt;
	}
}