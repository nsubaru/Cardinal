export module Cardinal.Memory:Allocators;

import :Base;

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 4595)

export namespace Cardinal::Memory
{
#pragma region Heaps classes
	/// <summary>
	/// Base heap type
	/// </summary>
	VirtualClass BasicHeap {
	public:
		ForceInline virtual ~BasicHeap() {};

		/// <summary>
		/// Memory allocation
		/// </summary>
		/// <param name='Size'>The area size in bytes</param>
		/// <returns>Allocated area</returns>
		virtual DeprecatedCall(L"Low-level work with memory") void* Allocate(DataSizeT Size) = 0;

		/// <summary>
		/// Aligned memory allocation
		/// </summary>
		/// <param name='Size'>The area size in bytes</param>
		/// <param name='AllocationReq'>Alignment borders</param>
		/// <returns>Allocated area</returns>
		virtual DeprecatedCall(L"Low-level work with memory") void* AllignedAllocate(DataSizeT Size, DataSizeT AllocationReq) = 0;

		/// <summary>
		/// Memory freeing
		/// </summary>
		/// <param name='memBlk'>The freeing memory block</param>
		virtual DeprecatedCall(L"Low-level work with memory") void* Free(void* memBlk) = 0;

		/// <summary>
		/// Memory freeing which was allocated along the border
		/// </summary>
		/// <param name='memBlk'>The freeing memory block</param>
		/// <param name='AllocationReq'>Alignment borders</param>
		virtual DeprecatedCall(L"Low-level work with memory") void* AllignedFree(void* memBlk, DataSizeT AllocationReq) = 0;

		/// <summary>
		/// The number of allocated memory blocks
		/// </summary>
		virtual SizeT CurrentMemBlkCount() = 0;

		/// <summary>
		/// The size of allocated memory in bytes
		/// </summary>
		virtual DataSizeT CurrentAllocatedMemory() = 0;

		/// <summary>
		/// The actual size of allocated memory in bytes
		/// </summary>
		virtual DataSizeT CurrentHeapUsage() = 0;
	};

	/// <summary>
	/// Generic heap
	/// </summary>
	VirtualClass Heap final : public BasicHeap{
	public:
		/// <summary>
		/// Standard constructor
		/// </summary>
		ForceInline Heap() = default;

		/// <summary>
		/// Standard copying operator
		/// </summary>
		ForceInline Heap& operator=(const Heap&) = default;

		/// <summary>
		/// Standard destructor
		/// </summary>
		ForceInline ~Heap() override = default;

	private:
		/// <summary>
		/// Heap initialization
		/// </summary>
		/// <returns>"True" if the initialization was successful</returns>
		static Boolean Init() noexcept;

		/// <summary>
		/// Heap stopping
		/// </summary>
		static void Stop() noexcept;

		/// <summary>
		/// Heap initialization
		/// </summary>
		/// <returns>"True" if the initialization was successful</returns>
		friend extern Boolean Init();

		/// <summary>
		/// Heap stopping
		/// </summary>
		friend extern void Stop();

	public:
		/// <summary>
		/// Memory allocation
		/// </summary>
		/// <param name='Size'>The area size in bytes</param>
		/// <returns>Allocated area</returns>
		virtual __declspec(restrict) DeprecatedCall(L"Low-level work with memory") void* Allocate(DataSizeT Size) override final;

		/// <summary>
		/// Aligned memory allocation
		/// </summary>
		/// <param name='Size'>The area size in bytes</param>
		/// <param name='AllocationReq'>Alignment borders</param>
		/// <returns>Allocated area</returns>
		virtual __declspec(restrict) DeprecatedCall(L"Low-level work with memory") void* AllignedAllocate(DataSizeT Size, DataSizeT AllocationReq) override final;

		/// <summary>
		/// Memory freeing
		/// </summary>
		/// <param name='memBlk'>The freeing memory block</param>
		virtual DeprecatedCall(L"Low-level work with memory") void* Free(void* Restrict memBlk) override final;

		/// <summary>
		/// Memory freeing which was allocated on the border
		/// </summary>
		/// <param name='memBlk'>The freeing memory block</param>
		/// <param name='AllocationReq'>Alignment borders</param>
		virtual DeprecatedCall(L"Low-level work with memory") void* AllignedFree(void* Restrict memBlk, DataSizeT AllocationReq) override final;

		/// <summary>
		/// The number of allocated memory blocks
		/// </summary>
		virtual SizeT CurrentMemBlkCount() override final;

		/// <summary>
		/// The size of allocated memory in bytes
		/// </summary>
		virtual DataSizeT CurrentAllocatedMemory() override final;

		/// <summary>
		/// The actual size of allocated memory in bytes
		/// </summary>
		virtual DataSizeT CurrentHeapUsage();
	};

	/// <summary>
	/// Sensitive heap
	/// </summary>
	/// <remarks>Memory is securely zeroing before allocation and after freeing</remarks>
	VirtualClass SensitiveDataHeap final : public BasicHeap {
	public:
		/// <summary>
		/// Standard constructor
		/// </summary>
		ForceInline SensitiveDataHeap() = default;

		/// <summary>
		/// Standard copying operator
		/// </summary>
		ForceInline SensitiveDataHeap& operator=(const SensitiveDataHeap&) = default;

		/// <summary>
		/// Standard destructor
		/// </summary>
		ForceInline ~SensitiveDataHeap() override = default;

	private:
		/// <summary>
		/// Heap initialization
		/// </summary>
		/// <returns>"True" if the initialization was successful</returns>
		static Boolean Init() noexcept;

		/// <summary>
		/// Heap stopping
		/// </summary>
		static void Stop() noexcept;

		/// <summary>
		/// Heap initialization
		/// </summary>
		/// <returns>"True" if the initialization was successful</returns>
		friend extern Boolean Init();

		/// <summary>
		/// Heap stopping
		/// </summary>
		friend extern void Stop();

	public:
		/// <summary>
		/// Memory allocation
		/// </summary>
		/// <param name='Size'>The area size in bytes</param>
		/// <returns>Allocated area</returns>
		virtual __declspec(restrict) DeprecatedCall(L"Low-level work with memory") void* Allocate(DataSizeT Size) override final;

		/// <summary>
		/// Aligned memory allocation
		/// </summary>
		/// <param name='Size'>The area size in bytes</param>
		/// <param name='AllocationReq'>Alignment borders</param>
		/// <returns>Allocated area or "nullptr"</returns>
		virtual __declspec(restrict) DeprecatedCall(L"Low-level work with memory") void* AllignedAllocate(DataSizeT Size, DataSizeT AllocationReq) override final;

		/// <summary>
		/// Memory freeing
		/// </summary>
		/// <param name='memBlk'>The freeing memory block</param>
		virtual DeprecatedCall(L"Low-level work with memory") void* Free(void* Restrict memBlk) override final;

		/// <summary>
		/// Memory freeing which was allocated along the border
		/// </summary>
		/// <param name='memBlk'>The freeing memory block</param>
		/// <param name='AllocationReq'>Alignment borders</param>
		virtual DeprecatedCall(L"Low-level work with memory") void* AllignedFree(void* Restrict memBlk, DataSizeT AllocationReq) override final;

		/// <summary>
		/// The number of allocated memory blocks
		/// </summary>
		virtual SizeT CurrentMemBlkCount() override final;

		/// <summary>
		/// The size of allocated memory in bytes
		/// </summary>
		virtual DataSizeT CurrentAllocatedMemory() override final;

		/// <summary>
		/// The actual size of allocated memory in bytes
		/// </summary>
		virtual DataSizeT CurrentHeapUsage() override final;
	};
#pragma endregion

#pragma region "Allocator class"
	/// <summary>
	/// Allocator class
	/// <para>Responsible for allocating and freeing the memory</para>
	/// </summary>
	template<class HeapClass>
	VirtualClass Allocator {
		static_assert(TypeTraits::IsBasedOn<BasicHeap, HeapClass>(), L"HeapClass in not heap");
	private:
		/// <value>
		/// A heap from which the allocation will be occured
		/// </value>
		inline static HeapClass heap;

	public:
		/// <summary>
		/// Static allocator data constructor
		/// (copying and moving operators and destructor is not needed)
		/// </summary>
		constexpr ForceInline Allocator() = default;

		/// <summary>
		/// Static allocator data constructor
		/// (copying and moving operators and destructor is not needed)
		/// </summary>
		constexpr ForceInline Allocator(const Allocator&) = default;

		/// <summary>
		/// Static allocator data constructor
		/// (copying and moving operators and destructor is not needed)
		/// </summary>
		constexpr ForceInline Allocator(Allocator&&) = default;

		/// <summary>
		/// Static allocator data constructor
		/// (copying and moving operators and destructor is not needed)
		/// </summary>
		constexpr Allocator& operator=(const Allocator&) = default;

		/// <summary>
		/// Static allocator data constructor
		/// (copying and moving operators and destructor is not needed)
		/// </summary>
		constexpr Allocator& operator=(Allocator&&) = default;

		/// <summary>
		/// Static allocator data constructor
		/// (copying and moving operators and destructor is not needed)
		/// </summary>
		constexpr ForceInline ~Allocator() = default;

		/// <summary>
		/// Memory allocation operator
		/// </summary>
		/// <param name = "Size">Memory block size</param>
		/// <returns>Returns the memory block</returns>
		static ForceInline void* operator new(SizeT Size) {
			return heap.Allocate(DataSizeT(Size));
		}

		/// <summary>
		/// Array of objects allocation operator
		/// </summary>
		/// <param name = "Size">Array of objects size</param>
		/// <returns>Returns the memory block</returns>
		static ForceInline void* operator new[] (SizeT Size) {
			return heap.Allocate(DataSizeT(Size));
		}

		/// <summary>
		/// Allocation operator of single object with specific alignment
		/// </summary>
		/// <param name = "Size">Memory block size</param>
		/// <param name = "Allign">Alignment borders</param>
		/// <returns>Returns the memory block</returns>
		static ForceInline void* operator new(Cardinal::SizeT Size, Cardinal::AllignRequirements Allign) {
			return heap.AllignedAllocate(DataSizeT(Size), DataSizeT(static_cast<Cardinal::SizeT>(Allign)));
		}

		/// <summary>
		/// Array of objects allocation operator with specific alignment
		/// </summary>
		/// <param name = "Size">Memory block size</param>
		/// <param name = "Allign">Alignment borders</param>
		/// <returns>Returns the memory block</returns>
		static ForceInline void* operator new[] (Cardinal::SizeT Size, Cardinal::AllignRequirements Allign) {
			return heap.AllignedAllocate(DataSizeT(Size), DataSizeT(static_cast<Cardinal::SizeT>(Allign)));
		}

		/// <summary>
		/// Deletion operator of single object
		/// </summary>
		/// <param name = "memBlk">Area of memory</param>
		static ForceInline void operator delete(void* memBlk) {
			heap.Free(memBlk);
		}

		/// <summary>
		/// Deletion operator of array of objects
		/// </summary>
		/// <param name = "memBlk">Area of memory</param>
		static ForceInline void operator delete[] (void* memBlk) {
			heap.Free(memBlk);
		}

		/// <summary>
		/// Deletion operator of single object with specific alignment
		/// </summary>
		/// <param name = "memBlk">Area of memory</param>
		/// <param name = "Allign">Alignment borders</param>
		static ForceInline void __cdecl operator delete(void* memBlk, Cardinal::AllignRequirements Allign) {
			heap.AllignedFree(memBlk, DataSizeT(static_cast<SizeT>(Allign)));
		}

		/// <summary>
		/// Deletion operator of array of objects with specific alignment
		/// </summary>
		/// <param name = "memBlk">Area of memory</param>
		/// <param name = "Allign">Alignment borders</param>
		static ForceInline void __cdecl operator delete[] (void* memBlk, Cardinal::AllignRequirements Allign) {
			heap.AllignedFree(memBlk, DataSizeT(static_cast<SizeT>(Allign)));
		}

#pragma region Placement New
		/// <summary>
		/// The "new" operator with the location of object in existing memory block
		/// </summary>
		/// <param name="size">Size of object</param>
		/// <param name="memBlk">A buffer for object</param>
		static ForceInline void* __cdecl operator new(Cardinal::SizeT size, void* memBlk) noexcept {
			(void) size;
			return memBlk;
		}

		/// <summary>
		/// The "delete" operator for the hosted object
		/// </summary>
		static ForceInline void __cdecl operator delete(void*, void*) noexcept {
			return;
		}

		/// <summary>
		/// The "new" operator with the location of object in existing memory block
		/// </summary>
		/// <param name="size">Size of object</param>
		/// <param name="memBlk">A buffer for object</param>
		static ForceInline void* __cdecl operator new[] (Cardinal::SizeT size, void* memBlk) noexcept {
			(void) size;
			return memBlk;
		}

		/// <summary>
		/// The "delete" operator for the hosted object
		/// </summary>
		static ForceInline void __cdecl operator delete[] (void*, void*) noexcept {
		}
	#pragma endregion 
	};
#pragma endregion

#pragma region Memory statistic
	/// <summary>
	/// The total number of allocated memory blocks in the internal runtime heap
	/// </summary>
	SizeT CurrentInternalHeapAllocatedMemBlks();

	/// <summary>
	/// The total number of allocated memory in the internal runtime heap
	/// </summary>
	DataSizeT CurrentInternalHeapAllocatedMemory();

	/// <summary>
	/// The total memory using in the internal runtime heap
	/// </summary>
	DataSizeT CurrentInternalHeapHeapsUsage();

	/// <summary>
	/// The total number of allocated memory blocks in all managed heaps
	/// </summary>
	SizeT CurrentTotalAllocatedMemBlks();

	/// <summary>
	/// The total number of allocated memory in all managed heaps
	/// </summary>
	DataSizeT CurrentTotalAllocatedMemory();

	/// <summary>
	/// The total memory using in all managed heaps
	/// </summary>
	DataSizeT CurrentTotalHeapsUsage();
#pragma endregion
}

#pragma region Default new and delete operators
#pragma warning(push)
#pragma warning(disable: 5043)
#pragma warning(disable: 4595)

/// <summary>Memory allocation operator</summary>
/// <param name = "Size">Memory block size</param>
/// <returns>Returns the memory block</returns>
[[nodiscard]]
__declspec(restrict) inline void* __cdecl operator new(Cardinal::SizeT Size) {
	return Cardinal::Memory::Allocator<Cardinal::Memory::Heap>::operator new(Size);
}

/// <summary>Array of objects allocation operator</summary>
/// <param name = "Size">Array of objects size</param>
/// <returns>Returns the memory block</returns>
export [[nodiscard]]
__declspec(restrict) inline void* __cdecl operator new[] (Cardinal::SizeT Size) {
	return Cardinal::Memory::Allocator<Cardinal::Memory::Heap>::operator new[](Size);
}

/// <summary>Allocation operator of single object with specific alignment</summary>
/// <param name = "Size">Memory block size</param>
/// <param name = "Allign">Alignment borders</param>
/// <returns>Returns the memory block</returns>
export [[nodiscard]]
__declspec(restrict) inline void* __cdecl operator new(Cardinal::SizeT Size, Cardinal::AllignRequirements Allign) {
	return Cardinal::Memory::Allocator<Cardinal::Memory::Heap>::operator new(Size, Allign);
}

/// <summary>Array of objects allocation operator with specific alignment</summary>
/// <param name = "Size">Memory block size</param>
/// <param name = "Allign">Alignment borders</param>
/// <returns>Returns the memory block</returns>
export [[nodiscard]]
__declspec(restrict) inline void* __cdecl operator new[] (Cardinal::SizeT Size, Cardinal::AllignRequirements Allign) {
	return Cardinal::Memory::Allocator<Cardinal::Memory::Heap>::operator new[](Size, Allign);
}

/// <summary>Deletion operator of single object</summary>
/// <param name = "memBlk">Area of memory</param>
export inline void __cdecl operator delete(void* Restrict memBlk) {
	Cardinal::Memory::Allocator<Cardinal::Memory::Heap>::operator delete(memBlk);
}

/// <summary>Deletion operator of array of objects</summary>
/// <param name = "memBlk">Area of memory</param>
export inline void __cdecl operator delete[] (void* Restrict memBlk) {
	Cardinal::Memory::Allocator<Cardinal::Memory::Heap>::operator delete[](memBlk);
}

/// <summary>Deletion operator of single object with specific alignment</summary>
/// <param name = "memBlk">Area of memory</param>
/// <param name = "Allign">Alignment borders</param>
export inline void __cdecl operator delete(void* Restrict memBlk, Cardinal::AllignRequirements Allign) {
	Cardinal::Memory::Allocator<Cardinal::Memory::Heap>::operator delete(memBlk, Allign);
}

/// <summary>Deletion operator of array of objects with specific alignment</summary>
/// <param name = "memBlk">Area of memory</param>
/// <param name = "Allign">Alignment borders</param>
export inline void __cdecl operator delete[] (void* Restrict memBlk, Cardinal::AllignRequirements Allign) {
	Cardinal::Memory::Allocator<Cardinal::Memory::Heap>::operator delete[](memBlk, Allign);
}

/// <summary>Deletion operator of single object with the object size transfer</summary>
/// <param name = "memBlk">Area of memory</param>
/// <param name = "Size">Memory block size</param>
export inline void __cdecl operator delete(void* Restrict memBlk, [[maybe_unused]] Cardinal::SizeT Size) {
	Cardinal::Memory::Allocator<Cardinal::Memory::Heap>::operator delete(memBlk);
}

/// <summary>Deletion operator of array of objects with the object size transfer</summary>
/// <param name = "memBlk">Area of memory</param>
/// <param name = "Size">Memory block size</param>
export inline void __cdecl operator delete[] (void* Restrict memBlk, [[maybe_unused]] Cardinal::SizeT Size) {
	Cardinal::Memory::Allocator<Cardinal::Memory::Heap>::operator delete[](memBlk);
}

/// <summary>Deletion operator of single object with specific alignment and size</summary>
/// <param name = "memBlk">Area of memory</param>
/// <param name = "Allign">Alignment borders</param>
/// <param name = "Size">Memory block size</param>
export inline void __cdecl operator delete(void* Restrict memBlk, [[maybe_unused]] Cardinal::SizeT Size, Cardinal::AllignRequirements Allign) {
	Cardinal::Memory::Allocator<Cardinal::Memory::Heap>::operator delete(memBlk, Allign);
}

/// <summary>Deletion operator of array of objects with specific alignment and size</summary>
/// <param name = "memBlk">Area of memory</param>
/// <param name = "Allign">Alignment borders</param>
/// <param name = "Size">Memory block size</param>
export inline void __cdecl operator delete[] (void* Restrict memBlk, [[maybe_unused]] Cardinal::SizeT Size, Cardinal::AllignRequirements Allign) {
	Cardinal::Memory::Allocator<Cardinal::Memory::Heap>::operator delete[](memBlk, Allign);
}

#pragma warning(push)
#pragma warning(disable: 4577) // 'noexcept' used with no exception handling mode specified
#pragma warning(disable: 4514) // 'operator new': unreferenced inline function has been removed

/// <summary>
/// The "new" operator with the location of object in existing memory block
/// </summary>
/// <param name="size">Size of object</param>
/// <param name="memBlk">A buffer for object</param>
export [[nodiscard]]
__declspec(restrict)
inline void* __cdecl operator new(Cardinal::SizeT size, void* Restrict memBlk) noexcept {
	(void) size;
	return memBlk;
}

/// <summary>
/// The "delete" operator for the hosted object
/// </summary>
export inline void __cdecl operator delete(void*, void*) noexcept {
	return;
}

/// <summary>
/// The "new" operator with the location of object in existing memory block
/// </summary>
/// <param name="size">Size of object</param>
/// <param name="memBlk">A buffer for object</param>
export [[nodiscard]]
__declspec(restrict)
inline void* __cdecl operator new[] (Cardinal::SizeT size, void* Restrict memBlk) noexcept {
	(void) size;
	return memBlk;
}

/// <summary>
/// The "delete" operator for the hosted object
/// </summary>
export inline void __cdecl operator delete[] (void*, void*) noexcept {}

#pragma warning(pop)

#pragma warning(pop)
#pragma endregion
#pragma warning(pop)