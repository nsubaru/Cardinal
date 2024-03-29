export module Cardinal.Containers.LINQ:IOrderable;

export import :Base;
export import :ICollection;

export namespace Cardinal::Containers::LINQ
{
	/// <summary>
	/// Orderable collections
	/// </summary>
	/// <typeparam name="TList">Collectin type</typeparam>
	/// <typeparam name="THeap">Memory heap type which collection is using</typeparam>
	/// <typeparam name="T">Collection elements type</typeparam>
	/// <typeparam name="TIsDynamic">Collection size dynamism flag</typeparam>
	/// <typeparam name="TIsConcurencySuport">Collection multithreading support flag</typeparam>
	template<template<typename THeap, typename T>typename TList, typename THeap, typename T, bool TIsDynamic, bool TIsConcurencySuport>
	VirtualClass IOrderable :
	public ICollection<TList, THeap, T, TIsDynamic, TIsConcurencySuport>{
	protected:
		using ThisCollection = TList<THeap, T>;
	
	public:
		/// <summary>
		/// Collection elements sorting (using ">=" or "<=" operator for sorting)
		/// </summary>
		/// <param name="isDescending">If "true" than sort in descending order</param>
		/// <returns>A reference to a current collection</returns>
		ForceInline ThisCollection& Sort(bool isDescending = true) & requires Concepts::IsComparableWith<T, T> {
			return static_cast<ThisCollection&>(*this).SortByImpl(isDescending);
		}

		/// <summary>
		/// Collection elements sorting (using the lambda which points how to compare the elements)
		/// </summary>
		/// <param name="lambda">A bool(const T&amp;, const T&amp;) lambda type</param>
		/// <returns>A reference to a current collection</returns>
		template<typename TLambda>
			requires Concepts::IsInvockableWithResult<TLambda, bool, const T&, const T&>
		ForceInline ThisCollection& SortBy(TLambda&& lambda) & {
			return static_cast<ThisCollection&>(*this).SortByLambdaImpl(lambda);
		}

		/// <summary>
		/// Collection elements sorting (using ">=" or "<=" operator for sorting)
		/// </summary>
		/// <param name="isDescending">If "true" than sort in descending order</param>
		/// <returns>A reference to a current collection</returns>
		ForceInline ThisCollection Sort(bool isDescending = true) && requires Concepts::IsComparableWith<T, T>  {
			static_cast<ThisCollection&>(*this).SortByImpl(isDescending);
			return static_cast<ThisCollection&&>(*this);
		}

		/// <summary>
		/// Collection elements sorting (using the lambda which points how to compare the elements)
		/// </summary>
		/// <param name="lambda">A bool(const T&amp;, const T&amp;) lambda type</param>
		/// <returns>A reference to a current collection</returns>
		template<typename TLambda>
			requires Concepts::IsInvockableWithResult<TLambda, bool, const T&, const T&>
		ForceInline ThisCollection SortBy(TLambda&& lambda) && {
			static_cast<ThisCollection&>(*this).SortByLambdaImpl(lambda);
			return static_cast<ThisCollection&&>(*this);
		}

	public:
		/// <summary>
		/// Reverse the elements order in collection
		/// </summary>
		/// <returns>A reference to a current collection</returns>
		ForceInline ThisCollection& Reverse() {
			return static_cast<ThisCollection&>(*this).ReverseImpl();
		}
	};
}