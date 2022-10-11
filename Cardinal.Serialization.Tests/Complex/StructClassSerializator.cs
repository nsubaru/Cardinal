using Cardinal.FS;
using Cardinal.Serialization.Tests;
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using Xunit;

namespace Cardinal.CTTI.Tests.Complex
{
    public class StructClassSerializator : BaseTest
    {
        [Fact]
        public void SerializeStructType_SizeCorrect()
        {
            var serializator = new CardinalBinarySerializator(rttiSection);
            var value = new VolumeInformation
            {
                FsName = "NTFS",
                Attributes = new VolumeAttributes{
                    Flags = 0x0
                },
                CreationTime = new Time.DateTime{
                    Ticks = (ulong)DateTime.Now.Ticks
                },
                Label = "System",
                SerialNumber = 0x0,
                Size = new VolumeSize
                {
                    ClusterSize = 4 * 1024,
                    AllocatedClusters = ((1024ul * 1024 * 1024 * 100) / (4 * 1024))  / 2,
                    TotalClusters = ((1024ul * 1024 * 1024 * 100) / (4 * 1024))
                }
            };

            var stream = new MemoryStream();

            serializator.Serialize(stream, value);
            stream.Position = 0;
            serializator.Deserialize(stream);

            Assert.Equal(100, stream.Length);
        }

        [Fact]
        public void SerializeStructType_ValueCorrect()
        {
            var serializator = new CardinalBinarySerializator(rttiSection);

            var value = new VolumeInformation
            {
                FsName = "NTFS",
                Attributes = new VolumeAttributes
                {
                    Flags = 0x0
                },
                CreationTime = new Time.DateTime
                {
                    Ticks = (ulong)DateTime.Now.Ticks
                },
                Label = "System",
                SerialNumber = 0x0,
                Size = new VolumeSize
                {
                    ClusterSize = 4 * 1024,
                    AllocatedClusters = ((1024ul * 1024 * 1024 * 100) / (4 * 1024)) / 2,
                    TotalClusters = ((1024ul * 1024 * 1024 * 100) / (4 * 1024))
                }
            };

            var stream = new MemoryStream();

            serializator.Serialize(stream, value);
            stream.Position = 0;
            var deserializedValue = (Cardinal.FS.VolumeInformation)serializator.Deserialize(stream);
            Assert.Equal(value, deserializedValue);
        }
    }
}
