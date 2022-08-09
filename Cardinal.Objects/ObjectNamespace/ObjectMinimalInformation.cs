using Cardinal.CTTI;

namespace Cardinal.Objects.ObjectNamespace;

[SerializableStruct("Cardinal::Session::ObjectNamespaceAPI::ObjectMinimalInformation")]
public class ObjectMinimalInformation
{
    [SerializableStructField("ObTypeId", "Cardinal::UInt64")]
    public ulong ObTypeId { get; set; }

    [SerializableStructField("Name", "Cardinal::String")]
    public string Name { get; set; } = string.Empty;
}
