namespace Cardinal.CTTI.MetaData;

public struct SerializationTypeRecord
{
    public string CardinalKernelTypeName { get => CardinalKernelTypeShema.TypeName; set => CardinalKernelTypeShema.TypeName = value; }
    public Type ClrType { get; set; }
    public ETypes CardinalKernelTypeClass { get => CardinalKernelTypeShema.Type; set => CardinalKernelTypeShema.Type = value; }
    public TypeMetaDataRecord CardinalKernelTypeShema { get; set; }
}
