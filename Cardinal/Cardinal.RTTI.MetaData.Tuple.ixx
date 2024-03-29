export module Cardinal.RTTI:MetaData.Tuple;

export import :MetaData.Base;

export namespace Cardinal::RTTI::MetaData
{
	/// <summary>
	/// Receiving the CTTI tuple field
	/// </summary>
	/// <returns>CTTI tuple field</returns>
	template<CTTI::Concepts::IsTupleCttiType T, SizeT Idx>
	inline constexpr auto GetCttiTupleField() {
		if constexpr (Idx < T::Count)
		{
			constexpr auto Item = GetCTTIItemMetaData<T, Idx>();
#pragma pack(push, 1)
			struct TupleField {
				SizeT OffsetToNext;
				decltype(Item) ItemType;
				decltype(GetCttiTupleField<T, Idx + 1>()) Rest;
			};
#pragma pack(pop)

			return TupleField{
				.OffsetToNext = StructOffsetOfMember(TupleField, Rest),
				.ItemType = Item,
				.Rest = GetCttiTupleField<T, Idx + 1>()
			};
		}
		else
		{
			return TypeTraits::TypeTag<void>();
		}
	}

	/// <summary>
	/// Receiving the CTTI metadata for tuple CTTI type
	/// </summary>
	/// <returns>CTTI metadata</returns>
	template<CTTI::Concepts::IsTupleCttiType T>
	inline constexpr auto GetCTTIMetaData() {
#pragma pack(push, 1)
		struct TupleCttiMetaData {
			SizeT OffsetToCollection;
			decltype(AsRttiCollection<T::Count>(GetCttiTupleField<T, 0>())) Fields;
		};
#pragma pack(pop)

		return TupleCttiMetaData{
			.OffsetToCollection = StructOffsetOfMember(TupleCttiMetaData, Fields),
			.Fields = AsRttiCollection<T::Count>(GetCttiTupleField<T, 0>())
		};
	}
}