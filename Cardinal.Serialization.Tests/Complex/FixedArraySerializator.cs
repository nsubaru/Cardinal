using Cardinal.Serialization.Tests;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Xunit;

namespace Cardinal.CTTI.Tests.Complex;

public class FixedArraySerializator : BaseTest
{
    [Fact]
    public void SerializeArrayIntsType_SizeCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);

        var value = new Int32[10];
        for (int i = 0; i < 10; i++)
        {
            value[i] = i;
        }

        var valueSize = (value.Count() * sizeof(Int32));

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

        var value = new Int32[10];
        for (int i = 0; i < 10; i++)
        {
            value[i] = i;
        }

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        var deserializedValue = (Int32[])serializator.Deserialize(stream);
        Assert.Equal(value, deserializedValue);
    }
}
