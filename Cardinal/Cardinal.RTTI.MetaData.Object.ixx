export module Cardinal.RTTI:MetaData.Object;

export import :MetaData.Object.Base;
export import :MetaData.Object.Constructor;
export import :MetaData.Object.Interface;
export import :MetaData.Object.Method;
export import :MetaData.Object.Property;

export namespace Cardinal::RTTI::MetaData
{
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

#pragma pack(push, 1)
		struct ObjectMetaData {
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
}