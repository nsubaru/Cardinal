export module Cardinal.RTTI:MetaData.Box;

export import :MetaData.Base;

export namespace Cardinal::RTTI::MetaData
{
	/// <summary>
	/// Receiving the CTTI metadata for box CTTI type
	/// </summary>
	/// <returns>CTTI metadata</returns>
	template<CTTI::Concepts::IsBoxCttiType T>
	inline constexpr auto GetCTTIMetaData() {
		constexpr auto Item = WithSize(GetCTTIItemMetaData<typename CTTI::TypeTraits::CTTITypeMetaData<T>::ValueType>(Cardinal::TypeTraits::CreateTypeStr(L"Item")));
#pragma pack(push, 1)
		struct MetaData {
			SizeT OffsetToCollection;
			decltype(AsRttiCollection<1>(Item)) ValueType;
		};
#pragma pack(pop)

		return MetaData{
			.OffsetToCollection = StructOffsetOfMember(MetaData, ValueType),
			.ValueType = AsRttiCollection<1>(Item),
		};
	}

	/// <summary>
	/// Receiving the CTTI metadata for pointer CTTI type
	/// </summary>
	/// <returns>CTTI metadata</returns>
	template<CTTI::Concepts::IsPointerCttiType T>
	inline constexpr auto GetCTTIMetaData() {
		constexpr auto Item = WithSize(GetCTTIItemMetaData<typename CTTI::TypeTraits::CTTITypeMetaData<T>::TargetTypeMetaData::Type>(Cardinal::TypeTraits::CreateTypeStr(L"Item")));
#pragma pack(push, 1)
		struct MetaData {
			SizeT OffsetToCollection;
			decltype(AsRttiCollection<1>(Item)) TargetTypeMetaData;
		};
#pragma pack(pop)

		return MetaData{
			.OffsetToCollection = StructOffsetOfMember(MetaData, TargetTypeMetaData),
			.TargetTypeMetaData = AsRttiCollection<1>(Item)
		};
	}

	/// <summary>
	/// Receiving the CTTI metadata for reference CTTI type
	/// </summary>
	/// <returns>CTTI metadata</returns>
	template<CTTI::Concepts::IsReferenceCttiType T>
	inline constexpr auto GetCTTIMetaData() {
		constexpr auto Item = WithSize(GetCTTIItemMetaData<typename CTTI::TypeTraits::CTTITypeMetaData<T>::TargetTypeMetaData::Type>(Cardinal::TypeTraits::CreateTypeStr(L"Item")));
#pragma pack(push, 1)
		struct MetaData {
			SizeT OffsetToCollection;
			Boolean IsLvalue;
			decltype(AsRttiCollection<1>(Item)) TargetTypeMetaData;
		};
#pragma pack(pop)

		return MetaData{
			.OffsetToCollection = StructOffsetOfMember(MetaData, TargetTypeMetaData),
			.IsLvalue = CTTI::TypeTraits::CTTITypeMetaData<T>::IsLvalue,
			.TargetTypeMetaData = AsRttiCollection<1>(Item)
		};
	}

	/// <summary>
	/// Receiving the CTTI metadata for fixed array CTTI type
	/// </summary>
	/// <returns>CTTI metadata</returns>
	template<CTTI::Concepts::IsFixedArrayCttiType T>
	inline constexpr auto GetCTTIMetaData() {
		constexpr auto Item = WithSize(GetCTTIItemMetaData<typename CTTI::TypeTraits::CTTITypeMetaData<T>::Target>(Cardinal::TypeTraits::CreateTypeStr(L"Item")));
#pragma pack(push, 1)
		struct MetaData {
			SizeT OffsetToCollection;
			SizeT ArrayLenght;
			decltype(AsRttiCollection<1>(Item)) ItemType;
		};
#pragma pack(pop)

		return MetaData{
			.OffsetToCollection = StructOffsetOfMember(MetaData, ItemType),
			.ArrayLenght = CTTI::TypeTraits::CTTITypeMetaData<T>::Count,
			.ItemType = AsRttiCollection<1>(Item)
		};
	}
}