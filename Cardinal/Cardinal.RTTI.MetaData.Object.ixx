export module Cardinal.RTTI:MetaData.Object;

export import :MetaData.Object.Base;
export import :MetaData.Object.Constructor;
export import :MetaData.Object.Interface;
export import :MetaData.Object.Method;
export import :MetaData.Object.Property;

export namespace Cardinal::RTTI::MetaData
{
#pragma pack(push, 1)
	template<typename TConstructors, typename TInterfaces, typename TProperties, typename TMethods>
	struct CtObjectMetaData {
		SizeT ConstructorsOffset;
		SizeT InterfacesOffset;
		SizeT PropertiesOffset;
		SizeT MethodsOffset;
		TConstructors Constructors;
		TInterfaces Interfaces;
		TProperties Properties;
		TMethods Methods;
	};
#pragma pack(pop)

	template<typename T> requires ((CTTI::Concepts::IsObjectCttiType<T> || CTTI::Concepts::IsAbstractCttiType<T>) && !CTTI::Concepts::IsCustomCttiType<T>)
		inline constexpr auto GetCTTIMetaData() {
		using ClassRecord = typename CTTI::TypeTraits::CTTITypeMetaData<T>::ClassRecord;

		using TConstructors = typename ClassRecord::Constructors::Constructors;
		using TInterfaces = typename ClassRecord::Interfaces;
		using TProperties = typename ClassRecord::Properties;
		using TMethods = typename ClassRecord::Methods;

		constexpr auto ConstructorsCollection = AsRttiCollection<TConstructors::Count>(GetConstructors<TConstructors>());
		constexpr auto InterfacesCollection = AsRttiCollection<TInterfaces::Count>(GetInterface<TInterfaces>());
		constexpr auto PropertiesCollection = AsRttiCollection<TProperties::Count>(GetProperties<TProperties>());
		constexpr auto MethodsCollection = AsRttiCollection<TMethods::Count>(GetCttiMethods<TMethods>());

		using tObjectMetaData = CtObjectMetaData<
			decltype(ConstructorsCollection),
			decltype(InterfacesCollection),
			decltype(PropertiesCollection),
			decltype(MethodsCollection)>;

		return tObjectMetaData{
			.ConstructorsOffset = StructOffsetOfMember(tObjectMetaData, Constructors),
			.InterfacesOffset = StructOffsetOfMember(tObjectMetaData, Interfaces),
			.PropertiesOffset = StructOffsetOfMember(tObjectMetaData, Properties),
			.MethodsOffset = StructOffsetOfMember(tObjectMetaData, Methods),
			.Constructors = ConstructorsCollection,
			.Interfaces = InterfacesCollection,
			.Properties = PropertiesCollection,
			.Methods = MethodsCollection
		};
	}
}