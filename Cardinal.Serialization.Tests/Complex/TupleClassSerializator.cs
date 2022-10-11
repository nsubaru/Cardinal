using Cardinal.Serialization.Tests;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Xunit;

namespace Cardinal.CTTI.Tests.Complex;

public class TupleClassSerializator : BaseTest
{
    [Fact]
    public void SerializeArrayIntsType_SizeCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);

        var value = new ValueTuple<Int32, Int64, bool>(Int32.MinValue, long.MaxValue, true);

        var valueSize = sizeof(Int32) + sizeof(Int64) + sizeof(bool);

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

        var value = new ValueTuple<Int32, Int64, bool>(Int32.MinValue, long.MaxValue, true);

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        var deserializedValue = (ValueTuple<Int32, Int64, bool>)serializator.Deserialize(stream);
        Assert.Equal(value, deserializedValue);
    }
}
