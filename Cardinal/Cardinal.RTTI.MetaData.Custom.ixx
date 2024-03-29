export module Cardinal.RTTI:MetaData.Custom;

export import :MetaData.Base;
export import :MetaData.Struct;
export import :MetaData.Object;

export namespace Cardinal::RTTI::MetaData
{
	/// <summary>
	/// Generate compile time rtti meta data for custom type without any format
	/// </summary>
	/// <typeparam name="T">Type which characterized as CTTI custom type</typeparam>
	/// <returns>Compile time RTTI meta data</returns>
	template<typename T> requires (CTTI::TypeTraits::CTTITypeMetaData<T>::CustomTypeSchemaFormat == CTTI::TypeTraits::ECustomTypeSchemaFormat::None)
		inline constexpr auto GetCttiCustomTypeMetaDataFormatSpectification() {
#pragma pack(push, 1)
		struct StructWithAccessorMetaData {
			CTTI::TypeTraits::ECustomTypeSchemaFormat Format;
		};
#pragma pack(pop)

		return StructWithAccessorMetaData{
			.Format = CTTI::TypeTraits::ECustomTypeSchemaFormat::StructSchema
		};
	}

	/// <summary>
	/// Generate compile time rtti meta data for custom type with struct or remote struct schema
	/// </summary>
	/// <typeparam name="T">Type which characterized as CTTI custom type</typeparam>
	/// <returns>Compile time RTTI meta data</returns>
	template<typename T> requires (
		CTTI::TypeTraits::CTTITypeMetaData<T>::CustomTypeSchemaFormat == CTTI::TypeTraits::ECustomTypeSchemaFormat::StructSchema ||
		CTTI::TypeTraits::CTTITypeMetaData<T>::CustomTypeSchemaFormat == CTTI::TypeTraits::ECustomTypeSchemaFormat::RemoteStructSchema
		)
		inline constexpr auto GetCttiCustomTypeMetaDataFormatSpectification() {
#pragma pack(push, 1)
		struct StructWithAccessorMetaData {
			CTTI::TypeTraits::ECustomTypeSchemaFormat Format;
			SizeT OffsetToCollection;
			TUnsafeGet BaseGetter;
			decltype(AsRttiCollection<CTTI::TypeTraits::CTTITypeMetaData<T>::Fields.Count>(GetMetaDataStructFields<T, 0>())) Fields;
		};
#pragma pack(pop)

		return StructWithAccessorMetaData{
			.Format = CTTI::TypeTraits::ECustomTypeSchemaFormat::StructSchema,
			.OffsetToCollection = StructOffsetOfMember(StructWithAccessorMetaData, Fields),
			.BaseGetter = nullptr,
			.Fields = AsRttiCollection<CTTI::TypeTraits::CTTITypeMetaData<T>::Fields.Count>(GetMetaDataStructFields<T, 0>())
		};
	}

	/// <summary>
	/// Generate compile time rtti meta data for custom type with struct with accessor method
	/// </summary>
	/// <typeparam name="T">Type which characterized as CTTI custom type</typeparam>
	/// <returns>Compile time RTTI meta data</returns>
	template<typename T> requires (CTTI::TypeTraits::CTTITypeMetaData<T>::CustomTypeSchemaFormat == CTTI::TypeTraits::ECustomTypeSchemaFormat::StructWithAccessorMethodSchema)
		inline constexpr auto GetCttiCustomTypeMetaDataFormatSpectification() {
#pragma pack(push, 1)
		struct StructWithAccessorMetaData {
			CTTI::TypeTraits::ECustomTypeSchemaFormat Format;
			SizeT OffsetToCollection;
			TUnsafeGet BaseGetter;
			decltype(AsRttiCollection<CTTI::TypeTraits::CTTITypeMetaData<T>::Fields.Count>(GetMetaDataStructFields<T, 0>())) Fields;
		};
#pragma pack(pop)

		return StructWithAccessorMetaData{
			.Format = CTTI::TypeTraits::ECustomTypeSchemaFormat::StructSchema,
			.OffsetToCollection = StructOffsetOfMember(StructWithAccessorMetaData, Fields),
			.BaseGetter = &Cardinal::CTTI::TypeTraits::GetObjectOffset<T>,
			.Fields = AsRttiCollection<CTTI::TypeTraits::CTTITypeMetaData<T>::Fields.Count>(GetMetaDataStructFields<T, 0>())
		};
	}

	/// <summary>
	/// Generate compile time rtti meta data for custom type with object or remote object schema
	/// </summary>
	/// <typeparam name="T">Type which characterized as CTTI custom type</typeparam>
	/// <returns>Compile time RTTI meta data</returns>
	template<typename T> requires (
		CTTI::TypeTraits::CTTITypeMetaData<T>::CustomTypeSchemaFormat == CTTI::TypeTraits::ECustomTypeSchemaFormat::ObjectSchema ||
		CTTI::TypeTraits::CTTITypeMetaData<T>::CustomTypeSchemaFormat == CTTI::TypeTraits::ECustomTypeSchemaFormat::RemoteObjectSchema
		)
		inline constexpr auto GetCttiCustomTypeMetaDataFormatSpectification() {
		using ClassRecord = typename CTTI::TypeTraits::CTTITypeMetaData<T>::ClassRecord;

		using TConstructors = typename ClassRecord::Constructors::Constructors;
		using TInterfaces = typename ClassRecord::Interfaces;
		using TProperties = typename ClassRecord::Properties;
		using TMethods = typename ClassRecord::Methods;

		constexpr auto ConstructorsCollection = AsRttiCollection<TConstructors::Count>(GetConstructors<TConstructors>());
		constexpr auto InterfacesCollection = AsRttiCollection<TInterfaces::Count>(GetInterface<TInterfaces>());
		constexpr auto PropertiesCollection = AsRttiCollection<TProperties::Count>(GetProperties<TProperties>());
		constexpr auto MethodsCollection = AsRttiCollection<TMethods::Count>(GetCttiMethods<TMethods>());

#pragma pack(push, 1)
		struct ObjectMetaData {
			CTTI::TypeTraits::ECustomTypeSchemaFormat Format;
			SizeT ConstructorsOffset;
			SizeT InterfacesOffset;
			SizeT PropertiesOffset;
			SizeT MethodsOffset;
			decltype(ConstructorsCollection) Constructors;
			decltype(InterfacesCollection) Interfaces;
			decltype(PropertiesCollection) Properties;
			decltype(MethodsCollection) Methods;
		};
#pragma pack(pop)
		return ObjectMetaData{
			.Format = CTTI::TypeTraits::ECustomTypeSchemaFormat::ObjectSchema,
			.ConstructorsOffset = StructOffsetOfMember(ObjectMetaData, Constructors),
			.InterfacesOffset = StructOffsetOfMember(ObjectMetaData, Interfaces),
			.PropertiesOffset = StructOffsetOfMember(ObjectMetaData, Properties),
			.MethodsOffset = StructOffsetOfMember(ObjectMetaData, Methods),
			.Constructors = ConstructorsCollection,
			.Interfaces = InterfacesCollection,
			.Properties = PropertiesCollection,
			.Methods = MethodsCollection
		};
	}

	/// <summary>
	/// Receiving the CTTI metadata for custom CTTI type
	/// </summary>
	/// <returns>CTTI metadata</returns>
	template<typename T> requires CTTI::Concepts::IsCttiType<T, CTTI::EType::Custom>
	inline constexpr auto GetCTTIMetaData() {
		return GetCttiCustomTypeMetaDataFormatSpectification<T>();
	}
}