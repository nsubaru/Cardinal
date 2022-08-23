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
		if constexpr (!Concepts::IsSame<T, void>)
		{
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
		else
		{
#pragma pack(push, 1)
			struct MetaData {
				SizeT OffsetToCttiMetaData;
				UniversalTypeIdentificator TypeId;
				CTTI::EType TypeClass;
				MetaDataForDynamicRecord MetaDataForDynamic;
				TypeModificator TypeModificators;
				decltype(GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<void>::TypeName)) Name;
				decltype(GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<void>::CXXTypeName)) CxxName;
				decltype(GetTextRecord(TypeTraits::CreateTypeStr(L""))) SerializationSchema;
				decltype(GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<void>::TemplateTypeName)) TemplateTypeName;
			};
#pragma pack(pop)

			return MetaData{
				.OffsetToCttiMetaData = Math::Max<SizeT>(),
				.TypeId = GetUniversalTypeIdentificatorByType<void>(),
				.TypeClass = CTTI::TypeTraits::CTTITypeMetaData<void>::TypeClass,
				.MetaDataForDynamic = GetFieldMetaDataRecordForType<void>(),
				.TypeModificators = GetTypeModificator<void>(),
				.Name = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<void>::TypeName),
				.CxxName = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<void>::CXXTypeName),
				.SerializationSchema = GetTextRecord(TypeTraits::CreateTypeStr(L"")),
				.TemplateTypeName = GetTextRecord(CTTI::TypeTraits::CTTITypeMetaData<void>::TemplateTypeName)
			};
		}
	}

	template<typename T>
	inline const MetaData* GetTypeRecord() {
		static constexpr auto cttiTypeData = GetMetaData<T>();
		UnusedData __declspec(allocate(".type$vb")) const volatile static auto typeData = cttiTypeData;
		UnusedData __declspec(allocate(".type$fb")) const volatile static auto typeDataPtr = &typeData;
		return (MetaData*) typeDataPtr;
	}
}