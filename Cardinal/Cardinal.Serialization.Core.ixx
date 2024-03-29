export module Cardinal.Serialization:Core;

export import :Base;
export import :TypeMetaRecord;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

export namespace Cardinal::Serialization
{

    template<typename T>
    struct SerializationHandler;

    template<typename T>
    constexpr bool IsNotSerlizable =
        !CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable;

    template<typename T> requires IsNotSerlizable<T>
    struct SerializationHandler<T> {
        static void SerializeImpl(const T& data, Byte* rawStream, SizeT& offset) {
            Internals::Exceptions::ThrowNonSerializableType(SourcePosition, CTTI::template TypeName<T>());
        }

        static constexpr SizeT DataSize(const T& data) {
            Internals::Exceptions::ThrowNonSerializableType(SourcePosition, CTTI::template TypeName<T>());
        }

        static constexpr Boolean IsMemCopy() {
            Internals::Exceptions::ThrowNonSerializableType(SourcePosition, CTTI::template TypeName<T>());
        }

        static constexpr auto TypeHint() {
            return
                TypeTraits::CreateTypeStr(L"N/A");
        }

        static constexpr auto ConstexprSize() {
            return
                TypeTraits::ToCharStr<0>();
        }

        static void DeserializeImpl(T& data, const Byte* rawStream, SizeT& offset) {
            Internals::Exceptions::ThrowNonSerializableType(SourcePosition, CTTI::TypeName<T>());
            __assume(false);
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
                TypeTraits::CreateTypeStr(L"}");
        }

        template<typename THeap>
        static BaseTypeMetaDataRecord<THeap> GetTypeMetaDataRecord() {
            return BaseTypeMetaDataRecord<THeap>(
                CTTI::TypeName<T>(),
                CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass,
                sizeof(T),
                0,
                TypeHint()
                );
        }
    };

    template<typename T>
    constexpr bool IsSerializablePrimitive =
        CTTI::IsSimpleTypes(CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass) &&
        CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable;

    template<typename T> requires IsSerializablePrimitive<T>
    struct SerializationHandler<T> {
        static void SerializeImpl(const T& data, Byte* rawStream, SizeT& offset) {
            Memory::Copy(reinterpret_cast<const Byte*>(&data), rawStream + offset, sizeof(T)); offset += sizeof(T);
        }

        static constexpr SizeT DataSize(const T& data) {
            return sizeof(data);
        }

        static constexpr Boolean IsMemCopy() {
            return true;
        }

        static constexpr auto TypeHint() {
            if constexpr (CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass == CTTI::EType::Character)
                return CTTI::TypeTraits::CTTITypeMetaData<T>::CodePage;
            else
                return TypeTraits::CreateTypeStr(L"");
        }

        static constexpr auto ConstexprSize() {
            return
                TypeTraits::ToCharStr<sizeof(T)>();
        }

        static void DeserializeImpl(T& data, const Byte* rawStream, SizeT& offset) {
            data = *(reinterpret_cast<const T*>(rawStream + offset)); offset += sizeof(T);
        }

        static constexpr auto InnerType() {
            if constexpr (CTTI::Concepts::IsNumericCttiTypeWithInnerType<T>)
            {
                return SerializationHandler<typename T::TInner>::TypeMetaData();
            }
            else
            {
                return TypeTraits::CreateTypeStr(L"");
            }
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
                InnerType() +
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

    template<typename T>
    constexpr bool IsCFixedArraySerlizable =
        CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass == CTTI::EType::FixedArray &&
        CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable;

    template<typename T> requires IsCFixedArraySerlizable<T>
    struct SerializationHandler<T> {
        using ItemType = typename CTTI::TypeTraits::CTTITypeMetaData<T>::Target;

        static void SerializeImpl(const T& data, Byte* rawStream, SizeT& offset) {
            if constexpr (IsMemCopy())
            {
                SizeT dataSize = DataSize(data);
                Memory::Copy(reinterpret_cast<const Byte*>(&data), rawStream + offset, dataSize);
                offset += dataSize;
            }
            else
            {
                for (SizeT i = 0; i < CTTI::TypeTraits::CTTITypeMetaData<T>::Count; i++)
                {
                    SerializationHandler<ItemType>::SerializeImpl(data[i], rawStream, offset);
                }
            }

        }

        static constexpr SizeT DataSize(const T& data) {
            if constexpr (IsMemCopy())
            {
                return sizeof(ItemType) * CTTI::TypeTraits::CTTITypeMetaData<T>::Count;
            }
            else
            {
                SizeT Size = 1;
                for (SizeT i = 0; i < CTTI::TypeTraits::CTTITypeMetaData<T>::Count; i++)
                {
                    Size *= SerializationHandler<ItemType>::DataSize(data[i]);
                }
                return Size;
            }
        }

        static constexpr Boolean IsMemCopy() {
            return SerializationHandler<ItemType>::IsMemCopy();
        }

        static constexpr auto TypeHint() {
            return
                TypeTraits::CreateTypeStr(L"");
        }

        static constexpr auto ConstexprSize() {
            return
                TypeTraits::ToCharStr<CTTI::TypeTraits::CTTITypeMetaData<T>::Count>();
        }

        static void DeserializeImpl(T& data, const Byte* rawStream, SizeT& offset) {
            if constexpr (IsMemCopy())
            {
                SizeT dataSize = sizeof(T);
                Memory::Copy(rawStream + offset, data, dataSize); offset += dataSize;
            }
            else
            {
                for (SizeT i = 0; i < CTTI::TypeTraits::CTTITypeMetaData<T>::Count; i++)
                {
                    SerializationHandler<ItemType>::DeserializeImpl(data[i], rawStream, offset);
                }
            }
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
                SerializationHandler<ItemType>::TypeMetaData() +
                TypeTraits::CreateTypeStr(L"}");
        }

        template<typename THeap>
        static BaseTypeMetaDataRecord<THeap> GetTypeMetaDataRecord() {
            return BaseTypeMetaDataRecord<THeap>(
                CTTI::TypeName<T>(),
                CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass,
                sizeof(T),
                CTTI::TypeTraits::CTTITypeMetaData<T>::Count,
                TypeHint(),
                { SerializationHandler<ItemType>::template GetTypeMetaDataRecord<THeap>() }
            );
        }
    };
}

#pragma warning(pop)