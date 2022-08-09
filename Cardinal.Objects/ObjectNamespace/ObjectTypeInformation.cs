using Cardinal.CTTI;

namespace Cardinal.Objects.ObjectNamespace;

[SerializableStruct("Cardinal::Session::ObjectNamespaceAPI::ObjectTypeInformation")]
public class ObjectTypeInformation
{
    [SerializableStructField("ObTypeId", "Cardinal::UInt64")]
    public UInt64 TypeId { get; set; }

    [SerializableStructField("TypeName", "Cardinal::String")]
    public string TypeName { get; set; } = string.Empty;

    [SerializableStructField("IsDirectory", "Cardinal::Boolean")]
    public Boolean IsDirectory { get; set; }
}
