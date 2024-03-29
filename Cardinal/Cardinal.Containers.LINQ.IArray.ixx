export module Cardinal.Containers.LINQ:IArray;

export import :IOrderable;

export namespace Cardinal::Containers::LINQ
{
	/// <summary>
	/// A "LINQ" array extension class
	/// </summary>
	/// <typeparam name="TArray">Array type</typeparam>
	/// <typeparam name="THeap">Array heap</typeparam>
	/// <typeparam name="T">Elements type</typeparam>
	/// <typeparam name="TIsConcurencySuport">Indicates whether collection supports multithreading</typeparam>
	template<template<typename THeap, typename T>typename TArray, typename THeap, typename T, bool TIsConcurencySuport>
	VirtualClass IArray :
		public IOrderable<TArray, THeap, T, false, TIsConcurencySuport> {
	protected:
		using ThisCollection = TArray<THeap, T>;

	public:
		/// <summary>
		/// Read all the elements in a collection
		/// </summary>
		/// <typeparam name="Lambda">Lambda type which is called for every element in collection</typeparam>
		/// <param name="lambda">A lamda which is called for every element in collection(Its appearance is void(*)(T&amp;) or void(*)(T&amp;, SizeT))</param>
		/// <param name="count">Number of elements which is needed to read</param>
		/// <param name="startIndex">A first element index which is needed to read</param>
		/// <returns>A reference to a current collection</returns>
		template<typename Lambda> 
			requires (Concepts::IsInvockable<Lambda, T&> || Concepts::IsInvockable<Lambda, T&, SizeT> ||
				Concepts::IsInvockable<Lambda, const T&> || Concepts::IsInvockable<Lambda, const T&, SizeT>)
		ForceInline ThisCollection& ForEach(Lambda&& lambda, SizeT startIndex = Math::Min<SizeT>(), SizeT count = Math::Max<SizeT>()) {
			return static_cast<ThisCollection&>(*this).ForEachImpl(MoveRValue(lambda), startIndex, count);
		}

		/// <summary>
		/// Read all the elements in a collection
		/// </summary>
		/// <typeparam name="Lambda">Lambda type which is called for every element in collection</typeparam>
		/// <param name="lambda">A lamda which is called for every element in collection(Its appearance is void(*)(const T&amp;) or void(*)(const T&amp;, SizeT))</param>
		/// <param name="count">Number of elements which is needed to read</param>
		/// <param name="startIndex">A first element index which is needed to read</param>
		/// <returns>A reference to a current collection</returns>
		template<typename Lambda> 
			requires (Concepts::IsInvockable<Lambda, const T&> || Concepts::IsInvockable<Lambda, const T&, SizeT>)
		ForceInline const ThisCollection& ForEach(Lambda&& lambda, SizeT startIndex = Math::Min<SizeT>(), SizeT count = Math::Max<SizeT>()) const {
			return static_cast<const ThisCollection&>(*this).ForEachImpl(MoveRValue(lambda), startIndex, count);
		}
	};
}