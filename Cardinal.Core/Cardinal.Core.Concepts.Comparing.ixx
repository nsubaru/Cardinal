export module Cardinal.Core:Concepts.Comparing;

import :Concepts.Base;

export namespace Cardinal::Concepts
{
	/// <summary>
	/// Checking if "T" can compare with "U"
	/// </summary>
	template<typename T, typename U>
	concept IsComparableWith = requires(T x, U y) {
		{ x == y } -> IsSame<bool>;
		{ x != y } -> IsSame<bool>;
		{ x <= y } -> IsSame<bool>;
		{ x >= y } -> IsSame<bool>;

		{ x < y } -> IsSame<bool>;
		{ x > y } -> IsSame<bool>;
	};

	/// <summary>
	/// Checking if "T" can equal with "U"
	/// </summary>
	template<typename T, typename U>
	concept HaveEqual = requires(T x, U y) { { x == y } -> IsSame<bool>; };

	/// <summary>
	/// Checking if "T" can nonequal with "U"
	/// </summary>
	template<typename T, typename U>
	concept HaveNotEqual = requires(T x, U y) { { x != y } -> IsSame<bool>; };
}