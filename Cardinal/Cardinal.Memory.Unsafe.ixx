export module Cardinal.Memory:Unsafe;

import :Base;

export namespace Cardinal::Memory::UnSafe
{
	/// <summary>
	/// Concept for types which allow to use as offset in memory
	/// </summary>
	template<typename T>
	concept AddressOffsetType = TypeTraits::IsIntegerNumber<T> || TypeTraits::IsSameV<T, DataSizeT>;

	/// <summary>
	/// Extract integer type for AddOffset or AddOffsetAs
	/// </summary>
	/// <typeparam name="U">Target type</typeparam>
	template<typename U>
	using IntegerType = typename TypeTraits::ConditionV<TypeTraits::IsSameV<U, DataSizeT>, SizeT, U>;

	/// <summary>
	/// Add offset to passed pointer and return calculated value
	/// </summary>
	/// <typeparam name="T">Type of pointer value</typeparam>
	/// <param name="ptr">Pointer</param>
	/// <param name="firstOffset">Offset</param>
	/// <returns>New value</returns>
	template<typename T, AddressOffsetType TFirstOffset>
	ForceInline InternalApiCall T* AddOffset(T* ptr, TFirstOffset firstOffset) {
		return reinterpret_cast<T*>((SizeT) (Byte*) (ptr) +(IntegerType<TFirstOffset>) firstOffset);
	}

	/// <summary>
	/// Add offset to passed pointer and return calculated value
	/// </summary>
	/// <typeparam name="T">Type of pointer value</typeparam>
	/// <param name="ptr">Pointer</param>
	/// <param name="firstOffset">First offset</param>
	/// <param name="...offset">Offset</param>
	/// <returns>New value</returns>
	template<typename T, AddressOffsetType TFirstOffset, AddressOffsetType ... TOffsets>
	ForceInline InternalApiCall T* AddOffset(T* ptr, TFirstOffset firstOffset, TOffsets ... offset) {
		auto newPtr = reinterpret_cast<T*>((SizeT) (Byte*) (ptr) +(IntegerType<TFirstOffset>) firstOffset);
		return AddOffset<T, TOffsets...>(newPtr, offset...);
	}

	/// <summary>
	/// Add offset to passed pointer and return calculated value as pointer to U
	/// </summary>
	/// <typeparam name="T">Type of pointer value</typeparam>
	/// <typeparam name="U">Type of result pointer</typeparam>
	/// <param name="ptr">Pointer</param>
	/// <param name="offset">Offsset</param>
	/// <returns>New value of pointer to U</returns>
	template<typename T, typename U, AddressOffsetType TFirstOffset>
	ForceInline InternalApiCall T* AddOffsetAs(U* ptr, TFirstOffset offset) {
		return reinterpret_cast<T*>((SizeT) (Byte*) (ptr) +((IntegerType<TFirstOffset>) offset));
	}

	/// <summary>
	/// Add offset to passed pointer and return calculated value as pointer to U
	/// </summary>
	/// <typeparam name="T">Type of pointer value</typeparam>
	/// <typeparam name="U">Type of result pointer</typeparam>
	/// <param name="ptr">Pointer</param>
	/// <param name="...offset">Offsset</param>
	/// <returns>New value of pointer to U</returns>
	template<typename T, typename U, AddressOffsetType TFirstOffset, AddressOffsetType ... TOffsets>
	ForceInline InternalApiCall T* AddOffsetAs(U* ptr, TFirstOffset firstOffset, TOffsets ... offset) {
		auto newPtr = reinterpret_cast<U*>((SizeT) (Byte*) (ptr) +((IntegerType<TFirstOffset>) firstOffset));
		return AddOffsetAs<T, U, TOffsets...>(newPtr, offset...);
	}
}