using Cardinal.Serialization.Tests;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Xunit;

namespace Cardinal.CTTI.Tests.Primitives;

public class LogicClassSerializator : BaseTest
{
    [Fact]
    public void SerializeLogicType_SizeCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, true);
        stream.Position = 0;
        serializator.Deserialize(stream);

        Assert.Equal(TestUtils.GetFullSize(sizeof(bool)), stream.Length);
    }

    [Theory]
    [InlineData(false)]
    [InlineData(true)]
    public void SerializeLogicType_ValueCorrect(bool value)
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        bool deserializedValue = (bool) serializator.Deserialize(stream);
        Assert.Equal(value, deserializedValue);
    }
}
