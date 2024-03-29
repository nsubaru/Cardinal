export module Cardinal.Containers.Base;

export import Cardinal.Core;

export namespace Cardinal::Containers
{
	/// <summary>
	/// Default character comparator use operator<=>
	/// </summary>
	constexpr auto DefaultCharacterComparator = [](Char l, Char r) InlineLambda->StrongOrdering{ return l <=> r; };

	/// <summary>
	/// Case insesitive character comparator use Characters::CaseInsesitiveCompare function
	/// </summary>
	constexpr auto CaseInsesitiveCharacterComparator = [](Char l, Char r) InlineLambda->StrongOrdering{ return Characters::CaseInsesitiveCompare(l, r); };

	/// <summary>
	/// Character comparator function type
	/// </summary>
	export using TCharacterComparator = StrongOrdering(*)(Char l, Char r);
}