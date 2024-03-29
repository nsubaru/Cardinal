export module Cardinal.Serialization:MetaData;

export import :Core;

export namespace Cardinal::Serialization
{
    template<typename T>
    constexpr bool IsSerializableMetData =
        CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass == CTTI::EType::MetaData;

    template<typename TMetaDataHeap> requires IsSerializableMetData<BaseTypeMetaDataRecord<TMetaDataHeap>>
    struct SerializationHandler<BaseTypeMetaDataRecord<TMetaDataHeap>> {
        using T = typename BaseTypeMetaDataRecord<TMetaDataHeap>;

        static void SerializeImpl(const T& data, Byte* rawStream, SizeT& offset) {
            SerializationHandler<typename CTTI::TypeTraits::CTTITypeMetaData<T>::StringType>::SerializeImpl
            (data.ToString(), rawStream, offset);
        }

        static constexpr SizeT DataSize(const T& data) {
            return SerializationHandler<typename CTTI::TypeTraits::CTTITypeMetaData<T>::StringType>::DataSize
            (data.ToString());
        }

        static constexpr Boolean IsMemCopy() {
            return false;
        }

        static constexpr auto TypeHint() {
            return TypeTraits::CreateTypeStr(L"");
        }

        static constexpr auto ConstexprSize() {
            return
                TypeTraits::ToCharStr<sizeof(T)>();
        }

        static void DeserializeImpl(T& data, const Byte* rawStream, SizeT& offset) {
            typename CTTI::TypeTraits::CTTITypeMetaData<T>::StringType strValue = {};

            SerializationHandler<typename CTTI::TypeTraits::CTTITypeMetaData<T>::StringType>::DeserializeImpl
            (strValue, rawStream, offset);

            data = BaseTypeMetaDataDecoder<TMetaDataHeap>().DecodeTypeMetaData(strValue);
        }

        static constexpr auto TypeMetaData() {
            return
                TypeTraits::CreateTypeStr(L"{") +
                CTTI::TypeName<T>() +
                TypeTraits::CreateTypeStr(L"|") +
                TypeTraits::ToCharStr<static_cast<SizeT>(CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass)>() +
                TypeTraits::CreateTypeStr(L"|") +
                TypeTraits::ToCharStr<sizeof(T)>() +
                TypeTraits::CreateTypeStr(L"|") +
                ConstexprSize() +
                TypeTraits::CreateTypeStr(L"|") +
                TypeHint() +
                TypeTraits::CreateTypeStr(L"|") +
                TypeTraits::CreateTypeStr(L"") +
                TypeTraits::CreateTypeStr(L"}");
        }

        template<typename THeap>
        static BaseTypeMetaDataRecord<THeap> GetTypeMetaDataRecord() {
            return BaseTypeMetaDataRecord<THeap>(
                CTTI::TypeName<T>(),
                CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass,
                sizeof(T),
                sizeof(T),
                TypeHint()
                );
        }
    };
}