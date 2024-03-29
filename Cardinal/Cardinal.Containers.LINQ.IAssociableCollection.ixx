export module Cardinal.Containers.LINQ:IAssociableCollection;

export import :Base;

import Cardinal.Exceptions.Internals;

export namespace Cardinal::Containers::LINQ
{
	/// <summary>
	/// "LINQ" Extension for associable collections
	/// </summary>
	/// <typeparam name="TCollection">Associable collection type</typeparam>
	/// <typeparam name="THeap">A heap type</typeparam>
	/// <typeparam name="TKey">Key type</typeparam>
	/// <typeparam name="TValue">Value type</typeparam>
	/// <typeparam name="TPair">A key-value pair type</typeparam>
	/// <typeparam name="TIsConcurencySuport">A multithreading collection support flag</typeparam>
	template<template<typename THeap, typename TKey, typename TValue> typename TCollection, typename THeap, typename TKey, typename TValue, typename TPair, bool IsConcurencySuportT>
	VirtualClass IAssociableCollection :
		public IAssociableCollectionBase<TCollection, THeap, TKey, TValue, TPair, IsConcurencySuportT>
	{
	private:
		/// <summary>
		/// Associable collection type
		/// </summary>
		using ThisCollection = TCollection<THeap, TKey, TValue>;
	
	public:
		/// <summary>
		/// Reading the associable collection (calls lambda for every key-value pair)
		/// </summary>
		/// <typeparam name="Lambda">Lambda type</typeparam>
		/// <param name="lambda">Lambda function which will be called for every key-value pair (can receive pair or pair and number)</param>
		/// <returns>Reference to the collection</returns>
		template<typename TLambda>
			requires 
			(Concepts::IsInvockable<TLambda, TPair&> || Concepts::IsInvockable<TLambda, TPair&, SizeT> ||
				Concepts::IsInvockable<TLambda, const TPair&> || Concepts::IsInvockable<TLambda, const TPair&, SizeT>)
		ForceInline ThisCollection& ForEach(TLambda&& lambda) {
			return static_cast<ThisCollection&>(*this).ForEachImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Reading the constant associable collection (calls lambda for every key-value pair)
		/// </summary>
		/// <typeparam name="Lambda">Lambda type</typeparam>
		/// <param name="lambda">Lambda function which will be called for every key-value pair (can receive reference to pair or pair number)</param>
		/// <returns>Reference to the constant collection</returns>
		template<typename TLambda>
			requires Concepts::IsInvockable<TLambda, const TPair&> || Concepts::IsInvockable<TLambda, const TPair&, SizeT>
		ForceInline const ThisCollection& ForEach(TLambda&& lambda) const {
			return static_cast<const ThisCollection&>(*this).ForEachImpl(MoveRValue(lambda));
		}
	
	public:
		/// <summary>
		/// Reading the associable collection (calls lambda for every key)
		/// </summary>
		/// <typeparam name="Lambda">Lambda type</typeparam>
		/// <param name="lambda">Lambda which will be called for every key (can receive a constant reference to key and element number or only reference)</param>
		/// <returns>Reference to the collection</returns>
		template<typename TLambda>
			requires Concepts::IsInvockable<TLambda, const TKey&> || Concepts::IsInvockable<TLambda, const TKey&, SizeT>
		ForceInline ThisCollection& ForEachKey(TLambda&& lambda) {
			return static_cast<ThisCollection&>(*this).ForEachKeyImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Reading the constant associable collection (calls lambda for every key)
		/// </summary>
		/// <typeparam name="Lambda">Lambda type</typeparam>
		/// <param name="lambda">Lambda which will be called for every key (can receive a constant reference to key and element number or only reference)</param>
		/// <returns>Reference to the constant collection</returns>
		template<typename TLambda>
			requires Concepts::IsInvockable<TLambda, const TKey&> || Concepts::IsInvockable<TLambda, const TKey&, SizeT>
		ForceInline const ThisCollection& ForEachKey(TLambda&& lambda) const {
			return static_cast<const ThisCollection&>(*this).ForEachKeyImpl(MoveRValue(lambda));
		}
	
	public:
		/// <summary>
		/// Reading the associable collection (calls lambda for every value)
		/// </summary>
		/// <typeparam name="Lambda">Lambda type</typeparam>
		/// <param name="lambda">Lambda which will be called for every value (can receive a reference to value and element number or only reference)</param>
		/// <returns>Reference to the collection</returns>
		template<typename TLambda>
			requires Concepts::IsInvockable<TLambda, TValue&> || Concepts::IsInvockable<TLambda, TValue&, SizeT>
		ForceInline ThisCollection& ForEachValue(TLambda&& lambda) {
			return static_cast<ThisCollection&>(*this).ForEachValueImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Reading the constant associable collection (calls lambda for every value)
		/// </summary>
		/// <typeparam name="Lambda">Lambda type</typeparam>
		/// <param name="lambda">Lambda which will be called for every value (can receive a reference to value and element number or only reference)</param>
		/// <returns>Reference to the constant collection</returns>
		template<typename TLambda>
			requires Concepts::IsInvockable<TLambda, const TValue&> || Concepts::IsInvockable<TLambda, const TValue&, SizeT>
		ForceInline const ThisCollection& ForEachValue(TLambda&& lambda) const {
			return static_cast<const ThisCollection&>(*this).ForEachValueImpl(MoveRValue(lambda));
		}
	
	public:
		/// <summary>
		/// Deletes all elements from the collection
		/// </summary>
		/// <returns>Reference to the collection</returns>
		ForceInline ThisCollection& RemoveAll() {
			return static_cast<ThisCollection&>(*this).RemoveAllImpl();
		}

		/// <summary>
		/// Deletes all elements from the collection and calls lambda for each pair before deleting
		/// </summary>
		/// <typeparam name="Lambda">Lambda type</typeparam>
		/// <param name="lambda">Lambda which receives a "rvalue" reference to pair</param>
		/// <returns>Reference to the collection</returns>
		template<typename TLambda>
			requires Concepts::IsInvockable<TLambda, TPair&&>
		ForceInline ThisCollection& RemoveAllWith(TLambda&& lambda) {
			return static_cast<ThisCollection&>(*this).RemoveAllWithImpl(MoveRValue(lambda));
		}
	
	public:
		/// <summary>
		/// Copies elements to a collection
		/// </summary>
		/// <typeparam name="Lambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="FilterLambda">Elements filtering lambda type</typeparam>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="UHeap">A new collection heap type</typeparam>
		/// <typeparam name="U">Elements type in new collection</typeparam>
		/// <returns>A collection with copy of all elements</returns>
		template<
			template<typename UHeap, typename U> class NewCollection,
			typename TLambda,
			typename TFilterLambda, 
			typename UHeap, 
			typename U = TypeTraits::InvockableResult<TLambda, const TPair&>
		>
		requires (Concepts::IsInvockable<TLambda, const TPair&>&&
				  Concepts::IsInvockable<TFilterLambda, const TPair&>&&
				  Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<UHeap, U>>&&
				  Concepts::IsSame<bool, TypeTraits::InvockableResult<TFilterLambda, const TPair&>> &&
				  !NewCollection<UHeap, U>::IsReadonly)
		NewCollection<UHeap, U> CopyToCollection(TFilterLambda&& filterLambda, TLambda&& lambda) const {
			if constexpr (NewCollection<UHeap, U>::IsDynamic == false)
			{
				TFilterLambda filterCopy = filterLambda;
				NewCollection<UHeap, U> result = NewCollection<UHeap, U>(static_cast<const ThisCollection&>(*this)
					.GetCountIf(MoveRValue(filterCopy)));

				SizeT index = 0;
				this->ForEach([&] (const TPair& pair) InlineLambda
				{
					if (filterLambda(pair))
					{
						result.UnsafeSet(lambda(pair), index);
						index++;
					}
				});
						
				return result;
			}
			else
			{
				NewCollection<UHeap, U> result;
						
				this->ForEach([&] (const TPair& pair) InlineLambda
				{
					if (filterLambda(pair))
					{
						result.UnsafeAdd(lambda(pair));
					}
				});
						
				return result;
			}
		}

		/// <summary>
		/// Copies elements to a collection
		/// </summary>
		/// <typeparam name="Lambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="FilterLambda">Elements filtering lambda type</typeparam>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="UHeap">A new collection heap type</typeparam>
		/// <typeparam name="U">Elements type in new collection</typeparam>
		/// <returns>A collection with copy of all elements</returns>
		template<
			template<typename U> class NewCollection,
			typename TLambda,
			typename TFilterLambda,  
			typename U = TypeTraits::InvockableResult<TLambda, const TPair&>
		>
		requires (Concepts::IsInvockable<TLambda, const TPair&> &&
				  Concepts::IsInvockable<TFilterLambda, const TPair&> &&
				  Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<U>> &&
				  Concepts::IsSame<bool, TypeTraits::InvockableResult<TFilterLambda, const TPair&>> &&
				  !NewCollection<U>::IsReadonly)
		NewCollection<U> CopyToCollectionIf(TFilterLambda&& filterLambda, TLambda&& lambda) const {
			if constexpr (NewCollection<U>::IsDynamic == false)
			{
				TFilterLambda filterCopy = filterLambda;
				NewCollection<U> result = NewCollection<U>(static_cast<const ThisCollection&>(*this)
					.GetCountIf(MoveRValue(filterCopy)));

				SizeT index = 0;
				this->ForEach([&] (const TPair& pair) InlineLambda
				{
					if (filterLambda(pair))
					{
						result.UnsafeSet(lambda(pair), index);
						index++;
					}
				});
				return result;
			}
			else
			{
				NewCollection<U> result;
						
				this->ForEach([&] (const TPair& pair) InlineLambda
				{
					if (filterLambda(pair))
					{
						result.UnsafeAdd(lambda(pair));
					}
				});
				return result;
			}
		}

		/// <summary>
		/// Copies elements to a collection
		/// </summary>
		/// <typeparam name="Lambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="U">Elements type in new collection</typeparam>
		/// <returns>A collection with copy of all elements</returns>
		template<
			template<typename UHeap, typename U> class NewCollection,
			typename UHeap,
			typename TLambda,
			typename U = typename TypeTraits::LambdaType<TLambda>::ReturnType
		>
		requires(
				Concepts::IsInvockable<TLambda, const TPair&>&&
				Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<UHeap, U>> &&
				!NewCollection<UHeap, U>::IsReadonly)
		NewCollection<UHeap, U> CopyToCollection(TLambda&& lambda) const {
			if constexpr (NewCollection<UHeap, U>::IsDynamic == false)
			{
				NewCollection<UHeap, U> result = NewCollection<UHeap, U>(static_cast<const ThisCollection&>(*this).GetCount());
						
				this->ForEach([&] (const TPair& pair, SizeT index) InlineLambda
				{
					result.UnsafeSet(lambda(pair), index);
				});
				return result;
			}
			else
			{
				NewCollection<UHeap, U> result;
						
				this->ForEach([&] (const TPair& pair) InlineLambda
				{
					result.UnsafeAdd(lambda(pair));
				});
				return result;
			}
		}

		/// <summary>
		/// Copies elements to a collection
		/// </summary>
		/// <typeparam name="Lambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="U">Elements type in new collection</typeparam>
		/// <returns>A collection with copy of all elements</returns>
		template<
			template<typename U> class NewCollection,
			typename TLambda,
			typename U = typename TypeTraits::LambdaType<TLambda>::ReturnType
		>
		requires(Concepts::IsInvockable<TLambda, const TPair&>&&
				 Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<U>> &&
				 !NewCollection<U>::IsReadonly)
		NewCollection<U> CopyToCollection(TLambda&& lambda) const {
			if constexpr (NewCollection<U>::IsDynamic == false)
			{
				NewCollection<U> result = NewCollection<U>(static_cast<const ThisCollection&>(*this).GetCount());
						
				this->ForEach([&] (const TPair& pair, SizeT index) InlineLambda
				{
					result.UnsafeSet(lambda(pair), index);
				});
				return result;
			}
			else
			{
				NewCollection<U> result;
						
				this->ForEach([&] (const TPair& pair) InlineLambda
				{
					result.UnsafeAdd(lambda(pair));
				});
				return result;
			}
		}

		/// <summary>
		/// Copies elements to a collection
		/// </summary>
		/// <typeparam name="Lambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="UHeap">A new collection heap type</typeparam>
		/// <typeparam name="U">Elements type in new collection</typeparam>
		/// <returns>A collection with copy of all elements</returns>
		template<
			template<typename UHeap, typename U> class NewCollection,
			typename UHeap
		>
		requires (Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<UHeap, TPair>> &&
				  !NewCollection<UHeap, TPair>::IsReadonly)
		ForceInline NewCollection<UHeap, TPair> CopyToCollection() const {
			return this->template CopyToCollection<NewCollection, UHeap>([] (const TPair& pair) -> TPair InlineLambda
			{
				return pair;
			});
		}

		/// <summary>
		/// Copies elements to a collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="U">Elements type in new collection</typeparam>
		/// <returns>A collection with copy of all elements</returns>
		template<template<typename U> class NewCollection>
		requires (Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<TPair>> &&
				  !NewCollection<TPair>::IsReadonly)
		ForceInline NewCollection<TPair> CopyToCollection() const {
			return CopyToCollection<NewCollection>([] (const TPair& pair) -> TPair InlineLambda
			{
				return pair;
			});
		}

		/// <summary>
		/// Copies elements to a collection
		/// </summary>
		/// <typeparam name="FilterLambda">Elements filtering lambda type</typeparam>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="U">Elements type in new collection</typeparam>
		/// <returns>A collection with copy of all elements</returns>
		template<
			template<typename U> class NewCollection,
			typename TFilterLambda
		> 
		requires (
			Concepts::IsInvockableWithResult<TFilterLambda, bool, const TPair&>&&
			Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<TPair>> &&
			!NewCollection<TPair>::IsReadonly)
		ForceInline NewCollection<TPair> CopyToCollectionIf(TFilterLambda&& filter) const {
			return CopyToCollectionIf<NewCollection>(MoveRValue(filter), [] (const TPair& pair) -> TPair InlineLambda
			{
				return pair;
			});
		}
	
	public:
		/// <summary>
		/// Copies keys to a collection
		/// </summary>
		/// <typeparam name="Lambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="FilterLambda">Elements filtering lambda type</typeparam>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="UHeap">A new collection heap type</typeparam>
		/// <typeparam name="U">Elements type in new collection</typeparam>
		/// <returns>A collection with copy of all elements</returns>
		template<
			template<typename UHeap, typename U> class NewCollection,
			typename UHeap,
			typename TLambda,
			typename TFilterLambda,  
			typename U = typename TypeTraits::LambdaType<TLambda>::ReturnType
		>
		requires(Concepts::IsInvockable<TLambda, const TKey&>&&
				 Concepts::IsInvockable<TFilterLambda, const TKey&>&&
				 Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<UHeap, U>>&&
				 Concepts::IsSame<bool, TypeTraits::InvockableResult<TFilterLambda, const TKey&>>&&
				 NewCollection<UHeap, U>::IsReadonly)
		NewCollection<UHeap, U> CopyKeyToCollection(TFilterLambda&& filterLambda, TLambda&& lambda) const {
			if constexpr (NewCollection<UHeap, U>::IsDynamic == false)
			{
				NewCollection<UHeap, U> result = NewCollection<UHeap, U>(static_cast<const ThisCollection&>(*this)
					.GetCountIf([&] (const TPair& pair) InlineLambda
				{
					return filterLambda(pair.Key);
				}));

				SizeT Index = 0;
				this->ForEachKey([&] (const TKey& key) InlineLambda
				{
					if (filterLambda(key))
					{
						result.UnsafeSet(lambda(key), Index);
					}
				});
				return result;
			}
			else
			{
				NewCollection<UHeap, U> result;
						
				this->ForEachKey([&] (const TKey& key) InlineLambda
				{
					if (filterLambda(key))
					{
						result.UnsafeAdd(lambda(key));
					}
				});
				return result;
			}
		}

		/// <summary>
		/// Copies keys to a collection
		/// </summary>
		/// <typeparam name="Lambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="FilterLambda">Elements filtering lambda type</typeparam>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="UHeap">A new collection heap type</typeparam>
		/// <typeparam name="U">Elements type in new collection</typeparam>
		/// <returns>A collection with copy of all elements</returns>
		template<
			template<typename U> class NewCollection,
			typename TLambda,
			typename TFilterLambda,  
			typename U = typename TypeTraits::LambdaType<TLambda>::ReturnType
		>
		requires (Concepts::IsInvockable<TLambda, const TKey&>&&
				  Concepts::IsInvockableWithResult<TFilterLambda, bool, const TKey&>&&
				  Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<U>>&&
				  Concepts::IsSame<bool, TypeTraits::InvockableResult<TFilterLambda, const TKey&>> &&
				  !NewCollection<U>::IsReadonly)
		NewCollection<U> CopyKeyToCollectionIf(TFilterLambda&& filterLambda, TLambda&& lambda) const {
			if constexpr (NewCollection<U>::IsDynamic == false)
			{
				NewCollection<U> result = NewCollection<U>(static_cast<const ThisCollection&>(*this) 
					.GetCountIf([&] (const TPair& pair) InlineLambda
				{
					return filterLambda(pair.Key);
				}));

				this->ForEachKey([&] (const TKey& key, SizeT index) InlineLambda
				{
					if (filterLambda(key))
					{
						result.UnsafeSet(lambda(key), index);
					}
				});
				return result;
			}
			else
			{
				NewCollection<U> result;
				this->ForEachKey([&] (const TKey& key) InlineLambda
				{
					if (filterLambda(key))
					{
						result.UnsafeAdd(lambda(key));
					}
				});
				return result;
			}
		}

		/// <summary>
		/// Copies keys to a collection
		/// </summary>
		/// <typeparam name="Lambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="UHeap">A new collection heap type</typeparam>
		/// <typeparam name="U">Elements type in new collection</typeparam>
		/// <returns>A collection with copy of all elements</returns>
		template<
			template<typename UHeap, typename U> class NewCollection,
			typename UHeap,
			typename TLambda,
			typename U = typename TypeTraits::LambdaType<TLambda>::ReturnType
		>
		requires(
			Concepts::IsInvockableWithResult<TLambda, U, const TKey&>&&
			Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<UHeap, U>> &&
			!NewCollection<UHeap, U>::IsReadonly)
		NewCollection<UHeap, U> CopyKeyToCollection(TLambda&& lambda) const {
			if constexpr (NewCollection<UHeap, U>::IsDynamic == false)
			{
				NewCollection<UHeap, U> result = NewCollection<UHeap, U>(static_cast<const ThisCollection&>(*this).GetCount());
						
				this->ForEachKey([&] (const TKey& pair, SizeT index) InlineLambda
				{
					result.UnsafeSet(lambda(pair), index);
				});
				return result;
			}
			else
			{
				NewCollection<UHeap, U> result;
						
				this->ForEachKey([&] (const TKey& pair) InlineLambda
				{
					result.UnsafeAdd(lambda(pair));
				});
				return result;
			}
		}

		/// <summary>
		/// Copies keys to a collection
		/// </summary>
		/// <typeparam name="Lambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="U">Elements type in new collection</typeparam>
		/// <returns>A collection with copy of all elements</returns>
		template<
			template<typename U> class NewCollection,
			typename TLambda,
			typename U = typename TypeTraits::LambdaType<TLambda>::ReturnType
		>
		requires(
			Concepts::IsInvockable<TLambda, const TKey&>&&
			Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<U>> &&
			!NewCollection<U>::IsReadonly)
		NewCollection<U> CopyKeyToCollection(TLambda&& lambda) const {
			if constexpr (NewCollection<U>::IsDynamic == false)
			{
				NewCollection<U> result = NewCollection<U>(static_cast<const ThisCollection&>(*this).GetCount());
						
				this->ForEachKey([&] (const TKey& pair, SizeT index) InlineLambda
				{
					result.UnsafeSet(lambda(pair), index);
				});
				return result;
			}
			else
			{
				NewCollection<U> result;
						
				this->ForEachKey([&] (const TKey& pair) InlineLambda
				{
					result.UnsafeAdd(lambda(pair));
				});
				return result;
			}
		}

		/// <summary>
		/// Copies keys to a collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <returns>A collection with copy of all elements</returns>
		template<template<typename U> class NewCollection>
		requires(
			Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<TKey>> &&
			!NewCollection<TKey>::IsReadonly)
		ForceInline NewCollection<TKey> CopyKeyToCollection() const {
			return CopyKeyToCollection<NewCollection>([] (const TKey& pair) -> TKey InlineLambda
			{
				return pair;
			});
		}

		/// <summary>
		/// Copies keys to a collection
		/// </summary>
		/// <typeparam name="FilterLambda">Elements filtering lambda type</typeparam>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="U">Elements type in new collection</typeparam>
		/// <returns>A collection with copy of all elements</returns>
		template<
			template<typename U> class NewCollection,
			typename TFilterLambda
		> 
		requires (
			Concepts::IsInvockableWithResult<TFilterLambda, bool, const TKey&>&&
			Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<TKey>> &&
			!NewCollection<TKey>::IsReadonly)
		ForceInline NewCollection<TKey> CopyKeyToCollectionIf(TFilterLambda&& filter) const {
			return CopyKeyToCollectionIf<NewCollection>(MoveRValue(filter), [] (const TKey& item) -> TKey InlineLambda
			{
				return item;
			});
		}
	
	public:
		/// <summary>
		/// Copies values to a collection
		/// </summary>
		/// <typeparam name="Lambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="FilterLambda">Elements filtering lambda type</typeparam>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="UHeap">A new collection heap type</typeparam>
		/// <typeparam name="U">Elements type in new collection</typeparam>
		/// <returns>A collection with copy of all elements</returns>
		template<
			template<typename UHeap, typename U> class NewCollection,
			typename UHeap,
			typename TLambda,
			typename TFilterLambda, 
			typename U = typename TypeTraits::LambdaType<TLambda>::ReturnType
		>
		requires (
			Concepts::IsInvockable<TLambda, const TValue&>&&
			Concepts::IsInvockableWithResult<TFilterLambda, bool, const TValue&>&&
			Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<UHeap, U>> &&
			!NewCollection<UHeap, U>::IsReadonly)
		NewCollection<UHeap, U> CopyValueToCollection(TFilterLambda&& filterLambda, TLambda&& lambda) const {
			if constexpr (NewCollection<UHeap, U>::IsDynamic == false)
			{
				NewCollection<UHeap, U> result = NewCollection<UHeap, U>(static_cast<const ThisCollection&>(*this)
					.GetCountIf([&](const TPair& pair) InlineLambda
						{
							return filterLambda(pair.Value);
						}));

				SizeT Index = 0;
				this->ForEachValue([&](const TValue& value) InlineLambda
					{
						if (filterLambda(value))
						{
							result.UnsafeSet(lambda(value), Index);
							Index++;
						}
					});
				return result;
			}
			else
			{
				NewCollection<UHeap, U> result;
				this->ForEachValue([&](const TValue& value) InlineLambda
					{
						if (filterLambda(value))
						{
							result.UnsafeAdd(lambda(value));
						}
					});
				return result;
			}
		}

		/// <summary>
		/// Copies values to a collection
		/// </summary>
		/// <typeparam name="Lambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="FilterLambda">Elements filtering lambda type</typeparam>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="U">Elements type in new collection</typeparam>
		/// <returns>A collection with copy of all elements</returns>
		template<
			template<typename U> class NewCollection,
			typename TLambda,
			typename TFilterLambda,
			typename U = typename TypeTraits::LambdaType<TLambda>::ReturnType
		>
		requires(
			Concepts::IsInvockable<TLambda, const TValue&>&&
			Concepts::IsInvockable<TFilterLambda, const TValue&>&&
			Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<U>>&&
			Concepts::IsSame<bool, TypeTraits::InvockableResult<TFilterLambda, const TValue&>> &&
			!NewCollection<U>::IsReadonly)
		NewCollection<U> CopyValueIfToCollection(TFilterLambda&& filterLambda, TLambda&& lambda) const {
				if constexpr (NewCollection<U>::IsDynamic == false)
				{
					NewCollection<U> result = NewCollection<U>(static_cast<const ThisCollection&>(*this)
						.GetCountIf([&] (const TPair& pair) InlineLambda
					{
						return filterLambda(pair.Value);
					}));

				SizeT index = 0;
				this->ForEachValue([&] (const TValue& value) InlineLambda
				{
					if (filterLambda(value))
					{
						result.UnsafeSet(lambda(value), index);
						index++;
					}
				});
				return result;
			}
			else
			{
				NewCollection<U> result;
				this->ForEachValue([&] (const TValue& value) InlineLambda
				{
					if (filterLambda(value))
					{
						result.UnsafeAdd(lambda(value));
					}
				});
				return result;
			}
		}

		/// <summary>
		/// Copies values to a collection
		/// </summary>
		/// <typeparam name="Lambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="UHeap">A new collection heap type</typeparam>
		/// <typeparam name="U">Elements type in new collection</typeparam>
		/// <returns>A collection with copy of all elements</returns>
		template<
			template<typename UHeap, typename U> class NewCollection,
			typename TLambda,
			typename UHeap, 
			typename U = typename TypeTraits::LambdaType<TLambda>::ReturnType
		>
		requires(Concepts::IsInvockable<TLambda, const TValue&>&&
				 Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<UHeap, U>> &&
				 !NewCollection<UHeap, U>::IsReadonly)
		NewCollection<UHeap, U> CopyValueToCollection(TLambda&& lambda) const {
			if constexpr (NewCollection<THeap, U>::IsDynamic == false)
			{
				NewCollection<THeap, U> result = NewCollection<THeap, U>(static_cast<const ThisCollection&>(*this).GetCount());
				this->ForEachValue([&] (const TValue& pair, SizeT index) InlineLambda
				{
					result.UnsafeSet(lambda(pair), index);
				});
				return result;
			}
			else
			{
				NewCollection<THeap, U> result;
				this->ForEachValue([&] (const TValue& pair) InlineLambda
				{
					result.UnsafeAdd(lambda(pair));
				});
				return result;
			}
		}

		/// <summary>
		/// Copies values to a collection
		/// </summary>
		/// <typeparam name="Lambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="U">Elements type in new collection</typeparam>
		/// <returns>A collection with copy of all elements</returns>
		template<
			template<typename U> class NewCollection,
			typename TLambda,
			typename U = typename TypeTraits::LambdaType<TLambda>::ReturnType
		>
		requires(Concepts::IsInvockable<TLambda, const TValue&>&&
				 Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<U>> &&
				 !NewCollection<U>::IsReadonly)
		NewCollection<U> CopyValueToCollection(TLambda&& lambda) const {
			if constexpr (NewCollection<U>::IsDynamic == false)
			{
				NewCollection<U> result = NewCollection<U>(static_cast<const ThisCollection&>(*this).GetCount());
				this->ForEachValue([&] (const TValue& pair, SizeT index) InlineLambda
				{
					result.UnsafeSet(lambda(pair), index);
				});
				return result;
			}
			else
			{
				NewCollection<U> result;
				this->ForEachValue([&] (const TValue& pair) InlineLambda
				{
					result.UnsafeAdd(lambda(pair));
				});
				return result;
			}
		}

		/// <summary>
		/// Copies values to a collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <returns>A collection with copy of all elements</returns>
		template<template<typename U> class NewCollection>
		requires(
			Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<TValue>> &&
			!NewCollection<TValue>::IsReadonly)
		NewCollection<TValue> CopyValueToCollection() const {
			return CopyValueToCollection<NewCollection>([] (const TValue& pair) -> TValue
			{
				return pair;
			});
		}

		/// <summary>
		/// Copies values to a collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="U">Elements type in new collection</typeparam>
		/// <returns>A collection with copy of all elements</returns>
		template<
			template<typename U> class NewCollection,
			typename TFilterLambda
		>
		requires(
			Concepts::IsInvockable<TFilterLambda, const TValue&>&&
			Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<TValue>>&&
			Concepts::IsSame<bool, TypeTraits::InvockableResult<TFilterLambda, const TValue&>> &&
			!NewCollection<TValue>::IsReadonly)
		ForceInline NewCollection<TValue> CopyValueIfToCollection(TFilterLambda&& filter) const {
			return CopyValueIfToCollection<NewCollection>(MoveRValue(filter), [] (const TValue& pair) -> TValue InlineLambda
			{
				return pair;
			});
		}
	};
}