using Cardinal.CTTI;

namespace Cardinal.RTTI.Record.Object;

/// <summary>
/// A base RTTI argument record class
/// </summary>
public unsafe class RttiArgumentRecord : IRttiIterableObject<RttiArgumentRecord>
{
    internal RttiIterator<RttiArgumentRecord>? it;

    ArgumentMetaData* GetRecord() { return (ArgumentMetaData*)it!.item; }

    public RttiArgumentRecord()
    {
        it = null;
    }

    /// <summary>
    /// Constructor
    /// </summary>
    public RttiArgumentRecord(RttiIterator<RttiArgumentRecord> it)
    {
        this.it = it;
    }

    /// <summary>
    /// Create instance from iterator
    /// </summary>
    /// <param name="iterator">Object data</param>
    /// <returns>New instance</returns>
    public RttiArgumentRecord CreateInstance(RttiIterator<RttiArgumentRecord> iterator)
    {
        return new RttiArgumentRecord(iterator);
    }

    /// <summary>
    /// Receiving the argument record with type class
    /// </summary>
    /// <returns>Record with type class</returns>
    public ETypes TypeClass => GetRecord()->TypeClass;

    /// <summary>
    /// Receiving the argument record with type ID
    /// </summary>
    /// <returns>Record with type ID</returns>
    public SizeT TypeId => GetRecord()->TypeId.TypeId + it!.imageBase;

    /// <summary>
    /// Receiving the argument record with full type ID
    /// </summary>
    /// <returns>Record with full type ID</returns>
    public UniversalTypeIdentificator FullTypeId => GetRecord()->TypeId;

    /// <summary>
    /// Receiving the argument record with type name
    /// </summary>
    /// <returns>Record with type name</returns>
    public string TypeName => Marshal.PtrToStringAuto((IntPtr)GetRecord()->TypeName.Text) ?? string.Empty;

    /// <summary>
    /// Arguments type cv modificators
    /// </summary>
    public CvRefModificators Flags => GetRecord()->Flags;

    /// <summary>
    /// Argument type memory size
    /// </summary>
    public SizeT BinarySize => GetRecord()->MetaDataForDynamic.BinarySize;

    /// <summary>
    /// "True" if the record is serializable
    /// </summary>
    public bool IsSerializable => GetRecord()->IsSerializable;

    /// <summary>
    /// Create collection of objects
    /// </summary>
    /// <param name="record">Collection pointer</param>
    /// <param name="isValidCxxTypeId">True, if CxxTypeInfo available</param>
    /// <param name="imageBase">Rtti section base</param>
    /// <returns>Rtti collection of objects</returns>
    static internal RttiCollection<RttiArgumentRecord> CreateCollection(Collection* record, bool isValidCxxTypeId, SizeT imageBase)
    {
        return new RttiCollection<RttiArgumentRecord>(record, imageBase, isValidCxxTypeId);
    }
}
