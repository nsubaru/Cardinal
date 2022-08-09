using Cardinal.RTTI.Record;
using System.Text;

namespace Cardinal.RTTI;

/// <summary>
/// This type contain raw internal rtti section
/// <remarks>
/// <para>RTTI section in executable have name .type and have next structure:</para>
/// <para> .type$a  - Magic number (<c>Cardinal::Core::RTTI::SectionMagicNumber</c> variable)</para>
/// <para> .type$b  - Section header (have all neaded information and have type <c>Cardinal.RTTI.SectionHeaderType</c></para>
/// <para> .type$fa - Start of pointer subsection (this subsection store pointers to all type records)</para>
/// <para> .type$fz - End of pointer subsection</para>
/// <para> .type$va - Start of variable subsection (this subsection store type records of internal type <c>Cardinal.RTTI.MetaData</c>)</para>
/// <para> .type$vz - End of variable subsection</para>
/// <para> .type$z  - End of rtti section</para>
/// <para> 
/// For iterating over all rtti records this class iterate over pointer subsection and using adresses of meta data records build RttiTypeRecord 
/// Before and during iterating this class convert absolut pointers adresses to current one
/// </para>
/// </remarks>
/// </summary>
public unsafe class RTTISection : IDisposable
{
    static readonly string MagicNumer = "CARDINAL_RXX_RTTI\0";

    private Byte* rawArrayPtr;
    private SizeT rawArrayLen;
    private GCHandle rawArrayPtrHandle;

    private SizeT headerOffset;
    private SizeT* fa;
    private SizeT fCount;
    private SizeT offset;
    private Boolean direction;

    private List<RttiTypeRecord> registeredRecords;
    private bool disposedValue;

    /// <summary>
    /// Load RTTI section from raw blob
    /// </summary>
    /// <param name="data">Blob with rtti section</param>
    public RTTISection(byte[] data)
    {
        rawArrayPtrHandle = GCHandle.Alloc(data, GCHandleType.Pinned);
        rawArrayPtr = (Byte*)rawArrayPtrHandle.AddrOfPinnedObject();
        rawArrayLen = (SizeT)data.Length;

        FixupAddreses();

        registeredRecords = new List<RttiTypeRecord>();
    }

    private void FixupAddreses()
    {
        var magicNumer = Encoding.ASCII.GetBytes(MagicNumer.ToCharArray());

        var magicNumberOffset = Utils.Utils.BinnarySearchInBlob(rawArrayPtr, rawArrayLen, magicNumer, 0);

        headerOffset = Utils.Utils.BinnarySearchInBlob(rawArrayPtr, rawArrayLen, magicNumer, magicNumberOffset + 1);

        if (magicNumberOffset == SizeT.MaxValue || headerOffset == SizeT.MaxValue)
            throw new InvalidDataException();

        var @base = rawArrayPtr;

        SectionHeaderType* header = (SectionHeaderType*)(rawArrayPtr + headerOffset);

        var origHeaderAddr = (SizeT)header->OriginHeaderAddr;

        bool direction = ((SizeT)@base) + magicNumberOffset < origHeaderAddr;

        SizeT offset =
            magicNumberOffset < origHeaderAddr
            ? origHeaderAddr - (((SizeT)@base) + magicNumberOffset)
            : ((((SizeT)@base) + magicNumberOffset) - origHeaderAddr);

        var fa = (SizeT*)((direction ? ((SizeT)header->FaAddr) - offset : ((SizeT)header->FaAddr) + offset));
        var fz = (SizeT*)((direction ? ((SizeT)header->FzAddr) - offset : ((SizeT)header->FzAddr) + offset));

        var count = (SizeT)(fz) - (SizeT)(fa);

        this.fa = (SizeT*)fa;
        this.fCount = count / sizeof(SizeT);
        this.offset = offset;
        this.direction = direction;
    }

    protected virtual void Dispose(bool disposing)
    {
        if (!disposedValue)
        {
            if (disposing)
            {
                // TODO: dispose managed state (managed objects)
            }

            rawArrayPtrHandle.Free();
            disposedValue = true;
        }
    }

    ~RTTISection()
    {
        Dispose(disposing: false);
    }

    public void Dispose()
    {
        Dispose(disposing: true);
        GC.SuppressFinalize(this);
    }

    void LoadAllRegisteredTypes()
    {
        var @base = this.rawArrayPtr;

        SectionHeaderType* header = (SectionHeaderType*)(@base + this.headerOffset);

        SizeT* start = this.fa + 1;

        SizeT count = 0;
        for (SizeT i = 0; i < this.fCount; i++)
        {
            if (*(this.fa + i) != 0)
                count++;
        }

        var records = new List<RttiTypeRecord>((int)count);

        SizeT j = 0;
        for (SizeT i = 0; i < count; i++, j++)
        {
            while ((SizeT)(start + j) == 0)
            {
                j++;
            }

            var relativePtrAddr = *(start + j);

            var absolutPreAddr = this.direction
                ? relativePtrAddr - this.offset
                : relativePtrAddr + this.offset;

            records.Add(new RttiTypeRecord(
                (MetaData*)absolutPreAddr,
                (SizeT)header->ImageBase,
                false));
        }

        this.registeredRecords = records;
    }

    /// <summary>
    /// Return RttiTypeRecord from current section
    /// This records have some limitation during that they don't have 
    /// </summary>
    /// <returns>Lis of </returns>
    internal List<RttiTypeRecord> GetAllRegisteredTypes()
    {
        if (this.registeredRecords.Count == 0)
            LoadAllRegisteredTypes();

        return this.registeredRecords;
    }

    /// <summary>
    /// All records stored in this rtti section, 
    /// not all records valid so ypu should check IsValid before use
    /// </summary>
    public List<RttiTypeRecord> AllRegisteredTypes => GetAllRegisteredTypes();

    /// <summary>
    /// Return kernel version with which this section was builded
    /// </summary>
    /// <returns>KernelVersion struct</returns>
    internal KernelVersionT GetKernelVersion()
    {
        var @base = this.rawArrayPtr;

        SectionHeaderType* header = (SectionHeaderType*)(@base + this.headerOffset);

        return header->KernelVersion;
    }

    /// <summary>
    /// Kernel version with which this section was builded
    /// </summary>
    public KernelVersionT KernelVersion => GetKernelVersion();
}
