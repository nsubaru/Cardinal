export module Cardinal.Serialization:Box;

export import :Struct;

export namespace Cardinal::Serialization
{
    template<typename T>
    concept IsSerializableBox =
        CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass == CTTI::EType::Box
        &&
        CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable;

    template<typename T, typename TLazyHeap> requires IsSerializableBox<BaseLazy<TLazyHeap, T>>
    struct SerializationHandler<BaseLazy<TLazyHeap, T>> {
        static void SerializeImpl(const BaseLazy<TLazyHeap, T>& data, Byte* rawStream, SizeT& offset) {
            SerializationHandler<T>::SerializeImpl(*data, rawStream, offset);
        }

        static constexpr SizeT DataSize(const BaseLazy<TLazyHeap, T>& data) {
            return SerializationHandler<T>::DataSize(*data);
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

        static void DeserializeImpl(BaseLazy<TLazyHeap, T>& data, const Byte* rawStream, SizeT& offset) {
            T val = T();

            SerializationHandler<T>::DeserializeImpl(val, rawStream, offset);

            data = BaseLazy<TLazyHeap, T>(MoveRValue(val));
        }

        static constexpr auto TypeMetaData() {
            return
                TypeTraits::CreateTypeStr(L"{") +
                CTTI::TypeName<BaseLazy<TLazyHeap, T>>() +
                TypeTraits::CreateTypeStr(L"|") +
                TypeTraits::ToCharStr<static_cast<SizeT>(CTTI::TypeTraits::CTTITypeMetaData<BaseLazy<TLazyHeap, T>>::TypeClass)>() +
                TypeTraits::CreateTypeStr(L"|") +
                TypeTraits::ToCharStr<sizeof(BaseLazy<TLazyHeap, T>)>() +
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

    template<typename T, typename TSmartPtrHeap> requires IsSerializableBox<BaseSmartPtr<TSmartPtrHeap, T>>
    struct SerializationHandler<BaseSmartPtr<TSmartPtrHeap, T>> {
        static void SerializeImpl(const BaseSmartPtr<TSmartPtrHeap, T>& data, Byte* rawStream, SizeT& offset) {
            SerializationHandler<T>::SerializeImpl(*data, rawStream, offset);
        }

        static constexpr SizeT DataSize(const BaseSmartPtr<TSmartPtrHeap, T>& data) {
            return SerializationHandler<T>::DataSize(*data);
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

        static void DeserializeImpl(BaseSmartPtr<TSmartPtrHeap, T>& data, const Byte* rawStream, SizeT& offset) {
            T* val = new T();

            SerializationHandler<T>::DeserializeImpl(*val, rawStream, offset);

            data = BaseSmartPtr<TSmartPtrHeap, T>(MoveRValue(val));
        }

        static constexpr auto TypeMetaData() {
            return
                TypeTraits::CreateTypeStr(L"{") +
                CTTI::TypeName<BaseSmartPtr<TSmartPtrHeap, T>>().Resize<CTTI::TypeName<BaseSmartPtr<TSmartPtrHeap, T>>().Count - 1>() +
                TypeTraits::CreateTypeStr(L"|") +
                TypeTraits::ToCharStr<static_cast<SizeT>(CTTI::TypeTraits::CTTITypeMetaData<BaseSmartPtr<TSmartPtrHeap, T>>::TypeClass)>() +
                TypeTraits::CreateTypeStr(L"|") +
                TypeTraits::ToCharStr<sizeof(BaseSmartPtr<TSmartPtrHeap, T>)>() +
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