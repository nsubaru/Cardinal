export module Cardinal.Containers.LINQ:IDictionary;

export import :IAssociableCollection;

export namespace Cardinal::Containers::LINQ
{
	/// <summary>
	/// A "LINQ" dictionary methods abstraction
	/// </summary>
	/// <typeparam name="TCollection">Dictionary collection type</typeparam>
	/// <typeparam name="THeap">Collection heap type</typeparam>
	/// <typeparam name="TKey">Element key type</typeparam>
	/// <typeparam name="TValue">A value of element type</typeparam>
	/// <typeparam name="TPair">Key-value pair type</typeparam>
	/// <typeparam name="TIsConcurencySuport">Collection multithreading support flag</typeparam>
	template<template<typename THeap, typename TKey, typename TValue> typename TCollection, typename THeap, typename TKey, typename TValue, typename TPair, bool TIsConcurencySuport>
	VirtualClass IDictionary : public IAssociableCollection<TCollection, THeap, TKey, TValue, TPair, TIsConcurencySuport>{ };
}