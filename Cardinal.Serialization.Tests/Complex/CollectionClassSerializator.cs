using Cardinal.Serialization.Tests;
using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.IO;
using System.Linq;
using System.Text;
using Xunit;

namespace Cardinal.CTTI.Tests.Complex;

public class CollectionClassSerializator : BaseTest
{
    [Fact]
    public void SerializeLinkedListIntsType_SizeCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);

        var value = new LinkedList<Int32>();
        for (int i = 0; i < 5; i++)
        {
            value.AddLast(i);
        }
        var valueSize = sizeof(UInt64) + (value.Count * sizeof(Int32));

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        serializator.Deserialize(stream);

        Assert.Equal(TestUtils.GetFullSize(valueSize), stream.Length);
    }

    [Fact]
    public void SerializeListIntsType_SizeCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);

        var value = new List<Int32>();
        for (int i = 0; i < 5; i++)
        {
            value.Add(i);
        }
        var valueSize = sizeof(UInt64) + (value.Count * sizeof(Int32));

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        serializator.Deserialize(stream);

        Assert.Equal(TestUtils.GetFullSize(valueSize), stream.Length);
    }

    [Fact]
    public void SerializeLinkedListIntsType_ValueCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);

        var value = new LinkedList<Int32>();
        for (int i = 0; i < 5; i++)
        {
            value.AddLast(i);
        }

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        LinkedList<Int32> deserializedValue = (LinkedList<Int32>)serializator.Deserialize(stream);
        Assert.Equal(value, deserializedValue);
    }

    [Fact]
    public void SerializeListIntsType_ValueCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);


        var value = new List<Int32>();
        for (int i = 0; i < 5; i++)
        {
            value.Add(i);
        }

        var stream = new MemoryStream();

        serializator.Serialize(stream, value);
        stream.Position = 0;
        List<Int32> deserializedValue = (List<Int32>)serializator.Deserialize(stream);
        Assert.Equal(value, deserializedValue);
    }
}
