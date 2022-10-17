using Cardinal.Serialization.Tests;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Xunit;

namespace Cardinal.CTTI.Tests.Complex;

public class BoxClassSerializator : BaseTest
{
    [Fact]
    public void SerializeArrayIntsType_SizeCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);

        var value = new Lazy<Int32>(() => 10);

        var valueSize = sizeof(Int32);

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        serializator.Deserialize(stream);

        Assert.Equal(TestUtils.GetFullSize(valueSize), stream.Length);
    }

    [Fact]
    public void SerializeArrayIntsType_ValueCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);

        var value = new Lazy<Int32>(() => 10);

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        var deserializedValue = (Lazy<Int32>)serializator.Deserialize(stream);
        Assert.Equal(value.Value, deserializedValue.Value);
    }
}
