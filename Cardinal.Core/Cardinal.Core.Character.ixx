export module Cardinal.Core:Character;

import :Types;
import :Comparing;

namespace Cardinal::Core
{
	/// <summary>
	/// General functions for working with characters
	/// </summary>
	class Characters
	{
	public:
		/// <summary>
		/// Indicates whether the variable is a digit
		/// </summary>
		/// <param name="ch">Given character</param>
		/// <returns>"True" if digit</returns>
		static bool IsDigit(Char ch)
		{
			return L'0' <= ch && ch <= L'9';
		}

		/// <summary>
		/// Case insesitive character comparing
		/// </summary>
		/// <param name="left">Left operand</param>
		/// <param name="right">Right operand</param>
		/// <returns>Comparing result</returns>
		static StrongOrdering CaseInsesitiveCompare(AChar left, AChar right);

		/// <summary>
		/// Case insesitive character comparing
		/// </summary>
		/// <param name="left">Left operand</param>
		/// <param name="right">Right operand</param>
		/// <returns>Comparing result</returns>
		static StrongOrdering CaseInsesitiveCompare(UChar left, UChar right);
	};
}