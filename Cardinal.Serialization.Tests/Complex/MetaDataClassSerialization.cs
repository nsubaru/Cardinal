using Cardinal.CTTI.MetaData;
using Xunit;

namespace Cardinal.CTTI.Tests.Complex;

public class MetaDataClassSerialization
{
    [Fact]
    public void SerializeStringType_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        var item = serializator.SerializationShema.Where(item => item.ClrType == typeof(TypeMetaDataRecord)).First();
        var itemDataSize = ((item.CardinalKernelTypeShema.ToString("S").Length + 1) * sizeof(char)) + sizeof(ulong);
        serializator.Serialize(stream, item.CardinalKernelTypeShema);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(((sizeof(char) * $"{item.CardinalKernelTypeName}\0".Length) + sizeof(ulong)) + itemDataSize, stream.Length);
    }

    [Fact]
    public void SerializeStringType_ValueCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        var value = serializator.SerializationShema.Where(item => item.ClrType == typeof(TypeMetaDataRecord)).First().CardinalKernelTypeShema;
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        var deserializaedValue = (TypeMetaDataRecord)serializator.Deserialize(stream);
        Assert.Equal(value.ToString("S"), deserializaedValue.ToString("S"));
    }
}
