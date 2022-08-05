namespace Cardinal.RTTI.Utils;

internal unsafe static class Utils
{
	/// <summary>
	/// Performe search byte array in another byte arrat startuing from specific offset
	/// </summary>
	/// <param name="blob">Array in which search will perfom</param>
	/// <param name="len">Array size</param>
	/// <param name="what">Array which will search</param>
	/// <param name="offset">Search start position</param>
	/// <returns>First occurs of "what" array</returns>
	public static SizeT BinnarySearchInBlob(Byte* blob, SizeT len, Byte[] what, SizeT offset)
	{
		for (SizeT i = offset; i < len - (SizeT)what.Length; i++)
		{
			if (blob[i] == what[0])
			{
				SizeT j = 1;

				for (; j < (SizeT)what.Length; j++)
				{
					if (blob[i + j] != what[j])
						break;
				}

				if (j != (SizeT)what.Length)
					continue;

				return i;
			}
		}

		return SizeT.MaxValue;
	}
}
