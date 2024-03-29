export module Cardinal.CTTI.Minimal:TypeTraits.CustomType;

export import :TypeTraits.BaseTypes;
export import :TypeTraits.Struct;
export import :TypeTraits.StructDefintionBuild;
export import :TypeTraits.StructRemoteDefinition;
export import :TypeTraits.Object;

export namespace Cardinal::CTTI::TypeTraits
{
	enum class ECustomTypeSchemaFormat : UInt8 {
		None,
		StructSchema,
		StructWithAccessorMethodSchema,
		ObjectSchema,
		RemoteStructSchema,
		RemoteObjectSchema,
		Undefined
	};

	template<typename T>
	struct CustomCttiTypeNoneSchemaDefinition {
		static constexpr auto CustomTypeSchemaFormat = ECustomTypeSchemaFormat::None;
	};

	template<typename T>
	struct CustomCttiTypeStructSchemaDefinition {
		static constexpr auto CustomTypeSchemaFormat = ECustomTypeSchemaFormat::StructSchema;

		static constexpr auto FieldMetaData = T::FieldMetaDataGetter();
		static constexpr auto Fields = FieldMetaData.DataFields;

		static constexpr decltype(auto) ToTuple(const T& data) {
			return FieldMetaData.ToTuple(data);
		}

		static constexpr bool IsSupportDeserilize = true;

		using TFields = decltype(Fields);

		template<SizeT Idx>
		ForceInline static decltype(auto) Selector(const T& object) {
			return TFields::template ItemType<Idx>::Selector(object);
		}

		template<SizeT Idx>
		ForceInline static decltype(auto) Selector(T& object) {
			return TFields::template ItemType<Idx>::Selector(object);
		}

		static constexpr bool HaveDataAccessorMethod = false;
	};

	template<typename T>
	struct CustomStructWithAccessorMethodSchemaDefinition {
		static constexpr auto CustomTypeSchemaFormat = ECustomTypeSchemaFormat::StructWithAccessorMethodSchema;

		static constexpr bool IsConvertibleToTuple = true;
		static constexpr auto Fields = typename T::FieldMetaData.DataFields;
		static constexpr bool IsSupportDeserilize = [] () constexpr {
			if constexpr (IsConfigureDeserilizationSupport<T>)
				return T::IsSupportDeserilize();
			else
				return true;
		}();

		using TFields = decltype(Fields);
		using FiledsTupleType = typename decltype(T::FieldMetaData)::FiledsTupleType;

		static constexpr FiledsTupleType ToTuple(const T& object) {
			using TFieldMetaData = decltype(T::FieldMetaData);
			using Type = typename TFieldMetaData::Type;
			return T::FieldMetaData.ToTuple(*(Type*) (ISerializable<T>::GetDataPtrStub(object)));
		}

		template<SizeT Idx>
		ForceInline static decltype(auto) Selector(const T& object) {
			using TFieldMetaData = decltype(T::FieldMetaData);
			using Type = typename TFieldMetaData::Type;
			return TFields::template ItemType<Idx>::Selector(*(Type*) (ISerializable<T>::GetDataPtrStub(object)));
		}

		template<SizeT Idx>
		ForceInline static decltype(auto) Selector(T& object) {
			using TFieldMetaData = decltype(T::FieldMetaData);
			using Type = typename TFieldMetaData::Type;
			return TFields::template ItemType<Idx>::Selector(*(Type*) (ISerializable<T>::GetDataPtrStub(object)));
		}

		static constexpr bool HaveDataAccessorMethod = true;
	};

	template<typename T>
	struct CustomCttiTypeObjectSchemaDefinition {
		static constexpr auto CustomTypeSchemaFormat = ECustomTypeSchemaFormat::ObjectSchema;

		static constexpr bool IsTypeHaveClassCttiRecord = true;
		using ClassRecord = decltype(T::__identifier(class) ());
	};

	template<typename T>
	struct CustomCttiTypeRemoteObjectSchemaDefinition {
		static constexpr auto CustomTypeSchemaFormat = ECustomTypeSchemaFormat::RemoteStructSchema;

		static constexpr auto FieldMetaData = StructCttiShema<T>::FieldMetaDataGetter();
		static constexpr auto Fields = FieldMetaData.DataFields;

		static constexpr decltype(auto) ToTuple(const T& data) {
			return FieldMetaData.ToTuple(data);
		}

		static constexpr bool IsSupportDeserilize = true;

		using TFields = decltype(Fields);

		template<SizeT Idx>
		ForceInline static decltype(auto) Selector(const T& object) {
			return TFields::template ItemType<Idx>::Selector(object);
		}

		template<SizeT Idx>
		ForceInline static decltype(auto) Selector(T& object) {
			return TFields::template ItemType<Idx>::Selector(object);
		}

		static constexpr bool HaveDataAccessorMethod = false;
	};

	template<typename T>
	struct CustomCttiTypeRemoteStructSchemaDefinition {
		static constexpr auto CustomTypeSchemaFormat = ECustomTypeSchemaFormat::RemoteObjectSchema;

		static constexpr bool IsTypeHaveClassCttiRecord = true;
		using ClassRecord = decltype(ObjectCttiSchema<T>::__identifier(class) ());
	};

	template<typename T>
	concept CustomCttiTypeStructSchema = Concepts::IsStructWithCttiDefinition<T>;

	template<typename T>
	concept CustomCttiTypeStructWithAccessorMethodSchema = Concepts::IsStructWithAccessorMethodCttiType<T>;

	template<typename T>
	concept CustomCttiTypeObjectSchema = Concepts::IsTypeHaveClassCttiRecord<T>;

	template<typename T>
	concept CustomCttiTypeRemoteObjectSchema = Concepts::IsTypeHaveRemoteObjectCttiShema<T>;

	template<typename T>
	concept CustomCttiTypeRemoteStructSchema = Concepts::IsTypeHaveRemoteStructCttiShema<T>;

	template<typename T>
	constexpr Containers::Tuple<ECustomTypeSchemaFormat, ECustomTypeSchemaFormat, ECustomTypeSchemaFormat, ECustomTypeSchemaFormat, ECustomTypeSchemaFormat> CustomTypeSchemaFormats = Containers::Tuple{
		CustomCttiTypeStructSchema<T> ? ECustomTypeSchemaFormat::StructSchema : ECustomTypeSchemaFormat::None,
		CustomCttiTypeStructWithAccessorMethodSchema<T> ? ECustomTypeSchemaFormat::StructWithAccessorMethodSchema : ECustomTypeSchemaFormat::None,
		CustomCttiTypeObjectSchema<T> ? ECustomTypeSchemaFormat::ObjectSchema : ECustomTypeSchemaFormat::None,
		CustomCttiTypeRemoteObjectSchema<T> ? ECustomTypeSchemaFormat::RemoteStructSchema : ECustomTypeSchemaFormat::None,
		CustomCttiTypeRemoteStructSchema<T> ? ECustomTypeSchemaFormat::RemoteObjectSchema : ECustomTypeSchemaFormat::None
	};

	template<typename T>
	using AvailableFormats = Containers::Tuple<
		typename Cardinal::TypeTraits::ConditionV<CustomCttiTypeStructSchema<T>, CustomCttiTypeStructSchemaDefinition<T>, CustomCttiTypeNoneSchemaDefinition<T>>,
		typename Cardinal::TypeTraits::ConditionV<CustomCttiTypeStructWithAccessorMethodSchema<T>, CustomStructWithAccessorMethodSchemaDefinition<T>, CustomCttiTypeNoneSchemaDefinition<T>>,
		typename Cardinal::TypeTraits::ConditionV<CustomCttiTypeObjectSchema<T>, CustomCttiTypeObjectSchemaDefinition<T>, CustomCttiTypeNoneSchemaDefinition<T>>,
		typename Cardinal::TypeTraits::ConditionV<CustomCttiTypeRemoteObjectSchema<T>, CustomCttiTypeRemoteObjectSchemaDefinition<T>, CustomCttiTypeNoneSchemaDefinition<T>>,
		typename Cardinal::TypeTraits::ConditionV<CustomCttiTypeRemoteStructSchema<T>, CustomCttiTypeRemoteStructSchemaDefinition<T>, CustomCttiTypeNoneSchemaDefinition<T>>
	>;

	template<typename T>
	constexpr SizeT CustomTypeSchemaFormatsCount = [] () constexpr {
		SizeT count = 0;

		count += CustomTypeSchemaFormats<T>.GetByIdx<0>() == ECustomTypeSchemaFormat::None ? 0 : 1;
		count += CustomTypeSchemaFormats<T>.GetByIdx<1>() == ECustomTypeSchemaFormat::None ? 0 : 1;
		count += CustomTypeSchemaFormats<T>.GetByIdx<2>() == ECustomTypeSchemaFormat::None ? 0 : 1;
		count += CustomTypeSchemaFormats<T>.GetByIdx<3>() == ECustomTypeSchemaFormat::None ? 0 : 1;
		count += CustomTypeSchemaFormats<T>.GetByIdx<4>() == ECustomTypeSchemaFormat::None ? 0 : 1;

		return count;
	} ();

	template<typename T>
	struct QueryCustomTypeFormat {
		using AvailableFormats = AvailableFormats<T>;;

		static constexpr auto FormatSearchLambda = []<typename U>() constexpr -> bool { return !IsSameV<U, CustomCttiTypeNoneSchemaDefinition<T>>; };

		static constexpr SizeT FormatSeachIdx = AvailableFormats::template FirstType<decltype(FormatSearchLambda)>();

		using FirstAvailableFormat = decltype(AvailableFormats().GetByIdx<FormatSeachIdx>());

		using CustomCttiTypeFormat = typename Cardinal::TypeTraits::ConditionV<
			(CustomTypeSchemaFormatsCount<T> == 0),
			CustomCttiTypeNoneSchemaDefinition<T>,
			FirstAvailableFormat
		>;
	};

	template<typename T, bool TIsSerializable, bool TIsConst, bool TIsVolatile>
	struct CustomCTTITypeData : BaseCTTITypeDataImpl<T, EType::Custom, TIsSerializable, TIsConst, TIsVolatile>, QueryCustomTypeFormat<T>::CustomCttiTypeFormat {
		static_assert(CustomTypeSchemaFormatsCount<T> < 2, "More than one available formats please leave only one of them");
	};

	template<typename T>
	inline constexpr bool IsSerializabelCustomType() {
		if constexpr (IsBasedOnV<ICustomStructWithOverridedSerializationBase<true>, T>)
		{
			return true;
		}
		else if constexpr (IsBasedOnV<ICustomStructWithOverridedSerializationBase<false>, T>)
		{
			return false;
		}
		else
		{
			if constexpr (CustomCttiTypeFormat<T>::CustomTypeSchemaFormat == ECustomTypeSchemaFormat::None)
			{
				return false;
			}
			else if constexpr (CustomCttiTypeFormat<T>::CustomTypeSchemaFormat == ECustomTypeSchemaFormat::StructSchema)
			{
				return (!IsUnion<T>) ? T::FieldMetaData.IsSerializable : false;
			}
			else if constexpr (CustomCttiTypeFormat<T>::CustomTypeSchemaFormat == ECustomTypeSchemaFormat::StructWithAccessorMethodSchema)
			{
				return (!IsUnion<T>) ? T::FieldMetaData.IsSerializable : false;
			}
			else if constexpr (CustomCttiTypeFormat<T>::CustomTypeSchemaFormat == ECustomTypeSchemaFormat::RemoteStructSchema)
			{
				return (!IsUnion<T>) ? StructCttiShema<T>::FieldMetaDataGetter().IsSerializable : false;
			}
			else if constexpr (CustomCttiTypeFormat<T>::CustomTypeSchemaFormat == ECustomTypeSchemaFormat::ObjectSchema)
			{
				return false;
			}
			else if constexpr (CustomCttiTypeFormat<T>::CustomTypeSchemaFormat == ECustomTypeSchemaFormat::RemoteObjectSchema)
			{
				return false;
			}
			else
			{
				return false;
			}
		}
	}

	template<typename T> requires Concepts::IsCustomCttiType<T>
	struct CTTITypeDataSpecialization<T> :
		CustomCTTITypeData<
		T,
		IsSerializabelCustomType<T>(),
		Cardinal::TypeTraits::IsConstV<T>,
		Cardinal::TypeTraits::IsVolatileV<T>
		> {
	};
}