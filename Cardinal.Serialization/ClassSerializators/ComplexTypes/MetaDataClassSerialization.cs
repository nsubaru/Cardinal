using Cardinal.CTTI.MetaData;

namespace Cardinal.CTTI.ClassSerializators.ComplexTypes;

class MetaDataClassSerialization : DynamicSizeTypes
{
    public override void Deserialize(ref object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        TypeMetaDataRecord serializationTextType = null;
        if (thisTypeSerializationTypeRecord.TypeName == "Cardinal::Core::Serialization::TypeMetaDataRecord")
            serializationTextType = serializationTypeRecords.First(type => type.CardinalKernelTypeName == "Cardinal::String").CardinalKernelTypeShema;
        else if (thisTypeSerializationTypeRecord.TypeName == "Cardinal::Core::Serialization::SensitiveTypeMetaDataRecord")
            serializationTextType = serializationTypeRecords.First(type => type.CardinalKernelTypeName == "Cardinal::String").CardinalKernelTypeShema;
        else
            throw new NotImplementedException();

        var testMetaData = DeserializeItem<String>(stream, serializationTypeRecords, serializationTextType, typeSerializators);

        data = new TypeMetaDataDecoder().DecodeTypeMetaData(testMetaData);
    }

    public override void Serialize(object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        TypeMetaDataRecord serializationTextType = null;
        if (thisTypeSerializationTypeRecord.TypeName == "Cardinal::Core::Serialization::TypeMetaDataRecord")
            serializationTextType = serializationTypeRecords.First(type => type.CardinalKernelTypeName == "Cardinal::String").CardinalKernelTypeShema;
        else if (thisTypeSerializationTypeRecord.TypeName == "Cardinal::Core::Serialization::SensitiveTypeMetaDataRecord")
            serializationTextType = serializationTypeRecords.First(type => type.CardinalKernelTypeName == "Cardinal::SensitiveString").CardinalKernelTypeShema;
        else
            throw new NotImplementedException();

        string serializationText = ((TypeMetaDataRecord)data).ToString("S");

        SerializeItem(serializationText, stream, serializationTypeRecords, serializationTextType, typeSerializators);
    }

    public override Type TryResolveTypeByTypeMetaDataRecord(TypeMetaDataRecord thisTypeSerializationTypeRecord, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        if (thisTypeSerializationTypeRecord.TypeName == "Cardinal::Core::Serialization::TypeMetaDataRecord" ||
           thisTypeSerializationTypeRecord.TypeName == "Cardinal::Core::Serialization::SensitiveTypeMetaDataRecord")
        {
            return typeof(TypeMetaDataRecord);
        }

        return null;
    }
}
