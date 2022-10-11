using Cardinal.Serialization.Tests;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Xunit;

namespace Cardinal.CTTI.Tests.Primitives;

public class NumericClassSerializator : BaseTest
{
    #region Signed integer
    #region Int8
    [Fact]
    public void SerializeInt8Type_SizeCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, sbyte.MinValue);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(TestUtils.GetFullSize(sizeof(sbyte)), stream.Length);
    }

    [Theory]
    [InlineData((sbyte)(0))]
    [InlineData((sbyte)(1))]
    [InlineData(sbyte.MaxValue)]
    [InlineData(sbyte.MinValue)]
    public void SerializeInt8Type_ValueCorrect(sbyte value)
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        var deserializedValue = (sbyte)serializator.Deserialize(stream);
        Assert.Equal(value, (sbyte) deserializedValue);
    }
    #endregion
    #region Int16
    [Fact]
    public void SerializeInt16Type_SizeCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, Int16.MinValue);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(TestUtils.GetFullSize(sizeof(Int16)), stream.Length);
    }

    [Theory]
    [InlineData((Int16)(0))]
    [InlineData((Int16)(1))]
    [InlineData(Int16.MaxValue)]
    [InlineData(Int16.MinValue)]
    public void SerializeInt16Type_ValueCorrect(Int16 value)
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        var deserializedValue = (Int16) serializator.Deserialize(stream);
        Assert.Equal(value, deserializedValue);
    }
    #endregion
    #region Int32
    [Fact]
    public void SerializeInt32Type_SizeCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, Int32.MinValue);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(TestUtils.GetFullSize(sizeof(Int32)), stream.Length);
    }

    [Theory]
    [InlineData((Int32)(0))]
    [InlineData((Int32)(1))]
    [InlineData(Int32.MaxValue)]
    [InlineData(Int32.MinValue)]
    public void SerializeInt32Type_ValueCorrect(Int32 value)
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        var deserializedValue = (Int32)serializator.Deserialize(stream);
        Assert.Equal(value, deserializedValue);
    }
    #endregion
    #region Int64
    [Fact]
    public void SerializeInt64Type_SizeCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, Int64.MinValue);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(TestUtils.GetFullSize(sizeof(Int64)), stream.Length);
    }

    [Theory]
    [InlineData((Int64)(0))]
    [InlineData((Int64)(1))]
    [InlineData(Int64.MaxValue)]
    [InlineData(Int64.MinValue)]
    public void SerializeInt64Type_ValueCorrect(Int64 value)
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        var deserializedValue = (Int64) serializator.Deserialize(stream);
        Assert.Equal(value, deserializedValue);
    }
    #endregion
    #endregion
    #region Unsigned integer
    #region UInt8
    [Fact]
    public void SerializeUInt8Type_SizeCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, byte.MinValue);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(TestUtils.GetFullSize(sizeof(byte)), stream.Length);
    }

    [Theory]
    [InlineData((sbyte)(1))]
    [InlineData(byte.MaxValue)]
    [InlineData(byte.MinValue)]
    public void SerializeUInt8Type_ValueCorrect(byte value)
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        var deserializedValue = (byte)serializator.Deserialize(stream);
        Assert.Equal(value,  deserializedValue);
    }
    #endregion
    #region UInt16
    [Fact]
    public void SerializeUInt16Type_SizeCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, UInt16.MinValue);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(TestUtils.GetFullSize(sizeof(UInt16)), stream.Length);
    }

    [Theory]
    [InlineData((UInt16)(1))]
    [InlineData(UInt16.MaxValue)]
    [InlineData(UInt16.MinValue)]
    public void SerializeUInt16Type_ValueCorrect(UInt16 value)
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        var deserializedValue = (UInt16) serializator.Deserialize(stream);
        Assert.Equal(value, deserializedValue);
    }
    #endregion
    #region UInt32
    [Fact]
    public void SerializeUInt32Type_SizeCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, UInt32.MinValue);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(TestUtils.GetFullSize(sizeof(UInt32)), stream.Length);
    }

    [Theory]
    [InlineData((UInt32)(1))]
    [InlineData(UInt32.MaxValue)]
    [InlineData(UInt32.MinValue)]
    public void SerializeUInt32Type_ValueCorrect(UInt32 value)
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        var deserializedValue = (UInt32)serializator.Deserialize(stream);
        Assert.Equal(value, deserializedValue);
    }
    #endregion
    #region UInt64
    [Fact]
    public void SerializeUInt64Type_SizeCorrect()
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, UInt64.MinValue);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(TestUtils.GetFullSize(sizeof(UInt64)), stream.Length);
    }

    [Theory]
    [InlineData((UInt64)(1))]
    [InlineData(UInt64.MaxValue)]
    [InlineData(UInt64.MinValue)]
    public void SerializeUInt64Type_ValueCorrect(UInt64 value)
    {
        var serializator = new CardinalBinarySerializator(rttiSection);
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        var deserializedValue = (UInt64) serializator.Deserialize(stream);
        Assert.Equal(value, deserializedValue);
    }
    #endregion
    #endregion
}
