using Xunit;

namespace Cardinal.CTTI.ClassSerializators.ComplexTypes;

public class CharacterClassSerializator
{
    [Fact]
    public void SerializeCharType_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, 'a');
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(((sizeof(char) * "Cardinal::UChar\0".Length) + sizeof(ulong)) + sizeof(char), stream.Length);
    }

    [Theory]
    [InlineData('\n')]
    [InlineData(char.MaxValue)]
    [InlineData(char.MinValue)]
    public void SerializeCharType_ValueCorrect(char value)
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        char deserializaedValue = (char)serializator.Deserialize(stream);
        Assert.Equal(value, deserializaedValue);
    }
}
