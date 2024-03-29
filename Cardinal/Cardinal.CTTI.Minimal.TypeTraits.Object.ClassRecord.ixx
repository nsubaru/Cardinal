export module Cardinal.CTTI.Minimal:TypeTraits.Object.ClassRecord;

export import :TypeTraits.Object.Base;
export import :TypeTraits.Object.Method;
export import :TypeTraits.Object.Property;
export import :TypeTraits.Object.Interfaces;
export import :TypeTraits.Object.SpecialMethods;

export namespace Cardinal::CTTI::TypeTraits
{
	template<typename TClass, typename TConstructors, typename TInterfaces, typename TProperties, typename TMethods>
	struct ClassRecord {
		using Class = TClass;
		using Constructors = TConstructors;
		using Interfaces = TInterfaces;
		using Properties = TProperties;
		using Methods = TMethods;
		using Destructor = DestructorRecord<TClass>;
	};
}
