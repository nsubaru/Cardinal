export module Cardinal.Containers.LINQ:IFixedCollection;

export import :Base;

import Cardinal.Exceptions.Internals;

export namespace Cardinal::Containers::LINQ
{
	/// <summary>
	/// A fixed collection abstraction
	/// </summary>
	/// <typeparam name="TCollection">Collection type</typeparam>
	/// <typeparam name="THeap">Collection heap type</typeparam>
	/// <typeparam name="TSize">Number of elements in a collection</typeparam>
	/// <typeparam name="T">Collection elements type</typeparam>
	/// <typeparam name="TIsConcurencySuport">Collection multithreading support flag</typeparam>
	template<template<typename, SizeT, typename> typename TCollection, typename THeap, typename T, SizeT TSize, Boolean TIsConcurencySuport>
	VirtualClass IFixedCollection : public IFixedCollectionBase<TCollection, THeap, T, TSize, TIsConcurencySuport> {
	private:
		using ThisCollection = TCollection<THeap, TSize, T>;

		template<SizeT USize, typename U>
		using ThisCollectionWithSize = TCollection<THeap, USize, U>;
	
	public:
		/// <summary>
		/// Copying elements to a fixed collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <returns>New collection with copies of elements</returns>
		template<template<SizeT, typename> typename NewCollection>
		NewCollection<TSize, T> CopyToFixed() const {
			NewCollection<TSize, T> newCollection;
						
			decltype(auto) thisCollection = static_cast<const ThisCollection&>(*this);
			thisCollection.ForEach([&](const T& value, SizeT index) InlineLambda{
				if constexpr (Concepts::HasCloneMethod<T>)
				{
					newCollection.UnsafeSet(value.Clone(), index);
				}
				else
				{
					newCollection.UnsafeSet(value, index);
				}
			});
			return newCollection;
		}

		/// <summary>
		/// Moving elements to a fixed collection
		/// </summary>
		/// <returns>New collection with moved elements</returns>
		template<template<SizeT, typename> typename NewCollection = ThisCollectionWithSize>
		NewCollection<TSize, T> MoveToFixed() && {
			NewCollection<TSize, T> newCollection;
						
			decltype(auto) thisCollection = static_cast<ThisCollection&>(*this);
			SizeT index = 0;
						
			thisCollection.RemoveAll([&](T&& value) -> void InlineLambda {
				newCollection.UnsafeSet(MoveRValue(value), index);
				index++;
			});
						
			return newCollection;
		}

	public:
		/// <summary>
		/// Copying elements to a new collection with different size
		/// </summary>
		/// <returns>New collection with copied elements</returns>
		template<SizeT TNewSize, template<SizeT, typename> typename NewCollection = ThisCollectionWithSize>
		NewCollection<TNewSize, T> CopyToFixed() const {
			if constexpr (TNewSize == TSize) {
				NewCollection<TNewSize, T> newCollection;
				decltype(auto) thisCollection = static_cast<ThisCollection&>(*this);
				thisCollection.ForEach([&](const T& value, SizeT index) InlineLambda {
					newCollection.UnsafeSet(value, index);
				});
							
				return newCollection;
			}
			else if constexpr (TNewSize < TSize) {
				NewCollection<TNewSize, T> newCollection;
				decltype(auto) thisCollection = static_cast<ThisCollection&>(*this);

				thisCollection.ForEach([&](const T& value, SizeT index) InlineLambda{
					if (index < TNewSize)
					{
						newCollection.UnsafeSet(value, index);
					}
				});

				return newCollection;
			}
			else
			{
				NewCollection<TNewSize, T> newCollection;
				decltype(auto) thisCollection = static_cast<ThisCollection&>(*this);

				thisCollection.ForEach([&](const T& value, SizeT index) InlineLambda{
					newCollection.UnsafeSet(value, index);
				});

				return newCollection;
			}
		}

		/// <summary>
		/// Copying elements to a new collection with different size
		/// </summary>
		/// <returns>New collection with copied elements</returns>
		template<SizeT TNewSize, template<SizeT, typename> typename NewCollection = ThisCollectionWithSize>
		NewCollection<TNewSize, T> MoveToFixed() const {
			if constexpr (TNewSize == TSize)
			{
				NewCollection<TNewSize, T> newCollection;
				decltype(auto) thisCollection = static_cast<ThisCollection&>(*this);
				SizeT index = 0;
				thisCollection.RemoveAll([&](T&& value) InlineLambda{
					newCollection.UnsafeSet(MoveRValue(value), index); index++;
				});

				return newCollection;
			}
			else if constexpr (TNewSize < TSize)
			{
				NewCollection<TNewSize, T> newCollection;
				decltype(auto) thisCollection = static_cast<ThisCollection&>(*this);

				SizeT index = 0;
				thisCollection.RemoveAll([&](T&& value) InlineLambda{
					if (index < TNewSize)
					{
						newCollection.UnsafeSet(MoveRValue(value), index);
						index++;
					}
				});

				return newCollection;
			}
			else
			{
				NewCollection<TNewSize, T> newCollection;
				decltype(auto) thisCollection = static_cast<ThisCollection&>(*this);

				SizeT index = 0;
				thisCollection.RemoveAll([&](T&& value) InlineLambda{
					newCollection.UnsafeSet(MoveRValue(value), index);
					index++;
				});

				return newCollection;
			}
		}

	public:
		/// <summary>
		/// Copy items to another collection
		/// </summary>
		/// <returns>New collection with copied items</returns>
		template<template <typename> typename TNewCollection> requires (
			Concepts::IsBasedOn<ICollectionGenericBase, TNewCollection<T>> && !TNewCollection<T>::IsReadonly)
		TNewCollection<T> CopyTo() const {
			decltype(auto) thisCollectionRef = static_cast<const ThisCollection&>(*this);
						
			if constexpr (TNewCollection<T>::IsDynamic == false)
			{
				TNewCollection<T> instance = TNewCollection<T>(thisCollectionRef.Count);

				thisCollectionRef.ForEach([&](const T& item, SizeT index) InlineLambda{
					instance.UnsafeSet(item, index);
				});

				return instance;
			}
			else
			{
				TNewCollection<T> instance = TNewCollection<T>();

				thisCollectionRef.ForEach([&](const T& item) InlineLambda
				{
					instance.UnsafeAdd(item);
				});

				return instance;
			}
		}

		/// <summary>
		/// Copying elements into a collection using lambda
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">Lambda</param>
		/// <returns>Copied collection</returns>
		template<template <typename> typename TNewCollection, Concepts::CopyFunction<T> TLambda,
			typename U = TypeTraits::CopyFunctionRet<T, TLambda>>
		requires (
			Concepts::IsBasedOn<ICollectionGenericBase, TNewCollection<T>> &&
			!TNewCollection<U>::IsReadonly)
		TNewCollection<U> CopyTo(TLambda&& lambda) const {
			decltype(auto) thisCollectionRef = static_cast<const ThisCollection&>(*this);

			if constexpr (TNewCollection<U>::IsDynamic == false)
			{
				TNewCollection<U> instance = TNewCollection<U>(thisCollectionRef.Count);

				thisCollectionRef.ForEach([&](const T& item, SizeT index) InlineLambda{
					if constexpr (Concepts::CopyFunctionWithoutIdx<TLambda, T>)
					{
						instance.UnsafeSet(lambda(item, index), index);
					}
					else
					{
						instance.UnsafeSet(lambda(item), index);
					}
				});

				return instance;
			}
			else
			{
				TNewCollection<U> instance = TNewCollection<U>();

				if constexpr (Concepts::CopyFunctionWithoutIdx<TLambda, T>)
				{
					thisCollectionRef.ForEach([&] (const T& item, SizeT index) InlineLambda
					{
						instance.UnsafeAdd(lambda(item, index));
					});
				}
				else
				{
					thisCollectionRef.ForEach([&] (const T& item) InlineLambda
					{
						instance.UnsafeAdd(lambda(item));
					});
				}

				return instance;
			}
		}
	
	public:
		/// <summary>
		/// Execute lambda for each element of a collection
		/// </summary>
		/// <typeparam name="Lambda">Lambda type</typeparam>
		/// <param name="lambda">Lambda which takes elements in turn</param>
		/// <returns>A reference to a current object</returns>
		template<typename TLambda>
		requires
		(
			TypeTraits::IsInvockable<TLambda, const T&> ||
			TypeTraits::IsInvockable<TLambda, const T&, SizeT> ||
			TypeTraits::IsInvockable<TLambda, T&> ||
			TypeTraits::IsInvockable<TLambda, T&, SizeT>
		)
		ForceInline ThisCollection& ForEach(TLambda&& lambda) {
			return static_cast<ThisCollection&>(*this).ForEachImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Execute lambda for each element of a collection
		/// </summary>
		/// <typeparam name="Lambda">Lambda type</typeparam>
		/// <param name="lambda">Lambda which takes elements in turn</param>
		/// <returns>A reference to a current object</returns>
		template<typename TLambda>
			requires
			(
				TypeTraits::IsInvockable<TLambda, const T&> ||
				TypeTraits::IsInvockable<TLambda, const T&, SizeT>
			)
		ForceInline const ThisCollection& ForEach(TLambda&& lambda) const {
			return static_cast<const ThisCollection&>(*this).ForEachImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Aggregate all elements of lambda using lambda
		/// </summary>
		/// <typeparam name="U">Output type</typeparam>
		/// <typeparam name="Lambda">Lambda type</typeparam>
		/// <param name="lambda">A U&amp;(*)(U&amp;, const T&amp;) lambda type</param>
		template<typename U = T, typename TLambda>
			requires (Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&> || 
				Concepts::IsInvockableWithResult<TLambda, U, U&, const T&>)
		ForceInline U Aggregate(TLambda&& lambda) const {
			return static_cast<const ThisCollection&>(*this).
				template AggregateImpl<U, TLambda>(MoveRValue(lambda), U());
		}

		/// <summary>
		/// Aggregate all elements of lambda using lambda and start value
		/// </summary>
		/// <typeparam name="U">Output type</typeparam>
		/// <typeparam name="Lambda">Lambda type</typeparam>
		/// <param name="lambda">A U&amp;(*)(U&amp;, const T&amp;) lambda type</param>
		/// <param name="startValue">Initial value</param>
		/// <returns>Calculation result</returns>
		template<typename U = T, typename TLambda>
			requires Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&>
		ForceInline U Aggregate(TLambda&& lambda, const U& startValue) const {
			return static_cast<const ThisCollection&>(*this).
				template AggregateImpl<U, TLambda>(MoveRValue(lambda), startValue);
		}

		/// <summary>
		/// Aggregate all elements of lambda using lambda and start value
		/// </summary>
		/// <typeparam name="U">Output type</typeparam>
		/// <typeparam name="Lambda">Lambda type</typeparam>
		/// <param name="lambda">A U&amp;(*)(U&amp;, const T&amp;) lambda type</param>
		/// <param name="startValue">Initial value</param>
		/// <returns>Calculation result</returns>
		template<typename U = T, typename TLambda>
			requires Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&>
		ForceInline U Aggregate(TLambda&& lambda, U&& startValue) const {
			return static_cast<const ThisCollection&>(*this).
				template AggregateImpl<U, TLambda>(MoveRValue(lambda), MoveRValue(startValue));
		}

		/// <summary>
		/// Returns "true" if container contains something
		/// </summary>
		/// <returns>Returns "true" if container contains something</returns>
		ForceInline bool Any() const {
			return static_cast<const ThisCollection&>(*this).AnyImpl();
		}

		/// <summary>
		/// Returns "true" if container contains something that satisfies the lambda condition
		/// </summary>
		/// <param name="lambda">A bool(*)(const T&amp;) lambda type</param>
		/// <returns>Returns "true" if container contains something that satisfies the lambda condition</returns>
		template<typename TLambda>
			requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
			ForceInline bool Any(TLambda&& lambda) const {
			return static_cast<const ThisCollection&>(*this).AnyImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Returns "true" if all the elements in container satisfies the lambda condition
		/// </summary>
		/// <param name="lambda">A bool(*)(const T&amp;) lambda type</param>
		/// <returns>Returns "true" if all the elements in container satisfies the lambda condition</returns>
		template<typename TLambda>
			requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
			ForceInline bool All(TLambda&& lambda) const {
			return static_cast<const ThisCollection&>(*this).AllImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Returns the first element which satisfies the lambda condition or throws exception
		/// </summary>
		/// <param name="lambda">A bool(*)(const T&amp;) lambda type</param>
		/// <returns>Returns the first element which satisfies the lambda condition or throws exception</returns>
		template<typename TLambda>
			requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
		ForceInline T& First(TLambda&& lambda) {
			return static_cast<ThisCollection&>(*this).FirstImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Returns the last element which satisfies the lambda condition or throws exception
		/// </summary>
		/// <param name="lambda">A bool(*)(const T&amp;) lambda type</param>
		/// <returns>Returns the last element which satisfies the lambda condition or throws exception</returns>
		template<typename TLambda>
			requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
		ForceInline T& Last(TLambda&& lambda) {
			return static_cast<ThisCollection&>(*this).LastImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Returns the first element which satisfies the lambda condition or throws exception
		/// </summary>
		/// <param name="lambda">A bool(*)(const T&amp;) lambda type</param>
		/// <returns>Returns the first element which satisfies the lambda condition or throws exception</returns>
		template<typename TLambda>
			requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
		ForceInline const T& First(TLambda&& lambda) const {
			return static_cast<const ThisCollection&>(*this).FirstImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Returns the last element which satisfies the lambda condition or throws exception
		/// </summary>
		/// <param name="lambda">A bool(*)(const T&amp;) lambda type</param>
		/// <returns>Returns the last element which satisfies the lambda condition or throws exception</returns>
		template<typename Lambda>
		ForceInline const T& Last(Lambda&& lambda) const {
			return static_cast<const ThisCollection&>(*this).LastImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Returns the first element which satisfies the lambda condition or second argument
		/// </summary>
		/// <param name="lambda">A bool(*)(const T&amp;) lambda type</param>
		/// <param name="item">An object that returns if container is empty</param>
		/// <returns>Returns the first element which satisfies the lambda condition or throws exception</returns>
		template<typename TLambda>
		requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
		ForceInline T FirstOr(TLambda&& lambda, T&& item) const {
				return static_cast<const ThisCollection&>(*this).FirstOrImpl(MoveRValue(lambda), MoveRValue(item));
			}

		/// <summary>
		/// Returns the last element which satisfies the lambda condition or second argument
		/// </summary>
		/// <param name="lambda">A bool(*)(const T&amp;) lambda type</param>
		/// <param name="item">An object that returns if container is empty</param>
		/// <returns>Returns the last element which satisfies the lambda condition or throws exception</returns>
		template<typename TLambda>
		requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
		ForceInline T LastOr(TLambda&& lambda, T&& item) const {
				return static_cast<const ThisCollection&>(*this).LastOrImpl(MoveRValue(lambda), MoveRValue(item));
			}

		/// <summary>
		/// Returns the first element which satisfies the lambda condition or second argument
		/// </summary>
		/// <param name="lambda">A bool(*)(const T&amp;) lambda type</param>
		/// <param name="item">An object that returns if container is empty</param>
		/// <returns>Returns the first element which satisfies the lambda condition or throws exception</returns>
		template<typename TLambda>
		requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
		ForceInline T FirstOr(TLambda&& lambda, const T& item) const {
				return static_cast<const ThisCollection&>(*this).FirstOrImpl(MoveRValue(lambda), item);
			}

		/// <summary>
		/// Returns the last element which satisfies the lambda condition or second argument
		/// </summary>
		/// <param name="lambda">A bool(*)(const T&amp;) lambda type</param>
		/// <param name="item">An object that returns if container is empty</param>
		/// <returns>Returns the last element which satisfies the lambda condition or throws exception</returns>
		template<typename TLambda>
		requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
		ForceInline T LastOr(TLambda&& lambda, const T& item) const {
				return static_cast<const ThisCollection&>(*this).LastOrImpl(MoveRValue(lambda), item);
			}

		/// <summary>
		/// Returns the first element which satisfies the lambda condition or second lambda argument
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">A bool(*)(const T&amp;) lambda type</param>
		/// <param name="otherLambda">A lambda which returns the object if container is empty</param>
		/// <returns>Returns the first element which satisfies the lambda condition or throws exception</returns>
		template<typename TLambda, typename TGetterLambda>
		requires (Concepts::IsInvockableWithResult<TLambda, bool, const T&> &&
				Concepts::IsInvockableWithResult<TGetterLambda, T>)
		ForceInline T FirstOr(TLambda&& lambda, TGetterLambda&& otherLambda) const {
				return static_cast<const ThisCollection&>(*this).FirstOrImpl(MoveRValue(lambda), MoveRValue(otherLambda));
			}

		/// <summary>
		/// Returns the last element which satisfies the lambda condition or second lambd argument
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">A bool(*)(const T&amp;) lambda type</param>
		/// <param name="otherLambda">A lambda which returns the object if container is empty</param>
		/// <returns>Returns the last element which satisfies the lambda condition or throws exception</returns>
		template<typename TLambda, typename TGetterLambda>
		requires (Concepts::IsInvockableWithResult<TLambda, bool, const T&> &&
				Concepts::IsInvockableWithResult<TGetterLambda, T>)
		ForceInline T LastOr(TLambda&& lambda, TGetterLambda&& otherLambda) const {
				return static_cast<const ThisCollection&>(*this).LastOrImpl(MoveRValue(lambda), MoveRValue(otherLambda));
			}

		/// <summary>
		/// Deletes all the elements from the collection
		/// </summary>
		/// <returns>A reference to a current object of collection</returns>
		ForceInline ThisCollection& RemoveAll() {
			return static_cast<ThisCollection&>(*this).RemoveAllImpl();
		}

		/// <summary>
		/// Deletes all the elements from collection and call lambda for every element before deleting
		/// </summary>
		/// <param name="lambda">A void(*)(T&amp;&amp;) lambda type</param>
		/// <returns>A reference to a current object of collection</returns>
		template<typename TLambda>
		requires Concepts::IsInvockable<TLambda, T&&>
		ForceInline ThisCollection& RemoveAll(TLambda&& lambda) {
			return static_cast<ThisCollection&>(*this).RemoveAllImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Reverses the element order in collection
		/// </summary>
		/// <returns>A reference to a current collection</returns>
		ForceInline ThisCollection& Reverse() {
			return static_cast<ThisCollection&>(*this).ReverseImpl();
		}

	public:
		/// <summary>
		/// Maximal element searching in collection
		/// </summary>
		/// <returns>Maximal value</returns>
		T Max() const requires Concepts::IsComparableWith<T, T>
		{
			decltype(auto) This = static_cast<const ThisCollection&>(*this);

			auto maxItem = This.First([] ([[maybe_unused]] const auto&) { return true; });

			This.ForEach([&maxItem](const T& item){
				if (maxItem < item)
					maxItem = item;
			});

			return maxItem;
		}

		/// <summary>
		/// Maximal element searching in a collection using lambda which returns element or field value
		/// </summary>
		/// <param name="lambda">A lambda, returning a value among which the maximal value will be searched</param>
		/// <returns>Maximal value</returns>
		template<typename TLambda>
		requires Concepts::IsInvockable<TLambda, const T&> &&
			(!Concepts::IsInvockableReturnVoid<TLambda, const T&>) &&
			Concepts::IsComparableWith<TypeTraits::InvockableResult<TLambda, const T&>, TypeTraits::InvockableResult<TLambda, const T&>>
		decltype(auto) Max(TLambda&& lambda) const {
			const ThisCollection& This = static_cast<const ThisCollection&>(*this);
			auto maxItem = lambda(This.First(ArrowFunct(x, true)));

			This.ForEach([&maxItem, lambda = MoveRValue(lambda)](const T& item){
				auto itemValue = lambda(item);
				if (maxItem < itemValue)
					maxItem = itemValue;
			});

			return maxItem;
		}

		/// <summary>
		/// Maximal element searching in a collection using lambda which returns element or field value
		/// </summary>
		/// <param name="lambda">A lambda, returning a value among which the maximal value will be searched</param>
		/// <returns>Maximal value</returns>
		template<typename TLambda>
			requires Concepts::IsInvockable<TLambda, const T&> && 
			(!Concepts::IsInvockableReturnVoid<TLambda, const T&>) && 
			Concepts::IsComparableWith<TypeTraits::InvockableResult<TLambda, const T&>, TypeTraits::InvockableResult<TLambda, const T&>>
		T MaxBy(TLambda&& lambda) const {
			decltype(auto) This = static_cast<const ThisCollection&>(*this);

			auto maxItem = This.First([] ([[maybe_unused]] const auto&) { return true; });

			This.ForEach([&maxItem, lambda = MoveRValue(lambda)](const T& item){
				if (lambda(maxItem) < lambda(item))
					maxItem = item;
			});

			return maxItem;
		}

		/// <summary>
		/// Minimal element searching in a collection
		/// </summary>
		/// <returns>Minimal value</returns>
		T Min() requires Concepts::IsComparableWith<T, T>
		{
			decltype(auto) This = static_cast<const ThisCollection&>(*this);

			auto maxItem = This.First([] ([[maybe_unused]] const auto&) { return true; });

			This.ForEach([&maxItem] (const T& item){
				if (maxItem > item)
					maxItem = item;
			});

			return maxItem;
		}

		/// <summary>
		/// Minimal element searching in a collection using lambda which returns element or field value
		/// </summary>
		/// <param name="lambda">A lambda, returning a value among which the minimal value will be searched</param>
		/// <returns>Minimal value</returns>
		template<typename TLambda>
			requires Concepts::IsInvockable<TLambda, const T&> && 
			(!Concepts::IsInvockableReturnVoid<TLambda, const T&>) && 
			Concepts::IsComparableWith<TypeTraits::InvockableResult<TLambda, const T&>, TypeTraits::InvockableResult<TLambda, const T&>>
		decltype(auto) Min(TLambda&& lambda) const {
			decltype(auto) This = static_cast<const ThisCollection&>(*this);
			auto maxItem = lambda(This.First([] ([[maybe_unused]] const auto&) { return true; }));

			This.ForEach([&maxItem, lambda = MoveRValue(lambda)] (const T& item){
				auto itemValue = lambda(item);
				if (maxItem > itemValue)
					maxItem = itemValue;
			});

			return maxItem;
		}

		/// <summary>
		/// Minimal element searching in collection using lambda which returns element or field value
		/// </summary>
		/// <param name="lambda">A lambda, returning a value among which the minimal value will be searched</param>
		/// <returns>Minimal value</returns>
		template<typename TLambda>
			requires Concepts::IsInvockable<TLambda, const T&> && 
			(!Concepts::IsInvockableReturnVoid<TLambda, const T&>) && 
			Concepts::IsComparableWith<TypeTraits::InvockableResult<TLambda, const T&>, TypeTraits::InvockableResult<TLambda, const T&>>
		T MinBy(TLambda&& lambda) const {
			decltype(auto) This = static_cast<const ThisCollection&>(*this);
			auto maxItem = This.First([] ([[maybe_unused]] const auto&) { return true; });

			This.ForEach([&maxItem, lambda = MoveRValue(lambda)] (const T& item) {
				if (lambda(maxItem) > lambda(item))
					maxItem = item;
			});

			return maxItem;
		}
	};
}