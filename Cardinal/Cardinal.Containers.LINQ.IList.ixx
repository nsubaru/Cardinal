export module Cardinal.Containers.LINQ:IList;

export import :IOrderable;

export namespace Cardinal::Containers::LINQ
{
	/// <summary>
	/// "LINQ" extension for list
	/// </summary>
	/// <typeparam name="TList">List type</typeparam>
	/// <typeparam name="THeap">Heap type</typeparam>
	/// <typeparam name="TIsConcurencySuport">Collection multithreading support flag</typeparam>
	template<template<typename THeap, typename T>typename TList, typename THeap, typename T, bool TIsConcurencySuport>
	VirtualClass IList : public IOrderable<TList, THeap, T, true, TIsConcurencySuport> {
	protected:
		using ListType = TList<THeap, T>;
	
	public:
		/// <summary>
		/// Deleting the elements which satisfies the "FilteringLambda" condition
		/// </summary>
		/// <typeparam name="FilteringLambda">Lambda type with bool(*)(const T&amp;) or bool(*)(const T&amp;, SizeT) signature</typeparam>
		/// <typeparam name="Lambda">Lambda type which deletes elements with void(*)(T&amp;&amp;) signature</typeparam>
		/// <param name="filter">Lambda with bool(*)(const T&amp;) or bool(*)(const T&amp;, SizeT) signature</param>
		/// <param name="lambda">Lambda which deletes elements with void(*)(T&amp;&amp;) signature</param>
		/// <returns>A reference to a current collection</returns>
		template<typename TLambda, typename TFilteringLambda>
			requires(Concepts::IsInvockableWithResult<TFilteringLambda, bool, const T&> ||
				Concepts::IsInvockableWithResult<TFilteringLambda, bool, const T&, SizeT>) &&
			(Concepts::IsInvockable<TLambda, T&&> || Concepts::IsInvockable<TLambda, T&&, SizeT>)
		ForceInline ListType& RemoveIf(TFilteringLambda&& filter, TLambda&& lambda) {
			return static_cast<ListType&>(*this).RemoveIfImpl(MoveRValue(filter), MoveRValue(lambda));
		}

		/// <summary>
		/// Deleting the elements which satisfies the "FilteringLambda" condition
		/// </summary>
		/// <typeparam name="FilteringLambda">Lambda type with bool(*)(const T&amp;) or bool(*)(const T&amp;, SizeT) signature</typeparam>
		/// <param name="filter">Lambda with bool(*)(const T&amp;) or bool(*)(const T&amp;, SizeT) signature</param>
		/// <returns>A reference to a current collection</returns>
		template<typename TFilteringLambda>
		requires (Concepts::IsInvockableWithResult<TFilteringLambda, bool, const T&> ||
			Concepts::IsInvockableWithResult<TFilteringLambda, bool, const T&, SizeT>)
		ForceInline ListType& RemoveIf(TFilteringLambda&& filter) {
			return static_cast<ListType&>(*this).RemoveIfImpl(MoveRValue(filter), [] (T&&) InlineLambda {});
		}
	
	public:
		/// <summary>
		/// Returns iterator to the first object which satisfies the lambda condition in collection
		/// </summary>
		/// <param name="lambda">A bool(*)(const T&amp;) lambda type</param>
		/// <returns>Returns iterator to the first object which satisfies the lambda condition in collection</returns>
		template<typename TLambda>
			requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
		ForceInline auto FirstIt(TLambda&& lambda) {
			return static_cast<ListType&>(*this).FirstItImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Returns iterator to the last object which satisfies the lambda condition in collection
		/// </summary>
		/// <param name="lambda">A bool(*)(const T&amp;) lambda type</param>
		/// <returns>Returns iterator to the last object which satisfies the lambda condition in collection</returns>
		template<typename TLambda>
			requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
		ForceInline auto LastIt(TLambda&& lambda) {
			return static_cast<ListType&>(*this).LastItImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Returns constant iterator to the first object which satisfies the lambda condition in collection
		/// </summary>
		/// <param name="lambda">A bool(*)(const T&amp;) lambda type</param>
		/// <returns>Returns constant iterator to the first object which satisfies the lambda condition in collection</returns>
		template<typename TLambda>
			requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
		ForceInline auto FirstIt(TLambda&& lambda) const {
			return static_cast<const ListType&>(*this).FirstItImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Returns constant iterator to the last object which satisfies the lambda condition in collection
		/// </summary>
		/// <param name="lambda">A bool(*)(const T&amp;) lambda type</param>
		/// <returns>Returns constant iterator to the last object which satisfies the lambda condition in collection</returns>
		template<typename TLambda>
			requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
		ForceInline auto LastIt(TLambda&& lambda) const {
			return static_cast<const ListType&>(*this).LastItImpl(MoveRValue(lambda));
		}
	};
}