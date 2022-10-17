using Cardinal.Serialization.Tests;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Xunit;

namespace Cardinal.CTTI.Tests.Complex;

public class StringClassSerialization : BaseTest
{
    [Fact]
    public void SerializeStringType_SizeCorrect()
    {
        var text = "string";
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, text);
        Assert.Equal(TestUtils.GetFullSize((text.Length + 1) * sizeof(char) + sizeof(ulong)), stream.Length);
    }

    [Theory]
    [InlineData("")]
    [InlineData("1")]
    [InlineData("str")]
    [InlineData("текст")]
    [InlineData("текст1text")]
    public void SerializeStringType_ValueCorrect(string value)
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        string deserializedValue = (string)serializator.Deserialize(stream);
        Assert.Equal(value, deserializedValue);
    }
}
