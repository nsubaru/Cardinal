using Cardinal.Serialization.Tests;
using Microsoft.VisualStudio.TestPlatform.PlatformAbstractions.Interfaces;
using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using Xunit;

namespace Cardinal.CTTI.Tests.Primitives
{
    public class FloatNumericClassSerializator : BaseTest
    {
        [Fact]
        public void SerializeFloatType_SizeCorrect()
        {
            var serializator = new CardinalBinarySerializator(rttiSection);
            var stream = new MemoryStream();
            serializator.Serialize(stream, 1.0f);
            stream.Position = 0;
            serializator.Deserialize(stream);
            Assert.Equal(TestUtils.GetFullSize(sizeof(float)), stream.Length);
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
            var serializator = new CardinalBinarySerializator(rttiSection);
            var stream = new MemoryStream();
            serializator.Serialize(stream, value);
            stream.Position = 0;
            var deserializedValue = serializator.Deserialize(stream);
            Assert.Equal(value, deserializedValue);
        }

        [Fact]
        public void SerializeDoubleType_SizeCorrect()
        {
            var serializator = new CardinalBinarySerializator(rttiSection);
            var stream = new MemoryStream();
            serializator.Serialize(stream, 1.0);
            stream.Position = 0;
            serializator.Deserialize(stream);
            Assert.Equal(TestUtils.GetFullSize(sizeof(double)), stream.Length);
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
            var serializator = new CardinalBinarySerializator(rttiSection);
            var stream = new MemoryStream();
            serializator.Serialize(stream, value);
            stream.Position = 0;
            var deserializedValue = (double)serializator.Deserialize(stream);
            Assert.Equal(value, deserializedValue);
        }
    }
}
