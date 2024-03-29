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
	/// A compile time type meta data record type
	/// </summary>
#pragma pack(push, 1)
	template<SizeT TypeNameSz, SizeT CXXTypeNameSz, SizeT SerializationSchemaSz, SizeT TemplateTypeNameSz, typename TCttiMetaData>
	struct CMetaData {
		SizeT OffsetToCttiMetaData;
		UniversalTypeIdentificator TypeId;
		CTTI::EType TypeClass;
		MetaDataForDynamicRecord MetaDataForDynamic;
		TypeModificator TypeModificators;
		CttiString<TypeNameSz> Name;
		CttiString<CXXTypeNameSz> CxxName;
		CttiString<SerializationSchemaSz> SerializationSchema;
		CttiString<TemplateTypeNameSz> TemplateTypeName;
		TCttiMetaData CttiMetaData;
	};
#pragma pack(pop)

	/// <summary>
	/// A method for receiving the metadata
	/// </summary>
	/// <typeparam name="T">A type from which the metadata is need to get</typeparam>
	/// <returns>Type metadata</returns>
	template<typename T>
	inline constexpr auto GetMetaData() {
		constexpr auto data = GetCTTIMetaData<T>();

		using tMetaData = CMetaData<
			CTTI::TypeTraits::CTTITypeMetaData<T>::TypeName.Count,
			CTTI::TypeTraits::CTTITypeMetaData<T>::CXXTypeName.Count,
			Serialization::CTTIMetaDataGen<T>().Count,
			CTTI::TypeTraits::CTTITypeMetaData<T>::TemplateTypeName.Count,
			decltype(data)
		>;

		return tMetaData{
			.OffsetToCttiMetaData = StructOffsetOfMember(tMetaData, CttiMetaData),
			.TypeId = GetUniversalTypeIdentificatorByType<T>(),
			.TypeClass = CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass,
			.MetaDataForDynamic = GetFieldMetaDataRecordForType<T>(),
			.TypeModificators = GetTypeModificator<T>(),
			.Name = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::TypeName),
			.CxxName = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::CXXTypeName),
			.SerializationSchema = GetTextRecord(Serialization::CTTIMetaDataGen<T>()),
			.TemplateTypeName = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<T>::TemplateTypeName),
			.CttiMetaData = data
		};
	}

	/// <summary>
	/// A method for receiving the metadata
	/// </summary>
	/// <typeparam name="T">A type from which the metadata is need to get</typeparam>
	/// <returns>Type metadata</returns>
	template<>
	inline constexpr auto GetMetaData<void>() {
		using tMetaData = CMetaData<
			Cardinal::TypeTraits::CreateTypeStr(L"void").Count,
			Cardinal::TypeTraits::CreateTypeStr(L"void").Count,
			Cardinal::TypeTraits::CreateTypeStr(L"").Count,
			Cardinal::TypeTraits::CreateTypeStr(L"void").Count,
			Cardinal::TypeTraits::TypeTag<void>
		>;

		return tMetaData{
			.OffsetToCttiMetaData = StructOffsetOfMember(tMetaData, CttiMetaData),
			.TypeId = GetUniversalTypeIdentificatorByType<void>(),
			.TypeClass = CTTI::TypeTraits::CTTITypeMetaData<void>::TypeClass,
			.MetaDataForDynamic = GetFieldMetaDataRecordForType<void>(),
			.TypeModificators = GetTypeModificator<void>(),
			.Name = GetTextRecord(Cardinal::TypeTraits::CreateTypeStr(L"void")),
			.CxxName = GetTextRecord(Cardinal::TypeTraits::CreateTypeStr(L"void")),
			.SerializationSchema = GetTextRecord(TypeTraits::CreateTypeStr(L"")),
			.TemplateTypeName = GetTextRecord(Cardinal::TypeTraits::CreateTypeStr(L"void")),
			.CttiMetaData = {}
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