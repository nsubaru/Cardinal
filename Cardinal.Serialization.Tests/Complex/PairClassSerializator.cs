using Cardinal.Serialization.Tests;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Xunit;

namespace Cardinal.CTTI.Tests.Complex
{
    public class PairClassSerializator : BaseTest
    {
        [Fact]
        public void SerializePairType_SizeCorrect()
        {
            var serializator = new CardinalBinarySerializator(rttiSection);

            var value = new KeyValuePair<Int32, String>(0, "");
            var valueSize = (sizeof(Int32) + TestUtils.GetCorrectStringSize(""));

            var stream = new MemoryStream();

            serializator.Serialize(stream, value);
            stream.Position = 0;
            serializator.Deserialize(stream);

            Assert.Equal(TestUtils.GetFullSize(valueSize), stream.Length);
        }

        [Fact]
        public void SerializePairType_ValueCorrect()
        {
            var serializator = new CardinalBinarySerializator(rttiSection);
            var value = new KeyValuePair<Int32, String>(0, "text");

            var stream = new MemoryStream();
            serializator.Serialize(stream, value);
            stream.Position = 0;
            var deserializedValue = (KeyValuePair<Int32, String>)serializator.Deserialize(stream);
            Assert.Equal(value.Key, deserializedValue.Key);
            Assert.Equal(value.Value, deserializedValue.Value);
        }
    }
}
