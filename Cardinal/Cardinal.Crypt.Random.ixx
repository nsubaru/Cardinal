export module Cardinal.Crypt:Random;

import :Base;

export namespace Cardinal::Crypt
{
	/// <summary>
	/// Returns the random number in the "0 - (Max UInt32 - 1)" range
	/// </summary>
	UInt32 Random();

	/// <summary>
	/// Generates the unique value within the launch
	/// </summary>
	UUID UniqueUUID();

	/// <summary>
	/// Generates the unique data block with given size
	/// </summary>
	/// <param name='data'>Memory block</param>
	/// <param name='size'>The memory area size in bytes</param>
	InternalApiCall void GetUniqueDataUnsafe(Byte* data, SizeT size);

	/// <summary>
	/// Generates the unique data block with given size
	/// </summary>
	/// <returns>A fixed array</returns>
	template<SizeT TCount>
	Containers::SensitiveFixedArray<TCount, Byte> GetUniqueData() {
		auto data = Containers::SensitiveFixedArray<TCount, Byte>{};
		using RawType = const Byte(&)[TCount];
		GetUniqueDataUnsafe(static_cast<Byte*>(static_cast<RawType>(data)), TCount);
	}
}