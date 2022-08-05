using Cardinal.CTTI.MetaData;
using Xunit;

namespace Cardinal.CTTI.Tests.Complex;

public class FixedCollectionClassSerializator
{
    static SerializationTypeRecord ListSerializationTypeRecord = new SerializationTypeRecord
    {
        ClrType = typeof(List<Int32>),
        CardinalKernelTypeShema = new TypeMetaDataRecord
        {
            NestedTypes = new List<TypeMetaDataRecord>
            {
                new CardinalBinnarySerializator().SerializationShema.Where(shema => shema.CardinalKernelTypeName == "Cardinal::Int32").First().CardinalKernelTypeShema
            },
            SizeOfSize = sizeof(ulong),
            Type = ETypes.FixedCollection,
            TypeName = "Cardinal::Containers::FixedArray<Cardinal::Int32, 10ui64>",
            StaticSize = 0,
            AdditionalInfo = "10"
        }
    };

    [Fact]
    public void SerializeListIntsType_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(ListSerializationTypeRecord);

        var value = new List<Int32>();
        for (int i = 0; i < 10; i++)
        {
            value.Add(i);
        }
        var valueSize = sizeof(UInt64) + (value.Count * sizeof(Int32));

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        serializator.Deserialize(stream);

        Assert.Equal(((sizeof(char) * "Cardinal::Containers::FixedArray<Cardinal::Int32, 10ui64>\0".Length) + sizeof(ulong)) + valueSize, stream.Length);
    }

    [Fact]
    public void SerializeListIntsType_ValueCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(ListSerializationTypeRecord);

        var value = new List<Int32>();
        for (int i = 0; i < 10; i++)
        {
            value.Add(i);
        }

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        List<Int32> deserializaedValue = (List<Int32>)serializator.Deserialize(stream);
        Assert.Equal(value, deserializaedValue);
    }
}
