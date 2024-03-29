export module Cardinal.RTTI:MetaData.Dynamic;

export import :MetaData.Base;

export namespace Cardinal::RTTI::MetaData
{
	/// <summary>
	/// Receiving the CTTI metadata for dynamic CTTI type
	/// </summary>
	/// <returns>CTTI metadata</returns>
	template<CTTI::Concepts::IsDynamicCttiType T>
	inline constexpr auto GetCTTIMetaData() {
		return TypeTraits::TypeTag<void>{};
	}
}