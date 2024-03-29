export module Cardinal.Containers.LINQ:IString;

export import :Base;

export namespace Cardinal::Containers::LINQ
{
	/// <summary>
	/// A "LINQ" extension class for string type
	/// </summary>
	/// <typeparam name="TArray">String type</typeparam>
	/// <typeparam name="THeap">String heap</typeparam>
	/// <typeparam name="TChar">Symbols (elements) type</typeparam>
	/// <typeparam name="TIsConcurencySuport">Collection multithreading support flag</typeparam>
	template<template<typename THeap> typename TString, typename THeap, typename TChar, Boolean TIsConcurencySuport>
	VirtualClass IString : public IStringCollectionBaseLinq<TString, THeap, TChar, TIsConcurencySuport> {
	private:
		using ThisCollection = TString<THeap>;
	
	public:
		/// <summary>
		/// Reading all the elements in a collection
		/// </summary>
		/// <typeparam name="Lambda">Lambda type which will be called for every elements in a collection</typeparam>
		/// <param name="lambda">Lambda which will be called for every elements in a collection (its appearance is void(*)(TChar&amp;) or void(*)(TChar&amp;, SizeT))</param>
		/// <returns>A reference to a current collection</returns>
		template<typename TLambda> 
			requires (Concepts::IsInvockable<TLambda, TChar&> || Concepts::IsInvockable<TLambda, TChar&, SizeT> ||
				Concepts::IsInvockable<TLambda, const TChar&> || Concepts::IsInvockable<TLambda, const TChar&, SizeT>)
		ForceInline ThisCollection& ForEach(TLambda&& lambda) {
			return static_cast<ThisCollection&>(*this).ForEachImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Reading all the elements in a collection
		/// </summary>
		/// <typeparam name="Lambda">Lambda type which will be called for every elements in a collection</typeparam>
		/// <param name="lambda">Lambda which will be called for every elements in a collection (its appearance is void(*)(TChar&amp;) or void(*)(TChar&amp;, SizeT))</param>
		/// <returns>A reference to a current collection</returns>
		template<typename TLambda> 
			requires (Concepts::IsInvockable<TLambda, const TChar&> || Concepts::IsInvockable<TLambda, const TChar&, SizeT>)
		ForceInline const ThisCollection& ForEach(TLambda&& lambda) const {
			return static_cast<const ThisCollection&>(*this).ForEachImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Reading all the elements in a collection
		/// </summary>
		/// <typeparam name="Lambda">Lambda type which will be called for every elements in a collection</typeparam>
		/// <param name="lambda">Lambda which will be called for every elements in a collection (its appearance is void(*)(TChar&amp;) or void(*)(TChar&amp;, SizeT))</param>
		/// <param name="count">A number of elements which will pass the cycle</param>
		/// <returns>A reference to a current collection</returns>
		template<typename TLambda> 
			requires (Concepts::IsInvockable<TLambda, TChar&> || Concepts::IsInvockable<TLambda, TChar&, SizeT> ||
				Concepts::IsInvockable<TLambda, const TChar&> || Concepts::IsInvockable<TLambda, const TChar&, SizeT>)
		ForceInline ThisCollection& ForEach(TLambda&& lambda, SizeT count) {
			return static_cast<ThisCollection&>(*this).ForEachImpl(MoveRValue(lambda), count);
		}

		/// <summary>
		/// Defines the number of elements which satisfies the condition
		/// </summary>
		/// <typeparam name="TLambda">Predicate lambda type</typeparam>
		/// <param name="lambda">Predicate lambda object</param>
		/// <returns>The number of elements which satisfies the condition</returns>
		template<typename TLambda> requires Concepts::IsInvockableWithResult<TLambda, bool, const TChar&>
		ForceInline SizeT CountIf(TLambda&& lambda) const {
			SizeT count = 0Sz;
			static_cast<const ThisCollection&>(*this).ForEach([&](const TChar& ch) InlineLambda{
				if (lambda(ch))
					count++;
				});

			return count;
		}

		/// <summary>
		/// Reading all the elements in a collection
		/// </summary>
		/// <typeparam name="Lambda">Lambda type which will be called for every elements in a collection</typeparam>
		/// <param name="lambda">Lambda which will be called for every elements in a collection (its appearance is void(*)(TChar&amp;) or void(*)(TChar&amp;, SizeT))</param>
		/// <param name="count">A number of elements which will pass the cycle</param>
		/// <returns>A reference to a current collection</returns>
		template<typename TLambda> 
			requires (Concepts::IsInvockable<TLambda, const TChar&> || Concepts::IsInvockable<TLambda, const TChar&, SizeT>)
		ForceInline const ThisCollection& ForEach(TLambda&& lambda, SizeT count) const {
			return static_cast<const ThisCollection&>(*this).ForEachImpl(MoveRValue(lambda), count);
		}
	
	public:
		/// <summary>
		/// Returns "true" if a container contains something
		/// </summary>
		/// <returns>Returns "true" if a container contains something</returns>
		ForceInline bool Any() const {
			return static_cast<const ThisCollection&>(*this).AnyImpl();
		}

		/// <summary>
		/// Returns "true" if a container contains something that satisfies the lambda condition
		/// </summary>
		/// <param name="lambda">A bool(*)(const T&amp;) lambda type</param>
		/// <returns>Returns "true" if a container contains something that satisfies the lambda condition</returns>
		template<typename TLambda>
			requires (
				Concepts::IsInvockableWithResult<TLambda, bool, TChar> ||
				Concepts::IsInvockableWithResult<TLambda, bool, const TChar&>)
		ForceInline bool Any(TLambda&& lambda) const {
			return static_cast<const ThisCollection&>(*this).AnyImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Returns "true" if all the elements in container satisfies the lambda condition
		/// </summary>
		/// <param name="lambda">A bool(*)(const T&amp;) lambda type</param>
		/// <returns>Returns "true" if all the elements in container satisfies the lambda condition</returns>
		template<typename TLambda>
			requires (
				Concepts::IsInvockableWithResult<TLambda, bool, TChar> ||
				Concepts::IsInvockableWithResult<TLambda, bool, const TChar&>)
		ForceInline bool All(TLambda&& lambda) const  {
			return static_cast<const ThisCollection&>(*this).AllImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Reverse the elements order in collection
		/// </summary>
		/// <returns>A reference to a current collection</returns>
		ForceInline ThisCollection& Reverse()& {
			return static_cast<ThisCollection&>(*this).ReverseImpl();
		}

		/// <summary>
		/// Reverse the elements order in collection
		/// </summary>
		/// <returns>A reference to a current collection</returns>
		ForceInline ThisCollection Reverse()&& {
			static_cast<ThisCollection&>(*this).ReverseImpl();
			return MoveRValue(static_cast<ThisCollection&&>(*this));
		}
	};
}