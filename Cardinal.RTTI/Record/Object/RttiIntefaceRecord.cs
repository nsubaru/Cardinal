namespace Cardinal.RTTI.Record.Object;

/// <summary>
/// A base RTTI interface record class
/// </summary>
public unsafe class RttiIntefaceRecord : IRttiIterableObject<RttiIntefaceRecord>
{
    internal RttiIterator<RttiIntefaceRecord>? it;

    InterfaceRecord* GetRecord() { return (InterfaceRecord*)it!.item; }

    public RttiIntefaceRecord()
    {
        it = null;
    }

    /// <summary>
    /// Constructor
    /// </summary>
    public RttiIntefaceRecord(RttiIterator<RttiIntefaceRecord> it)
    {
        this.it = it;
    }

    /// <summary>
    /// Create instance from iterator
    /// </summary>
    /// <param name="iterator">Object data</param>
    /// <returns>New instance</returns>
    public RttiIntefaceRecord CreateInstance(RttiIterator<RttiIntefaceRecord> iterator)
    {
        return new RttiIntefaceRecord(iterator);
    }

    /// <summary>
    /// Interface type ID
    /// </summary>
    public SizeT TypeId => GetRecord()->TypeId.TypeId + it!.imageBase;

    /// <summary>
    /// Interface type full ID
    /// </summary>
    public UniversalTypeIdentificator FullTypeId => GetRecord()->TypeId;

    /// <summary>
    /// Interface type name
    /// </summary>
    public string TypeName => Marshal.PtrToStringAuto((IntPtr)GetRecord()->TypeName.Text) ?? string.Empty;

    /// <summary>
    /// Create collection of objects
    /// </summary>
    /// <param name="record">Collection pointer</param>
    /// <param name="isValidCxxTypeId">True, if CxxTypeInfo available</param>
    /// <param name="imageBase">Rtti section base</param>
    /// <returns>Rtti collection of objects</returns>
    static internal RttiCollection<RttiIntefaceRecord> CreateCollection(Collection* record, bool isValidCxxTypeId, SizeT imageBase)
    {
        return new RttiCollection<RttiIntefaceRecord>(record, imageBase, isValidCxxTypeId);
    }
}
