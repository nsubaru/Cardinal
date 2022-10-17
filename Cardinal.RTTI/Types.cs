namespace Cardinal.RTTI;

//HACK: Enum values is incorect,
//      because Cardinal kernel EnumFlags use item index starting from 1 as index of bit
//      whereas C# [Flags] atributed enum use enum value as bit mask

[Flags]
public enum ETypeModificators : byte
{
	Const = 1,
	Volatile = 2,
	Serilizable = 4,
	TemplateType = 8
}

[Flags]
public enum ECvRefModificators : SizeT
{
	Ref = 1,
	RValueRef = 2,
	Const = 4,
	Volatile = 8
}

public enum ECustomTypeSchemaFormat : byte
{
	None,
	StructSchema,
	StructWithAccessorMethodSchema,
	ObjectSchema,
	RemoteStructSchema,
	RemoteObjectSchema,
	Undefined
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode, Pack = 0)]
public unsafe struct KernelVersionT
{
	[FieldOffset(0)]
	public UInt16 Major;

	[FieldOffset(4)]
	public UInt16 Minor;

	[FieldOffset(4)]
	public UInt16 StableBuildNumber;

	[FieldOffset(6)]
	public UInt16 BuildNumber;
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode, Pack = 0)]
internal unsafe struct TextMetaData
{
	[FieldOffset(0)]
	public SizeT Size;

	[FieldOffset(8)]
	public fixed Сhar Text[1];
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode, Pack = 0)]
internal unsafe struct MetaDataForDynamicRecord
{
	[FieldOffset(0)]
	public TEmptyCtor EmptyConstruct;

	[FieldOffset(8)]
	public TCopyCtor CopyCtor;

	[FieldOffset(16)]
	public TMoveCtor MoveCtor;

	[FieldOffset(24)]
	public TDtor Dtor;

	[FieldOffset(32)]
	public TGetRttiRecord GetRttiRecord;

	[FieldOffset(40)]
	public SizeT BinarySize;

	[FieldOffset(48)]
	public TSerialize FSerialize;

	[FieldOffset(56)]
	public TDeserialize FDeserialize;

	[FieldOffset(64)]
	public TSerializizationSize FSerializizationSize;

	[FieldOffset(72)]
	public TToString FToString;
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode, Pack = 0)]
public unsafe struct UniversalTypeIdentificator
{
	[FieldOffset(0)]
	public UUID Id;

	[FieldOffset(0)]
	public KernelVersionT KernelVersion;

	[FieldOffset(4)]
	public ID TypeId;

    public override int GetHashCode()
    {
        return base.GetHashCode();
    }

    public override bool Equals(object? obj)
    {
        var item = (UniversalTypeIdentificator?)obj;

        return this == item;
    }

    public override string ToString()
    {
        return this.Id.ToString();
    }

    static public bool operator ==(UniversalTypeIdentificator? item1, UniversalTypeIdentificator? item2)
    {
        return item1?.Id == item2?.Id;
    }

    static public bool operator !=(UniversalTypeIdentificator? item1, UniversalTypeIdentificator? item2)
    {
        return item1?.Id == item2?.Id;
    }
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode, Pack = 0)]
internal unsafe struct MetaData
{
	[FieldOffset(0)]
	public SizeT OffsetToCttiMetaData;

	[FieldOffset(16)]
	public UniversalTypeIdentificator TypeId;

	[FieldOffset(32)]
	public ETypes TypeClass;

	[FieldOffset(33)]
	public MetaDataForDynamicRecord MetaDataForDynamic;

	[FieldOffset(113)]
	public TypeModificator TypeModificators;

	[FieldOffset(114)]
	public TextMetaData TypeName;
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode, Pack = 0)]
internal unsafe struct StructMetaData
{
	[FieldOffset(0)]
	public SizeT OffsetToCollection;

	[FieldOffset(8)]
	public TUnsafeGet BaseGetter;
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode, Pack = 0)]
internal unsafe struct Collection
{
	[FieldOffset(0)]
	public SizeT OffsetToFirstItem;

	[FieldOffset(8)]
	public SizeT Count;

	[FieldOffset(16)]
	public SizeT SizeOfCollection;
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode, Pack = 0)]
internal unsafe struct ItemBase
{
	[FieldOffset(0)]
	public SizeT SizeOfItem;
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode, Pack = 0)]
internal unsafe struct FieldMetaDataRecord
{
	[FieldOffset(0)]
	public UniversalTypeIdentificator TypeId;

	[FieldOffset(16)]
	public ETypes TypeClass;

	[FieldOffset(17)]
	public SizeT Offset;

	[FieldOffset(25)]
	public MetaDataForDynamicRecord MetaDataForDynamic;

	[FieldOffset(105)]
	public TypeModificator TypeModificators;

	[FieldOffset(106)]
	public TextMetaData FieldName;
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode, Pack = 0)]
internal unsafe struct FieldMetaData
{
	[FieldOffset(0)]
	public SizeT SizeOfItem;

	[FieldOffset(16)]
	public UniversalTypeIdentificator TypeId;

	[FieldOffset(32)]
	public ETypes TypeClass;

	[FieldOffset(33)]
	public SizeT Offset;

	[FieldOffset(41)]
	public MetaDataForDynamicRecord MetaDataForDynamic;

	[FieldOffset(121)]
	public TypeModificator TypeModificators;

	[FieldOffset(122)]
	public TextMetaData FieldName;
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode, Pack = 0)]
internal unsafe struct ItemMetaData
{
	[FieldOffset(0)]
	public SizeT SizeOfItem;

	[FieldOffset(16)]
	public UniversalTypeIdentificator TypeId;

	[FieldOffset(32)]
	public ETypes TypeClass;

	[FieldOffset(33)]
	public MetaDataForDynamicRecord MetaDataForDynamic;

	[FieldOffset(113)]
	public TypeModificator TypeModificators;

	[FieldOffset(114)]
	public TextMetaData TypeName;
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode, Pack = 0)]
internal unsafe struct ArgumentMetaData
{
	[FieldOffset(0)]
	public SizeT SizeOfItem;

	[FieldOffset(16)]
	public UniversalTypeIdentificator TypeId;

	[FieldOffset(32)]
	public ETypes TypeClass;

	[FieldOffset(33)]
	public MetaDataForDynamicRecord MetaDataForDynamic;

	[FieldOffset(113)]
	public Boolean IsSerializable;

	[FieldOffset(114)]
	public CvRefModificators Flags;

	[FieldOffset(122)]
	public TextMetaData TypeName;
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode, Pack = 0)]
internal unsafe struct Constructor
{
	[FieldOffset(0)]
	public SizeT SizeOfItem;

	[FieldOffset(8)]
	public TGetRttiRecord GetRttiRecord;

	[FieldOffset(16)]
	public TDynamicCtorCaller DynamicCtorCaller;

	[FieldOffset(24)]
	public SizeT OffsetToArgs;

	[FieldOffset(32)]
	public Collection Args;
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode, Pack = 0)]
internal unsafe struct ObjectMetaData
{
	[FieldOffset(0)]
	public SizeT ConstructorsOffset;

	[FieldOffset(8)]
	public SizeT InterfacesOffset;

	[FieldOffset(16)]
	public SizeT PropertiesOffset;

	[FieldOffset(24)]
	public SizeT MethodsOffset;
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode, Pack = 0)]
internal unsafe struct InterfaceRecord
{
	[FieldOffset(0)]
	public SizeT SizeOfItem;

	[FieldOffset(16)]
	public UniversalTypeIdentificator TypeId;

	[FieldOffset(32)]
	public MetaDataForDynamicRecord MetaDataForDynamic;

	[FieldOffset(112)]
	public DynamicConverterFunc DynamicConverterTo;

	[FieldOffset(120)]
	public DynamicConverterFunc DynamicConverterFrom;

	[FieldOffset(128)]
	public TextMetaData TypeName;
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode, Pack = 0)]
internal unsafe struct MethodMetaData
{
	[FieldOffset(0)]
	public SizeT SizeOfItem;

	[FieldOffset(8)]
	public SizeT MethodArgsOffset;

	[FieldOffset(16)]
	public CvRefModificators MethodCVRefModificators;

	[FieldOffset(24)]
	public DynamicCallerFunc DynamicCaller;

	[FieldOffset(32)]
	public TextMetaData MethodName;
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode, Pack = 0)]
internal unsafe struct PropertyRecord
{
	[FieldOffset(0)]
	public SizeT SizeOfItem;

	[FieldOffset(8)]
	public SizeT OffsetToGetters;

	[FieldOffset(16)]
	public SizeT OffsetToSetters;

	[FieldOffset(24)]
	public TUnsafeGet UnsafeGet;

	[FieldOffset(32)]
	public FieldMetaDataRecord PropertyType;
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode, Pack = 0)]
internal unsafe struct CustomTypeMetaData
{
	[FieldOffset(0)]
	public ECustomTypeSchemaFormat SchemaFormat;
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode, Pack = 0)]
internal unsafe struct StructCustomTypeMetaData
{
	[FieldOffset(0)]
	public ECustomTypeSchemaFormat SchemaFormat;

	[FieldOffset(1)]
	public SizeT OffsetToCollection;

	[FieldOffset(9)]
	public TUnsafeGet BaseGetter;
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Unicode, Pack = 0)]
internal unsafe struct ObjectCustomTypeMetaData
{
	[FieldOffset(0)]
	public ECustomTypeSchemaFormat SchemaFormat;

	[FieldOffset(1)]
	public SizeT ConstructorsOffset;

	[FieldOffset(9)]
	public SizeT InterfacesOffset;

	[FieldOffset(17)]
	public SizeT PropertiesOffset;

	[FieldOffset(25)]
	public SizeT MethodsOffset;
}

[StructLayout(LayoutKind.Explicit, CharSet = CharSet.Ansi, Pack = 0)]
internal unsafe struct SectionHeaderType
{
	[FieldOffset(0)]
	public fixed Char SectionMagicNumbe[13];

	[FieldOffset(13)]
	public KernelVersionT KernelVersion;

	[FieldOffset(21)]
	public RawMemBlk OriginHeaderAddr;

	[FieldOffset(29)]
	public RawMemBlk FaAddr;

	[FieldOffset(37)]
	public RawMemBlk FzAddr;

	[FieldOffset(45)]
	public RawMemBlk VaAddr;

	[FieldOffset(53)]
	public RawMemBlk VzAddr;

	[FieldOffset(61)]
	public RawMemBlk ImageBase;
}