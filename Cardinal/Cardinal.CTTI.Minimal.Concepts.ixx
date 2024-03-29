export module Cardinal.CTTI.Minimal:Concepts;

export import :Concepts.Base;

export namespace Cardinal::CTTI::Concepts
{
	template<typename T, CTTI::EType TType>
	concept IsCttiType = CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass == TType;

	template<typename T>
	concept IsCttiLogic = IsCttiType<T, CTTI::EType::Logic>;

	template<typename T>
	concept IsCttiNumeric = IsCttiType<T, CTTI::EType::Numeric>;

	template<typename T>
	concept IsCttiSerializable = CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable;

	template<typename T>
	concept IsCttiNotSerializable = !CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable;

	template<typename T>
	concept IsCttiFloatNumeric = IsCttiType<T, CTTI::EType::FloatNumeric>;

	template<typename T>
	concept IsCttiString = IsCttiType<T, CTTI::EType::String>;

	template<typename T>
	concept IsCttiStruct = IsCttiType<T, CTTI::EType::Struct>;
}