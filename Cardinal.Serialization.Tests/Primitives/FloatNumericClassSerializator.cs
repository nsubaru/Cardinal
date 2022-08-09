using Xunit;

namespace Cardinal.CTTI.Tests.Primitives;

public class FloatNumericClassSerializator
{
    [Fact]
    public void SerializeFloatType_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, 1.0f);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(((sizeof(char) * "Cardinal::Float\0".Length) + sizeof(ulong)) + sizeof(float), stream.Length);
    }

    [Theory]
    [InlineData(1.0f)]
    [InlineData(0.0f)]
    [InlineData(Single.MaxValue)]
    [InlineData(Single.MinValue)]
    [InlineData(Single.PositiveInfinity)]
    [InlineData(Single.NegativeInfinity)]
    [InlineData(Single.NaN)]
    public void SerializeFloatType_ValueCorrect(float value)
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        float deserializaedValue = (float)serializator.Deserialize(stream);
        Assert.Equal(value, deserializaedValue);
    }

    [Fact]
    public void SerializeDoubleType_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, 1.0);
        stream.Position = 0;
        serializator.Deserialize(stream);
        Assert.Equal(((sizeof(char) * "Cardinal::Double\0".Length) + sizeof(ulong)) + sizeof(double), stream.Length);
    }

    [Theory]
    [InlineData(1.0)]
    [InlineData(0.0)]
    [InlineData(Double.MaxValue)]
    [InlineData(Double.MinValue)]
    [InlineData(Double.PositiveInfinity)]
    [InlineData(Double.NegativeInfinity)]
    [InlineData(Double.NaN)]
    public void SerializeDoubleType_ValueCorrect(double value)
    {
        var serializator = new CardinalBinnarySerializator();
        var stream = new MemoryStream();
        serializator.Serialize(stream, value);
        stream.Position = 0;
        double deserializaedValue = (double)serializator.Deserialize(stream);
        Assert.Equal(value, deserializaedValue);
    }
}
