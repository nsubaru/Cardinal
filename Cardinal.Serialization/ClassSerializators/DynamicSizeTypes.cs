using Cardinal.CTTI.MetaData;

namespace Cardinal.CTTI.ClassSerializators;

abstract class DynamicSizeTypes : ITypeSerializator
{
    public abstract void Deserialize(ref object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators);
    public abstract void Serialize(object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators);
    public abstract Type TryResolveTypeByTypeMetaDataRecord(TypeMetaDataRecord thisTypeSerializationTypeRecord, Dictionary<ETypes, ITypeSerializator> typeSerializators);

    #region Serialization implementation
    public void WriteItemsCount(
        UInt64 count,
        Stream stream,
        List<SerializationTypeRecord> serializationTypeRecords,
        Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        typeSerializators[ETypes.Numeric].Serialize(
            count,
            stream, serializationTypeRecords.First(item => item.CardinalKernelTypeName == "Cardinal::UInt64").CardinalKernelTypeShema,
            serializationTypeRecords,
            typeSerializators);
    }

    public void SerializeItem<T>(
        T item,
        Stream stream,
        List<SerializationTypeRecord> serializationTypeRecords,
        TypeMetaDataRecord thisTypeSerializationTypeRecord,
        Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        typeSerializators[thisTypeSerializationTypeRecord.Type]
            .Serialize(item, stream, thisTypeSerializationTypeRecord, serializationTypeRecords, typeSerializators);
    }

    #endregion

    #region Deserilization implementation
    public UInt64 GetItemsCount(
        Stream stream,
        List<SerializationTypeRecord> serializationTypeRecords,
        Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        object data = new object();
        data = (UInt64)0;

        typeSerializators[ETypes.Numeric].Deserialize(
            ref data,
            stream, serializationTypeRecords.First(item => item.CardinalKernelTypeName == "Cardinal::UInt64").CardinalKernelTypeShema,
            serializationTypeRecords,
            typeSerializators);

        return (UInt64)data;
    }

    public T DeserializeItem<T>(
        Stream stream,
        List<SerializationTypeRecord> serializationTypeRecords,
        TypeMetaDataRecord thisTypeSerializationTypeRecord,
        Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        T var = default;
        object data = var;

        typeSerializators[thisTypeSerializationTypeRecord.Type]
            .Deserialize(ref data, stream, thisTypeSerializationTypeRecord, serializationTypeRecords, typeSerializators);

        return (T)data;
    }
    #endregion

    #region TypeResolver

    public Type TryResolveSubTypeByTypeMetaDataRecord(TypeMetaDataRecord subTypeSerializationTypeRecord, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        return typeSerializators[subTypeSerializationTypeRecord.Type]
            .TryResolveTypeByTypeMetaDataRecord(subTypeSerializationTypeRecord, typeSerializators);
    }

    #endregion
}
