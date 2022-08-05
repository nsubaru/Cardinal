using Cardinal.CTTI.MetaData;

namespace Cardinal.CTTI.ClassSerializators.ComplexTypes;

class BoxClassSerializator : DynamicSizeTypes
{
    public object DeserializeLazyImpl<T>(
        Stream stream,
        TypeMetaDataRecord thisTypeSerializationTypeRecord,
        List<SerializationTypeRecord> serializationTypeRecords,
        Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        T value = DeserializeItem<T>(stream, serializationTypeRecords, thisTypeSerializationTypeRecord.NestedTypes[0], typeSerializators);
        Lazy<T> data = new Lazy<T>(() => value);
        return data;
    }

    public void SerializeLazyImpl<T>(
        Lazy<T> data,
        Stream stream,
        TypeMetaDataRecord thisTypeSerializationTypeRecord,
        List<SerializationTypeRecord> serializationTypeRecords,
        Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        SerializeItem(data.Value, stream, serializationTypeRecords, thisTypeSerializationTypeRecord.NestedTypes[0], typeSerializators);
    }

    public override void Deserialize(ref object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {

        data = this
           .GetType()
           .GetMethod("DeserializeLazyImpl")
           .MakeGenericMethod(serializationTypeRecords
                   .First(item => item.CardinalKernelTypeName == thisTypeSerializationTypeRecord.NestedTypes[0].TypeName)
                   .ClrType)
           .Invoke(this, new object[] { stream, thisTypeSerializationTypeRecord, serializationTypeRecords, typeSerializators });
    }

    public override void Serialize(object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        this
            .GetType()
            .GetMethod("SerializeLazyImpl")
            .MakeGenericMethod(serializationTypeRecords
                    .First(item => item.CardinalKernelTypeName == thisTypeSerializationTypeRecord.NestedTypes[0].TypeName)
                    .ClrType)
            .Invoke(this, new object[] { data, stream, thisTypeSerializationTypeRecord, serializationTypeRecords, typeSerializators });
    }

    public override Type TryResolveTypeByTypeMetaDataRecord(TypeMetaDataRecord thisTypeSerializationTypeRecord, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        var subType = TryResolveSubTypeByTypeMetaDataRecord(thisTypeSerializationTypeRecord.NestedTypes[0], typeSerializators);

        if (subType == null)
            return null;

        return typeof(Lazy<>).MakeGenericType(subType);
    }
}
