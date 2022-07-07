export module Cardinal.Core:Bytes;

import :Types;

export namespace Cardinal::Core
{
	/// <summary>
	/// General functions for working with bytes ordering
	/// </summary>
	class Bytes
	{
	public:
		/// <summary>
		/// Reverses the bytes order for Int32
		/// </summary>
		/// <param name="value">Given set of bytes</param>
		/// <returns>Reversed set of bytes</returns>
		static Int32 ReverseByteOrder(Int32 value)
		{
			return	((value >> 24) & 0xff)    |		// Move byte 3 to byte 0
				    ((value << 8) & 0xff0000) |		// Move byte 1 to byte 2
				    ((value >> 8) & 0xff00)   |		// Move byte 2 to byte 1
				    ((value << 24) & 0xff000000);	// Byte 0 to byte 3
		}
	};
}