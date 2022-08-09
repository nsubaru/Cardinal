using Cardinal.CTTI.MetaData;
using Xunit;

namespace Cardinal.CTTI.Tests.Complex;

public class AssociableCollectionClassSerializator
{
    static TypeMetaDataRecord PairShema = new TypeMetaDataRecord
    {
        NestedTypes = new List<TypeMetaDataRecord>
            {
                new CardinalBinnarySerializator().SerializationShema.Where(shema => shema.CardinalKernelTypeName == "Cardinal::Int32").First().CardinalKernelTypeShema,
                new CardinalBinnarySerializator().SerializationShema.Where(shema => shema.CardinalKernelTypeName == "Cardinal::Int32").First().CardinalKernelTypeShema
            },
        SizeOfSize = sizeof(ulong),
        Type = ETypes.Pair,
        TypeName = "Cardinal::Containers::Pair<Cardinal::Int32, Cardinal::Int32>",
        StaticSize = 0,
        AdditionalInfo = ""
    };

    static TypeMetaDataRecord MapShema = new TypeMetaDataRecord
    {
        NestedTypes = new List<TypeMetaDataRecord>
        {
            PairShema
        },
        SizeOfSize = sizeof(ulong),
        Type = ETypes.AssociableCollection,
        TypeName = "Cardinal::Containers::Map<Cardinal::Int32, Cardinal::Int32>",
        StaticSize = 0,
        AdditionalInfo = ""
    };

    static SerializationTypeRecord DictionarySerializationTypeRecord = new SerializationTypeRecord
    {
        ClrType = new CardinalBinnarySerializator().ResolveGenericTypeByTypeMetaData(MapShema),
        CardinalKernelTypeShema = MapShema
    };

    [Fact]
    public void SerializeMapType_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(DictionarySerializationTypeRecord);

        var value = new Dictionary<Int32, Int32> { { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 } };
        var valueSize = sizeof(UInt64) + (value.Count * (sizeof(Int32) + sizeof(Int32)));

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        serializator.Deserialize(stream);

        Assert.Equal(((sizeof(char) * $"{MapShema.TypeName}\0".Length) + sizeof(ulong)) + valueSize, stream.Length);
    }

    [Fact]
    public void SerializeMapType_ValueCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(DictionarySerializationTypeRecord);
        var value = new Dictionary<Int32, Int32> { { 1, 1 }, { 2, 2 }, { 3, 3 }, { 4, 4 }, { 5, 5 } };

        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        var deserializaedValue = (Dictionary<Int32, Int32>)serializator.Deserialize(stream);
        Assert.Equal(value.AsEnumerable(), deserializaedValue.AsEnumerable());
    }
}
