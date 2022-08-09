using System.Collections;

namespace Cardinal.RTTI.Record;

/// <summary>
/// Iterable object for RTTI iterator
/// </summary>
/// <typeparam name="T">Item type</typeparam>
public interface IRttiIterableObject<T> where T : class, IRttiIterableObject<T>, new()
{
    /// <summary>
    /// Create instance from iterator
    /// </summary>
    /// <param name="iterator">Object data</param>
    /// <returns>New instance</returns>
    internal T CreateInstance(RttiIterator<T> iterator);
}

/// <summary>
/// A base iterator class
/// </summary>
/// <typeparam name="THeap">Heap type</typeparam>
/// <typeparam name="T">Item type</typeparam>
public unsafe class RttiIterator<T> : IEnumerator<T>
    where T : class, IRttiIterableObject<T>, new()
{
    internal ItemBase* item;
    internal ItemBase* originalItem;
    internal SizeT idx;
    internal SizeT count;
    internal bool isValidCxxTypeId;
    internal SizeT imageBase;
    internal bool isStartedEnumeration;

    /// <summary>
    /// Iterator constructor
    /// </summary>
    /// <param name="item">Pointer to item value</param>
    /// <param name="idx">Iterator item odinal number</param>
    /// <param name="count">Items count in collection</param>
    /// <param name="imageBase">RTTI section image base</param>
    /// <param name="isValidCxxTypeId">True, if Cxx type info available</param>
    internal RttiIterator(void* item, SizeT idx, SizeT count, SizeT imageBase, bool isValidCxxTypeId)
    {
        this.originalItem = this.item = (ItemBase*)item;
        this.idx = idx;
        this.count = count;
        this.imageBase = imageBase;
        this.isValidCxxTypeId = isValidCxxTypeId;
        this.isStartedEnumeration = false;
    }

    /// <summary>
    /// Return current item
    /// </summary>
    /// <returns>Current item</returns>
    private T GetObject()
    {
        return new T().CreateInstance(this);
    }

    /// <summary>
    /// Current item
    /// </summary>
    T IEnumerator<T>.Current { get => GetObject(); }

    /// <summary>
    /// Current item
    /// </summary>
    object IEnumerator.Current { get => GetObject(); }

    /// <summary>
    /// Move iterator to next item
    /// </summary>
    /// <returns>True, if iterator moved to next item</returns>
    public bool MoveNext()
    {
        if (item == null || idx == count)
        {
            item = null;
            return false;
        }

        if (!isStartedEnumeration)
        {
            isStartedEnumeration = true;
            return true;
        }

        idx++;

        if (idx == count)
        {
            item = null;
            return false;
        }
        else
        {
            item = (ItemBase*)((byte*)item + item->SizeOfItem);
            return true;
        }

    }

    /// <summary>
    /// Move iterator to first item
    /// </summary>
    public void Reset()
    {
        idx = 0;
        item = originalItem;
        isStartedEnumeration = false;
    }

    public void Dispose() { }
}
