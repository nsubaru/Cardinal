using Cardinal.CTTI.MetaData;
using Xunit;

namespace Cardinal.CTTI.Tests.Complex;

public class DynamicClassSerializator
{
    static readonly TypeMetaDataRecord DynamicObjectShema = new TypeMetaDataRecord
    {
        NestedTypes = new List<TypeMetaDataRecord>(),
        SizeOfSize = sizeof(ulong),
        Type = ETypes.Dynamic,
        TypeName = "Cardinal::Containers::Dynamic",
        StaticSize = 0,
        AdditionalInfo = ""
    };

    static SerializationTypeRecord DynamicObjectTypeRecord = new SerializationTypeRecord
    {
        ClrType = new CardinalBinnarySerializator().ResolveGenericTypeByTypeMetaData(DynamicObjectShema),
        CardinalKernelTypeShema = DynamicObjectShema
    };

    [Fact]
    public void SerializeArrayIntsType_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(DynamicObjectTypeRecord);

        var value = new DynamicObject(10);

        var valueSize =
            ((sizeof(char) *
                $"{serializator.SerializationShema.First(schema => schema.ClrType == typeof(Int32)).CardinalKernelTypeName}\0".Length)) +
            sizeof(Int32) +
            sizeof(UInt64);

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        serializator.Deserialize(stream);

        Assert.Equal(
            (sizeof(char) * $"{DynamicObjectTypeRecord.CardinalKernelTypeName}\0".Length) + sizeof(ulong) + valueSize,
            stream.Length);
    }

    [Fact]
    public void SerializeArrayIntsType_ValueCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(DynamicObjectTypeRecord);

        var value = new DynamicObject(10);

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        var deserializaedValue = (DynamicObject)serializator.Deserialize(stream);
        Assert.Equal(value.Value, deserializaedValue.Value);
    }
}
