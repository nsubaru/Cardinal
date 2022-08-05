using System.Collections;

namespace Cardinal.RTTI.Record;

/// <summary>
/// A base RTTI field class
/// </summary>
/// <typeparam name="THeap">Heap type</typeparam>
/// <typeparam name="T"></typeparam>
public unsafe class RttiCollection<T> : IEnumerable<T> where T : class, IRttiIterableObject<T>, new()
{
    internal Collection* @base;
    internal SizeT imageBase;
    internal bool isValidCxxTypeId;

    /// <summary>
    /// Constructor
    /// </summary>
    internal RttiCollection(Collection* @base, ulong imageBase, bool isValidCxxTypeId)
    {
        this.@base = @base;
        this.imageBase = imageBase;
        this.isValidCxxTypeId = isValidCxxTypeId;
    }

    /// <summary>
    /// Receiving the iterator to collection
    /// </summary>
    /// <returns>Iterator to collection</returns>
    public IEnumerator<T> GetEnumerator()
    {
        if (@base != null && @base->Count != 0)
        {
            return new RttiIterator<T>(
                (byte*)@base + @base->OffsetToFirstItem,
                0,
                @base->Count,
                imageBase,
                isValidCxxTypeId);
        }
        else
        {
            return new RttiIterator<T>(null, 0, 0, 0, isValidCxxTypeId);
        }
    }

    /// <summary>
    /// Receiving the iterator to collection
    /// </summary>
    /// <returns>Iterator to collection</returns>
    IEnumerator IEnumerable.GetEnumerator()
    {
        return GetEnumerator();
    }

    /// <summary>
    /// Recieving the count of collection items
    /// </summary>
    /// <returns>Count of collection items</returns>
    public SizeT Count => @base == null ? 0 : @base->Count;
}
