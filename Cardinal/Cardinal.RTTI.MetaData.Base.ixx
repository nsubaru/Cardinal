export module Cardinal.RTTI:MetaData.Base;

export import :Exceptions;

export namespace Cardinal::RTTI::MetaData
{
	template<typename T>
	const MetaData* GetTypeRecord();

	/// <summary>
	/// Receiving the metadata of CTTI item for structure and union
	/// </summary>
	/// <typeparam name="T">CTTI item type</typeparam>
	/// <returns>Type metadata of CTTI item</returns>
	template<typename T, SizeT TIdx>
		requires CTTI::Concepts::IsStructCttiType<T> || CTTI::Concepts::IsUnionCttiType<T>
	inline constexpr auto GetCTTIItemMetaData() {
		using ItemType = typename decltype(CTTI::TypeTraits::CTTITypeMetaData<T>::Fields)::template ItemType<TIdx>::Type;
#pragma pack(push, 1)
		struct MetaDataField {
			UniversalTypeIdentificator TypeId;
			CTTI::EType TypeClass;
			SizeT Offset;
			MetaDataForDynamicRecord MetaDataForDynamic;
			TypeModificator TypeModificators;
			decltype(GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::Fields.GetByIdx<TIdx>().FieldName)) FieldName;
			decltype(GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<ItemType>::TypeName)) TypeName;
		};
#pragma pack(pop)

		return MetaDataField{
			.TypeId = GetUniversalTypeIdentificatorByType<ItemType>(),
			.TypeClass = CTTI::TypeTraits::CTTITypeMetaData<ItemType>::TypeClass,
			.Offset = CTTI::TypeTraits::CTTITypeMetaData<T>::Fields.GetByIdx<TIdx>().FieldOffset,
			.MetaDataForDynamic = GetFieldMetaDataRecordForType<ItemType>(),
			.TypeModificators = GetTypeModificator<ItemType>(),
			.FieldName = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::Fields.GetByIdx<TIdx>().FieldName),
			.TypeName = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<ItemType>::TypeName),
		};
	}

	/// <summary>
	/// Unsafe getter from tuple item
	/// </summary>
	/// <typeparam name="T">Tuple item type</typeparam>
	/// <param name="src">Source</param>
	/// <param name="dest">Destinatiom</param>
	template<typename T, SizeT TIdx>
	void UnsafeGetFromTuple(void* src, void* dest) {
		using ItemType = typename TypeTraits::RemoveCvRefV<typename T::template ItemType<TIdx>>;

		new (dest) ItemType(((T*) src)->GetByIdx<TIdx>());
	}

	/// <summary>
	/// Receiving the metadata of CTTI item for tuple
	/// </summary>
	/// <typeparam name="T">CTTI item type</typeparam>
	/// <returns>Type metadata of CTTI item</returns>
	template<typename T, SizeT TIdx> requires CTTI::Concepts::IsTupleCttiType<T>
	inline constexpr auto GetCTTIItemMetaData() {
		using ItemType = typename CTTI::TypeTraits::CTTITypeMetaData<T>::Type::template ItemType<TIdx>;
#pragma pack(push, 1)
		struct MetaDataField {
			UniversalTypeIdentificator TypeId;
			CTTI::EType TypeClass;
			TUnsafeGet UnsafeGet;
			MetaDataForDynamicRecord MetaDataForDynamic;
			TypeModificator TypeModificators;
			decltype(GetTextRecord(Cardinal::TypeTraits::ToCharStr<TIdx>())) FieldName;
			decltype(GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<ItemType>::TypeName)) TypeName;
		};
#pragma pack(pop)

		return MetaDataField{
			.TypeId = GetUniversalTypeIdentificatorByType<ItemType>(),
			.TypeClass = CTTI::TypeTraits::CTTITypeMetaData<ItemType>::TypeClass,
			.UnsafeGet = &UnsafeGetFromTuple<T, TIdx>,
			.MetaDataForDynamic = GetFieldMetaDataRecordForType<ItemType>(),
			.TypeModificators = GetTypeModificator<ItemType>(),
			.FieldName = GetTextRecord(Cardinal::TypeTraits::ToCharStr<TIdx>()),
			.TypeName = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<ItemType>::TypeName),
		};
	}

	/// <summary>
	/// Receiving the metadata of CTTI item for enumeration CTTI type
	/// </summary>
	/// <typeparam name="T">CTTI item type</typeparam>
	/// <returns>Type metadata of CTTI item</returns>
	template<typename T, SizeT TIdx> requires CTTI::Concepts::IsEnumertationCttiType<T>
	inline constexpr auto GetCTTIItemMetaData() {
#pragma pack(push, 1)
		struct MetaDataField {
			UniversalTypeIdentificator TypeId;
			CTTI::EType TypeClass;
			SizeT Offset;
			MetaDataForDynamicRecord MetaDataForDynamic;
			TypeModificator TypeModificators;
			decltype(GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::ItemsMap.GetByIdx<TIdx>().GetByIdx<1>())) FieldName; //-V807
			decltype(GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::ItemsMap.GetByIdx<TIdx>().GetByIdx<2>())) TypeName;
		};
#pragma pack(pop)

		return MetaDataField{
			.TypeId = {},
			.TypeClass = CTTI::EType::Undefine,
			.Offset = ~0Ui64,
			.MetaDataForDynamic = GetFieldMetaDataRecordForType<T>(),
			.TypeModificators = ETypeModificators::Const | ETypeModificators::Serilizable,
			.FieldName = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::ItemsMap.GetByIdx<TIdx>().GetByIdx<1>()),
			.TypeName = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::ItemsMap.GetByIdx<TIdx>().GetByIdx<2>()),
		};
	}

	/// <summary>
	/// Receiving the metadata of CTTI item
	/// </summary>
	/// <typeparam name="T">CTTI item type</typeparam>
	/// <returns>Type metadata of CTTI item</returns>
	template<typename T, SizeT TN> requires (!CTTI::Concepts::IsEnumertationCttiType<T>)
		inline constexpr auto GetCTTIItemMetaData(Cardinal::TypeTraits::ConstexprArray<Char, TN> FieldName) {
#pragma pack(push, 1)
		struct MetaDataField {
			UniversalTypeIdentificator TypeId;
			CTTI::EType TypeClass;
			SizeT Offset;
			MetaDataForDynamicRecord MetaDataForDynamic;
			TypeModificator TypeModificators;
			decltype(GetTextRecord(FieldName)) FieldName;
			decltype(GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::TypeName)) TypeName;
		};
#pragma pack(pop)

		return MetaDataField{
			.TypeId = GetUniversalTypeIdentificatorByType<T>(),
			.TypeClass = CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass,
			.Offset = ~0Ui64,
			.MetaDataForDynamic = GetFieldMetaDataRecordForType<T>(),
			.TypeModificators = GetTypeModificator<T>(),
			.FieldName = GetTextRecord(FieldName),
			.TypeName = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::TypeName),
		};
	}

	/// <summary>
	/// Receiving the metadata of CTTI property
	/// </summary>
	/// <typeparam name="T">CTTI property type</typeparam>
	/// <param name="FieldName">Field name</param>
	/// <returns>Type metadata of CTTI property</returns>
	template<typename T, SizeT TN>
	inline constexpr auto GetCTTIPropItemMetaData(Cardinal::TypeTraits::ConstexprArray<Char, TN> FieldName) {
#pragma pack(push, 1)
		struct MetaDataField {
			UniversalTypeIdentificator TypeId;
			CTTI::EType TypeClass;
			SizeT Offset;
			MetaDataForDynamicRecord MetaDataForDynamic;
			TypeModificator TypeModificators;
			decltype(GetTextRecord(FieldName)) FieldName;
			decltype(GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::TypeName)) TypeName;
		};
#pragma pack(pop)

		return MetaDataField{
			.TypeId = GetUniversalTypeIdentificatorByType<T>(),
			.TypeClass = CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass,
			.Offset = ~0Ui64,
			.MetaDataForDynamic = GetFieldMetaDataRecordForType<T>(),
			.TypeModificators = GetTypeModificator<T>(),
			.FieldName = GetTextRecord(FieldName),
			.TypeName = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::TypeName),
		};
	}

	/// <summary>
	/// Add size field before data
	/// </summary>
	/// <typeparam name="T">Metadata item type</typeparam>
	/// <param name="data">Metadata</param>
	/// <returns>Medatada with size</returns>
	template<typename T>
	inline constexpr auto WithSize(T data) {
#pragma pack(push, 1)
		struct WithSizeT {
			SizeT Size;
			T Data;
		};
#pragma pack(pop)
		return WithSizeT{
			.Size = sizeof(WithSizeT),
			.Data = data
		};
	}

	/// <summary>
	/// Cast the item as RTTI record
	/// </summary>
	/// <typeparam name="TItems">Items type</typeparam>
	/// <param name="items">Set of items</param>
	/// <returns>Cast result value</returns>
	template<SizeT TCount, typename TItems>
	inline constexpr auto AsRttiCollection(TItems items) {
		if constexpr (TCount != 0)
		{
#pragma pack(push, 1)
			struct RttiCollection {
				SizeT OffsetToFirstItem;
				SizeT Count;
				SizeT Size;
				TItems Items;
			};
#pragma pack(pop)
			return RttiCollection{
				.OffsetToFirstItem = StructOffsetOfMember(RttiCollection, Items),
				.Count = TCount,
				.Size = sizeof(RttiCollection),
				.Items = items
			};
		}
		else
		{
#pragma pack(push, 1)
			struct RttiCollection {
				SizeT OffsetToFirstItem;
				SizeT Count;
				SizeT Size;
			};
#pragma pack(pop)
			return RttiCollection{
				.OffsetToFirstItem = 0,
				.Count = TCount,
				.Size = sizeof(RttiCollection),
			};
		}
	}
}