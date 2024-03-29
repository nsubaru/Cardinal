export module Cardinal.Serialization:IntegratedTypes;

export import :Core;

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

export namespace Cardinal::Serialization
{
    template<typename T>
    constexpr bool IsSerializableEnum = TypeTraits::IsEnumV<T> && CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable;

    template<typename T> requires IsSerializableEnum<T>
    struct SerializationHandler<T> {
        static void SerializeImpl(const T& data, Byte* rawStream, SizeT& offset) {
            Memory::Copy(reinterpret_cast<const Byte*>(&data), rawStream + offset, sizeof(T)); offset += sizeof(T);
        }

        static constexpr SizeT DataSize(const T& data) {
            return sizeof(T);
        }

        static constexpr Boolean IsMemCopy() {
            return true;
        }

        static constexpr auto TypeHint() {
            if constexpr (CTTI::TypeTraits::CTTITypeMetaData<T>::IsContainAdditionalInfo)
            {
                constexpr auto enumValuesData = TypeTraits::CreateTypeStr(L"[") +
                    CTTI::TypeTraits::CTTITypeMetaData<T>::ItemsMap.Sum([] (auto item, auto I) constexpr {
                    return
                        item.template GetByIdx<1>() +
                        TypeTraits::CreateTypeStr(L" ") +
                        item.template GetByIdx<2>() +
                        TypeTraits::CreateTypeStr(L",");
                        });

                return enumValuesData.Resize<enumValuesData.Count - 1>() + TypeTraits::CreateTypeStr(L"]");
            }
            else
            {
                return
                    TypeTraits::CreateTypeStr(L"");
            }
        }

        static constexpr auto ConstexprSize() {
            return
                TypeTraits::ToCharStr<sizeof(T)>();
        }

        static void DeserializeImpl(T& data, const Byte* rawStream, SizeT& offset) {
            data = *(reinterpret_cast<const T*>(rawStream + offset)); offset += sizeof(T);
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
                SerializationHandler<typename TypeTraits::EnumUnderlyingTypeV<T>>::TypeMetaData() +
                TypeTraits::CreateTypeStr(L"}");
        }

        template<typename THeap>
        static BaseTypeMetaDataRecord<THeap> GetTypeMetaDataRecord() {
            return BaseTypeMetaDataRecord<THeap>(
                CTTI::TypeName<T>(),
                CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass,
                sizeof(T),
                sizeof(T),
                TypeHint(),
                { SerializationHandler<typename TypeTraits::EnumUnderlyingTypeV<T>>::template GetTypeMetaDataRecord<THeap>() }
            );
        }
    };
}

#pragma warning(pop)