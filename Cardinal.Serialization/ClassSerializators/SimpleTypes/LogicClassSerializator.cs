using Cardinal.CTTI.MetaData;

namespace Cardinal.CTTI.ClassSerializators;

class LogicClassSerializator : SimpeTypeSerializator
{
    public override void Deserialize(ref object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        switch (thisTypeSerializationTypeRecord.TypeName)
        {
            case "Cardinal::Boolean":
                {
                    byte[] buffer = new byte[sizeof(bool)];
                    stream.Read(buffer, 0, sizeof(bool));
                    data = BitConverter.ToBoolean(buffer, 0);
                }
                break;
            default:
                throw new NotImplementedException($"Deserialization of type '{thisTypeSerializationTypeRecord.TypeName}' and type class '{thisTypeSerializationTypeRecord.Type}' not implemented");
        }
    }

    public override void Serialize(object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        switch (thisTypeSerializationTypeRecord.TypeName)
        {
            case "Cardinal::Boolean":
                {
                    bool val = (bool)data;
                    stream.Write(BitConverter.GetBytes(val), 0, 1);
                }
                break;
            default:
                throw new NotImplementedException($"Serialization of type  '{thisTypeSerializationTypeRecord.TypeName}' and type class '{thisTypeSerializationTypeRecord.Type}' not implemented");
        }
    }

    public override List<SerializationTypeRecord> GetSupportedTypes() => new List<SerializationTypeRecord>
    {
        new SerializationTypeRecord
        {
            ClrType = typeof(bool),
            CardinalKernelTypeShema = new TypeMetaDataRecord
            {
                TypeName = "Cardinal::Boolean",
                SizeOfSize = sizeof(double),
                StaticSize = sizeof(double),
                AdditionalInfo = "",
                Type = ETypes.Logic
            }
        }
    };
}
