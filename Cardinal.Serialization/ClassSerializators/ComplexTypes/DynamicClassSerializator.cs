using Cardinal.CTTI.MetaData;

namespace Cardinal.CTTI.ClassSerializators.ComplexTypes;

class DynamicClassSerializator : DynamicSizeTypes
{
    public object DeserializeDynamic<T>(
       Stream stream,
       TypeMetaDataRecord thisTypeSerializationTypeRecord,
       List<SerializationTypeRecord> serializationTypeRecords,
       Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        T value = DeserializeItem<T>(stream, serializationTypeRecords, thisTypeSerializationTypeRecord, typeSerializators);
        return value;
    }

    public void SerializeDynamic<T>(
        T data,
        Stream stream,
        TypeMetaDataRecord thisTypeSerializationTypeRecord,
        List<SerializationTypeRecord> serializationTypeRecords,
        Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        SerializeItem(data, stream, serializationTypeRecords, thisTypeSerializationTypeRecord, typeSerializators);
    }

    public override void Deserialize(ref object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        object obj = "";
        typeSerializators[ETypes.String].Deserialize(
            ref obj,
            stream,
            serializationTypeRecords.Where(record => record.CardinalKernelTypeName == "Cardinal::String").First().CardinalKernelTypeShema,
            serializationTypeRecords,
            typeSerializators);

        string typeName = obj as string;

        var typeRecord = serializationTypeRecords.Where(record => record.CardinalKernelTypeName == typeName).First();

        data = new DynamicObject(this
           .GetType()
           .GetMethod("DeserializeDynamic")
           .MakeGenericMethod(typeRecord.ClrType)
           .Invoke(this, new object[] { stream, typeRecord.CardinalKernelTypeShema, serializationTypeRecords, typeSerializators }));

    }

    public override void Serialize(object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        var obj = (data as DynamicObject).Value;

        var typeRecord = serializationTypeRecords
            .Where(record => record.ClrType == obj.GetType())
            .First();

        typeSerializators[ETypes.String].Serialize(
            typeRecord.CardinalKernelTypeName,
            stream,
            serializationTypeRecords.Where(record => record.CardinalKernelTypeName == "Cardinal::String").First().CardinalKernelTypeShema,
            serializationTypeRecords,
            typeSerializators);

        this
            .GetType()
            .GetMethod("SerializeDynamic")
            .MakeGenericMethod(typeRecord.ClrType)
            .Invoke(this, new object[] { obj, stream, typeRecord.CardinalKernelTypeShema, serializationTypeRecords, typeSerializators });
    }

    public override Type TryResolveTypeByTypeMetaDataRecord(TypeMetaDataRecord thisTypeSerializationTypeRecord, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        if (thisTypeSerializationTypeRecord.TypeName == "Cardinal::Containers::Dynamic" ||
            thisTypeSerializationTypeRecord.TypeName == "Cardinal::Reflection::DynamicObject")
        {
            return typeof(DynamicObject);
        }
        else
        {
            return null;
        }
    }
}
