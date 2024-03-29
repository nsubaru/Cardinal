export module Cardinal.Memory:Base;

export import Cardinal.Core;
export import Cardinal.Memory.DataSizeT;

#pragma warning(push)
#pragma warning(disable: 4996)

export namespace Cardinal::Memory
{
#pragma region Miscenelious memory function
	/// <summary>
	/// Area of memory copying
	/// </summary>
	/// <param name='source'>Copying area of memory</param>
	/// <param name='destination'>Area of memory in which in copying</param>
	/// <param name='size'>Number of bytes which is need to copy</param>
	DeprecatedCall(L"Low-level work with memory") void Copy(
		const void* Restrict source,
		void* Restrict destination,
		SizeT size);

	/// <summary>
	/// Moving data from source to destination
	/// </summary>
	/// <param name='source'>Copying area of memory</param>
	/// <param name='destination'>Area of memory in which in copying</param>
	/// <param name='size'>Number of bytes which is need to copy</param>
	DeprecatedCall(L"Low-level work with memory") void Move(
		const void* source,
		void* destination,
		SizeT size);

	/// <summary>
	/// Fills the memory block by specified values
	/// </summary>
	/// <param name='memBlk'>Memory block</param>
	/// <param name='size'>The memory area size in bytes</param>
	/// <param name='filler'>A value which fills the memory area</param>
	DeprecatedCall(L"Low-level work with memory") void MemBlkFill(
		void* memBlk,
		SizeT size,
		const Byte filler);

	/// <summary>
	/// Fills the memory block by zero
	/// </summary>
	/// <param name='memBlk'>Memory block</param>
	/// <param name='size'>The memory area size in bytes</param>
	DeprecatedCall(L"Low-level work with memory") void SecureMemBlkZero(
		void* memBlk,
		SizeT size);

	/// <summary>
	/// Area of memory copying
	/// </summary>
	/// <param name="source">Copying area of memory</param>
	/// <param name="destination">Area of memory in which in copying</param>
	template<Concepts::IsPod T, SizeT count>
	ForceInline void Copy(const T(&source)[count], T(&destination)[count]) {
		Copy(source, destination, count * sizeof(T));
	}

	/// <summary>
	/// Guarantee object nulling
	/// </summary>
	/// <param name="target">The object</param>
	template<Concepts::IsPod T>
	ForceInline void ZeroMemory(T& target) {
		MemBlkFill((void*) &target, sizeof(T), 0);
	}

	/// <summary>
	/// Guarantee object nulling
	/// </summary>
	/// <param name="target">The object</param>
	template<Concepts::IsPod T>
	ForceInline void SecureZeroMemory(T& target) {
		SecureMemBlkZero(static_cast<void*>(&target), sizeof(T));
	}

	/// <summary>
	/// Guarantee array of objects nulling
	/// </summary>
	/// <param name="target">Array of objects</param>
	template<Concepts::IsPod T, SizeT count>
	ForceInline void SecureZeroMemory(T(&target)[count]) {
		SecureMemBlkZero(static_cast<void*>(&target), sizeof(T) * count);
	}
#pragma endregion
#pragma warning(pop)
}