export module Cardinal.Containers.LINQ:IFixedArray;

export import :IFixedCollection;

export namespace Cardinal::Containers::LINQ
{
	/// <summary>
	/// Fixed array abstraction
	/// </summary>
	/// <typeparam name="TCollection">Fixed array type</typeparam>
	/// <typeparam name="THeap">Collection heap type</typeparam>
	/// <typeparam name="TSize">Number of elements in a collection</typeparam>
	/// <typeparam name="T">Collection elements type</typeparam>
	/// <typeparam name="TIsConcurencySuport">Collection multithreading support flag</typeparam>
	template<template<typename, SizeT, typename> typename TCollection, typename THeap, typename T, SizeT TSize, Boolean TIsConcurencySuport>
	VirtualClass IFixedArray : public IFixedCollection<TCollection, THeap, T, TSize, TIsConcurencySuport> {
		using ThisCollection = TCollection<THeap, TSize, T>;
	
	public:
		/// <summary>
		/// Collection elements sorting (using "&gt;=" or "&lt;=" operator for sorting)
		/// </summary>
		/// <param name = "isDescending">If "true" than sort in descending order</param>
		/// <returns>The reference on a current collection</returns>
		ForceInline ThisCollection& SortBy(bool isDescending = true) {
			return static_cast<ThisCollection&>(*this).SortByImpl(isDescending);
		}

		/// <summary>
		/// Collection generating with elements which lambda generates
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="generator">A format function-generator T(*)() or T(*)(SizeT)</param>
		/// <returns>Collection with elements</returns>
		template<typename TLambda>
		requires
			Concepts::IsInvockableWithResult<TLambda, T> ||
			Concepts::IsInvockableWithResult<TLambda, T, SizeT>
		static ThisCollection Generate(TLambda generator) {
			if constexpr (TypeTraits::IsInvockableWithResult<TLambda, T>)
			{
				if constexpr (!ThisCollection::IsDynamic)
				{
					ThisCollection collection = ThisCollection();
					for (SizeT i = 0; i < TSize; i++)
					{
						collection.UnsafeSet(MoveRValue(generator()), i);
					}
					return collection;
				}
				else
				{
					ThisCollection collection = ThisCollection();
					for (SizeT i = 0; i < TSize; i++)
					{
						collection.UnsafeAdd(MoveRValue(generator()));
					}
					return collection;
				}
			}
			else
			{
				if constexpr (!ThisCollection::IsDynamic)
				{
					ThisCollection collection = ThisCollection();
					for (SizeT i = 0; i < TSize; i++)
					{
						collection.UnsafeSet(generator(i), i);
					}
					return collection;
				}
				else
				{
					ThisCollection collection = ThisCollection();
					for (SizeT i = 0; i < TSize; i++)
					{
						collection.UnsafeAdd(generator(i));
					}
					return collection;
				}
			}
		}
	};
}