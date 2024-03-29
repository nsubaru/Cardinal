export module Cardinal.RTTI:MetaData.Enumeration;

export import :MetaData.Base;

export namespace Cardinal::RTTI::MetaData
{
	/// <summary>
	/// Receiving the CTTI enumeration items
	/// </summary>
	/// <returns>CTTI enumeration items</returns>
	template<CTTI::Concepts::IsEnumertationCttiType T, SizeT Idx>
	inline constexpr auto GetCttiEnumerationItems() {
		if constexpr (CTTI::TypeTraits::CTTITypeMetaData<T>::ItemsCount > Idx)
		{
#pragma pack(push, 1)
			struct ItemsMetaData {
				decltype(WithSize(GetCTTIItemMetaData<T, Idx>())) Data;
				decltype(GetCttiEnumerationItems<T, Idx + 1>()) Rest;
			};
#pragma pack(pop)

			return ItemsMetaData{
				.Data = WithSize(GetCTTIItemMetaData<T, Idx>()),
				.Rest = GetCttiEnumerationItems<T, Idx + 1>()
			};
		}
		else
		{
			return TypeTraits::TypeTag<void>();
		}
	}

	/// <summary>
	/// Receiving the enumeration items in collection
	/// </summary>
	/// <returns>Enumeration items in collection</returns>
	template<CTTI::Concepts::IsEnumertationCttiType T>
	inline constexpr auto GetEnumerationItemsCollection() {
		constexpr auto innerType = WithSize(GetCTTIItemMetaData<typename CTTI::TypeTraits::CTTITypeMetaData<T>::UnderlyingType>(Cardinal::TypeTraits::CreateTypeStr(L"UnderlyingType")));
#pragma pack(push, 1)
		struct MetaData {
			decltype(innerType) InnerType;
			decltype(GetCttiEnumerationItems<T, 0>()) Values;
		};
#pragma pack(pop)

		return MetaData{
			.InnerType = innerType,
			.Values = GetCttiEnumerationItems<T, 0>()
		};
	}

	/// <summary>
	/// Receiving the CTTI metadata for enumeration CTTI type
	/// </summary>
	/// <returns>CTTI metadata</returns>
	template<CTTI::Concepts::IsEnumertationCttiType T>
	inline constexpr auto GetCTTIMetaData() {
		constexpr auto collection = GetEnumerationItemsCollection<T>();
#pragma pack(push, 1)
		struct MetaData {
			SizeT OffsetToCollection;
			decltype(AsRttiCollection<CTTI::TypeTraits::CTTITypeMetaData<T>::ItemsCount + 1>(collection)) Collection;
		};
#pragma pack(pop)

		return MetaData{
			.OffsetToCollection = StructOffsetOfMember(MetaData, Collection),
			.Collection = AsRttiCollection<CTTI::TypeTraits::CTTITypeMetaData<T>::ItemsCount + 1>(collection)
		};
	}
}