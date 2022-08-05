using Cardinal.FS;
using Xunit;

namespace Cardinal.CTTI.Tests.Complex;

public class StructClassSerializator
{
    [Fact]
    public void ResolvingStructTypeTest()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(typeof(Cardinal.FS.VolumeInformation));
        Assert.Contains(serializator.SerializationShema, schema => schema.ClrType == typeof(Cardinal.FS.VolumeInformation));
    }

    [Fact]
    public void SerializeStructType_SizeCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(typeof(Cardinal.FS.VolumeInformation));
        var schema = serializator.SerializationShema.First(schema => schema.ClrType == typeof(Cardinal.FS.VolumeInformation));
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
        serializator.Deserialize(stream);

        Assert.Equal(146, stream.Length);
    }

    [Fact]
    public void SerializeStructType_ValueCorrect()
    {
        var serializator = new CardinalBinnarySerializator();
        serializator.RegisterType(typeof(Cardinal.FS.VolumeInformation));

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
        var deserializaedValue = (Cardinal.FS.VolumeInformation)serializator.Deserialize(stream);
        Assert.Equal(value, deserializaedValue);
    }
}
