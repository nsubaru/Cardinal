export module Cardinal.Containers.LINQ:IFixedQueue;

export import :IFixedCollection;

export namespace Cardinal::Containers::LINQ
{
	/// <summary>
	/// Fixed queue abstraction
	/// </summary>
	/// <typeparam name="TCollection">Fixed queue type/typeparam>
	/// <typeparam name="THeap">Collection heap type</typeparam>
	/// <typeparam name="TSize">Number of elements in a collection</typeparam>
	/// <typeparam name="T">Collection elements type</typeparam>
	/// <typeparam name="TIsConcurencySuport">Collection multithreading support flag</typeparam>
	template<template<typename, SizeT, typename> typename TCollection, typename THeap, typename T, SizeT TSize, Boolean TIsConcurencySuport>
	VirtualClass IFixedQueue : public IFixedCollection<TCollection, THeap, T, TSize, TIsConcurencySuport>{ };
}