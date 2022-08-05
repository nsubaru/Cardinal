using Cardinal.CTTI.MetaData;

namespace Cardinal.CTTI.ClassSerializators.ComplexTypes;

class AssociableCollectionClassSerializator : DynamicSizeTypes
{
    #region Serialization implementation
    public void SerializeDictionaryImpl<T, U>(
        Dictionary<T, U> arrayList,
        Stream stream,
        TypeMetaDataRecord thisTypeSerializationTypeRecord,
        List<SerializationTypeRecord> serializationTypeRecords,
        Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        WriteItemsCount((ulong)arrayList.Count, stream, serializationTypeRecords, typeSerializators);

        foreach (var item in arrayList)
        {
            SerializeItem(item, stream, serializationTypeRecords, thisTypeSerializationTypeRecord.NestedTypes[0], typeSerializators);
        }
    }
    #endregion

    #region Deserialization implementation
    public object DeserializeDictionaryImpl<T, U>(
        Stream stream,
        TypeMetaDataRecord thisTypeSerializationTypeRecord,
        List<SerializationTypeRecord> serializationTypeRecords,
        Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        var count = GetItemsCount(stream, serializationTypeRecords, typeSerializators);

        var items = new Dictionary<T, U>();

        for (UInt64 i = 0; i < count; i++)
        {
            var item = DeserializeItem<KeyValuePair<T, U>>(stream, serializationTypeRecords, thisTypeSerializationTypeRecord.NestedTypes[0], typeSerializators);
            items.Add(item.Key, item.Value);
        }

        return items;
    }
    #endregion

    public override void Deserialize(ref object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        var pairShema = thisTypeSerializationTypeRecord.NestedTypes[0];

        var keyShema = pairShema.NestedTypes[0];
        var valueShema = pairShema.NestedTypes[1];

        var keyType = serializationTypeRecords
                    .First(item => item.CardinalKernelTypeName == keyShema.TypeName)
                    .ClrType;

        var valueType = serializationTypeRecords
                    .First(item => item.CardinalKernelTypeName == valueShema.TypeName)
                    .ClrType;

#pragma warning disable CS8601 // Possible null reference assignment.
        data = this
            .GetType()
            !.GetMethod("DeserializeDictionaryImpl")
            !.MakeGenericMethod(new Type[] { keyType, valueType })
            !.Invoke(this, new object[] { stream, thisTypeSerializationTypeRecord, serializationTypeRecords, typeSerializators });
#pragma warning restore CS8601 // Possible null reference assignment.
    }

    public override void Serialize(object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        var pairShema = thisTypeSerializationTypeRecord.NestedTypes[0];

        var keyShema = pairShema.NestedTypes[0];
        var valueShema = pairShema.NestedTypes[1];

        var keyType = serializationTypeRecords
                    .First(item => item.CardinalKernelTypeName == keyShema.TypeName)
                    .ClrType;

        var valueType = serializationTypeRecords
                    .First(item => item.CardinalKernelTypeName == valueShema.TypeName)
                    .ClrType;

#pragma warning disable CS8602 // Dereference of a possibly null reference.
        _ = this
            .GetType()
            .GetMethod("SerializeDictionaryImpl")
            .MakeGenericMethod(new Type[] { keyType, valueType })
            .Invoke(this, new object[] { data, stream, thisTypeSerializationTypeRecord, serializationTypeRecords, typeSerializators });
#pragma warning restore CS8602 // Dereference of a possibly null reference.
    }

    public override Type TryResolveTypeByTypeMetaDataRecord(TypeMetaDataRecord thisTypeSerializationTypeRecord, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        var subType = TryResolveSubTypeByTypeMetaDataRecord(thisTypeSerializationTypeRecord.NestedTypes[0], typeSerializators);

        var pairTypes = subType.GetGenericArguments();

        return typeof(Dictionary<,>).MakeGenericType(pairTypes);
    }
}
