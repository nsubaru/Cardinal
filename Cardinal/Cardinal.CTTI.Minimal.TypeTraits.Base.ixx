export module Cardinal.CTTI.Minimal:TypeTraits.Base;

export import :Base;
export import :EType;
export import :TypeName;

export namespace Cardinal::CTTI::TypeTraits
{
	template<typename T>
	struct IsDynamicT : Cardinal::TypeTraits::LogicFalse {};

	template<typename T>
	struct FunctionCttiType : Cardinal::TypeTraits::LogicFalse {};

	template<typename TRet, typename TArgs>
	struct FunctionCttiType<TRet(TArgs...)> : Cardinal::TypeTraits::LogicTrue {
		using SignatureTypes = Containers::Tuple<TRet, TArgs...>;
	};
}

export namespace Cardinal::CTTI::TypeTraits
{
	VirtualStruct ISerilizableGenericBase{};

	VirtualStruct ICustomStructBase{};

	template<bool TIsSerializable>
	VirtualStruct ICustomStructWithOverridedSerializationBase : ICustomStructBase{
		static constexpr bool IsSerializable = TIsSerializable;
	};

	template<typename T>
	struct StructCttiShema : Cardinal::TypeTraits::LogicFalse {};

	template<typename T>
	struct ObjectCttiSchema : Cardinal::TypeTraits::LogicFalse {};
}

export namespace Cardinal::CTTI::Concepts
{
	template<typename T>
	concept IsLogicCttiType = Cardinal::Concepts::IsSame<T, Boolean> || Cardinal::Concepts::IsSame<T, bool>;

	template<typename T>
	concept IsNumericCttiType =
		Cardinal::TypeTraits::IsIntegerNumber<T> ||
		Cardinal::TypeTraits::IsSameV<RefCountT, T> ||
		Cardinal::TypeTraits::IsSameV<Memory::DataSizeT, T> ||
		Cardinal::TypeTraits::IsEnumFlagsV<T>;

	template<typename T>
	concept IsNumericCttiTypeWithInnerType = Cardinal::Concepts::IsSame<Cardinal::TypeTraits::RemoveCvRefV<T>, T> && requires{ typename T::TInner; };

	template<typename T>
	concept IsFloatPointNumberCttiType = Cardinal::TypeTraits::IsFloatPointNumber<T>;

	template<typename T>
	concept IsEnumertationCttiType = Cardinal::TypeTraits::IsEnumV<T>;

	template<typename T>
	concept IsCharacterCttiType = Cardinal::TypeTraits::IsCharacter<T>;

	template<typename T>
	concept IsCollectionCttiType = Cardinal::TypeTraits::IsBasedOnV<Containers::LINQ::ICollectionGenericBase, T>;

	template<typename T>
	concept IsStringCttiType = Cardinal::TypeTraits::IsBasedOnV<Containers::LINQ::IStringCollectionGenericBase, T>;

	template<typename T>
	concept IsFixedCollectionCttiType = Cardinal::TypeTraits::IsBasedOnV<Containers::LINQ::IFixedCollectionGenericBase, T>;

	template<typename T>
	concept IsAssociableCollectionCttiType = Cardinal::TypeTraits::IsBasedOnV<Containers::LINQ::IAssociableCollectionGenericBase, T>;

	template<typename T>
	concept IsBoxCttiType = Cardinal::TypeTraits::IsBasedOnV<Containers::LINQ::ISmartPtrGenericBase, T> ||
		Cardinal::TypeTraits::IsBasedOnV<Containers::LINQ::ILazyGenericBase, T>;

	template<typename T>
	concept IsTupleCttiType = Cardinal::TypeTraits::IsBasedOnV<Containers::LINQ::ITupleGenericBase, T>;

	template<typename T>
	concept IsPairCttiType = Cardinal::TypeTraits::Collection::IsPair<T>;

	template<typename T>
	concept IsPointerCttiType = Cardinal::TypeTraits::IsPointerT<T>::State;

	template<typename T>
	concept IsReferenceCttiType = Cardinal::TypeTraits::IsReferenceV<T>;

	template<typename T>
	concept IsFixedArrayCttiType = Cardinal::TypeTraits::IsCArray<T>::State;

	template<typename T>
	concept IsTypeHaveRemoteStructCttiShema = Cardinal::Concepts::IsSame<Cardinal::TypeTraits::RemoveCvRefV<T>, T>
		&& requires{ TypeTraits::StructCttiShema<T>::FieldMetaDataGetter(); };

	template<typename T>
	concept IsStructWithCttiDefinition = Cardinal::Concepts::IsSame<Cardinal::TypeTraits::RemoveCvRefV<T>, T>
		&& requires { T::FieldMetaDataGetter(); };

	template<typename T>
	concept IsStructCttiType =
		(IsStructWithCttiDefinition<T> ||
			TypeTraits::StructCttiShema<T>::State ||
			Cardinal::TypeTraits::IsBasedOnV<TypeTraits::ISerilizableGenericBase, T>
			) && (!Cardinal::TypeTraits::IsUnion<T>);

	template<typename T>
	concept IsUnionCttiType =
		(IsStructWithCttiDefinition<T> ||
			TypeTraits::StructCttiShema<T>::State ||
			Cardinal::TypeTraits::IsBasedOnV<TypeTraits::ISerilizableGenericBase, T>) &&
		(Cardinal::TypeTraits::IsUnion<T>);

	template<typename T>
	concept IsStructWithAccessorMethodCttiType = Cardinal::TypeTraits::IsBasedOnV<TypeTraits::ISerilizableGenericBase, T>;

	template<typename T>
	concept IsDynamicCttiType = TypeTraits::IsDynamicT<T>::State;

	template<typename T>
	concept IsPodCttiType = Cardinal::TypeTraits::IsClassOrStructV<T> &&
		Cardinal::TypeTraits::IsPodV<T> &&
		!(IsStructCttiType<T>);

	template<typename T>
	concept IsTypeHaveClassCttiRecord = Cardinal::Concepts::IsSame<Cardinal::TypeTraits::RemoveCvRefV<T>, T>
		&& requires{
			{ T::__identifier(class) };
	};

	template<typename T>
	concept IsCttiTypeObjectWithCttiRecord = IsTypeHaveClassCttiRecord<T> && !Cardinal::TypeTraits::IsAbstract<T>;

	template<typename T>
	concept IsCttiTypeAbstractWithCttiRecord = IsTypeHaveClassCttiRecord<T> && Cardinal::TypeTraits::IsAbstract<T>;

	template<typename T>
	concept IsTypeHaveRemoteObjectCttiShema = Cardinal::Concepts::IsSame<Cardinal::TypeTraits::RemoveCvRefV<T>, T>
		&& requires{ TypeTraits::ObjectCttiSchema<T>::__identifier(class); };

	template<typename T>
	concept IsObjectCttiType = IsCttiTypeObjectWithCttiRecord<T> || (IsTypeHaveRemoteObjectCttiShema<T> && !Cardinal::TypeTraits::IsAbstract<T>);

	template<typename T>
	concept IsAbstractCttiType = IsCttiTypeAbstractWithCttiRecord<T> || (IsTypeHaveRemoteObjectCttiShema<T> && Cardinal::TypeTraits::IsAbstract<T>);

	template<typename T>
	concept IsFunctionCttiType = CTTI::TypeTraits::FunctionCttiType<T>::State;

	template<typename T>
	concept IsCustomCttiType = Cardinal::TypeTraits::IsBasedOnV<TypeTraits::ICustomStructBase, T>;
}