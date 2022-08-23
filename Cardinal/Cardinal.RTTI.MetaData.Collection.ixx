export module Cardinal.RTTI:MetaData.Collection;

export import :MetaData.Base;

namespace Cardinal::RTTI::MetaData
{
	/// <summary>
	/// Receiving the CTTI metadata for collection CTTI type
	/// </summary>
	/// <returns>CTTI metadata</returns>
	template<CTTI::Concepts::IsCollectionCttiType T>
	inline constexpr auto GetCTTIMetaData() {
		constexpr auto Item = WithSize(GetCTTIItemMetaData<typename CTTI::TypeTraits::CTTITypeMetaData<T>::ItemType>(Core::TypeTraits::CreateTypeStr(L"Item")));
#pragma pack(push, 1)
		struct MetaData {
			SizeT OffsetToCollection;
			Boolean IsConcurencySupport;
			Boolean IsIndexable;
			decltype(AsRttiCollection<1>(Item)) ItemType;
		};
#pragma pack(pop)

		return MetaData{
			.OffsetToCollection = StructOffsetOfMember(MetaData, ItemType),
			.IsConcurencySupport = CTTI::TypeTraits::CTTITypeMetaData<T>::IsConcurencySupport,
			.IsIndexable = CTTI::TypeTraits::CTTITypeMetaData<T>::IsIndexable,
			.ItemType = AsRttiCollection<1>(Item)
		};
	}

	/// <summary>
	/// Receiving the CTTI metadata for string CTTI type
	/// </summary>
	/// <returns>CTTI metadata</returns>
	template<CTTI::Concepts::IsStringCttiType T>
	inline constexpr auto GetCTTIMetaData() {
		constexpr auto Item = WithSize(GetCTTIItemMetaData<typename CTTI::TypeTraits::CTTITypeMetaData<T>::Character>(Core::TypeTraits::CreateTypeStr(L"CharacterType")));
#pragma pack(push, 1)
		struct MetaData {
			SizeT OffsetToCollection;
			decltype(AsRttiCollection<1>(Item)) CharacterType;
		};
#pragma pack(pop)

		return MetaData{
			.OffsetToCollection = StructOffsetOfMember(MetaData, CharacterType),
			.CharacterType = AsRttiCollection<1>(Item)
		};
	}

	/// <summary>
	/// Receiving the CTTI metadata for fixed collection CTTI type
	/// </summary>
	/// <returns>CTTI metadata</returns>
	template<CTTI::Concepts::IsFixedCollectionCttiType T>
	inline constexpr auto GetCTTIMetaData() {
		constexpr auto Item = WithSize(GetCTTIItemMetaData<typename CTTI::TypeTraits::CTTITypeMetaData<T>::ItemType>(Core::TypeTraits::CreateTypeStr(L"Item")));
#pragma pack(push, 1)
		struct MetaData {
			SizeT OffsetToCollection;
			Boolean IsConcurencySupport;
			SizeT CollectionItemsCount;
			decltype(AsRttiCollection<1>(Item)) ItemType;
		};
#pragma pack(pop)

		return MetaData{
			.OffsetToCollection = StructOffsetOfMember(MetaData, ItemType),
			.IsConcurencySupport = CTTI::TypeTraits::CTTITypeMetaData<T>::IsConcurencySupport,
			.CollectionItemsCount = CTTI::TypeTraits::CTTITypeMetaData<T>::ItemCount,
			.ItemType = AsRttiCollection<1>(Item)
		};
	}

	/// <summary>
	/// Receiving the associable collection items
	/// </summary>
	/// <returns>Associable collection items</returns>
	template<CTTI::Concepts::IsAssociableCollectionCttiType T>
	inline constexpr auto GetAssociableCollectionItems() {
		constexpr auto Item = WithSize(GetCTTIItemMetaData<typename CTTI::TypeTraits::CTTITypeMetaData<T>::ItemType>(Core::TypeTraits::CreateTypeStr(L"Item")));
		constexpr auto Key = WithSize(GetCTTIItemMetaData<typename CTTI::TypeTraits::CTTITypeMetaData<T>::KeyType>(Core::TypeTraits::CreateTypeStr(L"Key")));
		constexpr auto Value = WithSize(GetCTTIItemMetaData<typename CTTI::TypeTraits::CTTITypeMetaData<T>::ValueType>(Core::TypeTraits::CreateTypeStr(L"Value")));
#pragma pack(push, 1)
		struct MetaData {
			decltype(Item) ItemType;
			decltype(Key) KeyType;
			decltype(Value) ValueType;
		};
#pragma pack(pop)

		return MetaData{
			.ItemType = Item,
			.KeyType = Key,
			.ValueType = Value
		};
	}

	/// <summary>
	/// Receiving the CTTI metadata for associable collection CTTI type
	/// </summary>
	/// <returns>CTTI metadata</returns>
	template<CTTI::Concepts::IsAssociableCollectionCttiType T>
	inline constexpr auto GetCTTIMetaData() {
#pragma pack(push, 1)
		struct MetaData {
			SizeT OffsetToCollection;
			Boolean IsConcurencySupport;
			decltype(AsRttiCollection<3>(GetAssociableCollectionItems<T>())) Items;
		};
#pragma pack(pop)

		return MetaData{
			.OffsetToCollection = StructOffsetOfMember(MetaData, Items),
			.IsConcurencySupport = CTTI::TypeTraits::CTTITypeMetaData<T>::IsConcurencySupport,
			.Items = AsRttiCollection<3>(GetAssociableCollectionItems<T>())
		};
	}

	/// <summary>
	/// Receiving the pair type items
	/// </summary>
	/// <returns>Pair items</returns>
	template<CTTI::Concepts::IsPairCttiType T>
	inline constexpr auto GetPairItems() {
		constexpr auto Key = WithSize(GetCTTIItemMetaData<typename CTTI::TypeTraits::CTTITypeMetaData<T>::KeyType>(Core::TypeTraits::CreateTypeStr(L"Key")));
		constexpr auto Value = WithSize(GetCTTIItemMetaData<typename CTTI::TypeTraits::CTTITypeMetaData<T>::ValueType>(Core::TypeTraits::CreateTypeStr(L"Value")));
#pragma pack(push, 1)
		struct MetaData {
			decltype(Key) KeyType;
			decltype(Value) ValueType;
		};
#pragma pack(pop)

		return MetaData{
			.KeyType = Key,
			.ValueType = Value
		};
	}

	/// <summary>
	/// Receiving the CTTI metadata for pair CTTI type
	/// </summary>
	/// <returns>CTTI metadata</returns>
	template<CTTI::Concepts::IsPairCttiType T>
	inline constexpr auto GetCTTIMetaData() {
#pragma pack(push, 1)
		struct MetaData {
			SizeT OffsetToCollection;
			decltype(AsRttiCollection<2>(GetPairItems<T>())) Items;
		};
#pragma pack(pop)

		return MetaData{
			.OffsetToCollection = StructOffsetOfMember(MetaData, Items),
			.Items = AsRttiCollection<2>(GetPairItems<T>())
		};
	}
}