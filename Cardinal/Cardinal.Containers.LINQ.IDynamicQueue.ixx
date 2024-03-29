export module Cardinal.Containers.LINQ:IDynamicQueue;

export import :ICollection;

export namespace Cardinal::Containers::LINQ
{
	/// <summary>
	/// Dynamic queue abstraction
	/// </summary>
	/// <typeparam name="TDynamicQueue">Dynamic queue type</typeparam>
	/// <typeparam name="THeap">Collection heap type</typeparam>
	/// <typeparam name="T">Type of elements</typeparam>
	/// <typeparam name="TIsConcurencySuport">Collection multithreading support flag</typeparam>
	template<template<typename THeap, typename T>typename TDynamicQueue, typename THeap, typename T, bool TIsConcurencySuport>
	VirtualClass IDynamicQueue : public ICollection<TDynamicQueue, THeap, T, false, TIsConcurencySuport>{ };
}