using Cardinal.CTTI;

namespace Cardinal.Objects.ObjectNamespace;

[SerializableStruct("Cardinal::Session::ObjectNamespaceAPI::ObjectFullInformation")]
public class ObjectFullInformation : ObjectMinimalInformation
{
    [SerializableStructField("AdditionalData", "Cardinal::Containers::Dictionary<Cardinal::String, Cardinal::RTTI::Dynamic>")]
    public Dictionary<String, DynamicObject> AdditionalData { get; set; } = new Dictionary<String, DynamicObject>();
}
