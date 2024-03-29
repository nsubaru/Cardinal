export module Cardinal.CTTI.Minimal:TypeTraits.Enum;

export import :TypeTraits.Base;
export import :TypeTraits.BaseTypes;

export template<typename T>
struct __identifier(enum) : Cardinal::TypeTraits::LogicFalse {};

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 4595)

export namespace Cardinal::CTTI::TypeTraits::Details
{
	template<typename E, E V>
	constexpr auto GetEnumValue() {
		constexpr auto value = Cardinal::TypeTraits::CompileStringView(L"" __FUNCSIG__);
		constexpr auto comaPos = value.Find(L',');
		constexpr auto endClosure = value.LeftFind(L'>');
		constexpr auto enumNamePos = value.LeftFind(L':', endClosure) + 1;
		constexpr auto result = Cardinal::TypeTraits::CompileStringView(L"" __FUNCSIG__)
			.Shrink(endClosure)
			.Offset(enumNamePos)
			.AsArray<endClosure - enumNamePos>();
		return result;
	}

	static_assert(GetEnumValue<EType, EType::Tuple>() == Cardinal::TypeTraits::CreateTypeStr(L"Tuple"));

	template<typename E, E V>
	constexpr auto GetEnumIntegerValueAsConstCharArray() {
		return Cardinal::TypeTraits::ToCharStrEx<
			Cardinal::TypeTraits::EnumUnderlyingTypeV<E>,
			static_cast<Cardinal::TypeTraits::EnumUnderlyingTypeV<E>>(V)
		>();
	}

	export template<typename E, E... Values >
	struct EnumDefinitionT {
		static constexpr auto Enums =
			Containers::Tuple(
				Containers::Tuple(
					static_cast<Cardinal::TypeTraits::EnumUnderlyingTypeV<E>>(Values),
					GetEnumIntegerValueAsConstCharArray<E, Values>(),
					GetEnumValue<E, Values>())...);
	};
}

export namespace Cardinal::CTTI::TypeTraits
{
	EnumCttiDefinition(
		EType,
		EType::MetaData,
		EType::Logic,
		EType::Numeric,
		EType::Enumeration,
		EType::FloatNumeric,
		EType::Character,
		EType::Collection,
		EType::String,
		EType::FixedCollection,
		EType::AssociableCollection,
		EType::Box,
		EType::Tuple,
		EType::Pair,
		EType::Pointer,
		EType::Reference,
		EType::FixedArray,
		EType::Struct,
		EType::Object,
		EType::Dynamic,
		EType::Abstract,
		EType::Function,
		EType::Union,
		EType::Pod,
		EType::Undefine);
}

export namespace Cardinal::CTTI::TypeTraits::Details
{
	template<
		typename T,
		bool TIsConst,
		bool TIsVolatile,
		bool TIsContainAdditionalInfo = false
	> struct CTTIBaseEnumTypeDataImpl : BaseCTTITypeDataImpl<
		T,
		EType::Enumeration,
		true,
		TIsConst,
		TIsVolatile,
		false
	> {
		using UnderlyingType = typename Cardinal::TypeTraits::EnumUnderlyingTypeV<T>;
		using UnderlyingTypeMetaData = CTTITypeMetaData<UnderlyingType>;

		static constexpr bool IsContainAdditionalInfo = TIsContainAdditionalInfo;
	};

	template<
		typename T,
		bool TIsConst,
		bool TIsVolatile
	> struct CTTIExtendEnumTypeDataImpl : CTTIBaseEnumTypeDataImpl<
		T,
		TIsConst,
		TIsVolatile,
		true
	> 
	{
		static constexpr auto MetaDataType = __identifier(enum)<T>::Data;
		static constexpr auto ItemsMap = MetaDataType.Enums;
		static constexpr SizeT ItemsCount = MetaDataType.Enums.Count;
	};

	template<
		typename T,
		bool TIsConst,
		bool TIsVolatile
	> struct CTTIMinimalEnumTypeDataImpl : CTTIBaseEnumTypeDataImpl< 
		T,
		TIsConst,
		TIsVolatile,
		false
	> {
		static constexpr auto ItemsMap = Containers::Tuple<>();
		static constexpr SizeT ItemsCount = ItemsMap.Count;
	};

	template<
		typename T,
		bool TIsConst,
		bool TIsVolatile
	>
	using CttiEnumData = typename Cardinal::TypeTraits::ConditionV<
		__identifier(enum)<T>::State,
		CTTIExtendEnumTypeDataImpl<T, TIsConst, TIsVolatile>,
		CTTIMinimalEnumTypeDataImpl<T, TIsConst, TIsVolatile>
	>;

	export template<typename T> requires Concepts::IsEnumertationCttiType<T>
	struct CTTITypeDataSpecialization<T>
		: CttiEnumData<
		Cardinal::TypeTraits::IdentitV<Cardinal::TypeTraits::RemoveCv<T>>,
		Cardinal::TypeTraits::IsConstV<T>,
		Cardinal::TypeTraits::IsVolatileV<T>
		> {
	};

	static_assert(CTTI::TypeTraits::CTTITypeMetaData<CTTI::EType>::ItemsMap.GetByIdx<0>().GetByIdx<2>() == Cardinal::TypeTraits::CreateTypeStr(L"MetaData"));

	static_assert([] () constexpr {
		bool result = false;

		CTTI::TypeTraits::CTTITypeMetaData<CTTI::EType>::ItemsMap.ForEach([&] <typename T> (const T & item) constexpr {
			if (item.GetByIdx<2>() == Cardinal::TypeTraits::CreateTypeStr(L"Pod"))
			{
				result = true;
			}
		});

		return result;
	}());

	static_assert([] () constexpr {
		bool result = false;

		CTTI::TypeTraits::CTTITypeMetaData<CTTI::EType>::ItemsMap.ForEach([&] <typename T> (const T & item) constexpr {
			if ((CTTI::EType) item.GetByIdx<0>() ==  CTTI::EType::Pod)
			{
				result = true;
			}
		});

		return result;
	}());

	static_assert([] () constexpr {
		bool result = false;

		CTTI::TypeTraits::CTTITypeMetaData<CTTI::EType>::ItemsMap.ForEach([&] <typename T> (const T& item) constexpr {
			if ((CTTI::EType) item.GetByIdx<0>() == CTTI::EType::Pod)
			{
				result = item.GetByIdx<2>() == Cardinal::TypeTraits::CreateTypeStr(L"Pod");
			}
		});

		return result;
	}());
}

#pragma warning(pop)