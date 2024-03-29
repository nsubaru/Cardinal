export module Cardinal.Serialization:PublicAPI;

export import :Base;

export import :TypeMetaRecord;

export import :Core;
export import :IntegratedTypes;
export import :Composits;
export import :Collection;
export import :Box;
export import :Struct;
export import :MetaData;

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

export namespace Cardinal::Serialization
{
    template<typename THeap, typename T>
    Containers::BaseArray<THeap, Byte> BaseSerialize(const T& Object) {
        SizeT offset = 0;
        SizeT rawStreamSize = SerializationHandler<T>::DataSize(Object);

        auto array = Containers::BaseArray<THeap, Byte>(rawStreamSize);
        auto rawStream = (Byte*) (const Byte* const) array;

        SerializationHandler<T>::SerializeImpl(Object, rawStream, offset);

        return MoveRValue(array);
    }

    template<typename THeap, typename T>
    void BaseDeserilize(const Containers::BaseArray<THeap, Byte>& stream, T& Object) {
        auto rawStream = (const Byte*) stream;
        SizeT offset = 0;
        SerializationHandler<T>::DeserializeImpl(Object, rawStream, offset);
    }

    template<typename T>
    Containers::Array<Byte> Serialize(const T& Object) {
        return BaseSerialize<Memory::Heap, T>(Object);
    }

    template<typename T>
    Containers::SensitiveArray<Byte> SensitiveSerialize(const T& Object) {
        return BaseSerialize<Memory::SensitiveDataHeap, T>(Object);
    }

    template<typename T>
    void Deserialize(const Containers::Array<Byte>& stream, T& Object) {
        BaseDeserilize<Memory::Heap, T>(stream, Object);
    }

    template<typename T>
    T SensitiveDeserilize(const Containers::Array<Byte>& stream, T& Object) {
        BaseDeserilize<Memory::SensitiveDataHeap, T>(stream, Object);
    }

    template<typename T>
    SizeT SerializedDataSize(const T& object) {
        return SerializationHandler<T>::DataSize(object);
    }

    template<typename THeap, typename T>
    BaseString<THeap> BaseMetaDataGen() {
        return BaseString<THeap>(SerializationHandler<T>::TypeMetaData());
    }

    template<typename T>
    String MetaDataGen() {
        return String(SerializationHandler<T>::TypeMetaData());
    }

    template<typename T>
    SensitiveString SensitiveMetaDataGen() {
        return SensitiveString(SerializationHandler<T>::TypeMetaData());
    }

    template<typename T>
    constexpr auto CTTIMetaDataGen() {
        return SerializationHandler<T>::TypeMetaData();
    }

    template<typename THeap, typename T>
    constexpr BaseTypeMetaDataRecord<THeap> GetBaseTypeRecordForType() {
        return SerializationHandler<T>::template GetTypeMetaDataRecord<THeap>();
    }

    template<typename T>
    constexpr TypeMetaDataRecord GetTypeRecordForType() {
        return SerializationHandler<T>::template GetTypeMetaDataRecord<Memory::Heap>();
    }

    template<typename T>
    constexpr SensitiveTypeMetaDataRecord GetSensitiveTypeRecordForType() {
        return SerializationHandler<T>::template GetTypeMetaDataRecord<Memory::SensitiveDataHeap>();
    }
}

#pragma warning(pop)