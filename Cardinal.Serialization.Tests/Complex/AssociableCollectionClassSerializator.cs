using Cardinal.Serialization.Tests;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Xunit;

namespace Cardinal.CTTI.Tests.Complex;

public class AssociableCollectionClassSerializator : BaseTest
{
    [Fact]
    public void SerializeMapType_SizeCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);

        var value = new Dictionary<Int32, string> { { 1, "text" }, { 2, "text" }, { 3, "text" }, { 4, "text" }, { 5, "text" } };
        var valueSize = sizeof(UInt64) + (value.Count * sizeof(Int32));

        foreach (var item in value)
        {
            valueSize += TestUtils.GetCorrectStringSize(item.Value);
        }

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        serializator.Deserialize(stream);

        Assert.Equal(TestUtils.GetFullSize(valueSize), stream.Length);
    }

    [Fact]
    public void SerializeMapType_ValueCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var value = new Dictionary<Int32, string> { { 1, "text" }, { 2, "text" }, { 3, "text" }, { 4, "text" }, { 5, "text" } };

        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        var deserializedValue = (Dictionary<Int32, string>)serializator.Deserialize(stream);
        Assert.Equal(value.AsEnumerable(), deserializedValue.AsEnumerable());
    }
}
