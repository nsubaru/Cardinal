export module Cardinal.Crypt:Crc;

import :Base;

namespace Cardinal::Crypt::Internals
{
	extern "C" {
		extern unsigned __int64 _mm_crc32_u64(unsigned __int64 /* crc */, unsigned __int64 /* v */);
	}
}

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
			salt = Internals:: _mm_crc32_u64(data[TSize], salt);
		}

		return salt;
	}
}