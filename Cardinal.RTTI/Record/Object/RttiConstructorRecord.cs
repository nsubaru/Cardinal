namespace Cardinal.RTTI.Record.Object;

/// <summary>
/// A base RTTI constructor record class
/// </summary>
public unsafe class RttiConstructorRecord : IRttiIterableObject<RttiConstructorRecord>
{
    internal RttiIterator<RttiConstructorRecord>? it;

    Constructor* GetRecord() { return (Constructor*)it!.item; }

    /// <summary>
    /// Constructor
    /// </summary>
    public RttiConstructorRecord()
    {
        it = null;
    }

    /// <summary>
    /// Constructor
    /// </summary>
    public RttiConstructorRecord(RttiIterator<RttiConstructorRecord> it)
    {
        this.it = it;
    }

    /// <summary>
    /// Create instance from iterator
    /// </summary>
    /// <param name="iterator">Object data</param>
    /// <returns>New instance</returns>
    public RttiConstructorRecord CreateInstance(RttiIterator<RttiConstructorRecord> iterator)
    {
        return new RttiConstructorRecord(iterator);
    }

    /// <summary>
    /// Receiving the constructor record with arguments
    /// </summary>
    /// <returns>Record with arguments</returns>
    internal RttiCollection<RttiArgumentRecord> GetArguments()
    {
        var collectionBase = (Collection*)((byte*)GetRecord() + GetRecord()->OffsetToArgs);

        return RttiArgumentRecord.CreateCollection(collectionBase, it!.isValidCxxTypeId, it!.imageBase);
    }

    /// <summary>
    /// Constructor arguments
    /// </summary>
    public RttiCollection<RttiArgumentRecord> Arguments => GetArguments();

    /// <summary>
    /// Create collection of objects
    /// </summary>
    /// <param name="record">Collection pointer</param>
    /// <param name="isValidCxxTypeId">True, if CxxTypeInfo available</param>
    /// <param name="imageBase">Rtti section base</param>
    /// <returns>Rtti collection of objects</returns>
    static internal RttiCollection<RttiConstructorRecord> CreateCollection(Collection* record, bool isValidCxxTypeId, SizeT imageBase)
    {
        return new RttiCollection<RttiConstructorRecord>(record, imageBase, isValidCxxTypeId);
    }
}
