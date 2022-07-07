module;

#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

module Cardinal.Memory;

using namespace Cardinal::Core;

void PrintToBoot(wchar_t* text)
{
	UNICODE_STRING str;
	RtlInitUnicodeString(&str, text);
	NtDrawText(&str);
}

PVOID hHeap;

/// <summary>
/// Creates heap
/// </summary>
void Cardinal::Memory::InitHeap() {
	hHeap = RtlCreateHeap(HEAP_GROWABLE, 0, 0, 0, NULL, NULL);
}

/// <summary>
/// Destroys heap
/// </summary>
void Cardinal::Memory::DestroyHeap() {
	RtlDestroyHeap(hHeap);
}

/// <summary>
/// Allocates memory in heap
/// </summary>
/// <param name="size">Size of allocated memory</param>
void* Cardinal::Memory::Allocate(SizeT size) {
	return RtlAllocateHeap(hHeap, 0, size);
}

/// <summary>
/// Frees heap
/// </summary>
/// <param name="buffer">Data buffer</param>
void Cardinal::Memory::Free(void* buffer) {
	RtlFreeHeap(hHeap, 0, buffer);
}