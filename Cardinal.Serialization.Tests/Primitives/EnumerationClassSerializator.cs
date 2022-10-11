using Cardinal.Serialization.Tests;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Text;
using Xunit;

namespace Cardinal.CTTI.Tests.Primitives;

public class EnumerationClassSerializator : BaseTest
{
    [Fact]
    public void SerializeEnumType_SizeCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);

        var stream = new MemoryStream();
        serializator.Serialize(stream, ETypes.Enumeration);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(TestUtils.GetFullSize(sizeof(ETypes)), stream.Length);
    }

    [Theory]
    [InlineData(ETypes.Enumeration)]
    [InlineData(ETypes.Numeric)]
    [InlineData(ETypes.FloatNumeric)]
    [InlineData(ETypes.Character)]
    [InlineData(ETypes.String)]
    public void SerializeEnumType_ValueCorrect(ETypes value)
    {
        var serializator = new CardinalBinarySerializator(rttiSection);

        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        var deserializedValue = (ETypes) serializator.Deserialize(stream);
        Assert.Equal(value, deserializedValue);
    }
}
