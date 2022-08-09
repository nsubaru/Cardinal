using Cardinal.CTTI.MetaData;
using Xunit;

namespace Cardinal.CTTI.Tests.Complex;

public class BoxClassSerializator
{
    static readonly TypeMetaDataRecord FixedLazyShema = new TypeMetaDataRecord
    {
        NestedTypes = new List<TypeMetaDataRecord>
            {
                new CardinalBinnarySerializator().SerializationShema.Where(shema => shema.CardinalKernelTypeName == "Cardinal::Int32").First().CardinalKernelTypeShema
            },
        SizeOfSize = sizeof(ulong),
        Type = ETypes.Box,
        TypeName = "Cardinal::Lazy<Cardinal::Int32>",
        StaticSize = 0,
        AdditionalInfo = ""
    };

    static SerializationTypeRecord LazyTypeRecord = new SerializationTypeRecord
    {
        ClrType = new CardinalBinnarySerializator().ResolveGenericTypeByTypeMetaData(FixedLazyShema),
        CardinalKernelTypeShema = FixedLazyShema
    };

    [Fact]
    public void SerializeArrayIntsType_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(LazyTypeRecord);

        var value = new Lazy<Int32>(() => 10);

        var valueSize = sizeof(Int32);

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        serializator.Deserialize(stream);

        Assert.Equal(((sizeof(char) * $"{LazyTypeRecord.CardinalKernelTypeName}\0".Length) + sizeof(ulong)) + valueSize, stream.Length);
    }

    [Fact]
    public void SerializeArrayIntsType_ValueCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(LazyTypeRecord);

        var value = new Lazy<Int32>(() => 10);

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        var deserializaedValue = (Lazy<Int32>)serializator.Deserialize(stream);
        Assert.Equal(value.Value, deserializaedValue.Value);
    }
}
