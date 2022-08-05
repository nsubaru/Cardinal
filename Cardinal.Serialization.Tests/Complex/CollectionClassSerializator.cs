using Cardinal.CTTI.MetaData;
using Xunit;

namespace Cardinal.CTTI.Tests.Complex;

public class CollectionClassSerializator
{
    static SerializationTypeRecord LinkedListSerializationTypeRecord = new SerializationTypeRecord
    {
        ClrType = typeof(LinkedList<Int32>),
        CardinalKernelTypeShema = new TypeMetaDataRecord
        {
            NestedTypes = new List<TypeMetaDataRecord>
            {
                new CardinalBinnarySerializator().SerializationShema.Where(shema => shema.CardinalKernelTypeName == "Cardinal::Int32").First().CardinalKernelTypeShema
            },
            SizeOfSize = sizeof(ulong),
            Type = ETypes.Collection,
            TypeName = "Cardinal::Containers::List<Cardinal::Int32>",
            StaticSize = 0,
            AdditionalInfo = ""
        }
    };

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
            Type = ETypes.Collection,
            TypeName = "Cardinal::Containers::Array<Cardinal::Int32>",
            StaticSize = 0,
            AdditionalInfo = ""
        }
    };

    [Fact]
    public void SerializeLinkedListIntsType_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(LinkedListSerializationTypeRecord);

        var value = new LinkedList<Int32>();
        for (int i = 0; i < 5; i++)
        {
            value.AddLast(i);
        }
        var valueSize = sizeof(UInt64) + (value.Count * sizeof(Int32));

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        serializator.Deserialize(stream);

        Assert.Equal(((sizeof(char) * "Cardinal::Containers::List<Cardinal::Int32>\0".Length) + sizeof(ulong)) + valueSize, stream.Length);
    }

    [Fact]
    public void SerializeListIntsType_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(ListSerializationTypeRecord);

        var value = new List<Int32>();
        for (int i = 0; i < 5; i++)
        {
            value.Add(i);
        }
        var valueSize = sizeof(UInt64) + (value.Count * sizeof(Int32));

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        serializator.Deserialize(stream);

        Assert.Equal(((sizeof(char) * "Cardinal::Containers::Array<Cardinal::Int32>\0".Length) + sizeof(ulong)) + valueSize, stream.Length);
    }

    [Fact]
    public void SerializeLinkedListIntsType_ValueCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(LinkedListSerializationTypeRecord);

        var value = new LinkedList<Int32>();
        for (int i = 0; i < 5; i++)
        {
            value.AddLast(i);
        }

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        LinkedList<Int32> deserializaedValue = (LinkedList<Int32>)serializator.Deserialize(stream);
        Assert.Equal(value, deserializaedValue);
    }

    [Fact]
    public void SerializeListIntsType_ValueCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(ListSerializationTypeRecord);

        var value = new List<Int32>();
        for (int i = 0; i < 5; i++)
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
