export module Cardinal.RTTI:MetaData;

export import :MetaData.Base;
export import :MetaData.Primitives;
export import :MetaData.Enumeration;
export import :MetaData.Box;
export import :MetaData.Collection;
export import :MetaData.Tuple;
export import :MetaData.Struct;
export import :MetaData.Dynamic;
export import :MetaData.Object;
export import :MetaData.Custom;

export namespace Cardinal::RTTI::MetaData
{
	/// <summary>
	/// A method for receiving the metadata
	/// </summary>
	/// <typeparam name="T">A type from which the metadata is need to get</typeparam>
	/// <returns>Type metadata</returns>
	template<typename T>
	inline constexpr auto GetMetaData() {
#pragma pack(push, 1)
		struct MetaData {
			SizeT OffsetToCttiMetaData;
			UniversalTypeIdentificator TypeId;
			CTTI::EType TypeClass;
			MetaDataForDynamicRecord MetaDataForDynamic;
			TypeModificator TypeModificators;
			decltype(GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::TypeName)) Name;
			decltype(GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::CXXTypeName)) CxxName;
			decltype(GetTextRecord(Serialization::CTTIMetaDataGen<T>())) SerializationSchema;
			decltype(GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::TemplateTypeName)) TemplateTypeName;
			decltype(GetCTTIMetaData<T>()) CttiMetaData;
		};
#pragma pack(pop)

		return MetaData{
			.OffsetToCttiMetaData = StructOffsetOfMember(MetaData, CttiMetaData),
			.TypeId = GetUniversalTypeIdentificatorByType<T>(),
			.TypeClass = CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass,
			.MetaDataForDynamic = GetFieldMetaDataRecordForType<T>(),
			.TypeModificators = GetTypeModificator<T>(),
			.Name = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::TypeName),
			.CxxName = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::CXXTypeName),
			.SerializationSchema = GetTextRecord(Serialization::CTTIMetaDataGen<T>()),
			.TemplateTypeName = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::TemplateTypeName),
			.CttiMetaData = GetCTTIMetaData<T>()
		};
	}

	/// <summary>
	/// A method for receiving the metadata
	/// </summary>
	/// <typeparam name="T">A type from which the metadata is need to get</typeparam>
	/// <returns>Type metadata</returns>
	template<>
	inline constexpr auto GetMetaData<void>() {
#pragma pack(push, 1)
		struct MetaData {
			SizeT OffsetToCttiMetaData;
			UniversalTypeIdentificator TypeId;
			CTTI::EType TypeClass;
			MetaDataForDynamicRecord MetaDataForDynamic;
			TypeModificator TypeModificators;
			decltype(GetTextRecord(Cardinal::TypeTraits::CreateTypeStr(L"void"))) Name;
			decltype(GetTextRecord(Cardinal::TypeTraits::CreateTypeStr(L"void"))) CxxName;
			decltype(GetTextRecord(TypeTraits::CreateTypeStr(L""))) SerializationSchema;
			decltype(GetTextRecord(Cardinal::TypeTraits::CreateTypeStr(L"void"))) TemplateTypeName;
		};
#pragma pack(pop)

		return MetaData{
			.OffsetToCttiMetaData = Math::Max<SizeT>(),
			.TypeId = GetUniversalTypeIdentificatorByType<void>(),
			.TypeClass = CTTI::TypeTraits::CTTITypeMetaData<void>::TypeClass,
			.MetaDataForDynamic = GetFieldMetaDataRecordForType<void>(),
			.TypeModificators = GetTypeModificator<void>(),
			.Name = GetTextRecord(Cardinal::TypeTraits::CreateTypeStr(L"void")),
			.CxxName = GetTextRecord(Cardinal::TypeTraits::CreateTypeStr(L"void")),
			.SerializationSchema = GetTextRecord(TypeTraits::CreateTypeStr(L"")),
			.TemplateTypeName = GetTextRecord(Cardinal::TypeTraits::CreateTypeStr(L"void"))
		};
	}

	template<typename T>
	inline const MetaData* GetTypeRecord() {
		static constexpr auto cttiTypeData = GetMetaData<T>();
		__declspec(allocate(".type$vb")) const volatile static auto typeData = cttiTypeData;
		__declspec(allocate(".type$fb")) const volatile static auto typeDataPtr = &typeData;
		return (MetaData*) typeDataPtr;
	}
}