using Cardinal.CTTI.MetaData;

namespace Cardinal.CTTI.ClassSerializators.SimpleTypes;

class EnumerationClassSerializator : ITypeSerializator
{
    public void Deserialize(ref object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        Type type = data.GetType();

        if (type.IsEnum)
        {
            object underlyingData = Convert.ChangeType(data, Enum.GetUnderlyingType(type));
            typeSerializators[ETypes.Numeric]
                .Deserialize(ref underlyingData, stream, thisTypeSerializationTypeRecord.NestedTypes[0], serializationTypeRecords, typeSerializators);
            data = Enum.ToObject(type, underlyingData);
        }
        else
        {
            typeSerializators[ETypes.Numeric]
                .Deserialize(ref data, stream, thisTypeSerializationTypeRecord.NestedTypes[0], serializationTypeRecords, typeSerializators);
        }
    }

    public void Serialize(object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        Type type = data.GetType();

        if (type.IsEnum)
        {
            object underlyingData = Convert.ChangeType(data, Enum.GetUnderlyingType(type));
            typeSerializators[ETypes.Numeric]
                .Serialize(underlyingData, stream, thisTypeSerializationTypeRecord.NestedTypes[0], serializationTypeRecords, typeSerializators);
        }
        else
        {
            typeSerializators[ETypes.Numeric]
                .Serialize(data, stream, thisTypeSerializationTypeRecord.NestedTypes[0], serializationTypeRecords, typeSerializators);
        }
    }

    public Type TryResolveTypeByTypeMetaDataRecord(TypeMetaDataRecord thisTypeSerializationTypeRecord, Dictionary<ETypes, ITypeSerializator> typeSerializators)
        => typeSerializators[ETypes.Numeric].TryResolveTypeByTypeMetaDataRecord(thisTypeSerializationTypeRecord.NestedTypes[0], typeSerializators);
}
