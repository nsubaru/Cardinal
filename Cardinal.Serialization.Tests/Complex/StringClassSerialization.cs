using Xunit;

namespace Cardinal.CTTI.Tests.Complex;

public class StringClassSerialization
{
    [Fact]
    public void SerializeStringType_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, "string");
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(((sizeof(char) * "Cardinal::String\0".Length) + sizeof(ulong)) + (sizeof(char) * "string\0".Length) + sizeof(ulong), stream.Length);
    }

    [Theory]
    [InlineData("")]
    [InlineData("1")]
    [InlineData("str")]
    [InlineData("текст")]
    [InlineData("текст1text")]
    public void SerializeStringType_ValueCorrect(string value)
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        string deserializaedValue = (string)serializator.Deserialize(stream);
        Assert.Equal(value, deserializaedValue);
    }
}
