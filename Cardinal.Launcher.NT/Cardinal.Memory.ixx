export module Cardinal.Memory;

import Cardinal.Core;

export namespace Cardinal::Memory
{
	void InitHeap();
	void DestroyHeap();
}

using namespace Cardinal::Core;

namespace Cardinal::Memory 
{
	void* Allocate(SizeT size);
	void Free(void* buffer);
}

#pragma warning(push)
#pragma warning(disable: 4595)

export;

/// <summary>
/// Memory allocation operator
/// </summary>
/// <param name = "Size">Memory block size</param>
/// <returns>Memory block</returns>
[[nodiscard]]
__declspec(restrict) inline void* __cdecl operator new(SizeT size) {
	return Cardinal::Memory::Allocate(size);
}

/// <summary>
/// Array of objects allocation operator
/// </summary>
/// <param name = "Size">Array of objects size</param>
/// <returns>Memory block</returns>
[[nodiscard]]
__declspec(restrict) inline void* __cdecl operator new[](SizeT size) {
	return Cardinal::Memory::Allocate(size);
}

/// <summary>
/// Single object deletion operator
/// </summary>
/// <param name = "memBlk">Memory block</param>
inline void __cdecl operator delete(void* __restrict memBlk) {
	Cardinal::Memory::Free(memBlk);
}

/// <summary>
/// Single object deletion operator
/// </summary>
/// <param name = "memBlk">Memory block</param>
/// <param name = "size">Memory block size</param>
inline void __cdecl operator delete(void* __restrict memBlk, UnusedData SizeT size) {
	Cardinal::Memory::Free(memBlk);
}

/// <summary>Array of objects deletion operator</summary>
/// <param name = "memBlk">Memory block</param>
inline void __cdecl operator delete[](void* __restrict memBlk) {
	Cardinal::Memory::Free(memBlk);
}

/// <summary>Array of objects deletion operator</summary>
/// <param name = "memBlk">Memory block</param>
/// <param name = "size">Memory block size</param>
inline void __cdecl operator delete[](void* __restrict memBlk, UnusedData SizeT size) {
	Cardinal::Memory::Free(memBlk);
}

#pragma warning(pop)