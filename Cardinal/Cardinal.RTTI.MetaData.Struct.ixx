export module Cardinal.RTTI:MetaData.Struct;

export import :MetaData.Base;

export namespace Cardinal::CTTI::TypeTraits
{
	/// <summary>
	/// Receiving the object offset
	/// </summary>
	/// <typeparam name="T">Object type</typeparam>
	/// <param name="src">Source</param>
	/// <param name="desc">Description</param>
	template<typename T>
	inline void GetObjectOffset(void* src, void* desc) {
		desc = ((Serialization::ISerializable<T>*)(T*)(src))->GetDataPtr();
	}
}

export namespace Cardinal::RTTI::MetaData
{
	/// <summary>
	/// Receiving the structure fields of metatada
	/// </summary>
	/// <typeparam name="T">Object type</typeparam>
	/// <returns>Structure fields of metatada</returns>
	template<typename T, SizeT TFieldIdx> requires (CTTI::Concepts::IsStructCttiType<T> || CTTI::Concepts::IsUnionCttiType<T>)
		inline constexpr auto GetMetaDataStructFields() {
		if constexpr (TFieldIdx < CTTI::TypeTraits::CTTITypeMetaData<T>::Fields.Count)
		{
			constexpr auto Item = GetCTTIItemMetaData<T, TFieldIdx>();
#pragma pack(push, 1)
			struct StructItemMetaData {
				SizeT OffsetToNext;
				decltype(Item) Item;
				decltype(GetMetaDataStructFields<T, TFieldIdx + 1>()) Rest;
			};
#pragma pack(pop)

			return StructItemMetaData{
				.OffsetToNext = StructOffsetOfMember(StructItemMetaData, Rest),
				.Item = Item,
				.Rest = GetMetaDataStructFields<T, TFieldIdx + 1>()
			};
		}
		else
		{
			return TypeTraits::TypeTag<void>{};
		}
	}

	/// <summary>
	/// Receiving the CTTI metadata
	/// </summary>
	/// <typeparam name="T">CTTI item type</typeparam>
	/// <returns>CTTI metadata</returns>
	template<typename T> requires ((Cardinal::CTTI::Concepts::IsStructCttiType<T> || CTTI::Concepts::IsUnionCttiType<T>) && !Cardinal::CTTI::Concepts::IsStructWithAccessorMethodCttiType<T>)
		inline constexpr auto GetCTTIMetaData() {
#pragma pack(push, 1)
		struct StructMetaData {
			SizeT OffsetToCollection;
			SizeT BaseGetter;
			decltype(AsRttiCollection<CTTI::TypeTraits::CTTITypeMetaData<T>::Fields.Count>(GetMetaDataStructFields<T, 0>())) Fields;
		};
#pragma pack(pop)

		return StructMetaData{
			.OffsetToCollection = StructOffsetOfMember(StructMetaData, Fields),
			.BaseGetter = 0,
			.Fields = AsRttiCollection<CTTI::TypeTraits::CTTITypeMetaData<T>::Fields.Count>(GetMetaDataStructFields<T, 0>())
		};
	}

	/// <summary>
	/// Receiving the CTTI metadata
	/// </summary>
	/// <returns>CTTI metadata</returns>
	template<CTTI::Concepts::IsStructWithAccessorMethodCttiType T>
	inline constexpr auto GetCTTIMetaData() {
#pragma pack(push, 1)
		struct StructWithAccessorMetaData {
			SizeT OffsetToCollection;
			TUnsafeGet BaseGetter;
			decltype(AsRttiCollection<CTTI::TypeTraits::CTTITypeMetaData<T>::Fields.Count>(GetMetaDataStructFields<T, 0>())) Fields;
		};
#pragma pack(pop)

		return StructWithAccessorMetaData{
			.OffsetToCollection = StructOffsetOfMember(StructWithAccessorMetaData, Fields),
			.BaseGetter = &Cardinal::CTTI::TypeTraits::GetObjectOffset<T>,
			.Fields = AsRttiCollection<CTTI::TypeTraits::CTTITypeMetaData<T>::Fields.Count>(GetMetaDataStructFields<T, 0>())
		};
	}
}