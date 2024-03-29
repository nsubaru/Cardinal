export module Cardinal.RTTI:MetaData.Object.Base;

export import :MetaData.Base;

export namespace Cardinal::RTTI::MetaData
{
	/// <summary>
	/// Receiving the type record of method
	/// </summary>
	/// <typeparam name="TOriginType">CTTI detailed type information</typeparam>
	/// <returns>The type record of method</returns>
	template<typename TOriginType>
	inline constexpr auto GetMethodTypeRecord() {
		using T = typename TOriginType::Type;
#pragma pack(push, 1)
		struct MetaDataField {
			UniversalTypeIdentificator TypeId;
			CTTI::EType TypeClass;
			MetaDataForDynamicRecord MetaDataForDynamic;
			Boolean IsSerializable;
			EnumFlags<Cardinal::CTTI::TypeTraits::ECvRefModificators> Flags;
			decltype(GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::TypeName)) TypeName;
		};
#pragma pack(pop)
		return MetaDataField{
			.TypeId = GetUniversalTypeIdentificatorByType<T>(),
			.TypeClass = CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass,
			.MetaDataForDynamic = GetFieldMetaDataRecordForType<T>(),
			.IsSerializable = CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable,
			.Flags = TOriginType::Flags,
			.TypeName = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::TypeName)
		};
	}
}