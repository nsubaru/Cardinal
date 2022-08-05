using Cardinal.CTTI.MetaData;
using Xunit;

namespace Cardinal.CTTI.Tests.Complex;

public class PairClassSerializator
{
    static TypeMetaDataRecord PairShema = new TypeMetaDataRecord
    {
        NestedTypes = new List<TypeMetaDataRecord>
            {
                new CardinalBinnarySerializator().SerializationShema.Where(shema => shema.CardinalKernelTypeName == "Cardinal::Int32").First().CardinalKernelTypeShema,
                new CardinalBinnarySerializator().SerializationShema.Where(shema => shema.CardinalKernelTypeName == "Cardinal::Int32").First().CardinalKernelTypeShema
            },
        SizeOfSize = sizeof(Int32) + sizeof(Int32),
        Type = ETypes.Pair,
        TypeName = "Cardinal::Containers::Pair<Cardinal::Int32, Cardinal::Int32>",
        StaticSize = 0,
        AdditionalInfo = ""
    };

    static SerializationTypeRecord PairSerializationTypeRecord = new SerializationTypeRecord
    {
        ClrType = new CardinalBinnarySerializator().ResolveGenericTypeByTypeMetaData(PairShema),
        CardinalKernelTypeShema = PairShema
    };

    [Fact]
    public void SerializePairType_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(PairSerializationTypeRecord);

        var value = new KeyValuePair<Int32, Int32>();
        var valueSize = (sizeof(Int32) + sizeof(Int32));

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        serializator.Deserialize(stream);

        Assert.Equal(((sizeof(char) * $"{PairShema.TypeName}\0".Length) + sizeof(ulong)) + valueSize, stream.Length);
    }

    [Fact]
    public void SerializePairType_ValueCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(PairSerializationTypeRecord);
        var value = new KeyValuePair<int, int>(1, 1);

        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        var deserializaedValue = (KeyValuePair<Int32, Int32>)serializator.Deserialize(stream);
        Assert.Equal(value.Key, deserializaedValue.Key);
        Assert.Equal(value.Value, deserializaedValue.Value);
    }
}
