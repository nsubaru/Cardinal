export module Cardinal.Serialization:Struct;

export import :Composits;

export namespace Cardinal::Serialization
{
	template<typename T>
	constexpr bool IsSerializableStruct = CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass == CTTI::EType::Struct
		&& CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable;

	template<typename TStruct> requires IsSerializableStruct<TStruct>
	struct SerializationHandler<TStruct> {
		static constexpr auto StructFieldsMetaData = CTTI::TypeTraits::CTTITypeMetaData<TStruct>::Fields;

		template<SizeT I = 0>
		constexpr static auto TypeHintFieldsImp() {
			if constexpr (I < StructFieldsMetaData.Count)
			{
				constexpr auto FieldMetaData = StructFieldsMetaData.template GetByIdx<I>();
				using FieldMetaDataType = decltype(FieldMetaData);
				using Type = typename FieldMetaDataType::Type;

				if constexpr (I + 1 < StructFieldsMetaData.Count)
					return
					TypeTraits::CreateTypeStr(L"[") +
					FieldMetaData.FieldName +
					TypeTraits::CreateTypeStr(L"-") +
					CTTI::TypeName<Type>() +
					TypeTraits::CreateTypeStr(L"]")
					+
					TypeHintFieldsImp<I + 1>();
				else
					return
					TypeTraits::CreateTypeStr(L"[") +
					FieldMetaData.FieldName +
					TypeTraits::CreateTypeStr(L"-") +
					CTTI::TypeName<Type>() +
					TypeTraits::CreateTypeStr(L"]");
			}
			else
			{
				return TypeTraits::CreateTypeStr(L"");
			}
		}

		template<SizeT I = 0>
		constexpr static auto TypeMetaDataFieldsImp() {
			if constexpr (I < StructFieldsMetaData.Count)
			{
				constexpr auto FieldMetaData = StructFieldsMetaData.template GetByIdx<I>();
				using FieldMetaDataType = decltype(FieldMetaData);
				using Type = typename FieldMetaDataType::Type;

				if constexpr (I + 1 < StructFieldsMetaData.Count)
					return SerializationHandler<Type>::TypeMetaData()
					+
					TypeMetaDataFieldsImp<I + 1>();
				else
					return SerializationHandler<Type>::TypeMetaData();
			}
			else
			{
				return TypeTraits::CreateTypeStr(L"");
			}
		}

		static void SerializeImpl(const TStruct& data, Byte* rawStream, SizeT& offset) {
			CTTI::TypeTraits::CTTITypeMetaData<TStruct>::Fields.ForEachType([&]<typename TField, SizeT TIdx>() mutable InlineLambda {
				using TFieldType = typename TField::Type;
				SerializationHandler<TFieldType>::SerializeImpl(
					CTTI::TypeTraits::CTTITypeMetaData<TStruct>::Selector<TIdx>(data),
					rawStream,
					offset);
			});
		}

		static void DeserializeImpl(TStruct& data, const Byte* rawStream, SizeT& offset) {
			CTTI::TypeTraits::CTTITypeMetaData<TStruct>::Fields.ForEachType([&]<typename TField, SizeT TIdx>() mutable InlineLambda {
				using TFieldType = typename TField::Type;
				SerializationHandler<TFieldType>::DeserializeImpl(
					CTTI::TypeTraits::CTTITypeMetaData<TStruct>::Selector<TIdx>(data),
					rawStream,
					offset);
			});
		}

		static constexpr Boolean IsMemCopy() {
			return false;
		}

		static constexpr SizeT DataSize(const TStruct& data) {
			SizeT sz = 0;
			CTTI::TypeTraits::CTTITypeMetaData<TStruct>::Fields.ForEachType([&]<typename TField, SizeT TIdx>() mutable InlineLambda {
				using TFieldType = typename TField::Type;
				sz += SerializationHandler<TFieldType>::DataSize(CTTI::TypeTraits::CTTITypeMetaData<TStruct>::Selector<TIdx>(data));
			});
			return sz;
		}

		static constexpr auto TypeHint() {
			return
				TypeHintFieldsImp();
		}

		static constexpr auto ConstexprSize() {
			return
				TypeTraits::CreateTypeStr(L"0");
		}

		static constexpr auto TypeMetaData() {
			return
				TypeTraits::CreateTypeStr(L"{") +
				CTTI::TypeName<TStruct>() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeTraits::ToCharStr<static_cast<SizeT>(CTTI::TypeTraits::CTTITypeMetaData<TStruct>::TypeClass)>() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeTraits::ToCharStr<sizeof(TStruct)>() +
				TypeTraits::CreateTypeStr(L"|") +
				ConstexprSize() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeHint() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeMetaDataFieldsImp() +
				TypeTraits::CreateTypeStr(L"}");
		}

		template<typename THeap, SizeT I = 0>
		static Containers::BaseList<THeap, BaseTypeMetaDataRecord<THeap>> GetFieldsTypeMetaDataRecord() {
			if constexpr (I < StructFieldsMetaData.Count)
			{
				auto FieldMetaData = StructFieldsMetaData.template GetByIdx<I>();
				using Type = decltype(FieldMetaData)::template Type;

				if constexpr (I + 1 < StructFieldsMetaData.Count)
					return
					Containers::BaseList<THeap, BaseTypeMetaDataRecord<THeap>>{
					SerializationHandler<Type>::template GetTypeMetaDataRecord<THeap>()
				}
				+=
					GetFieldsTypeMetaDataRecord<THeap, I + 1>();
				else
					return
					Containers::BaseList<THeap, BaseTypeMetaDataRecord<THeap>>{
					SerializationHandler<Type>::template GetTypeMetaDataRecord<THeap>()
				};
			}
			else
			{
				return Containers::BaseList<THeap, BaseTypeMetaDataRecord<THeap>>{};
			}
		}

		template<typename THeap>
		static BaseTypeMetaDataRecord<THeap> GetTypeMetaDataRecord() {
			return BaseTypeMetaDataRecord<THeap>(
				CTTI::TypeName<TStruct>(),
				CTTI::TypeTraits::CTTITypeMetaData<TStruct>::TypeClass,
				sizeof(TStruct),
				0,
				TypeHint(),
				GetFieldsTypeMetaDataRecord<THeap>()
				);
		}
	};
}