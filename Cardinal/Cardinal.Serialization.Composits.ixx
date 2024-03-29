export module Cardinal.Serialization:Composits;

export import :Collection;

export namespace Cardinal::Serialization
{
	template<typename T>
	constexpr bool IsSerializableTuple = CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass == CTTI::EType::Tuple
		&& CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable;

	template<template<typename ... TTypes> typename TTuple, typename ... TTypes> requires IsSerializableTuple<TTuple<TTypes...>>
	struct SerializationHandler<TTuple<TTypes...>> {
		using Tuple = TTuple<TTypes...>;

		template<SizeT Index = 0>
		constexpr static auto TypeMetaDataTuppleItem() {
			if constexpr (Index == sizeof...(TTypes))
			{
				return TypeTraits::CreateTypeStr(L"");
			}
			else
			{
				using Type = typename Tuple::template ItemType<Index>;

				return SerializationHandler<Type>::TypeMetaData()
					+
					TypeMetaDataTuppleItem<Index + 1>();
			}
		}

		static void SerializeImpl(const Tuple& data, Byte* rawStream, SizeT& offset) {
			data.ForEach([&]<typename TField>(const TField & item) InlineLambda {
				SerializationHandler<TField>::SerializeImpl(item, rawStream, offset);
			});
		}

		static constexpr Boolean IsMemCopy() {
			return false;
		}

		static constexpr SizeT DataSize(const Tuple& data) {
			SizeT sz = 0;
			data.ForEach([&]<typename TField>(const TField & item) InlineLambda {
				sz += SerializationHandler<TField>::DataSize(item);
			});
			return sz;
		}

		static constexpr auto TypeHint() {
			return
				TypeTraits::ToCharStr<sizeof...(TTypes)>();
		}

		static constexpr auto ConstexprSize() {
			return
				TypeTraits::CreateTypeStr(L"0");
		}

		static constexpr auto TypeMetaData() {
			return
				TypeTraits::CreateTypeStr(L"{") +
				CTTI::TypeName<Tuple>() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeTraits::ToCharStr<static_cast<SizeT>(CTTI::TypeTraits::CTTITypeMetaData<Tuple>::TypeClass)>() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeTraits::ToCharStr<sizeof(Tuple)>() +
				TypeTraits::CreateTypeStr(L"|") +
				ConstexprSize() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeHint() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeMetaDataTuppleItem() +
				TypeTraits::CreateTypeStr(L"}");
		}

		static void DeserializeImpl(Tuple& data, const Byte* rawStream, SizeT& offset) {
			data.ForEach([&]<typename TField>(TField & item) InlineLambda {
				SerializationHandler<TField>::DeserializeImpl(item, rawStream, offset);
			});
		}

		template<typename THeap, SizeT Index = 0>
		static Containers::BaseList<THeap, BaseTypeMetaDataRecord<THeap>> GetFieldsTypeMetaDataRecord() {
			if constexpr (Index < sizeof...(TTypes))
			{
				using Type = typename Tuple::template ItemType<Index>;

				if constexpr (Index + 1 < sizeof...(TTypes))
					return
					Containers::BaseList<THeap, BaseTypeMetaDataRecord<THeap>>{
					SerializationHandler<Type>::template GetTypeMetaDataRecord<THeap>()
				}
				+=
					GetFieldsTypeMetaDataRecord<THeap, Index + 1>();
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
				CTTI::TypeName<Tuple>(),
				CTTI::TypeTraits::CTTITypeMetaData<Tuple>::TypeClass,
				sizeof(Tuple),
				0,
				TypeHint(),
				GetFieldsTypeMetaDataRecord<THeap>()
				);
		}
	};

	template<typename T>
	constexpr bool IsSerializableObject = CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass == CTTI::EType::Object
		&& CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable;

	template<typename TObject> requires IsSerializableObject<TObject>
	struct SerializationHandler<TObject> {
		static constexpr auto StructFieldsMetaData = CTTI::TypeTraits::CTTITypeMetaData<TObject>::Fields;
		using ISerializable = CTTI::TypeTraits::ISerializable<TObject>;

		template<SizeT I = 0>
		static void SerializeFieldsImpl(Byte* fiedsPtr, Byte* rawStream, SizeT& offset) {
			if constexpr (I < StructFieldsMetaData.Count)
			{
				auto FieldMetaData = StructFieldsMetaData.template GetByIdx<I>();
				using Type = decltype(FieldMetaData)::template Type;

				if constexpr (I + 1 < StructFieldsMetaData.Count)
				{
					SerializationHandler<Type>::SerializeImpl(
						*reinterpret_cast<Type*>(fiedsPtr + FieldMetaData.FieldOffset),
						rawStream,
						offset);
					SerializiaFieldsImpl<I + 1>(fiedsPtr, rawStream, offset);
				}
				else
				{
					SerializationHandler<Type>::SerializeImpl(
						*reinterpret_cast<Type*>(fiedsPtr + FieldMetaData.FieldOffset),
						rawStream,
						offset);
				}
			}
			else
			{
				return;
			}
		}

		template<SizeT I = 0>
		static void DeserializeFieldsImp(Byte* fiedsPtr, const Byte* rawStream, SizeT& offset) {
			if constexpr (I < StructFieldsMetaData.Count)
			{
				auto FieldMetaData = StructFieldsMetaData.template GetByIdx<I>();
				using Type = decltype(FieldMetaData)::template Type;

				if constexpr (I + 1 < StructFieldsMetaData.Count)
				{
					SerializationHandler<Type>::DeserializeImpl(
						*reinterpret_cast<Type*>(fiedsPtr + FieldMetaData.FieldOffset),
						rawStream,
						offset);
					DeserializeFieldsImp<I + 1>(fiedsPtr, rawStream, offset);
				}
				else
				{
					SerializationHandler<Type>::DeserializeImpl(
						*reinterpret_cast<Type*>(fiedsPtr + FieldMetaData.FieldOffset),
						rawStream,
						offset);
				}
			}
			else
			{
				return;
			}
		}

		template<SizeT I = 0>
		static SizeT DataSizeFieldsImp(Byte* fiedsPtr) {
			if constexpr (I < StructFieldsMetaData.Count)
			{
				auto FieldMetaData = StructFieldsMetaData.template GetByIdx<I>();
				using Type = decltype(FieldMetaData)::template Type;

				if constexpr (I + 1 < StructFieldsMetaData.Count)
					return
					SerializationHandler<Type>::DataSize(
						*reinterpret_cast<Type*>(fiedsPtr + FieldMetaData.FieldOffset))
					+
					DataSizeFieldsImp<I + 1>(fiedsPtr);
				else
					return
					SerializationHandler<Type>::DataSize(
						*reinterpret_cast<Type*>(fiedsPtr + FieldMetaData.FieldOffset));
			}
			else
			{
				return 0;
			}
		}

		template<SizeT I = 0>
		constexpr static auto TypeHintFieldsImp() {
			if constexpr (I < StructFieldsMetaData.Count)
			{
				auto FieldMetaData = StructFieldsMetaData.template GetByIdx<I>();
				using Type = decltype(FieldMetaData)::template Type;

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
				auto FieldMetaData = StructFieldsMetaData.template GetByIdx<I>();
				using Type = decltype(FieldMetaData)::template Type;

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

		static void SerializeImpl(const TObject& data, Byte* rawStream, SizeT& offset) {
			const auto& iSerializeable = static_cast<const ISerializable&>(data);
			SerializeFieldsImpl(iSerializeable.GetDataPtr(), rawStream, offset);
		}

		static void DeserializeImpl(TObject& data, const Byte* rawStream, SizeT& offset) {
			if constexpr (CTTI::TypeTraits::CTTITypeMetaData<TObject>::IsSupportDeserialize)
			{
				auto& iSerializeable = static_cast<ISerializable&>(data);
				DeserializeFieldsImp(iSerializeable.GetDataPtr(), rawStream, offset);
			}
			else
			{
				Throw(NotImplementedException(SourcePosition));
			}
		}

		static constexpr Boolean IsMemCopy() {
			return false;
		}

		static constexpr SizeT DataSize(const TObject& data) {
			const auto& iSerializeable = static_cast<const ISerializable&>(data);
			return DataSizeFieldsImp(iSerializeable.GetDataPtr());
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
				CTTI::TypeName<TObject>() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeTraits::ToCharStr<static_cast<SizeT>(CTTI::TypeTraits::CTTITypeMetaData<TObject>::TypeClass)>() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeTraits::ToCharStr<sizeof(TObject)>() +
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
				CTTI::TypeName<TObject>(),
				CTTI::TypeTraits::CTTITypeMetaData<TObject>::TypeClass,
				sizeof(TObject),
				0,
				TypeHint(),
				GetFieldsTypeMetaDataRecord<THeap>()
				);
		}
	};
}