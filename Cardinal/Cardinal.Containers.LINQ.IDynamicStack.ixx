export module Cardinal.Containers.LINQ:IDynamicStack;

export import :ICollection;

export namespace Cardinal::Containers::LINQ
{
	/// <summary>
	/// Dynamic stack abstraction
	/// </summary>
	/// <typeparam name="TDynamicQueue">Dynamic stack type</typeparam>
	/// <typeparam name="THeap">Collection heap type</typeparam>
	/// <typeparam name="T">Type of elements</typeparam>
	/// <typeparam name="TIsConcurencySuport">Collection multithreading support flag</typeparam>
	template<template<typename THeap, typename T>typename TDynamicStack, typename THeap, typename T, bool TIsConcurencySuport>
	VirtualClass IDynamicStack : public ICollection<TDynamicStack, THeap, T, false, TIsConcurencySuport>{ };
}