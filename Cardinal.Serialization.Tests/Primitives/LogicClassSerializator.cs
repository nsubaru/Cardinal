using Xunit;

namespace Cardinal.CTTI.Tests.Primitives;

public class LogicClassSerializator
{
    [Fact]
    public void SerializeLogicType_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, true);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(((sizeof(char) * "Cardinal::Boolean\0".Length) + sizeof(ulong)) + 1, stream.Length);
    }

    [Theory]
    [InlineData(false)]
    [InlineData(true)]
    public void SerializeLogicType_ValueCorrect(bool value)
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        bool deserializaedValue = (bool)serializator.Deserialize(stream);
        Assert.Equal(value, deserializaedValue);
    }
}
