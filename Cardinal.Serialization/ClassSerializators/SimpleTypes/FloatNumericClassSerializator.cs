using Cardinal.CTTI.MetaData;

namespace Cardinal.CTTI.ClassSerializators;

class FloatNumericClassSerializator : SimpeTypeSerializator
{
    public override void Deserialize(ref object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        switch (thisTypeSerializationTypeRecord.TypeName)
        {
            case "Cardinal::Float":
                {
                    byte[] buffer = new byte[sizeof(float)];
                    stream.Read(buffer, 0, sizeof(float));
                    data = BitConverter.ToSingle(buffer, 0);
                }
                break;
            case "Cardinal::Double":
                {
                    byte[] buffer = new byte[sizeof(double)];
                    stream.Read(buffer, 0, sizeof(double));
                    data = BitConverter.ToDouble(buffer, 0);
                }
                break;
            default:
                throw new NotImplementedException($"Deserialization of type  '{thisTypeSerializationTypeRecord.TypeName} not implemented");
        }
    }

    public override void Serialize(object data, Stream stream, TypeMetaDataRecord thisTypeSerializationTypeRecord, List<SerializationTypeRecord> serializationTypeRecords, Dictionary<ETypes, ITypeSerializator> typeSerializators)
    {
        switch (thisTypeSerializationTypeRecord.TypeName)
        {
            case "Cardinal::Float":
                {
                    var val = (float)data;
                    stream.Write(BitConverter.GetBytes(val), 0, sizeof(float));
                }
                break;
            case "Cardinal::Double":
                {
                    var val = (double)data;
                    stream.Write(BitConverter.GetBytes(val), 0, sizeof(double));
                }
                break;
            default:
                throw new NotImplementedException($"Serialization of type  ' {thisTypeSerializationTypeRecord.TypeName} ' and type class ' {thisTypeSerializationTypeRecord.Type} ' not implemented");
        }
    }

    public override List<SerializationTypeRecord> GetSupportedTypes() => new List<SerializationTypeRecord>
    {
        new SerializationTypeRecord
        {
            ClrType = typeof(float),
            CardinalKernelTypeShema = new TypeMetaDataRecord
            {
                TypeName = "Cardinal::Float",
                SizeOfSize = sizeof(float),
                StaticSize = sizeof(float),
                AdditionalInfo = "",
                Type = ETypes.FloatNumeric
            }
        },

        new SerializationTypeRecord
        {
            ClrType = typeof(double),
            CardinalKernelTypeShema = new TypeMetaDataRecord
            {
                TypeName = "Cardinal::Double",
                SizeOfSize = sizeof(double),
                StaticSize = sizeof(double),
                AdditionalInfo = "",
                Type = ETypes.FloatNumeric
            }
        }
    };
}
