export module Cardinal.CTTI.Minimal:TypeTraits.BaseTypes;

export import :TypeTraits.Base;

export namespace Cardinal::CTTI::TypeTraits
{
	using CTTITypeRttiIdGetter = const TypeInfo* (*)();
}

export namespace Cardinal::CTTI::TypeTraits
{
	template<typename T>
	extern __declspec(noinline) const TypeInfo* TypeIdGetter() { return &typeid(T); };

	template<typename T, EType TTypeClass, bool TIsSerializable, bool TIsConst, bool TIsVolatile, bool TIsConvertibleToTuple = false>
	struct BaseCTTITypeDataImpl {
		using Type = T;
		static constexpr EType TypeClass = TTypeClass;
		static constexpr SizeT Size = sizeof(T);
		static constexpr Boolean IsSerializable = TIsSerializable;
		static constexpr Boolean IsConst = TIsConst;
		static constexpr Boolean IsVolatile = TIsVolatile;
		static constexpr auto TypeName = CTTI::TypeName<T>();
		static constexpr auto CXXTypeName = CTTI::CXXTypeName<T>();
		static constexpr auto TemplateTypeName = [] () constexpr {
			if constexpr (Cardinal::TypeTraits::IsTemplateTypeV<T>)
				return CTTI::TemplateTypeName<T>();
			else
				return Cardinal::TypeTraits::ConstexprArray<Char, 0>();
		} ();

		static constexpr Boolean IsConvertibleToTuple = TIsConvertibleToTuple;

		static constexpr CTTITypeRttiIdGetter TypeIdGetter = &TypeIdGetter<T>;

		static constexpr CTTITypeRttiIdGetter DecayTypeIdGetter =
			BaseCTTITypeDataImpl<T, TTypeClass, TIsSerializable, false, false, TIsConvertibleToTuple>::TypeIdGetter;

		static constexpr bool IsDecayType = TypeIdGetter == DecayTypeIdGetter;

		static constexpr Boolean IsContaineSubTypes = IsComplexTypes(TypeClass);

		static constexpr Boolean IsTemplateType = Cardinal::TypeTraits::IsTemplateTypeV<T>;
	};

	template<EType TTypeClass, bool TIsSerializable, bool TIsConst, bool TIsVolatile, bool TIsConvertibleToTuple>
	struct BaseCTTITypeDataImpl<void, TTypeClass, TIsSerializable, TIsConst, TIsVolatile, TIsConvertibleToTuple> {
		using Type = void;
		static constexpr EType TypeClass = EType::Undefine;
		static constexpr SizeT Size = 0;
		static constexpr Boolean IsSerializable = false;
		static constexpr Boolean IsConst = TIsConst;
		static constexpr Boolean IsVolatile = TIsVolatile;
		static constexpr auto TypeName = CTTI::TypeName<void>();
		static constexpr auto CXXTypeName = CTTI::CXXTypeName<void>();
		static constexpr auto TemplateTypeName = Cardinal::TypeTraits::ConstexprArray<Char, 0>();
		static constexpr Boolean IsConvertibleToTuple = false;

		static constexpr CTTITypeRttiIdGetter TypeIdGetter = &TypeIdGetter<void>;

		static constexpr CTTITypeRttiIdGetter DecayTypeIdGetter =
			BaseCTTITypeDataImpl<void, TTypeClass, TIsSerializable, false, false, false>::TypeIdGetter;

		static constexpr bool IsDecayType = TypeIdGetter == DecayTypeIdGetter;

		static constexpr Boolean IsContaineSubTypes = false;

		static constexpr Boolean IsTemplateType = false;
	};
#pragma endregion

#pragma region Default Implementation
	template<typename T, bool TIsConst, bool TIsVolatile>
	struct CTTITypeDataImpl : BaseCTTITypeDataImpl<T, EType::Undefine, false, TIsConst, TIsVolatile> {};

	template<typename T>
	struct CTTITypeDataSpecialization
		: CTTITypeDataImpl<typename Cardinal::TypeTraits::RemoveCv<T>, Cardinal::TypeTraits::IsConstV<T>, Cardinal::TypeTraits::IsVolatileV<T>> {
	};

	template<typename T>
	struct CTTITypeData : CTTITypeDataSpecialization<T> {
	};
#pragma endregion

#pragma region Pointer, Reference and CArray implementations

	template<typename T, bool TIsConst, bool TIsVolatile>
	struct CTTITypeDataImpl<T*, TIsConst, TIsVolatile> : BaseCTTITypeDataImpl<T*, EType::Pointer, false, TIsConst, TIsVolatile> {
		using Target = T;
		using TargetTypeMetaData = CTTITypeData<T>;
	};

	template<typename T, bool TIsConst, bool TIsVolatile>
	struct CTTITypeDataImpl<T&, TIsConst, TIsVolatile> : BaseCTTITypeDataImpl<T&, EType::Reference, false, TIsConst, TIsVolatile> {
		static constexpr Boolean IsLvalue = true;

		using Target = T;
		using TargetTypeMetaData = CTTITypeData<T>;
	};

	template<typename T, bool TIsConst, bool TIsVolatile>
	struct CTTITypeDataImpl<T&&, TIsConst, TIsVolatile> : BaseCTTITypeDataImpl<T&, EType::Reference, false, TIsConst, TIsVolatile> {
		static constexpr Boolean IsLvalue = false;

		using Target = T;
		using TargetTypeMetaData = CTTITypeData<T>;
	};

	template<typename T, SizeT TSize, bool TIsConst, bool TIsVolatile>
	struct CTTITypeDataImpl<T[TSize], TIsConst, TIsVolatile> : BaseCTTITypeDataImpl<T[TSize], EType::FixedArray, CTTITypeData<T>::IsSerializable, TIsConst, TIsVolatile> {
		static constexpr SizeT Count = TSize;

		using Target = T;
		using TargetTypeMetaData = CTTITypeData<T>;
	};
#pragma endregion
}

export namespace Cardinal::CTTI::TypeTraits
{
#pragma region Public using
	template<typename T>
	using CTTITypeMetaData = CTTITypeData<T>;

	template<typename T>
	constexpr CTTITypeRttiIdGetter CTTITypeId = CTTITypeData<T>::TypeIdGetter;

	template<typename T>
	constexpr CTTITypeRttiIdGetter CTTIDecayTypeId = CTTITypeData<T>::DecayTypeIdGetter;

	template<typename T> requires (CTTITypeMetaData<T>::IsConvertibleToTuple)
		auto ConvertTypeToTuple(const T& item) {
		return CTTITypeMetaData<T>::ToTuple(item);
	}

	template<typename T> requires (CTTITypeMetaData<T>::IsConvertibleToTuple)
		auto ConvertTypeToTuple(T&& item) -> typename CTTITypeMetaData<T>::FiledsTupleType {
		return CTTITypeMetaData<T>::ToTuple(item);
	}
#pragma endregion
}