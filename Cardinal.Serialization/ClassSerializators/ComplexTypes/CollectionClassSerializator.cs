using Cardinal.CTTI.MetaData;

namespace Cardinal.CTTI.ClassSerializators;

class CollectionClassSerializator : DynamicSizeTypes
{
    #region Serialization implementation
    public void SerializeArrayList<T>(
        List<T> arrayList,
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

    public void SerializeLinkedList<T>(
        LinkedList<T> arrayList,
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
    public object DeserializeArrayList<T>(
        Stream stream,
        TypeMetaDataRecord thisTypeSerializationTypeRecord,
        List<SerializationTypeRecord> serializationTypeRecords,
        Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        var count = GetItemsCount(stream, serializationTypeRecords, typeSerializators);

        List<T> items = new List<T>();

        for (UInt64 i = 0; i < count; i++)
        {
            items.Add(DeserializeItem<T>(stream, serializationTypeRecords, thisTypeSerializationTypeRecord.NestedTypes[0], typeSerializators));
        }

        return items;
    }

    public object DeserializeLinkedList<T>(
        Stream stream,
        TypeMetaDataRecord thisTypeSerializationTypeRecord,
        List<SerializationTypeRecord> serializationTypeRecords,
        Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        var count = GetItemsCount(stream, serializationTypeRecords, typeSerializators);

        LinkedList<T> items = new LinkedList<T>();

        for (UInt64 i = 0; i < count; i++)
        {
            items.AddLast(DeserializeItem<T>(stream, serializationTypeRecords, thisTypeSerializationTypeRecord.NestedTypes[0], typeSerializators));
        }

        return items;
    }
    #endregion

    public override void Deserialize(ref object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        if (thisTypeSerializationTypeRecord.TypeName.StartsWith("Cardinal::Containers::List"))
        {
            data = this
                .GetType()
                .GetMethod("DeserializeLinkedList")
                .MakeGenericMethod(serializationTypeRecords
                        .First(item => item.CardinalKernelTypeName == thisTypeSerializationTypeRecord.NestedTypes[0].TypeName)
                        .ClrType)
                .Invoke(this, new object[] { stream, thisTypeSerializationTypeRecord, serializationTypeRecords, typeSerializators });
        }
        else
        {
            data = this
                .GetType()
                .GetMethod("DeserializeArrayList")
                .MakeGenericMethod(serializationTypeRecords
                        .First(item => item.CardinalKernelTypeName == thisTypeSerializationTypeRecord.NestedTypes[0].TypeName)
                        .ClrType)
                .Invoke(this, new object[] { stream, thisTypeSerializationTypeRecord, serializationTypeRecords, typeSerializators });
        }
    }

    public override void Serialize(object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        if (thisTypeSerializationTypeRecord.TypeName.StartsWith("Cardinal::Containers::List"))
        {
            this
                .GetType()
                .GetMethod("SerializeLinkedList")
                .MakeGenericMethod(serializationTypeRecords
                        .First(item => item.CardinalKernelTypeName == thisTypeSerializationTypeRecord.NestedTypes[0].TypeName)
                        .ClrType)
                .Invoke(this, new object[] { data, stream, thisTypeSerializationTypeRecord, serializationTypeRecords, typeSerializators });
        }
        else
        {
            this
                 .GetType()
                 .GetMethod("SerializeArrayList")
                 .MakeGenericMethod(serializationTypeRecords
                         .First(item => item.CardinalKernelTypeName == thisTypeSerializationTypeRecord.NestedTypes[0].TypeName)
                         .ClrType)
                 .Invoke(this, new object[] { data, stream, thisTypeSerializationTypeRecord, serializationTypeRecords, typeSerializators });
        }
    }

    public override Type TryResolveTypeByTypeMetaDataRecord(TypeMetaDataRecord thisTypeSerializationTypeRecord, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        if (thisTypeSerializationTypeRecord.TypeName.StartsWith("Cardinal::Containers::List"))
        {
            var subType = TryResolveSubTypeByTypeMetaDataRecord(thisTypeSerializationTypeRecord.NestedTypes[0], typeSerializators);

            if (subType == null)
                return null;

            return typeof(LinkedList<>).MakeGenericType(subType);
        }
        else
        {
            var subType = TryResolveSubTypeByTypeMetaDataRecord(thisTypeSerializationTypeRecord.NestedTypes[0], typeSerializators);

            if (subType == null)
                return null;

            return typeof(List<>).MakeGenericType(subType);
        }
    }
}
