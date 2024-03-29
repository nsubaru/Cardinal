export module Cardinal.Serialization:Optional;

export import :Box;

export namespace Cardinal::Serialization
{
    template<typename T> requires IsSerializableBox<Functionality::Optional<T>>
    struct SerializationHandler<Functionality::Optional<T>> {
        static void SerializeImpl(const Functionality::Optional<T>& data, Byte* rawStream, SizeT& offset) {
            auto hasValue = data.HasValue;
            SerializationHandler<bool>::SerializeImpl(hasValue, rawStream, offset);
            if (hasValue)
            {
                SerializationHandler<T>::SerializeImpl(data.Value, rawStream, offset);
            }
        }

        static constexpr SizeT DataSize(const Functionality::Optional<T>& data) {
            return sizeof(Boolean) + (data.HasValue ? SerializationHandler<T>::DataSize(data.Value) : 0);
        }

        static constexpr Boolean IsMemCopy() {
            return false;
        }

        static constexpr auto TypeHint() {
            return
                TypeTraits::CreateTypeStr(L"");
        }

        static constexpr auto ConstexprSize() {
            return
                TypeTraits::CreateTypeStr(L"0");
        }

        static void DeserializeImpl(Functionality::Optional<T>& data, const Byte* rawStream, SizeT& offset) {
            bool hasValue;
            SerializationHandler<Boolean>::DeserializeImpl(hasValue, rawStream, offset);
            if (hasValue)
            {
                T val = T();

                SerializationHandler<T>::DeserializeImpl(val, rawStream, offset);

                data.Value = val;
            }
        }

        static constexpr auto TypeMetaData() {
            return
                TypeTraits::CreateTypeStr(L"{") +
                CTTI::TypeName<Functionality::Optional<T>>() +
                TypeTraits::CreateTypeStr(L"|") +
                TypeTraits::ToCharStr<static_cast<SizeT>(CTTI::TypeTraits::CTTITypeMetaData<Functionality::Optional<T>>::TypeClass)>() +
                TypeTraits::CreateTypeStr(L"|") +
                TypeTraits::ToCharStr<sizeof(Functionality::Optional<T>)>() +
                TypeTraits::CreateTypeStr(L"|") +
                ConstexprSize() +
                TypeTraits::CreateTypeStr(L"|") +
                TypeHint() +
                TypeTraits::CreateTypeStr(L"|") +
                SerializationHandler<T>::TypeMetaData() +
                TypeTraits::CreateTypeStr(L"}");
        }

        template<typename THeap>
        static BaseTypeMetaDataRecord<THeap> GetTypeMetaDataRecord() {
            return BaseTypeMetaDataRecord<THeap>(
                CTTI::TypeName<T>(),
                CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass,
                sizeof(T),
                0,
                TypeHint(),
                { SerializationHandler<T>::template GetTypeMetaDataRecord<THeap>() }
            );
        }
    };
}