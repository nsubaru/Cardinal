export module Cardinal.Exceptions:OutOfMemory;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// Not enough RAM for the operation
	/// </summary>
	class OutOfMemory final : public Exception {
	private:
		/// <summary>
		/// Heap name
		/// </summary>
		ConstString HeapName;

		/// <summary>
		/// The number of bytes which are tried to allocate
		/// </summary>
		SizeT TryAllocate;

		/// <summary>
		/// The number of bytes used before allocation
		/// </summary>
		SizeT AllocatedBefore;

		/// <summary>
		/// A number of elements used before allocation
		/// </summary>
		SizeT MemBlkscountBefore;

	public:
		ForceInline OutOfMemory(const OutOfMemory& orig) :
			Exception(orig),
			HeapName(orig.HeapName),
			TryAllocate(orig.TryAllocate),
			AllocatedBefore(orig.AllocatedBefore),
			MemBlkscountBefore(orig.MemBlkscountBefore) {
		}

		ForceInline OutOfMemory(OutOfMemory&& orig) :
			Exception(MoveRValue(orig)),
			HeapName(orig.HeapName),
			TryAllocate(orig.TryAllocate),
			AllocatedBefore(orig.AllocatedBefore),
			MemBlkscountBefore(orig.MemBlkscountBefore) {
		}

		ForceInline OutOfMemory& operator=(const OutOfMemory& from) { this->~OutOfMemory(); this->OutOfMemory::OutOfMemory(from); return *this; }
		ForceInline OutOfMemory& operator=(OutOfMemory&& from) { this->~OutOfMemory(); this->OutOfMemory::OutOfMemory(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		/// <param name = 'HeapName'>Low cased heap name</param>
		/// <param name = 'TryAllocate'>The number of bytes which are tried to allocate</param>
		/// <param name = 'AllocatedBefore'>The number of bytes which was allocated at the time of exception generation</param>
		/// <param name = 'MemBlksCountBefore'>The number of blocks which was allocated at the time of exception generation</param>
		OutOfMemory(
			SourceLocation sourceLocation,
			const ConstString& HeapName,
			SizeT TryAllocate,
			SizeT AllocatedBefore,
			SizeT MemBlkscountBefore
		) :
			Exception(sourceLocation, L"Not enough RAM for the operation"),
			TryAllocate(TryAllocate), AllocatedBefore(AllocatedBefore), MemBlkscountBefore(MemBlkscountBefore), HeapName(HeapName) {
		}

		/// <summary>
		/// Returns the heap name in which was tried to allocate the memory
		/// </summary>
		ForceInline const ConstString& GetHeapName() const { return HeapName; }

		/// <summary>
		/// Returns the number of bytes which was tried to allocate
		/// </summary>
		ForceInline SizeT GetRequiredBytes() const { return TryAllocate; }

		/// <summary>
		/// Returns the number of bytes which was allocated at the moment of exception generation
		/// </summary>
		ForceInline SizeT GetAllocatedBytes() const { return AllocatedBefore; }

		/// <summary>
		/// Returns the number of memory blocks which was allocated at the moment of exception generation
		/// </summary>
		ForceInline SizeT GetAllocatedMemBlksCount() const { return MemBlkscountBefore; }

	public:
		/// <summary>
		/// The heap name in which was tried to allocate the memory
		/// </summary>
		ConstString Property(GetHeapName) HeapName;

		/// <summary>
		/// The number of bytes which was tried to allocate
		/// </summary>
		SizeT Property(GetRequiredBytes) RequiredBytes;

		/// <summary>
		/// The number of bytes which was allocated at the moment of exception generation
		/// </summary>
		SizeT Property(GetAllocatedBytes) AllocatedBytes;

		/// <summary>
		/// The number of memory blocks which was allocated at the moment of exception generation
		/// </summary>
		SizeT Property(GetAllocatedMemBlksCount) AllocatedMemBlksCount;

	public:
		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override final {
			return ExceptionString::Combine({
				ExceptionString(exceptionName),
				ExceptionString(L" in "), ExceptionString(HeapName), ExceptionString(L" heap"),
				ExceptionString(L" during the allocation "), ExceptionString::toString(TryAllocate),
				ExceptionString(L" bytes, at the moment of exception occuring was allocated "),
				ExceptionString::toString(AllocatedBefore), ExceptionString(L" bytes in "),
				ExceptionString::toString(MemBlkscountBefore),
				ExceptionString((MemBlkscountBefore == 1) ? ExceptionString(L" memory block. ") : (L"memory blocks. ")),
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new OutOfMemory(*this));
		}

	public:
		ForceInline virtual ~OutOfMemory() {}
	};
}