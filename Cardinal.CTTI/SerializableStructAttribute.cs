using Cardinal.CTTI.MetaData;

namespace Cardinal.CTTI;

[AttributeUsage(AttributeTargets.All)]
public class CardinalSerializableAttribute : Attribute
{
    public string CardinalKernelTypeName { get; private set; }
    public ETypes TypeClass { get; private set; }

    public CardinalSerializableAttribute(string cardinalKernelTypeName, ETypes typeClass)
    {
        CardinalKernelTypeName = cardinalKernelTypeName;
        TypeClass = typeClass;
    }
}

[AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct)]
public class SerializableStructAttribute : CardinalSerializableAttribute
{
    public SerializableStructAttribute(string cardinalKernelTypeName)
        : base(cardinalKernelTypeName, ETypes.Struct)
    { }
}

[AttributeUsage(AttributeTargets.Class | AttributeTargets.Struct)]
public class SerializableObjectAttribute : CardinalSerializableAttribute
{
    public SerializableObjectAttribute(string cardinalKernelTypeName)
         : base(cardinalKernelTypeName, ETypes.Struct)
    { }
}

[AttributeUsage(AttributeTargets.Property)]
public class SerializableStructFieldAttribute : Attribute
{
    public StructFieldInformation FieldInformation { get; private set; }

    public SerializableStructFieldAttribute(string fieldName, string fieldTypeName)
    {
        FieldInformation = new StructFieldInformation
        {
            FieldName = fieldName,
            FieldTypeName = fieldTypeName
        };
    }
}
