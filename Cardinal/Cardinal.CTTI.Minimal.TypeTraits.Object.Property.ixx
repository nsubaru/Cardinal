export module Cardinal.CTTI.Minimal:TypeTraits.Object.Property;

export import :TypeTraits.Object.Method;

export namespace Cardinal::CTTI::TypeTraits
{
	template<typename TName, TName(*PName)(), typename TGets, typename TSets>
		requires(
	Cardinal::TypeTraits::IsTupleV<TGets>&&
		Cardinal::TypeTraits::IsTupleV<TSets>&&
		Cardinal::TypeTraits::IsConstexprArrayV<TName> &&
		(TGets::Count > 0 || TSets::Count > 0))
		struct PropertyRecord {
		constexpr static TName Name = PName();
		using Getters = TGets;
		using Setters = TSets;
		using T = typename Cardinal::TypeTraits::RemoveCvRefV<
			typename Cardinal::TypeTraits::ConditionV<(TGets::Count > 0),
			typename TGets,
			typename TSets
		>::template ItemType<0>::OriginalRet
		>;
	};
}
