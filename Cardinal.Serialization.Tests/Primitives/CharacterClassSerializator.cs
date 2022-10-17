using Cardinal.Serialization.Tests;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Xunit;

namespace Cardinal.CTTI.Tests.Primitives;

public class CharacterClassSerializator : BaseTest
{
    [Fact]
    public void SerializeCharType_SizeCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, 'a');
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(TestUtils.GetFullSize(sizeof(char)), stream.Length);
    }

    [Theory]
    [InlineData('\n')]
    [InlineData(char.MaxValue)]
    [InlineData(char.MinValue)]
    public void SerializeCharType_ValueCorrect(char value)
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        char deserializedValue = (char)serializator.Deserialize(stream);
        Assert.Equal(value, deserializedValue);
    }
}
