export module Cardinal.RTTI:Base.Base;

export import Cardinal.Core;
export import Cardinal.Memory;
export import Cardinal.CTTI;
export import Cardinal.Serialization;
export import Cardinal.Settings;
export import Cardinal.String;

export namespace Cardinal::RTTI
{
	using CTTI::EType;
	using CTTI::TypeTraits::ECvRefModificators;
	using CTTI::TypeTraits::ECustomTypeSchemaFormat;

	class Dynamic;

	template<typename THeap>
	class BaseTraveler;

	/// <summary>
	/// The types of type modificators
	/// </summary>
	enum class ETypeModificators : UInt8 {
		Const, Volatile, Serilizable, TemplateType
	};

	EnumCttiDefinition(ETypeModificators,
		ETypeModificators::Const,
		ETypeModificators::Volatile,
		ETypeModificators::Serilizable,
		ETypeModificators::TemplateType);

	/// <summary>
	/// Type modificator flags
	/// </summary>
	using TypeModificator = EnumFlags<ETypeModificators>;
}