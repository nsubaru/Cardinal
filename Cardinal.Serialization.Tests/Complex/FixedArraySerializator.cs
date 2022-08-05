using Cardinal.CTTI.MetaData;
using Xunit;

namespace Cardinal.CTTI.Tests.Complex;

public class FixedArraySerializator
{
    static TypeMetaDataRecord FixedArrayShema = new TypeMetaDataRecord
    {
        NestedTypes = new List<TypeMetaDataRecord>
            {
                new CardinalBinnarySerializator().SerializationShema.Where(shema => shema.CardinalKernelTypeName == "Cardinal::Int32").First().CardinalKernelTypeShema
            },
        SizeOfSize = sizeof(ulong),
        Type = ETypes.FixedArray,
        TypeName = "Cardinal::Int32[10]",
        StaticSize = 0,
        AdditionalInfo = "10"
    };

    static SerializationTypeRecord ArrayTypeRecord = new SerializationTypeRecord
    {
        ClrType = new CardinalBinnarySerializator().ResolveGenericTypeByTypeMetaData(FixedArrayShema),
        CardinalKernelTypeShema = FixedArrayShema
    };

    [Fact]
    public void SerializeArrayIntsType_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(ArrayTypeRecord);

        var value = new Int32[10];
        for (int i = 0; i < 10; i++)
        {
            value[i] = i;
        }

        var valueSize = (value.Count() * sizeof(Int32));

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        serializator.Deserialize(stream);

        Assert.Equal(((sizeof(char) * $"{ArrayTypeRecord.CardinalKernelTypeName}\0".Length) + sizeof(ulong)) + valueSize, stream.Length);
    }

    [Fact]
    public void SerializeArrayIntsType_ValueCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(ArrayTypeRecord);

        var value = new Int32[10];
        for (int i = 0; i < 10; i++)
        {
            value[i] = i;
        }

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        var deserializaedValue = (Int32[])serializator.Deserialize(stream);
        Assert.Equal(value, deserializaedValue);
    }
}
