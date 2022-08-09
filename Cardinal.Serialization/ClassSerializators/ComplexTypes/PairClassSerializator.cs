using Cardinal.CTTI.MetaData;

namespace Cardinal.CTTI.ClassSerializators.ComplexTypes;

class PairClassSerializator : DynamicSizeTypes
{
    public void SerilizeImpl<T, U>(KeyValuePair<T, U> pair, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        SerializeItem<T>(pair.Key, stream, serializationTypeRecords, thisTypeSerializationTypeRecord.NestedTypes[0], typeSerializators);
        SerializeItem<U>(pair.Value, stream, serializationTypeRecords, thisTypeSerializationTypeRecord.NestedTypes[1], typeSerializators);
    }

    public object DeserilizeImpl<T, U>(Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        return new KeyValuePair<T, U>
        (
            DeserializeItem<T>(stream, serializationTypeRecords, thisTypeSerializationTypeRecord.NestedTypes[0], typeSerializators),
            DeserializeItem<U>(stream, serializationTypeRecords, thisTypeSerializationTypeRecord.NestedTypes[1], typeSerializators)
        );

    }

    public override void Deserialize(ref object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        var keyType = serializationTypeRecords
                    .First(item => item.CardinalKernelTypeName == thisTypeSerializationTypeRecord.NestedTypes[0].TypeName)
                    .ClrType;

        var valueType = serializationTypeRecords
                    .First(item => item.CardinalKernelTypeName == thisTypeSerializationTypeRecord.NestedTypes[0].TypeName)
                    .ClrType;

        data = this
            .GetType()
            .GetMethod("DeserilizeImpl")
            .MakeGenericMethod(new Type[] { keyType, valueType })
            .Invoke(this, new object[] { stream, thisTypeSerializationTypeRecord, serializationTypeRecords, typeSerializators });
    }

    public override void Serialize(object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        var keyType = serializationTypeRecords
                    .First(item => item.CardinalKernelTypeName == thisTypeSerializationTypeRecord.NestedTypes[0].TypeName)
                    .ClrType;

        var valueType = serializationTypeRecords
                    .First(item => item.CardinalKernelTypeName == thisTypeSerializationTypeRecord.NestedTypes[0].TypeName)
                    .ClrType;

        this
            .GetType()
            .GetMethod("SerilizeImpl")
            .MakeGenericMethod(new Type[] { keyType, valueType })
            .Invoke(this, new object[] { data, stream, thisTypeSerializationTypeRecord, serializationTypeRecords, typeSerializators });
    }

    public override Type TryResolveTypeByTypeMetaDataRecord(TypeMetaDataRecord thisTypeSerializationTypeRecord, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        var keyTypeShema = thisTypeSerializationTypeRecord.NestedTypes[0];
        var valueTypeShema = thisTypeSerializationTypeRecord.NestedTypes[1];

        var keyType = TryResolveSubTypeByTypeMetaDataRecord(keyTypeShema, typeSerializators);
        var valueType = TryResolveSubTypeByTypeMetaDataRecord(valueTypeShema, typeSerializators);

        if (keyType == null || valueType == null)
            return null;

        return typeof(KeyValuePair<,>).MakeGenericType(new Type[] { keyType, valueType });
    }
}
