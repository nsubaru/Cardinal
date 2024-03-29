export module Cardinal.Containers.LINQ:ICollection;

export import :Base;

import Cardinal.Exceptions.Internals;

#pragma warning(push)
#pragma warning(disable: 4996)

export namespace Cardinal::Containers::LINQ
{
	/// <summary>
	/// "LINQ" extension for generic collections
	/// </summary>
	/// <typeparam name="TCollection">Collection type</typeparam>
	/// <typeparam name="THeap">Heap type</typeparam>
	/// <typeparam name="T">Type of elements</typeparam>
	/// <typeparam name="TIsDynamic">If collection is dynamically changes the size</typeparam>
	/// <typeparam name="TIsConcurencySuport">If collection supports multithreading</typeparam>
	template<
		template<typename, typename> typename TCollection,
		typename THeap,
		typename T,
		Boolean TIsDynamic,
		Boolean TIsConcurencySuport,
		Boolean TIsReadonly = false,
		Boolean TIsCollectionCanReturnRef = true
	>
	VirtualClass ICollection : public ICollectionBase<TCollection, THeap, T, TIsDynamic, TIsConcurencySuport, TIsReadonly, TIsCollectionCanReturnRef>
	{
	protected:
		using ThisCollection = TCollection<THeap, T>;

		template<typename U>
		using ThisCollectionTemplate = TCollection<THeap, U>;
	
	public:
		/// <summary>
		/// Copies collection elements to a new collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <returns>New collection with the copies of elements</returns>
		template<template <typename> typename NewCollection>
			requires (Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<T>> && !TIsReadonly)
		NewCollection<T> CopyTo() {
			decltype(auto) thisCollectionRef = static_cast<const ThisCollection&>(*this);
			if constexpr (NewCollection<T>::IsDynamic == false) {
				NewCollection<T> instance = NewCollection<T>(thisCollectionRef.Count);

				thisCollectionRef.ForEach([&](const T& item, SizeT index) InlineLambda {
					instance.UnsafeSet(item, index);
				});

				return instance;
			}
			else {
				NewCollection<T> instance = NewCollection<T>();

				thisCollectionRef.ForEach([&](const T& item) InlineLambda {
					instance.UnsafeAdd(item);
				});

				return instance;
			}
		}

		/// <summary>
		/// Copies collection elements to a new collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <returns>New collection with the copies of elements</returns>
		template<template <typename> typename NewCollection>
			requires (Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<T>> && !TIsReadonly)
		NewCollection<T> CopyTo() const {
			decltype(auto) thisCollectionRef = static_cast<const ThisCollection&>(*this);
			if constexpr (NewCollection<T>::IsDynamic == false) {
				NewCollection<T> instance = NewCollection<T>(thisCollectionRef.Count);

				thisCollectionRef.ForEach([&](const T& item, SizeT index) InlineLambda {
					instance.UnsafeSet(item, index);
				});

				return instance;
			}
			else {
				NewCollection<T> instance = NewCollection<T>();

				thisCollectionRef.ForEach([&](const T& item) InlineLambda {
					instance.UnsafeAdd(item);
				});

				return instance;
			}
		}

		/// <summary>
		/// Copies collection elements to a new associable collection
		/// </summary>
		/// <typeparam name="NewCollection">New associable collection type</typeparam>
		/// <returns>New associable collection with the copies of elements</returns>
		template<template <typename, typename> typename NewCollection>
		requires (
			Concepts::IsBasedOn<IPairCollectionGenericBase, T> &&
			Concepts::IsBasedOn<IAssociableCollectionGenericBase, NewCollection<typename T::KeyType, typename T::ValueType>>)
		auto CopyTo() {
			decltype(auto) thisCollectionRef = static_cast<const ThisCollection&>(*this);
			using NewCollectionT = NewCollection<typename T::KeyType, typename T::ValueType>;

			if constexpr (NewCollectionT::IsDynamic == false) {
				NewCollectionT instance = NewCollectionT(thisCollectionRef.Count);
				thisCollectionRef.ForEach([&](const T& item, SizeT index) InlineLambda {
					instance.UnsafeSet(item, index);
				});
				return instance;
			}
			else {
				NewCollectionT instance = NewCollectionT();
				thisCollectionRef.ForEach([&](const T& item) InlineLambda {
					instance.UnsafeAdd(item);
				});
				return instance;
			}
		}

		/// <summary>
		/// Copies collection elements to a new associable collection
		/// </summary>
		/// <typeparam name="NewCollection">New associable collection type</typeparam>
		/// <returns>New associable collection with the copies of elements</returns>
		template<template <typename, typename> typename NewCollection>
		requires (
			Concepts::IsBasedOn<IPairCollectionGenericBase, T> &&
			Concepts::IsBasedOn<IAssociableCollectionGenericBase, NewCollection<typename T::KeyType, typename T::ValueType>>)
		auto CopyTo() const {
			decltype(auto) thisCollectionRef = static_cast<const ThisCollection&>(*this);
			using NewCollectionT = NewCollection<typename T::KeyType, typename T::ValueType>;

			if constexpr (NewCollectionT::IsDynamic == false) {
				NewCollectionT instance = NewCollectionT(thisCollectionRef.Count);
				thisCollectionRef.ForEach([&](const T& item, SizeT index) InlineLambda {
					instance.UnsafeSet(item, index);
				});
				return instance;
			}
			else {
				NewCollectionT instance = NewCollectionT();
				thisCollectionRef.ForEach([&](const T& item) InlineLambda {
					instance.UnsafeAdd(item);
				});
				return instance;
			}
		}

		/// <summary>
		/// Moves collection elements to a new collection (removes elements on old collection)
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <returns>New collection with elements</returns>
		template<template <typename> typename NewCollection>
			requires (Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<T>> && !TIsReadonly)
		NewCollection<T> MoveTo() {
			decltype(auto) thisCollectionRef = static_cast<ThisCollection&>(*this);
			if constexpr (NewCollection<T>::IsDynamic == false) {
				NewCollection<T> instance = NewCollection<T>(thisCollectionRef.Count);
				SizeT index = 0;

				thisCollectionRef.RemoveAll([&](T&& item) InlineLambda {
					instance.UnsafeSet(MoveRValue(item), index);
					index++;
				});

				return instance;
			}
			else {
				NewCollection<T> instance = NewCollection<T>();

				thisCollectionRef.RemoveAll([&](T&& item) InlineLambda {
					instance.UnsafeAdd(MoveRValue(item));
				});

				return instance;
			}
		}
	
	public:
		/// <summary>
		/// Copies collection elements to a new collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="TLambda">Elements transforming lambda type</typeparam>
		/// <returns>New collection with the copies of elements</returns>
		template<
			template <typename> typename NewCollection,
			Concepts::CopyFunction<T> TLambda,
			typename U = TypeTraits::CopyFunctionRet<TLambda, T>
		>
		requires requires {
			Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<U>>;
			!NewCollection<U>::IsReadonly;
		}
		NewCollection<U> CopyTo(TLambda&& lambda) {
			decltype(auto) thisCollectionRef = static_cast<const ThisCollection&>(*this);
						
			if constexpr (NewCollection<U>::IsDynamic == false) {
				NewCollection<U> instance = NewCollection<U>(thisCollectionRef.Count);
							
				thisCollectionRef.ForEach([&](const T& item, SizeT index) InlineLambda {
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
			else {
				NewCollection<U> instance = NewCollection<U>();
							
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

		/// <summary>
		/// Copies collection elements to a new associable collection
		/// </summary>
		/// <typeparam name="NewCollection">New associable collection type</typeparam>
		/// <typeparam name="TLambda">Elements transforming lambda type</typeparam>
		/// <returns>New associable collection with the copies of elements</returns>
		template<template <typename, typename> typename NewCollection, Concepts::CopyFunction<T> TLambda,
			typename U = TypeTraits::CopyFunctionRet<T, TLambda>
		>
		requires (
			Concepts::IsBasedOn<IPairCollectionGenericBase, U>&&
			Concepts::IsBasedOn<IAssociableCollectionGenericBase, NewCollection<typename U::KeyType, typename U::ValueType>> && !TIsReadonly)
		auto CopyTo(TLambda&& lambda) {
			decltype(auto) thisCollectionRef = static_cast<const ThisCollection&>(*this);
			using NewCollectionT = NewCollection<typename U::KeyType, typename U::ValueType>;

			if constexpr (NewCollectionT::IsDynamic == false) {
				NewCollectionT instance = NewCollectionT(thisCollectionRef.Count);
							
				thisCollectionRef.ForEach([&](const T& item, SizeT index) InlineLambda {
					if constexpr (Concepts::CopyFunctionWithIdx<T, TLambda>)
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
			else {
				NewCollectionT instance = NewCollectionT();
				if constexpr (Concepts::CopyFunctionWithIdx<T, TLambda>)
				{
					thisCollectionRef.ForEach([&] (const T& item, SizeT index) InlineLambda
					{
						instance.UnsafeAdd(lambda(item, index));
					});
				}
				else
				{
					thisCollectionRef.ForEach([&] (const T& item, SizeT index) InlineLambda
					{
						instance.UnsafeAdd(lambda(item));
					});
				}
				return instance;
			}
		}

		/// <summary>
		/// Copies collection elements to a new associable collection
		/// </summary>
		/// <typeparam name="NewCollection">New associable collection type</typeparam>
		/// <typeparam name="TLambda">Elements transforming lambda type</typeparam>
		/// <returns>New associable collection with the copies of elements</returns>
		template<template <typename, typename> typename NewCollection, 
			Concepts::CopyFunction<T> TLambda,
			typename U = TypeTraits::CopyFunctionRet<T, TLambda>
		>
		requires (
			Concepts::IsBasedOn<IPairCollectionGenericBase, U>&&
			Concepts::IsBasedOn<IAssociableCollectionGenericBase, NewCollection<typename U::KeyType, typename U::ValueType>> && !TIsReadonly)
		auto CopyTo(TLambda&& lambda) const {
			decltype(auto) thisCollectionRef = static_cast<const ThisCollection&>(*this);
			using NewCollectionT = NewCollection<typename U::KeyType, typename U::ValueType>;

			if constexpr (NewCollectionT::IsDynamic == false) {
				NewCollectionT instance = NewCollectionT(thisCollectionRef.Count);
							
				thisCollectionRef.ForEach([&](const T& item, SizeT index) InlineLambda {
					if constexpr (Concepts::CopyFunctionWithIdx<T, TLambda>)
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
			else {
				NewCollectionT instance = NewCollectionT();
							
				if constexpr (Concepts::CopyFunctionWithIdx<T, TLambda>)
				{
					thisCollectionRef.ForEach([&] (const T& item, SizeT index) InlineLambda
					{
						instance.UnsafeAdd(lambda(item, index));
					});
				}
				else
				{
					thisCollectionRef.ForEach([&] (const T& item, SizeT index) InlineLambda
					{
						instance.UnsafeAdd(lambda(item));
					});
				}
							
				return instance;
			}
		}

		/// <summary>
		/// Copies collection elements to a new collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="TLambda">Elements transforming lambda type</typeparam>
		/// <returns>New collection with the copies of elements</returns>
		template<template <typename> typename NewCollection,
			Concepts::CopyFunction<T> TLambda,
			typename U = TypeTraits::CopyFunctionRet<T, TLambda>
		>
		requires (
			Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<T>> &&
			!NewCollection<U>::IsReadonly)
		NewCollection<U> CopyTo(TLambda&& lambda) const {
			decltype(auto) thisCollectionRef = static_cast<const ThisCollection&>(*this);
						
			if constexpr (NewCollection<U>::IsDynamic == false) {
				NewCollection<U> instance = NewCollection<U>(thisCollectionRef.Count);
							
				thisCollectionRef.ForEach([&](const T& item, SizeT index) InlineLambda {
					if constexpr (Concepts::CopyFunctionWithIdx<T, TLambda>)
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
			else {
				NewCollection<U> instance = NewCollection<U>();
							
				if constexpr (Concepts::CopyFunctionWithIdx<T, TLambda>)
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

		/// <summary>
		/// Moves collection elements to a new collection (removes elements on old collection)
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <returns>New collection with elements</returns>
		template<template <typename> typename NewCollection,
			Concepts::MoveFunction<T> TLambda,
			typename U = TypeTraits::MoveFunctionRet<T, TLambda>>
		requires (
			Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<T>> &&
			!NewCollection<U>::IsReadonly &&
			!TIsReadonly)
		NewCollection<U> MoveTo(TLambda&& lambda) {
			decltype(auto) thisCollectionRef = static_cast<ThisCollection&>(*this);
						
			if constexpr (NewCollection<U>::IsDynamic == false) {
				NewCollection<U> instance = NewCollection<U>(thisCollectionRef.Count);
							
				SizeT index = 0;
				thisCollectionRef.RemoveAll([&](T&& item) InlineLambda {
					if constexpr (Concepts::CopyFunctionWithIdx<T, TLambda>)
					{
						instance.UnsafeSet(MoveRValue(lambda(MoveRValue(item), index)), index);
					}
					else
					{
						instance.UnsafeSet(MoveRValue(lambda(MoveRValue(item))), index);
					}
					index++;
				});

				return instance;
			}
			else 
			{
				NewCollection<U> instance = NewCollection<U>();	

				if constexpr (Concepts::CopyFunctionWithIdx<T, TLambda>)
				{
					SizeT index = 0;
					thisCollectionRef.RemoveAll([&] (T&& item) InlineLambda
					{
						instance.UnsafeAdd(MoveRValue(lambda(MoveRValue(item), index)));
						index++;
					});
				}
				else
				{
					thisCollectionRef.RemoveAll([&] (T&& item) InlineLambda
					{
						instance.UnsafeAdd(MoveRValue(lambda(MoveRValue(item))));
					});
				}
							
				return instance;
			}
		}
	
	public:
		/// <summary>
		/// Copies elements to a fixed collection
		/// </summary>
		/// <typeparam name="TNewCollection">Fixed collection where the elements will be copied</typeparam>
		/// <param name="ignoreSizeMismatch">Indicates whether ignore the elements number difference</param>
		/// <returns>A new fixed collection which contains copies of elements</returns>
		template<typename TNewFixedCollection>
		requires
			Concepts::IsBasedOn<IFixedCollectionGenericBase, TNewFixedCollection>&&
			Concepts::IsSame<
			typename TNewFixedCollection::DataType,
			typename TLinq<TCollection<THeap, T>, THeap, T, TIsDynamic, true, TIsConcurencySuport>::DataType>
		TNewFixedCollection CopyToFixed(const Boolean ignoreSizeMismatch = false) {
			decltype(auto) thisCollectionRef = static_cast<const TCollection<THeap, T>&>(*this);
			if (ignoreSizeMismatch)
			{
				if (TNewFixedCollection::FixedSize == thisCollectionRef.Count || TNewFixedCollection::FixedSize > thisCollectionRef.Count)
				{
					TNewFixedCollection instance{};

					thisCollectionRef.ForEach([&](const T& item, SizeT index) InlineLambda{
						if constexpr (Concepts::HasCloneMethod<T>)
						{
							instance.UnsafeSet(Cardinal::Clone(item), index);
						}
						else
						{
							instance.UnsafeSet(T(item), index);
						}
					});

					return instance;
				}
				else
				{
					TNewFixedCollection instance{};

					thisCollectionRef.ForEach([&](const T& item, SizeT index) InlineLambda{
						if constexpr (Concepts::HasCloneMethod<T>)
						{
							instance.UnsafeSet(Cardinal::Clone(item), index);
						}
						else
						{
							instance.UnsafeSet(T(item), index);
						}
					}, TNewFixedCollection::FixedSize);

					return instance;
				}
			}
			else
			{
				if (TNewFixedCollection::FixedSize != thisCollectionRef.Count)
					Internals::Exceptions::ThrowIndexOutOfRange(
						SourcePosition,
						thisCollectionRef.Count,
						TNewFixedCollection::FixedSize);
				else
				{
					TNewFixedCollection instance;
					thisCollectionRef.ForEach([&] (const T& item, SizeT index) {
						instance.UnsafeSet(item, index);
						});
					return instance;
				}
			}
		}

		/// <summary>
		/// Copies elements to a fixed collection
		/// </summary>
		/// <typeparam name="TNewCollection">Fixed collection where the elements will be copied</typeparam>
		/// <param name="ignoreSizeMismatch">Indicates whether ignore the elements number difference</param>
		/// <returns>A new fixed collection which contains copies of elements</returns>
		template<typename TNewFixedCollection>
		requires
			Concepts::IsBasedOn<IFixedCollectionGenericBase, TNewFixedCollection>&&
			Concepts::IsSame<
			typename TNewFixedCollection::DataType,
			typename TLinq<TCollection<THeap, T>, THeap, T, TIsDynamic, true, TIsConcurencySuport>::DataType>
		TNewFixedCollection CopyToFixed(Boolean ignoreSizeMismatch = false) const {
			decltype(auto) thisCollectionRef = static_cast<const TCollection<THeap, T>&>(*this);
			if (ignoreSizeMismatch)
			{
				if (TNewFixedCollection::FixedSize == thisCollectionRef.Count || TNewFixedCollection::FixedSize > thisCollectionRef.Count)
				{
					TNewFixedCollection instance{};
					thisCollectionRef.ForEach([&](const T& item, SizeT index) InlineLambda{
						if constexpr (Concepts::HasCloneMethod<T>)
						{
							instance.UnsafeSet(Cardinal::Clone(item), index);
						}
						else
						{
							instance.UnsafeSet(T(item), index);
						}
					});

					return instance;
				}
				else
				{
					TNewFixedCollection instance{};

					thisCollectionRef.ForEach([&](const T& item, SizeT index) InlineLambda{
						if constexpr (Concepts::HasCloneMethod<T>)
						{
							instance.UnsafeSet(Cardinal::Clone(item), index);
						}
						else
						{
							instance.UnsafeSet(T(item), index);
						}
					}, TNewFixedCollection::FixedSize);

					return instance;
				}
			}
			else
			{
				if (TNewFixedCollection::FixedSize != thisCollectionRef.Count)
					Internals::Exceptions::ThrowIndexOutOfRange(
						SourcePosition,
						thisCollectionRef.Count,
						TNewFixedCollection::FixedSize);
				else
				{
					TNewFixedCollection instance{};

					thisCollectionRef.ForEach([&](const T& item, SizeT index) InlineLambda{
						if constexpr (Concepts::HasCloneMethod<T>)
						{
							instance.UnsafeSet(Cardinal::Clone(item), index);
						}
						else
						{
							instance.UnsafeSet(T(item), index);
						}
					});

					return instance;
				}
			}
		}
	
	public:
		/// <summary>
		/// Selectively copies collection elements to a new collection 
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="TLambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="TFilterLambda">Elements predicate lambda type</typeparam>
		/// <returns>New collection with the copies of elements</returns>
		template<template <typename> typename NewCollection = ThisCollectionTemplate,
			Concepts::CopyFunction<T> TLambda,
			Concepts::ConditionFunction<T> TFilterLambda,
			typename U = TypeTraits::CopyFunctionRet<T, TLambda>
		>
		requires (
			Concepts::IsInvockableWithResult<TFilterLambda, bool, const T&>&&
			Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<T>> &&
			!NewCollection<U>::IsReadonly)
		NewCollection<U> CopyToIf(TLambda&& lambda, TFilterLambda&& filterLambda) const {
			decltype(auto) thisCollectionRef = static_cast<const ThisCollection&>(*this);

			if constexpr (NewCollection<U>::IsDynamic == false)
			{
				auto filterLambdaCp = filterLambda;
				NewCollection<U> instance = NewCollection<U>(thisCollectionRef.GetCountIf(MoveRValue(filterLambda)));

				SizeT index = 0;
				thisCollectionRef.ForEach([&](const T& item, UnusedData SizeT oldIdx) InlineLambda{
					bool isNeed = false;

					if constexpr (Concepts::ConditionFunctionWithIdx<T, TFilterLambda>)
					{
						isNeed = filterLambda(item, oldIdx);
					}
					else
					{
						isNeed = filterLambda(item);
					}

					if (isNeed)
					{
						if constexpr (Concepts::CopyFunctionWithIdx<T, TLambda>)
						{
							instance.UnsafeSet(lambda(item, oldIdx), index); index++;
						}
						else
						{
							instance.UnsafeSet(lambda(item), index); index++;
						}
					}
				});

				return instance;
			}
			else
			{
				NewCollection<U> instance = NewCollection<U>();

				thisCollectionRef.ForEach([&](const T& item, UnusedData SizeT oldIdx) InlineLambda
				{
					bool isNeed = false;

					if constexpr (Concepts::ConditionFunctionWithIdx<T, TFilterLambda>)
					{
						isNeed = filterLambda(item, oldIdx);
					}
					else
					{
						isNeed = filterLambda(item);
					}

					if (isNeed)
					{
						if constexpr (Concepts::CopyFunctionWithIdx<T, TLambda>)
						{
							instance.UnsafeAdd(lambda(item, oldIdx));
						}
						else
						{
							instance.UnsafeAdd(lambda(item));
						}
					}
				});

				return instance;
			}
		}

		/// <summary>
		/// Selectively moves collection elements to a new collection 
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="TLambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="TFilterLambda">Elements predicate lambda type</typeparam>
		/// <returns>New collection with moved elements</returns>
		template<template <typename> typename NewCollection,
			Concepts::MoveFunction<T> TLambda,
			Concepts::ConditionFunction<T> TFilterLambda,
			typename U = TypeTraits::MoveFunctionRet<T, TLambda>
		>
		requires
			(Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<T>>)
		NewCollection<U> MoveToIf(TLambda&& lambda, TFilterLambda&& filterLambda) {
			decltype(auto) thisCollectionRef = static_cast<ThisCollection&>(*this);

			if constexpr (NewCollection<U>::IsDynamic == false)
			{
				auto filterLambdaCp = filterLambda;
				NewCollection<U> instance = NewCollection<U>(thisCollectionRef.GetCountIf(MoveRValue(filterLambda)));

				SizeT index = 0; MaybeUnused SizeT oldIdx = 0;
				thisCollectionRef.RemoveAll([&](T&& item) InlineLambda {
					bool isNeed = false;

					if constexpr (Concepts::ConditionFunctionWithIdx<T, TFilterLambda>)
					{
						isNeed = filterLambda(item, oldIdx);
					}
					else
					{
						isNeed = filterLambda(item);
					}

					if (isNeed)
					{
						if constexpr (Concepts::MoveFunctionWithIdx<T, TLambda>)
						{
							instance.UnsafeSet(MoveRValue(lambda(MoveRValue(item), oldIdx)), index); index++;
						}
						else
						{
							instance.UnsafeSet(MoveRValue(lambda(MoveRValue(item))), index); index++;
						}
					}

					if constexpr (TypeTraits::IsIndexNeedForPerformeConditionalMove<T, TFilterLambda, TLambda>)
					{
						oldIdx++;
					}
				});

				return instance;
			}
			else
			{
				NewCollection<U> instance = NewCollection<U>();

				MaybeUnused SizeT oldIdx = 0;
				thisCollectionRef.RemoveAll([&](T&& item) InlineLambda
				{
					bool isNeed = false;

					if constexpr (Concepts::ConditionFunctionWithIdx<T, TFilterLambda>)
					{
						isNeed = filterLambda(item, oldIdx);
					}
					else
					{
						isNeed = filterLambda(item);
					}

					if (isNeed)
					{
						if constexpr (Concepts::CopyFunctionWithIdx<T, TLambda>)
						{
							instance.UnsafeAdd(MoveRValue(lambda(MoveRValue(item), oldIdx)));
						}
						else
						{
							instance.UnsafeAdd(MoveRValue(lambda(MoveRValue(item))));
						}
					}

					if constexpr (TypeTraits::IsIndexNeedForPerformeConditionalMove<T, TFilterLambda, TLambda>)
					{
						oldIdx++;
					}
				});

				return instance;
			}
		}

	public:
		/// <summary>
		/// Selectively copies collection elements to a new collection 
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="TFilterLambda">Elements predicate lambda type</typeparam>
		/// <returns>New collection with the copies of elements</returns>
		template<template <typename> typename NewCollection = ThisCollectionTemplate,
			Concepts::ConditionFunction<T> TFilterLambda = decltype([](const auto& item) {return true; })
		>
		requires
			(Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<T>> &&
			!NewCollection<T>::IsReadonly)
		NewCollection<T> CopyToIf(TFilterLambda&& filterLambda) const {
			return CopyToIf<NewCollection>([] (const auto& item) { return item; }, MoveRValue(filterLambda));
		}

		/// <summary>
		/// Selectively moves collection elements to a new collection 
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="TFilterLambda">Elements predicate lambda type</typeparam>
		/// <returns>New collection with moved elements</returns>
		template<template <typename> typename NewCollection = ThisCollectionTemplate,
			Concepts::ConditionFunction<T> TFilterLambda = decltype([] (const auto& item) {return true; })
		>
		requires
			(Concepts::IsBasedOn<ICollectionGenericBase, NewCollection<T>> &&
			!NewCollection<T>::IsReadonly)
		NewCollection<T> MoveToIf(TFilterLambda&& filterLambda) {
			return MoveToIf<NewCollection>([] (const auto& item) {return item; }, MoveRValue(filterLambda));
		}
	
	public:
		/// <summary>
		/// Copies collection elements to a new collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="TLambda">Elements transforming lambda type</typeparam>
		/// <returns>New collection with the copies of elements</returns>
		template<template <typename> typename NewCollection = ThisCollectionTemplate, Concepts::CopyFunction<T> TLambda, typename U = TypeTraits::InvockableResult<TLambda, const T&>>
		requires (!NewCollection<U>::IsReadonly)
		NewCollection<U> Select(TLambda&& lambda) const & {
			return CopyTo<NewCollection>(MoveRValue(lambda));
		}

		/// <summary>
		/// Copies collection elements to a new collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="TLambda">Elements transforming lambda type</typeparam>
		/// <returns>New collection with the copies of elements</returns>
		template<template <typename> typename NewCollection = ThisCollectionTemplate, Concepts::CopyFunction<T> TLambda, typename U = TypeTraits::InvockableResult<TLambda, const T&>>
		requires (!NewCollection<U>::IsReadonly)
		NewCollection<U> Select(TLambda&& lambda) const&& {
			return CopyTo<NewCollection>(MoveRValue(lambda));
		}

		/// <summary>
		/// Copies collection elements to a new collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="TLambda">Elements transforming lambda type</typeparam>
		/// <returns>New collection with the copies of elements</returns>
		template<template <typename> typename NewCollection = ThisCollectionTemplate, Concepts::CopyFunction<T> TLambda, typename U = TypeTraits::InvockableResult<TLambda, const T&>>
		requires (!NewCollection<U>::IsReadonly)
		NewCollection<U> Select(TLambda&& lambda) & {
			return CopyTo<NewCollection>(MoveRValue(lambda));
		}

		/// <summary>
		/// Moves collection elements to a new collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="TLambda">Elements transforming lambda type</typeparam>
		/// <returns>New collection with moved elements</returns>
		template<template <typename> typename NewCollection = ThisCollectionTemplate, typename TLambda, typename U = TypeTraits::InvockableResult<TLambda, T&&>>
			requires (Concepts::IsInvockable<TLambda, T&&> && !Concepts::IsInvockableReturnVoid<TLambda, T&&> && !NewCollection<U>::IsReadonly && !TIsReadonly)
		NewCollection<U> Select(TLambda&& lambda)&& {
			return static_cast<typename TypeTraits::AddRvalueReferenceV<decltype(*this)>>(*this).MoveTo<NewCollection>(MoveRValue(lambda));
		}

		/// <summary>
		/// Copies collection elements to a new collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <returns>New collection with the copies of elements</returns>
		template<template <typename> typename NewCollection = ThisCollectionTemplate> requires (!NewCollection<T>::IsReadonly)
			NewCollection<T> Select() const& {
			return CopyTo<NewCollection>();
		}

		/// <summary>
		/// Copies the elements of collection into a new collection
		/// </summary>
		/// <typeparam name="NewCollection">The type of new collection</typeparam>
		/// <returns>New collection with copies of elements</returns>
		template<template <typename> typename NewCollection = ThisCollectionTemplate> requires (!NewCollection<T>::IsReadonly)
			NewCollection<T> Select() const&& {
			return CopyTo<NewCollection>();
		}

		/// <summary>
		/// Copies collection elements to a new collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <returns>New collection with the copies of elements</returns>
		template<template <typename> typename NewCollection = ThisCollectionTemplate> requires (!NewCollection<T>::IsReadonly)
		NewCollection<T> Select()& {
			return CopyTo<NewCollection>();
		}

		/// <summary>
		/// Moves collection elements to a new collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <returns>New collection with moved elements</returns>
		template<template <typename> typename NewCollection = ThisCollectionTemplate> requires (!NewCollection<T>::IsReadonly && !TIsReadonly)
			NewCollection<T> Select()&& {
			return static_cast<typename TypeTraits::AddRvalueReferenceV<decltype(*this)>>(*this).MoveTo<NewCollection>();
		}
	
	public:
		/// <summary>
		/// Copies collection elements to a new collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="TLambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="TFilterLambda">Elements predicate lambda type</typeparam>
		/// <returns>New collection with the copies of elements</returns>
		template<
			template <typename> typename NewCollection = ThisCollectionTemplate, 
			Concepts::CopyFunction<T> TLambda,
			Concepts::ConditionFunction<T> TFilterLambda,
			typename U = TypeTraits::CopyFunctionRet<T, TLambda>>
		NewCollection<U> SelectIf(TLambda&& lambda, TFilterLambda&& filterLambda) const&& {
			return CopyToIf<NewCollection>(MoveRValue(lambda), MoveRValue(filterLambda));
		}

		/// <summary>
		/// Copies collection elements to a new collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="TLambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="TFilterLambda">Elements predicate lambda type</typeparam>
		/// <returns>New collection with the copies of elements</returns>
		template<
			template <typename> typename NewCollection = ThisCollectionTemplate,
			Concepts::CopyFunction<T> TLambda,
			Concepts::ConditionFunction<T> TFilterLambda,
			typename U = TypeTraits::CopyFunctionRet<T, TLambda>>
		NewCollection<U> SelectIf(TLambda&& lambda, TFilterLambda&& filterLambda) const& {
			return CopyToIf<NewCollection>(MoveRValue(lambda), MoveRValue(filterLambda));
		}

		/// <summary>
		/// Copies collection elements to a new collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="TLambda">Elements transforming lambda type</typeparam>
 		/// <typeparam name="TFilterLambda">Elements predicate lambda type</typeparam>
		/// <returns>New collection with the copies of elements</returns>
		template<
			template <typename> typename NewCollection = ThisCollectionTemplate,
			Concepts::CopyFunction<T> TLambda,
			Concepts::ConditionFunction<T> TFilterLambda,
			typename U = TypeTraits::CopyFunctionRet<T, TLambda>>
		NewCollection<U> SelectIf(TLambda&& lambda, TFilterLambda&& filterLambda) & {
			return CopyToIf<NewCollection>(MoveRValue(lambda), MoveRValue(filterLambda));
		}

		/// <summary>
		/// Moves collection elements to a new collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="TLambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="TFilterLambda">Elements predicate lambda type</typeparam>
		/// <returns>New collection with moved elements</returns>
		template<template <typename> typename NewCollection = ThisCollectionTemplate, 
			Concepts::MoveFunction<T> TLambda, 
			Concepts::ConditionFunction<T> TFilterLambda,
			typename U = TypeTraits::MoveFunctionRet<T, TLambda>>
		NewCollection<U> SelectIf(TLambda&& lambda, TFilterLambda&& filterLambda)&& {
			return MoveRValue(*this).MoveToIf<NewCollection>(MoveRValue(lambda), MoveRValue(filterLambda));
		}

	public:
		/// <summary>
		/// Copies collection elements to a new collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="TLambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="TFilterLambda">Elements predicate lambda type</typeparam>
		/// <returns>New collection with the copies of elements</returns>
		template<
			template <typename> typename NewCollection = ThisCollectionTemplate,
			Concepts::ConditionFunction<T> TFilterLambda>
		NewCollection<T> SelectIf(TFilterLambda&& filterLambda) const&& {
			return CopyToIf<NewCollection>(MoveRValue(filterLambda));
		}

		/// <summary>
		/// Copies collection elements to a new collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="TLambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="TFilterLambda">Elements predicate lambda type</typeparam>
		/// <returns>New collection with the copies of elements</returns>
		template<
			template <typename> typename NewCollection = ThisCollectionTemplate,
			Concepts::ConditionFunction<T> TFilterLambda>
		NewCollection<T> SelectIf(TFilterLambda&& filterLambda) const& {
			return CopyToIf<NewCollection>(MoveRValue(filterLambda));
		}

		/// <summary>
		/// Copies collection elements to a new collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="TLambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="TFilterLambda">Elements predicate lambda type</typeparam>
		/// <returns>New collection with the copies of elements</returns>
		template<
			template <typename> typename NewCollection = ThisCollectionTemplate,
			Concepts::ConditionFunction<T> TFilterLambda>
		NewCollection<T> SelectIf(TFilterLambda&& filterLambda)& {
			return CopyToIf<NewCollection>(MoveRValue(filterLambda));
		}

		/// <summary>
		/// Moves collection elements to a new collection
		/// </summary>
		/// <typeparam name="NewCollection">New collection type</typeparam>
		/// <typeparam name="TLambda">Elements transforming lambda type</typeparam>
		/// <typeparam name="TFilterLambda">Elements predicate lambda type</typeparam>
		/// <returns>New collection with moved elements</returns>
		template<template <typename> typename NewCollection = ThisCollectionTemplate,
			Concepts::ConditionFunction<T> TFilterLambda>
		NewCollection<T> SelectIf(TFilterLambda&& filterLambda)&& {
			return MoveRValue(*this).MoveToIf<NewCollection>(MoveRValue(filterLambda));
		}

	public:
		/// <summary>
		/// Execute lambda for each element of a collection
		/// </summary>
		/// <typeparam name="TLambda">The lambda type which will be called for every element in collection</typeparam>
		/// <param name="lambda">The lambda which will be called for every element in collection (its appearance is: void(*)(T&amp;) or void(*)(T&amp;, SizeT))</param>
		/// <returns>A reference to a current collection</returns>
		template<typename TLambda> 
		requires (Concepts::IsInvockable<TLambda, T&> || Concepts::IsInvockable<TLambda, T&, SizeT> ||
			Concepts::IsInvockable<TLambda, const T&> || Concepts::IsInvockable<TLambda, const T&, SizeT>)
		ForceInline ThisCollection ForEach(TLambda&& lambda) && {
			static_cast<ThisCollection&>(*this).ForEachImpl(MoveRValue(lambda));

			return MoveRValue(static_cast<ThisCollection&&>(*this));
		}

		/// <summary>
		/// Execute lambda for each element of a collection
		/// </summary>
		/// <typeparam name="TLambda">The lambda type which will be called for every element in collection</typeparam>
		/// <param name="lambda">The lambda which will be called for every element in collection (its appearance is: void(*)(T&amp;) or void(*)(T&amp;, SizeT))</param>
		/// <returns>A reference to a current collection</returns>
		template<typename TLambda> 
		requires (Concepts::IsInvockable<TLambda, T&> || Concepts::IsInvockable<TLambda, T&, SizeT> ||
			Concepts::IsInvockable<TLambda, const T&> || Concepts::IsInvockable<TLambda, const T&, SizeT>)
		ForceInline ThisCollection& ForEach(TLambda&& lambda) & {
			return static_cast<ThisCollection&>(*this).ForEachImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Reading all the collection elements
		/// </summary>
		/// <typeparam name="TLambda">The lambda type which will be called for every element in collection</typeparam>
		/// <param name="lambda">The lambda which will be called for every element in collection (its appearance is: void(*)(const T&amp;) or void(*)(const T&amp;, SizeT))</param>
		/// <returns>A reference to a current collection</returns>
		template<typename TLambda> 
			requires (Concepts::IsInvockable<TLambda, const T&> || Concepts::IsInvockable<TLambda, const T&, SizeT>)
		ForceInline const ThisCollection& ForEach(TLambda&& lambda) const& {
			return static_cast<const ThisCollection&>(*this).ForEachImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Reading all the collection elements
		/// </summary>
		/// <typeparam name="TLambda">The lambda type which will be called for every element in collection</typeparam>
		/// <param name="lambda">The lambda which will be called for every element in collection (its appearance is: void(*)(T&amp;) or void(*)(T&amp;, SizeT))</param>
		/// <param name="count">A number of elements to read</param>
		/// <returns>A reference to a current collection</returns>
		template<typename TLambda>
		requires (((Concepts::IsInvockable<TLambda, T&> || Concepts::IsInvockable<TLambda, T&, SizeT>) && !TIsReadonly) ||
			Concepts::IsInvockable<TLambda, const T&> || Concepts::IsInvockable<TLambda, const T&, SizeT>)
		ForceInline ThisCollection& ForEach(TLambda&& lambda, SizeT count) && {
			static_cast<ThisCollection&>(*this).ForEachImpl(MoveRValue(lambda), count);

			return MoveRValue(static_cast<ThisCollection&&>(*this));
		}

		/// <summary>
		/// Reading all the collection elements
		/// </summary>
		/// <typeparam name="TLambda">The lambda type which will be called for every element in collection</typeparam>
		/// <param name="lambda">The lambda which will be called for every element in collection (its appearance is: void(*)(T&amp;) or void(*)(T&amp;, SizeT))</param>
		/// <param name="count">A number of elements to read</param>
		/// <returns>A reference to a current collection</returns>
		template<typename TLambda>
		requires (((Concepts::IsInvockable<TLambda, T&> || Concepts::IsInvockable<TLambda, T&, SizeT>) && !TIsReadonly) ||
			Concepts::IsInvockable<TLambda, const T&> || Concepts::IsInvockable<TLambda, const T&, SizeT>)
		ForceInline ThisCollection& ForEach(TLambda&& lambda, SizeT count) & {
			return static_cast<ThisCollection&>(*this).ForEachImpl(MoveRValue(lambda), count);
		}

		/// <summary>
		/// Reading all the collection elements
		/// </summary>
		/// <typeparam name="TLambda">The lambda type which will be called for every element in collection</typeparam>
		/// <param name="lambda">The lambda which will be called for every element in collection (its appearance is: void(*)(const T&amp;) or void(*)(const T&amp;, SizeT))</param>
		/// <param name="count">A number of elements to read</param>
		/// <returns>A reference to a current collection</returns>
		template<typename TLambda> 
			requires (Concepts::IsInvockable<TLambda, const T&> || Concepts::IsInvockable<TLambda, const T&, SizeT>)
		ForceInline const ThisCollection& ForEach(TLambda&& lambda, SizeT count) const& {
			return static_cast<const ThisCollection&>(*this).ForEachImpl(MoveRValue(lambda), count);
		}
	
	public:
		/// <summary>
		/// Aggregate all elements of a collection using lambda passed as argument
		/// </summary>
		/// <typeparam name="U">Output type</typeparam>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">void(*)(U&amp;, const T&amp;) lambda type</param>
		template<typename U = T, typename TLambda>
			requires (Concepts::IsInvockableWithResult<TLambda, U, const U&, const T&> || 
				Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&>)
		ForceInline U Aggregate(TLambda&& lambda) const {
			return static_cast<const ThisCollection&>(*this).template AggregateImpl<U, TLambda>(MoveRValue(lambda), U());
		}

		/// <summary>
		/// Collection elements aggregation
		/// </summary>
		/// <typeparam name="U">Output type</typeparam>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">U&(*)(U&amp;, const T&amp;) lambda type</param>
		/// <param name="startValue">Initial value</param>
		/// <returns>Calculation result</returns>
		template<typename U, typename TLambda>
			requires (Concepts::IsInvockable<TLambda, U&, const T&>)
		ForceInline U Aggregate(TLambda&& lambda, const U& startValue) const {
			return static_cast<const ThisCollection&>(*this).template AggregateImpl<U, TLambda>(MoveRValue(lambda), startValue);
		}

		/// <summary>
		/// Collection elements aggregation
		/// </summary>
		/// <typeparam name="U">Output type</typeparam>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">U&(*)(U&amp;, const T&amp;) lambda type</param>
		/// <param name="startValue">Initial value</param>
		/// <returns>Calculation result</returns>
		template<typename U, typename TLambda>
			requires (Concepts::IsInvockable<TLambda, U&, const T&>)
		ForceInline U Aggregate(TLambda&& lambda, U&& startValue) const {
			return static_cast<const ThisCollection&>(*this).template AggregateImpl<U, TLambda>(MoveRValue(lambda), MoveRValue(startValue));
		}

		/// <summary>
		/// Returns "true" if container has something inside
		/// </summary>
		/// <returns>Returns "true" if container has something inside</returns>
		ForceInline bool Any() const {
			return static_cast<const ThisCollection&>(*this).AnyImpl();
		}

		/// <summary>
		/// Returns "true" if container has something inside that satisfies the condition of a lambda
		/// </summary>
		/// <param name="lambda">bool(*)(const T&amp;) lambda type</param>
		/// <returns>Returns "true" if container has something inside that satisfies the condition of a lambda</returns>
		template<typename TLambda>
		requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
		ForceInline bool Any(TLambda&& lambda) const {
			return static_cast<const ThisCollection&>(*this).AnyImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Returns "true" if all elements in container satisfies the condition of a lambda
		/// </summary>
		/// <param name="lambda">bool(*)(const T&amp;) lambda type</param>
		/// <returns>Returns "true" if all elements in container satisfies the condition of a lambda</returns>
		template<typename TLambda>
		requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
		ForceInline bool All(TLambda&& lambda) const  {
			return static_cast<const ThisCollection&>(*this).AllImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Returns the first element (if it exists) that satisfies the condition of a lambda in collection
		/// </summary>
		/// <param name="lambda">bool(*)(const T&amp;) lambda type</param>
		/// <returns>Returns the first element (if it exists) that satisfies the condition of a lambda in collection</returns>
		template<typename TLambda>
		requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
		ForceInline decltype(auto) First(TLambda&& lambda) {
			return static_cast<ThisCollection&>(*this).FirstImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Returns the last element (if it exists) that satisfies the condition of a lambda in collection
		/// </summary>
		/// <param name="lambda">bool(*)(const T&amp;) lambda type</param>
		/// <returns>Returns the last element (if it exists) that satisfies the condition of a lambda in collection</returns>
		template<typename TLambda>
		requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
		ForceInline decltype(auto) Last(TLambda&& lambda) {
			return static_cast<ThisCollection&>(*this).LastImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Returns the first element that satisfies the condition of lambda or throws exception
		/// </summary>
		/// <param name="lambda">bool(*)(const T&amp;) lambda type</param>
		/// <returns>Returns the first element (if it exists) that satisfies the condition of a lambda in collection</returns>
		template<typename TLambda>
		requires (Concepts::IsInvockableWithResult<TLambda, bool, const T&>&& TIsCollectionCanReturnRef)
		ForceInline decltype(auto) First(TLambda&& lambda) const {
			return static_cast<const ThisCollection&>(*this).FirstImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Returns the first element that satisfies the condition of lambda or throws exception
		/// </summary>
		/// <param name="lambda">bool(*)(const T&amp;) lambda type</param>
		/// <returns>Returns the last element (if it exists) that satisfies the condition of the lambda in collection</returns>
		template<typename TLambda>
		requires (Concepts::IsInvockableWithResult<TLambda, bool, const T&>&& TIsCollectionCanReturnRef)
		ForceInline decltype(auto) Last(TLambda&& lambda) const {
			return static_cast<const ThisCollection&>(*this).LastImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Returns the first element that satisfies the condition of lambda or second argument
		/// </summary>
		/// <param name="lambda">bool(*)(const T&amp;) lambda type</param>
		/// <param name="item">Returned object if container is empty</param>
		/// <returns>Returns the first element (if it exists) that satisfies the condition of a lambda in collection</returns>
		template<typename TLambda>
		requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
		ForceInline T FirstOr(TLambda&& lambda, T&& item) const {
			return static_cast<const ThisCollection&>(*this).FirstOrImpl(MoveRValue(lambda), MoveRValue(item));
		}

		/// <summary>
		/// Returns the last element that satisfies the condition of lambda or second argument
		/// </summary>
		/// <param name="lambda">bool(*)(const T&amp;) lambda type</param>
		/// <param name="item">Returned object if container is empty</param>
		/// <returns>Returns the last element (if it exists) that satisfies the condition of a lambda in collection</returns>
		template<typename TLambda>
		requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
		ForceInline T LastOr(TLambda&& lambda, T&& item) const {
			return static_cast<const ThisCollection&>(*this).LastOrImpl(MoveRValue(lambda), MoveRValue(item));
		}

		/// <summary>
		/// Returns the first element that satisfies the condition of lambda or second argument
		/// </summary>
		/// <param name="lambda">bool(*)(const T&amp;) lambda type</param>
		/// <param name="item">Returned object if container is empty</param>
		/// <returns>Returns the first element (if it exists) that satisfies the condition of a lambda in collection</returns>
		template<typename TLambda>
		requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
		ForceInline T FirstOr(TLambda&& lambda, const T& item) const {
			return static_cast<const ThisCollection&>(*this).FirstOrImpl(MoveRValue(lambda), item);
		}

		/// <summary>
		/// Returns the last element that satisfies the condition of lambda or second argument
		/// </summary>
		/// <param name="lambda">bool(*)(const T&amp;) lambda type</param>
		/// <param name="item">Returned object if container is empty</param>
		/// <returns>Returns the last element (if it exists) that satisfies the condition of a lambda in collection</returns>
		template<typename TLambda>
		requires Concepts::IsInvockableWithResult<TLambda, bool, const T&>
		ForceInline T LastOr(TLambda&& lambda, const T& item) const {
			return static_cast<const ThisCollection&>(*this).LastOrImpl(MoveRValue(lambda), item);
		}

		/// <summary>
		/// Returns the first element that satisfies the condition of lambda or the result of second lambda
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">bool(*)(const T&amp;) lambda type</param>
		/// <param name="otherLambda">Returned object if container is empty</param>
		/// <param name="otherLambda">A lambda which returns object if container is empty</param>
		/// <returns>Returns the first element (if it exists) that satisfies the condition of a lambda in collection</returns>
		template<typename TLambda, typename TGetterLambda>
		requires (Concepts::IsInvockableWithResult<TLambda, bool, const T&>&& Concepts::IsInvockableWithResult<TGetterLambda, T>)
		ForceInline T FirstOr(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			return static_cast<const ThisCollection&>(*this).FirstOrImpl(MoveRValue(lambda), MoveRValue(otherLambda));
		}

		/// <summary>
		/// Returns the last element that satisfies the condition of lambda or the result of second lambda
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">bool(*)(const T&amp;) lambda type</param>
		/// <param name="otherLambda">Returned object if container is empty</param>
		/// <param name="otherLambda">A lambda which returns object if container is empty</param>
		/// <returns>Returns the last element (if it exists) that satisfies the condition of a lambda in collection</returns>
		template<typename TLambda, typename TGetterLambda>
		requires (Concepts::IsInvockableWithResult<TLambda, bool, const T&>&& Concepts::IsInvockableWithResult<TGetterLambda, T>)
		ForceInline T LastOr(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			return static_cast<const ThisCollection&>(*this).LastOrImpl(MoveRValue(lambda), MoveRValue(otherLambda));
		}

		/// <summary>
		/// Delete all elements in a collection
		/// </summary>
		/// <returns>A reference to a current collection object</returns>
		ForceInline ThisCollection& RemoveAll() requires (!TIsReadonly) {
			return static_cast<ThisCollection&>(*this).RemoveAllImpl();
		}

		/// <summary>
		/// Delete all elements in a collection by calling lambda for each element
		/// </summary>
		/// <param name="lambda">void(*)(T&amp;&amp;) lambda type</param>
		/// <returns>A reference to a current collection object</returns>
		template<typename TLambda>
			requires ((Concepts::IsInvockable<TLambda, T&&> || Concepts::IsInvockable<TLambda, T&&, SizeT>) && !TIsReadonly)
		ForceInline ThisCollection& RemoveAll(TLambda&& lambda)  {
			return static_cast<ThisCollection&>(*this).RemoveAllImpl(MoveRValue(lambda));
		}

		/// <summary>
		/// Count a number of elements which satisfies the condition
		/// </summary>
		/// <param name="lambda">bool(*)(const T&amp;) lambda type</param>
		/// <returns>A number of elements which satisfies the condition</returns>
		template<typename TLambda>
			requires (Concepts::IsInvockableWithResult<TLambda, bool, const T&> && !TIsReadonly)
		ForceInline SizeT GetCountIf(TLambda && lambda) const {
			return static_cast<const ThisCollection&>(*this).GetCountIfImpl(MoveRValue(lambda));
		}
	
	public:
		/// <summary>
		/// Returns the first element from the collection which is equal to argument
		/// </summary>
		/// <typeparam name="U">Object type with which compares every element</typeparam>
		/// <param name="value">An object with which compares every element</param>
		/// <returns>An element or "NotFoundException" exception</returns>
		template<typename U>
			requires (Concepts::HaveEqual<T, U>&& TIsCollectionCanReturnRef)
		ForceInline const T& First(const U& value) const {
			return First([&] (const T& item) InlineLambda { return item == value; });
		}

		/// <summary>
		/// Returns the first element from the collection which is equal to argument
		/// </summary>
		/// <typeparam name="U">Object type with which compares every element</typeparam>
		/// <param name="value">An object with which compares every element</param>
		/// <returns>An element or "NotFoundException" exception</returns>
		template<typename U>
			requires (Concepts::HaveEqual<T, U>&& TIsCollectionCanReturnRef)
		ForceInline const T& First(U&& value) const {
			return First([&] (const T& item) InlineLambda{ return item == value; });
		}

		/// <summary>
		/// Returns the last element from the collection which is equal to argument
		/// </summary>
		/// <typeparam name="U">Object type with which compares every element</typeparam>
		/// <param name="value">An object with which compares every element</param>
		/// <returns>An element or "NotFoundException" exception</returns>
		template<typename U>
			requires (Concepts::HaveEqual<T, U>&& TIsCollectionCanReturnRef)
		ForceInline const T& Last(const U& value) const {
			return Last([&] (const T& item) InlineLambda{ return item == value; });
		}

		/// <summary>
		/// Returns the last element from the collection which is equal to argument
		/// </summary>
		/// <typeparam name="U">Object type with which compares every element</typeparam>
		/// <param name="value">An object with which compares every element</param>
		/// <returns>An element or "NotFoundException" exception</returns>
		template<typename U>
			requires (Concepts::HaveEqual<T, U>&& TIsCollectionCanReturnRef)
		ForceInline const T& Last(U&& value) const {
			return Last([&] (const T& item) InlineLambda{ return item == value; });
		}
	
	public:
		/// <summary>
		/// Returns the first element from the collection which is equal to argument
		/// </summary>
		/// <typeparam name="U">Object type with which compares every element</typeparam>
		/// <param name="value">An object with which compares every element</param>
		/// <returns>An element or "NotFoundException" exception</returns>
		template<typename U> 
			requires Concepts::HaveEqual<T, U>
		ForceInline T& First(const U& value) {
			return First([&] (const T& item) InlineLambda { return item == value; });
		}

		/// <summary>
		/// Returns the first element from the collection which is equal to argument
		/// </summary>
		/// <typeparam name="U">Object type with which compares every element</typeparam>
		/// <param name="value">An object with which compares every element</param>
		/// <returns>An element or "NotFoundException" exception</returns>
		template<typename U>
			requires (Concepts::HaveEqual<T, U>&& TIsCollectionCanReturnRef)
		ForceInline T& First(U&& value) {
			return First([&] (const T& item) InlineLambda{ return item == value; });
		}

		/// <summary>
		/// Returns the last element from the collection which is equal to argument
		/// </summary>
		/// <typeparam name="U">Object type with which compares every element</typeparam>
		/// <param name="value">An object with which compares every element</param>
		/// <returns>An element or "NotFoundException" exception</returns>
		template<typename U>
			requires (Concepts::HaveEqual<T, U>&& TIsCollectionCanReturnRef)
		ForceInline T& Last(const U& value) {
			return Last([&] (const T& item) InlineLambda{ return item == value; });
		}

		/// <summary>
		/// Returns the last element from the collection which is equal to argument
		/// </summary>
		/// <typeparam name="U">Object type with which compares every element</typeparam>
		/// <param name="value">An object with which compares every element</param>
		/// <returns>An element or "NotFoundException" exception</returns>
		template<typename U>
			requires (Concepts::HaveEqual<T, U>&& TIsCollectionCanReturnRef)
		ForceInline T& Last(U&& value) {
			return Last([&] (const T& item) InlineLambda{ return item == value; });
		}
	
	public:
		/// <summary>
		/// Returns "true" if at least one element is equal to argument
		/// </summary>
		/// <typeparam name="U">Object type with which compares every element</typeparam>
		/// <param name="value">An object with which compares every element</param>
		/// <returns>"true" if at least one element is equal to argument</returns>
		template<typename U>
			requires Concepts::HaveEqual<T, U>
		ForceInline bool Any(const U& value) const {
			return Any([&] (const T& item) InlineLambda{ return item == value; });
		}

		/// <summary>
		/// Returns "true" if at least one element is equal to argument
		/// </summary>
		/// <typeparam name="U">Object type with which compares every element</typeparam>
		/// <param name="value">An object with which compares every element</param>
		/// <returns>"true" if at least one element is equal to argument</returns>
		template<typename U>
			requires Concepts::HaveEqual<T, U>
		ForceInline bool Any(U&& value) const {
			return Any([&] (const T& item) InlineLambda{ return item == value; });
		}

		/// <summary>
		/// Returns "true" if all the elements are equal to argument
		/// </summary>
		/// <typeparam name="U">Object type with which compares every element</typeparam>
		/// <param name="value">An object with which compares every element</param>
		/// <returns>"true" if all the elements are equal to argument</returns>
		template<typename U>
			requires Concepts::HaveEqual<T, U>
		ForceInline bool All(const U& value) const {
			return All([&] (const T& item) InlineLambda{ return item == value; });
		}

		/// <summary>
		/// Returns true if all the elements are equal to argument
		/// </summary>
		/// <typeparam name="U">Object type with which compares every element</typeparam>
		/// <param name="value">An object with which compares every element</param>
		/// <returns>"true" if all the elements are equal to argument</returns>
		template<typename U>
			requires Concepts::HaveEqual<T, U>
		ForceInline bool All(U&& value) const {
			return All([&] (const T& item) InlineLambda{ return item == value; });
		}
	
	public:
		/// <summary>
		/// Maximal element in collection searching
		/// </summary>
		/// <returns>Maximal value</returns>
		T Max() const requires Concepts::IsComparableWith<T, T>
		{
			decltype(auto) This = static_cast<const ThisCollection&>(*this);

			auto maxItem = [&]() InlineLambda{
				if constexpr (TypeTraits::IsNumeric<T>)
				{
					return Math::Min<T>();
				}
				else
				{
					return This.FirstOr(
						[](UnusedData const auto&) InlineLambda{ return true; },
						[] () -> T InlineLambda { Internals::Exceptions::ThrowNotFound(SourcePosition); }
					);
				}
			}();

			This.ForEach([&maxItem](const T& item){
				if (maxItem < item)
					maxItem = item;
			});

			return maxItem;
		}

		/// <summary>
		/// Maximal element in collection searching using lambda which returns an alement or a field value
		/// </summary>
		/// <param name="lambda">Returning value lambda among which will be searching the maximal value</param>
		/// <returns>Maximal value</returns>
		template<typename TLambda>
			requires Concepts::IsInvockable<TLambda, const T&> && 
			(!Concepts::IsInvockableReturnVoid<TLambda, const T&>) && 
			Concepts::IsComparableWith<TypeTraits::InvockableResult<TLambda, const T&>, TypeTraits::InvockableResult<TLambda, const T&>>
		decltype(auto) Max(TLambda&& lambda) const {
			decltype(auto) This = static_cast<const ThisCollection&>(*this);
						
			auto maxItem = lambda([&]() InlineLambda{
				if constexpr (TypeTraits::IsNumeric<T>)
				{
					return Math::Min<T>();
				}
				else
				{
					return This.FirstOr(
						[](UnusedData const auto&) InlineLambda{ return true; },
						[] () -> T InlineLambda { Internals::Exceptions::ThrowNotFound(SourcePosition); }
					);
				}
			}());

			This.ForEach([&maxItem, lambda = MoveRValue(lambda)](const T& item){
				auto itemValue = lambda(item);
				if (maxItem < itemValue)
					maxItem = itemValue;
			});

			return maxItem;
		}

		/// <summary>
		/// Maximal element in collection searching using lambda which returns an alement or a field value
		/// </summary>
		/// <param name="lambda">Returning value lambda among which will be searching the maximal value</param>
		/// <returns>Maximal value</returns>
		template<typename TLambda>
			requires Concepts::IsInvockable<TLambda, const T&> && 
			(!Concepts::IsInvockableReturnVoid<TLambda, const T&>) && 
			Concepts::IsComparableWith<TypeTraits::InvockableResult<TLambda, const T&>, TypeTraits::InvockableResult<TLambda, const T&>>
		T MaxBy(TLambda&& lambda) const {
			const ThisCollection& This = static_cast<const ThisCollection&>(*this);

			auto maxItem = [&]() InlineLambda{
				if constexpr (TypeTraits::IsNumeric<T>)
				{
					return Math::Min<T>();
				}
				else
				{
					return This.FirstOr(
						[](UnusedData const auto&) InlineLambda{ return true; },
						[] () -> T InlineLambda { Internals::Exceptions::ThrowNotFound(SourcePosition); }
					);
				}
					}();

			This.ForEach([&maxItem, lambda = MoveRValue(lambda)](const T& item){
				if (lambda(maxItem) < lambda(item))
					maxItem = item;
			});

			return maxItem;
		}

		/// <summary>
		/// Minimal element in collection searching
		/// </summary>
		/// <returns>Minimal value</returns>
		T Min() requires Concepts::IsComparableWith<T, T>
		{
			decltype(auto) This = static_cast<const ThisCollection&>(*this);

			auto minItem = [&]() InlineLambda{
				if constexpr (TypeTraits::IsNumeric<T>)
				{
					return Math::Max<T>();
				}
				else
				{
					return This.FirstOr(
						[](UnusedData const auto&) InlineLambda{ return true; },
						[] () -> T InlineLambda { Internals::Exceptions::ThrowNotFound(SourcePosition); }
					);
				}
			}();

			This.ForEach([&minItem] (const T& item){
				if (minItem > item)
					minItem = item;
			});

			return minItem;
		}

		/// <summary>
		/// Minimal element in collection searching using lambda which returns an alement or a field value
		/// </summary>
		/// <param name="lambda">Returning value lambda among which will be searching the minimal value</param>
		/// <returns>Minimal value</returns>
		template<typename TLambda>
		requires Concepts::IsInvockable<TLambda, const T&> &&
			(!Concepts::IsInvockableReturnVoid<TLambda, const T&>) &&
			Concepts::IsComparableWith<TypeTraits::InvockableResult<TLambda, const T&>, TypeTraits::InvockableResult<TLambda, const T&>>
		decltype(auto) Min(TLambda&& lambda) const {
			decltype(auto) This = static_cast<const ThisCollection&>(*this);
			auto minItem = lambda([&]() InlineLambda{
					if constexpr (TypeTraits::IsNumeric<T>)
					{
						return Math::Max<T>();
					}
					else
					{
						return This.FirstOr(
							[](UnusedData const auto&) InlineLambda{ return true; },
							[]() -> T InlineLambda { Internals::Exceptions::ThrowNotFound(SourcePosition); }
						);
					}
			}());

			This.ForEach([&minItem, lambda = MoveRValue(lambda)] (const T& item){
				auto itemValue = lambda(item);
				if (minItem > itemValue)
					minItem = itemValue;
			});

			return minItem;
		}

		/// <summary>
		/// Minimal element in collection searching using lambda which returns an alement or a field value
		/// </summary>
		/// <param name="lambda">Returning value lambda among which will be searching the minimal value</param>
		/// <returns>Minimal value</returns>
		template<typename TLambda>
			requires Concepts::IsInvockable<TLambda, const T&> &&
		(!Concepts::IsInvockableReturnVoid<TLambda, const T&>) &&
			Concepts::IsComparableWith<TypeTraits::InvockableResult<TLambda, const T&>, TypeTraits::InvockableResult<TLambda, const T&>>
			T MinBy(TLambda&& lambda) const {
			decltype(auto) This = static_cast<const ThisCollection&>(*this);
			auto minItem = [&]() InlineLambda{
				if constexpr (TypeTraits::IsNumeric<T>)
				{
					return Math::Max<T>();
				}
				else
				{
					return This.FirstOr(
						[](UnusedData const auto&) InlineLambda{ return true; },
						[]() -> T InlineLambda { Internals::Exceptions::ThrowNotFound(SourcePosition); }
					);
				}
			}();

			This.ForEach([&minItem, lambda = MoveRValue(lambda)] (const T& item) {
				if (lambda(minItem) > lambda(item))
					minItem = item;
			});

			return minItem;
		}
	
	public:
		/// <summary>
		/// Generating collection with elements which generates lambda
		/// </summary>
		/// <param name="generator">Format function-generator T(*)() or T(*)(SizeT)</param>
		/// <param name="count">Number of elements in a collection</param>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		template<typename TLambda>
			requires (
		(Concepts::IsInvockableWithResult<TLambda, T> ||
			Concepts::IsInvockableWithResult<TLambda, T, SizeT>) &&
			!TIsReadonly)
		static ThisCollection Generate(TLambda&& generator, SizeT count) {
			if constexpr (TypeTraits::IsInvockableWithResult<TLambda, T>)
			{
				if constexpr (!ThisCollection::IsDynamic)
				{
					ThisCollection collection = ThisCollection(count);
					for (SizeT i = 0; i < count; i++)
					{
						collection.UnsafeSet(MoveRValue(generator()), i);
					}
					return collection;
				}
				else
				{
					ThisCollection collection = ThisCollection();
					for (SizeT i = 0; i < count; i++)
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
					ThisCollection collection = ThisCollection(count);
					for (SizeT i = 0; i < count; i++)
					{
						collection.UnsafeSet(generator(i), i);
					}
					return collection;
				}
				else
				{
					ThisCollection collection = ThisCollection();
					for (SizeT i = 0; i < count; i++)
					{
						collection.UnsafeAdd(generator(i));
					}
					return collection;
				}
			}
		}
	};
}

#pragma warning(pop)