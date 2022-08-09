using Cardinal.CTTI.MetaData;

namespace Cardinal.CTTI.ClassSerializators.ComplexTypes;

class FixedCollectionClassSerializator : DynamicSizeTypes
{
    #region Serialization implementation
    public void SerializeArrayList<T>(
        List<T> arrayList,
        Stream stream,
        TypeMetaDataRecord thisTypeSerializationTypeRecord,
        List<SerializationTypeRecord> serializationTypeRecords,
        Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        ulong ItemsCount = ulong.Parse(thisTypeSerializationTypeRecord.AdditionalInfo);
        WriteItemsCount(ItemsCount, stream, serializationTypeRecords, typeSerializators);

        int writeCount = arrayList.Count > (int)ItemsCount ? (int)ItemsCount : arrayList.Count;

        for (int i = 0; i < writeCount; i++)
        {
            SerializeItem(arrayList[i], stream, serializationTypeRecords, thisTypeSerializationTypeRecord.NestedTypes[0], typeSerializators);
        }

        if ((ulong)arrayList.Count < ItemsCount)
        {
            var defaultValueItemCount = ItemsCount - (ulong)arrayList.Count;

            for (ulong i = 0; i < defaultValueItemCount; i++)
            {
                SerializeItem(default(T), stream, serializationTypeRecords, thisTypeSerializationTypeRecord.NestedTypes[0], typeSerializators);
            }
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
    #endregion

    public override void Deserialize(ref object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        data = this
            .GetType()
            .GetMethod("DeserializeArrayList")
            .MakeGenericMethod(serializationTypeRecords
                    .First(item => item.CardinalKernelTypeName == thisTypeSerializationTypeRecord.NestedTypes[0].TypeName)
                    .ClrType)
            .Invoke(this, new object[] { stream, thisTypeSerializationTypeRecord, serializationTypeRecords, typeSerializators });
    }

    public override void Serialize(object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        this
            .GetType()
            .GetMethod("SerializeArrayList")
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

        return typeof(List<>).MakeGenericType(subType);
    }

}
