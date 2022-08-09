using Xunit;

namespace Cardinal.CTTI.Tests.Primitives;

public class NumericClassSerializator
{
    #region Signed integer
    #region Int8
    [Fact]
    public void SerializeInt8Type_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, sbyte.MinValue);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(((sizeof(char) * "Cardinal::Int8\0".Length) + sizeof(ulong)) + sizeof(sbyte), stream.Length);
    }

    [Theory]
    [InlineData((sbyte)(0))]
    [InlineData((sbyte)(1))]
    [InlineData(sbyte.MaxValue)]
    [InlineData(sbyte.MinValue)]
    public void SerializeInt8Type_ValueCorrect(sbyte value)
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        sbyte deserializaedValue = (sbyte)serializator.Deserialize(stream);
        Assert.Equal(value, deserializaedValue);
    }
    #endregion
    #region Int16
    [Fact]
    public void SerializeInt16Type_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, Int16.MinValue);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(((sizeof(char) * "Cardinal::Int16\0".Length) + sizeof(ulong)) + sizeof(Int16), stream.Length);
    }

    [Theory]
    [InlineData((Int16)(0))]
    [InlineData((Int16)(1))]
    [InlineData(Int16.MaxValue)]
    [InlineData(Int16.MinValue)]
    public void SerializeInt16Type_ValueCorrect(Int16 value)
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        Int16 deserializaedValue = (Int16)serializator.Deserialize(stream);
        Assert.Equal(value, deserializaedValue);
    }
    #endregion
    #region Int32
    [Fact]
    public void SerializeInt32Type_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, Int32.MinValue);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(((sizeof(char) * "Cardinal::Int32\0".Length) + sizeof(ulong)) + sizeof(Int32), stream.Length);
    }

    [Theory]
    [InlineData((Int32)(0))]
    [InlineData((Int32)(1))]
    [InlineData(Int32.MaxValue)]
    [InlineData(Int32.MinValue)]
    public void SerializeInt32Type_ValueCorrect(Int32 value)
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        Int32 deserializaedValue = (Int32)serializator.Deserialize(stream);
        Assert.Equal(value, deserializaedValue);
    }
    #endregion
    #region Int64
    [Fact]
    public void SerializeInt64Type_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, Int64.MinValue);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(((sizeof(char) * "Cardinal::Int64\0".Length) + sizeof(ulong)) + sizeof(Int64), stream.Length);
    }

    [Theory]
    [InlineData((Int64)(0))]
    [InlineData((Int64)(1))]
    [InlineData(Int64.MaxValue)]
    [InlineData(Int64.MinValue)]
    public void SerializeInt64Type_ValueCorrect(Int64 value)
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        Int64 deserializaedValue = (Int64)serializator.Deserialize(stream);
        Assert.Equal(value, deserializaedValue);
    }
    #endregion
    #endregion
    #region Unsigned integer
    #region UInt8
    [Fact]
    public void SerializeUInt8Type_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, byte.MinValue);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(((sizeof(char) * "Cardinal::UInt8\0".Length) + sizeof(ulong)) + sizeof(byte), stream.Length);
    }

    [Theory]
    [InlineData((sbyte)(1))]
    [InlineData(byte.MaxValue)]
    [InlineData(byte.MinValue)]
    public void SerializeUInt8Type_ValueCorrect(byte value)
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        byte deserializaedValue = (byte)serializator.Deserialize(stream);
        Assert.Equal(value, deserializaedValue);
    }
    #endregion
    #region UInt16
    [Fact]
    public void SerializeUInt16Type_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, UInt16.MinValue);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(((sizeof(char) * "Cardinal::UInt16\0".Length) + sizeof(ulong)) + sizeof(UInt16), stream.Length);
    }

    [Theory]
    [InlineData((UInt16)(1))]
    [InlineData(UInt16.MaxValue)]
    [InlineData(UInt16.MinValue)]
    public void SerializeUInt16Type_ValueCorrect(UInt16 value)
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        UInt16 deserializaedValue = (UInt16)serializator.Deserialize(stream);
        Assert.Equal(value, deserializaedValue);
    }
    #endregion
    #region UInt32
    [Fact]
    public void SerializeUInt32Type_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, UInt32.MinValue);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(((sizeof(char) * "Cardinal::UInt32\0".Length) + sizeof(ulong)) + sizeof(UInt32), stream.Length);
    }

    [Theory]
    [InlineData((UInt32)(1))]
    [InlineData(UInt32.MaxValue)]
    [InlineData(UInt32.MinValue)]
    public void SerializeUInt32Type_ValueCorrect(UInt32 value)
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        UInt32 deserializaedValue = (UInt32)serializator.Deserialize(stream);
        Assert.Equal(value, deserializaedValue);
    }
    #endregion
    #region UInt64
    [Fact]
    public void SerializeUInt64Type_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, UInt64.MinValue);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(((sizeof(char) * "Cardinal::UInt64\0".Length) + sizeof(ulong)) + sizeof(UInt64), stream.Length);
    }

    [Theory]
    [InlineData((UInt64)(1))]
    [InlineData(UInt64.MaxValue)]
    [InlineData(UInt64.MinValue)]
    public void SerializeUInt64Type_ValueCorrect(UInt64 value)
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        UInt64 deserializaedValue = (UInt64)serializator.Deserialize(stream);
        Assert.Equal(value, deserializaedValue);
    }
    #endregion
    #endregion
}
