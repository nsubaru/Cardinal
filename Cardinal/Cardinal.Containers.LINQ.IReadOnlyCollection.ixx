export module Cardinal.Containers.LINQ:IReadOnlyCollection;

export import :Base;
export import :ICollection;

export namespace Cardinal::Containers::LINQ
{
	/// <summary>
	/// Ordered collections
	/// </summary>
	/// <typeparam name="TList">Collection type</typeparam>
	/// <typeparam name="THeap">A type of memory heap, which the collection uses</typeparam>
	/// <typeparam name="T">The type of collection elements</typeparam>
	/// <typeparam name="TIsDynamic">The collection size dynamism flag</typeparam>
	/// <typeparam name="TIsConcurencySuport">The multithreading support flag</typeparam>
	template<template<typename THeap, typename T> typename TReadOnlyCollection, typename THeap, typename T, bool TIsDynamic, bool TIsConcurencySuport, bool IsCollectionCanReturnRef = true>
	VirtualClass IReadOnlyCollection :
	public ICollection<TReadOnlyCollection, THeap, T, TIsDynamic, TIsConcurencySuport, IsCollectionCanReturnRef>{
	protected:
		using ThisCollection = TReadOnlyCollection<THeap, T>;
	};
}