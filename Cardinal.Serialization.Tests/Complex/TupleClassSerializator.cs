using Cardinal.CTTI.MetaData;
using Xunit;

namespace Cardinal.CTTI.Tests.Complex;

public class TupleClassSerializator
{
    static readonly TypeMetaDataRecord TupleShema = new TypeMetaDataRecord
    {
        NestedTypes = new List<TypeMetaDataRecord>
            {
                new CardinalBinnarySerializator().SerializationShema.Where(shema => shema.CardinalKernelTypeName == "Cardinal::Boolean").First().CardinalKernelTypeShema,
                new CardinalBinnarySerializator().SerializationShema.Where(shema => shema.CardinalKernelTypeName == "Cardinal::Int32").First().CardinalKernelTypeShema,
                new CardinalBinnarySerializator().SerializationShema.Where(shema => shema.CardinalKernelTypeName == "Cardinal::UChar").First().CardinalKernelTypeShema
            },
        SizeOfSize = sizeof(bool) + sizeof(int) + sizeof(char),
        Type = ETypes.Tuple,
        TypeName = "Cardinal::Containers::Tuple<Cardinal::Boolean, Cardinal::Int32, Cardinal::Char>",
        StaticSize = 0,
        AdditionalInfo = ""
    };

    static SerializationTypeRecord TupleRecord = new SerializationTypeRecord
    {
        ClrType = new CardinalBinnarySerializator().ResolveGenericTypeByTypeMetaData(TupleShema),
        CardinalKernelTypeShema = TupleShema
    };

    [Fact]
    public void ResolvingTypeTest()
    {
        var serializator = new CardinalBinnarySerializator();
        Assert.NotNull(serializator.ResolveGenericTypeByTypeMetaData(TupleShema));
    }

    [Fact]
    public void SerializeArrayIntsType_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(TupleRecord);

        var value = new Tuple<bool, int, char>(true, 1, '1');

        var valueSize = sizeof(bool) + sizeof(int) + sizeof(char);

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        serializator.Deserialize(stream);

        Assert.Equal(((sizeof(char) * $"{TupleRecord.CardinalKernelTypeName}\0".Length) + sizeof(ulong)) + valueSize, stream.Length);
    }

    [Fact]
    public void SerializeArrayIntsType_ValueCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(TupleRecord);

        var value = new Tuple<bool, int, char>(true, 1, '1');

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        var deserializaedValue = (Tuple<bool, int, char>)serializator.Deserialize(stream);
        Assert.Equal(value, deserializaedValue);
    }
}
