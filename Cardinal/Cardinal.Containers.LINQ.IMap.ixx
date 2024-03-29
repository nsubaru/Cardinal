export module Cardinal.Containers.LINQ:IMap;

export import :IAssociableCollection;

export namespace Cardinal::Containers::LINQ
{
	/// <summary>
	/// "LINQ" associable array extension
	/// </summary>
	/// <typeparam name="TCollection">Associable array type</typeparam>
	/// <typeparam name="THeap">Heap type</typeparam>
	/// <typeparam name="TKey">Key type</typeparam>
	/// <typeparam name="TValue">Value type</typeparam>
	/// <typeparam name="TPair">Key-value pair type</typeparam>
	/// <typeparam name="TIsConcurencySuport">Collection multithreading support flag</typeparam>
	template<template<typename THeap, typename TKey, typename TValue> typename TCollection, typename THeap, typename TKey, typename TValue, typename TPair, bool IsConcurencySuportT>
	VirtualClass IMap : public IAssociableCollection<TCollection, THeap, TKey, TValue, TPair, IsConcurencySuportT>{ };
}