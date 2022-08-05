namespace Cardinal.RTTI.Record;

/// <summary>
/// A base RTTI field class
/// </summary>
/// <typeparam name="THeap">Heap type</typeparam>
public unsafe class RttiField : IRttiIterableObject<RttiField>
{
    internal RttiIterator<RttiField>? it;

    FieldMetaData* GetRecord() { return (FieldMetaData*)it!.item; }

    /// <summary>
    /// Constructor
    /// </summary>
    public RttiField()
    {
        it = null;
    }

    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="it">Iterator</param>
    public RttiField(RttiIterator<RttiField> it)
    {
        this.it = it;
    }

    /// <summary>
    /// Create instance from iterator
    /// </summary>
    /// <param name="iterator">Object data</param>
    /// <returns>New instance</returns>
    public RttiField CreateInstance(RttiIterator<RttiField> iterator)
    {
        return new RttiField(iterator);
    }

    /// <summary>
    /// Field type ID
    /// </summary>
    public SizeT TypeId => (SizeT)GetRecord()->TypeId.TypeId + it!.imageBase;

    /// <summary>
    /// Field full type ID
    /// </summary>
    public UniversalTypeIdentificator FullTypeId => GetRecord()->TypeId;

    /// <summary>
    /// Field type category
    /// </summary>
    public ETypes TypeClass => GetRecord()->TypeClass;

    /// <summary>
    /// Field type size
    /// </summary>
    public SizeT BinarySize => GetRecord()->MetaDataForDynamic.BinarySize;

    /// <summary>
    /// Field type CV modificators
    /// </summary>
    public TypeModificator TypeModificators => GetRecord()->TypeModificators;

    /// <summary>
    /// Field name, for enum item it is integer value
    /// </summary>
    public string FieldName => Marshal.PtrToStringAuto((IntPtr)GetRecord()->FieldName.Text) ?? string.Empty;

    /// <summary>
    /// Receiving the field record with type name
    /// </summary>
    /// <returns>Record with type name</returns>
    internal string GetTypeName()
    {
        var pTypeName = (TextMetaData*)(((Byte*)GetRecord()) +
            ((SizeT)Marshal.OffsetOf<FieldMetaData>("FieldName").ToInt64()) +
            ((SizeT)Marshal.OffsetOf<TextMetaData>("Text").ToInt64()) +
            (GetRecord()->FieldName.Size * sizeof(Char)));

        return Marshal.PtrToStringAuto((IntPtr)pTypeName->Text) ?? string.Empty;
    }

    /// <summary>
    /// Field type, for enum item it is item name
    /// </summary>
    public string TypeName => GetTypeName();

    /// <summary>
    /// Create collection of objects
    /// </summary>
    /// <param name="record">Collection pointer</param>
    /// <param name="isValidCxxTypeId">True, if CxxTypeInfo available</param>
    /// <param name="imageBase">Rtti section base</param>
    /// <returns>Rtti collection of objects</returns>
    static internal RttiCollection<RttiField> CreateCollection(Collection* record, bool isValidCxxTypeId, SizeT imageBase)
    {
        return new RttiCollection<RttiField>(record, imageBase, isValidCxxTypeId);
    }
}
