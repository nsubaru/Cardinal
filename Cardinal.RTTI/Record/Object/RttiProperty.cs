namespace Cardinal.RTTI.Record.Object;

/// <summary>
/// A base RTTI property record class
/// </summary>
/// <typeparam name="THeap">Heap type</typeparam>
public unsafe class RttiProperty : IRttiIterableObject<RttiProperty>
{
    internal RttiIterator<RttiProperty>? it;

    PropertyRecord* GetRecord() { return (PropertyRecord*)it!.item; }

    /// <summary>
    /// Constructor
    /// </summary>
    public RttiProperty()
    {
        it = null;
    }

    /// <summary>
    /// Constructor
    /// </summary>
    /// <param name="from">Source</param>
    public RttiProperty(RttiIterator<RttiProperty> it)
    {
        this.it = it;
    }

    /// <summary>
    /// Create instance from iterator
    /// </summary>
    /// <param name="iterator">Object data</param>
    /// <returns>New instance</returns>
    public RttiProperty CreateInstance(RttiIterator<RttiProperty> iterator)
    {
        return new RttiProperty(iterator);
    }

    /// <summary>
    /// Property type ID
    /// </summary>
    public SizeT TypeId => (SizeT)GetRecord()->PropertyType.TypeId.TypeId + it!.imageBase;

    /// <summary>
    /// Property full type ID
    /// </summary>
    public UniversalTypeIdentificator FullTypeId => GetRecord()->PropertyType.TypeId;

    /// <summary>
    /// Property type category
    /// </summary>
    public ETypes TypeClass => GetRecord()->PropertyType.TypeClass;

    /// <summary>
    /// Property type size
    /// </summary>
    public SizeT BinarySize => GetRecord()->PropertyType.MetaDataForDynamic.BinarySize;

    /// <summary>
    /// Property type CV modificators
    /// </summary>
    public TypeModificator TypeModificators => GetRecord()->PropertyType.TypeModificators;

    /// <summary>
    /// Property name
    /// </summary>
    public string Name => Marshal.PtrToStringAuto((IntPtr)GetRecord()->PropertyType.FieldName.Text) ?? string.Empty;

    /// <summary>
    /// Receiving the property record with type name
    /// </summary>
    /// <returns>Property type name</returns>
    internal string GetTypeName()
    {
        var pTypeName = (TextMetaData*)(((Byte*)GetRecord()) +
            ((SizeT)Marshal.OffsetOf<PropertyRecord>("PropertyType").ToInt64()) +
            ((SizeT)Marshal.OffsetOf<FieldMetaDataRecord>("FieldName").ToInt64()) +
            ((SizeT)Marshal.OffsetOf<TextMetaData>("Text").ToInt64()) +
            (GetRecord()->PropertyType.FieldName.Size * sizeof(Char)));

        return Marshal.PtrToStringAuto((IntPtr)pTypeName->Text) ?? string.Empty;
    }

    /// <summary>
    /// Property type name
    /// </summary>
    public string TypeName => GetTypeName();

    /// <summary>
    /// Receiving the property record with getters
    /// </summary>
    /// <returns>Record with getters</returns>
    internal RttiCollection<RttiMethodRecord> GetGetters()
    {
        var @base = (Collection*)(((Byte*)GetRecord()) + GetRecord()->OffsetToGetters);

        return RttiMethodRecord.CreateCollection(@base, it!.isValidCxxTypeId, it!.imageBase);
    }

    /// <summary>
    /// Property getters methods
    /// </summary>
    public RttiCollection<RttiMethodRecord> Getters => GetGetters();

    /// <summary>
    /// Receiving the property record with setters
    /// </summary>
    /// <returns>Record with setters</returns>
    internal RttiCollection<RttiMethodRecord> GetSetters()
    {
        var @base = (Collection*)(((Byte*)GetRecord()) + GetRecord()->OffsetToSetters);

        return RttiMethodRecord.CreateCollection(@base, it!.isValidCxxTypeId, it!.imageBase);
    }

    /// <summary>
    /// Property setters methods
    /// </summary>
    public RttiCollection<RttiMethodRecord> Setters => GetSetters();

    /// <summary>
    /// Indicates if the property is rw
    /// </summary>
    /// <returns>"True" if the record is rw</returns>
    internal bool GetIsRw()
    {
        var gettersRw = (Collection*)(((Byte*)GetRecord()) + GetRecord()->OffsetToGetters);
        var settersRw = (Collection*)(((Byte*)GetRecord()) + GetRecord()->OffsetToSetters);

        return gettersRw->Count > 0 && settersRw->Count > 0;
    }

    /// <summary>
    /// If true property support read and write
    /// </summary>
    public bool IsRw => GetIsRw();

    /// <summary>
    /// Create collection of objects
    /// </summary>
    /// <param name="record">Collection pointer</param>
    /// <param name="isValidCxxTypeId">True, if CxxTypeInfo available</param>
    /// <param name="imageBase">Rtti section base</param>
    /// <returns>Rtti collection of objects</returns>
    static internal RttiCollection<RttiProperty> CreateCollection(Collection* record, bool isValidCxxTypeId, SizeT imageBase)
    {
        return new RttiCollection<RttiProperty>(record, imageBase, isValidCxxTypeId);
    }
}
