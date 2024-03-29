export module Cardinal.Containers.Tuple;

export import Cardinal.Core;
export import Cardinal.Containers.LINQ;

namespace Cardinal::Containers::Details
{
	template<SizeT I, typename T, typename ... TRest>
	struct TypeGetter {
		using Type = typename TypeGetter<I - 1, TRest...>::Type;
	};

	template<typename T, typename ... TRest>
	struct TypeGetter<0, T, TRest...> {
		using Type = T;
	};

	template<SizeT Idx, typename T>
	struct V2Leaf {
		T value;

		ForceInline constexpr V2Leaf() requires (Concepts::IsDefaultConstructible<T>) = default;

		ForceInline constexpr V2Leaf(T&& value) : value(ForwardReference<T&&>(value)) {}

		ForceInline constexpr V2Leaf(const V2Leaf& leaf) : value(leaf.value) {}
		ForceInline constexpr V2Leaf(V2Leaf&& leaf) : value(ForwardReference<T&&>(leaf.value)) {}

		ForceInline constexpr V2Leaf& operator=(const V2Leaf& from) {
			this->~V2Leaf();
			this->V2Leaf::V2Leaf(from);
			return *this;
		}

		ForceInline constexpr V2Leaf& operator=(V2Leaf&& from) {
			this->~V2Leaf();
			this->V2Leaf::V2Leaf(MoveRValue(from));
			return *this;
		}
	};

	template<typename TIndexer, typename ...TTypes>
	class V2Leafs;

	template<SizeT ... Idxs, typename ... TTypes>
	struct V2Leafs<typename TypeTraits::Indexator<Idxs...>, TTypes...> : V2Leaf<(sizeof...(TTypes) - 1) - Idxs, TTypes>... {
		static_assert(sizeof...(Idxs) == sizeof...(TTypes));
		static constexpr SizeT Count = sizeof...(TTypes);

		ForceInline constexpr V2Leafs() requires (Concepts::IsDefaultConstructible<TTypes> && ...) : V2Leaf<(sizeof...(TTypes) - 1) - Idxs, TTypes>()... {}

		ForceInline constexpr V2Leafs(TTypes&&... data) : V2Leaf<(sizeof...(TTypes) - 1) - Idxs, TTypes>(ForwardReference<TTypes&&>(data))... {}

		ForceInline constexpr V2Leafs(const V2Leafs& leafs) : V2Leaf<(sizeof...(TTypes) - 1) - Idxs, TTypes>(static_cast<const V2Leaf<(sizeof...(TTypes) - 1) - Idxs, TTypes>&>(leafs)) ... {}
		ForceInline constexpr V2Leafs(V2Leafs&& leafs) : V2Leaf<(sizeof...(TTypes) - 1) - Idxs, TTypes>(static_cast<V2Leaf<(sizeof...(TTypes) - 1) - Idxs, TTypes>&&>(leafs)) ... {}

		template<SizeT Idx> requires (sizeof...(TTypes) > Idx)
			using TypeByIdx = typename TypeGetter<Idx, TTypes...>::Type;

		template<SizeT Idx>
			requires (sizeof...(TTypes) > Idx)
		ForceInline constexpr typename TypeByIdx<Idx> GetByIdx() {
			return static_cast<V2Leaf<Idx, typename TypeGetter<Idx, TTypes...>::Type>&>(*this).value;
		}

		template<SizeT Idx>
			requires (sizeof...(TTypes) > Idx)
		ForceInline constexpr typename TypeByIdx<Idx> GetByIdx() const {
			return static_cast<const V2Leaf<Idx, typename TypeGetter<Idx, TTypes...>::Type>&>(*this).value;
		}

		template<SizeT Idx>
			requires (sizeof...(TTypes) > Idx)
		ForceInline constexpr typename TypeByIdx<Idx>&& MoveByIdx() {
			return MoveRValue(static_cast<V2Leaf<Idx, typename TypeGetter<Idx, TTypes...>::Type>&&>(*this).value);
		}

		template<SizeT Idx>
			requires (sizeof...(TTypes) > Idx)
		ForceInline constexpr typename TypeTraits::AddLvalueReferenceV<TypeTraits::AddConstV<TypeByIdx<Idx>>> GetByIdxAsRef() const {
			return static_cast<const V2Leaf<Idx, typename TypeGetter<Idx, TTypes...>::Type>&>(*this).value;
		}

		template<SizeT Idx>
			requires (sizeof...(TTypes) > Idx)
		ForceInline constexpr typename TypeTraits::AddLvalueReferenceV<TypeByIdx<Idx>> GetByIdxAsRef() {
			return static_cast<V2Leaf<Idx, typename TypeGetter<Idx, TTypes...>::Type>&>(*this).value;
		}
	};
}

export namespace Cardinal::Containers
{
	template<typename ... TTypes>
	class Tuple;
}

namespace Cardinal::Containers::Details
{
	template<typename F, typename TTuple, typename TIndexator>
	struct Executor;

	template<typename F, typename ... TTypes, SizeT ... Idxs>
	struct Executor<F, Tuple<TTypes...>, typename TypeTraits::Indexator<Idxs...>> {
		ForceInline constexpr static decltype(auto) Execute(F&& f, const Tuple<TTypes...>& tuple) {
			if constexpr (TypeTraits::IsSameV<TypeTraits::InvockableResult<F, TTypes...>, void>)
			{
				f(tuple.GetByIdx<(sizeof...(TTypes) - 1) - Idxs>()...);
			}
			else
			{
				return f(tuple.GetByIdx<(sizeof...(TTypes) - 1) - Idxs>()...);
			}
		}

		ForceInline constexpr static decltype(auto) Execute(F&& f, Tuple<TTypes...>& tuple) {
			if constexpr (TypeTraits::IsSameV<TypeTraits::InvockableResult<F, TTypes...>, void>)
			{
				f(tuple.GetByIdx<(sizeof...(TTypes) - 1) - Idxs>()...);
			}
			else
			{
				return f(tuple.GetByIdx<(sizeof...(TTypes) - 1) - Idxs>()...);
			}
		}
	};

	template<typename T, typename F, typename TTuple, typename TIndexator>
	struct MethodExecutor;

	template<typename T, typename F, typename ... TTypes, SizeT ... Idxs>
	struct MethodExecutor<T, F, Tuple<TTypes...>, typename TypeTraits::Indexator<Idxs...>> {
		ForceInline constexpr static decltype(auto) Execute(T& obj, F&& f, const Tuple<TTypes...>& tuple) {
			if constexpr (TypeTraits::IsSameV<TypeTraits::InvockableResult<F, TTypes...>, void>)
			{
				(obj.*f)(tuple.GetByIdx<(sizeof...(TTypes) - 1) - Idxs>()...);
			}
			else
			{
				return (obj.*f)(tuple.GetByIdx<(sizeof...(TTypes) - 1) - Idxs>()...);
			}
		}

		ForceInline constexpr static decltype(auto) Execute(T& obj, F&& f, Tuple<TTypes...>& tuple) {
			if constexpr (TypeTraits::IsSameV<TypeTraits::InvockableResult<F, TTypes...>, void>)
			{
				(obj.*f)(tuple.GetByIdx<(sizeof...(TTypes) - 1) - Idxs>()...);
			}
			else
			{
				return (obj.*f)(tuple.GetByIdx<(sizeof...(TTypes) - 1) - Idxs>()...);
			}
		}
	};

	template<typename F, typename TTuple, typename TIndexator>
	struct Convertor;

	template<typename F, typename ... TTypes, SizeT ... Idxs>
	struct Convertor<F, Tuple<TTypes...>, typename TypeTraits::Indexator<Idxs...>> {
		ForceInline constexpr static decltype(auto) Convert(F&& f, Tuple<TTypes...>& tuple) {
			return Tuple<decltype(f(tuple.GetByIdx<(sizeof...(Idxs) - 1) - Idxs>()))...>(f(tuple.GetByIdx<(sizeof...(Idxs) - 1) - Idxs>())...);
		}

		ForceInline constexpr static decltype(auto) Convert(F&& f, const Tuple<TTypes...>& tuple) {
			return Tuple<decltype(f(tuple.GetByIdx<(sizeof...(Idxs) - 1) - Idxs>()))...>(f(tuple.GetByIdx<(sizeof...(Idxs) - 1) - Idxs>())...);
		}
	};

	template<typename TLambda, typename TTuple, typename TIndexator>
	struct ForEachImpl;

	template<typename TLambda, SizeT I, typename ... TTypes>
	struct ForEachItemImpl {
		ForceInline constexpr static void ForEachItem(TLambda& lambda, Tuple<TTypes...>& tuple) {
			if constexpr (Concepts::IsInvockable<TLambda, decltype(tuple.GetByIdx<I>())&>)
			{
				lambda(tuple.GetByIdxAsRef<I>());
			}
			else
			{
				lambda(tuple.GetByIdxAsRef<I>(), I);
			}

			if constexpr (I + 1 != tuple.Count)
			{
				ForEachItemImpl<TLambda, I + 1, TTypes...>::ForEachItem(lambda, tuple);
			}
		}

		ForceInline constexpr static void ForEachItem(TLambda& lambda, const Tuple<TTypes...>& tuple) {
			if constexpr (Concepts::IsInvockable<TLambda, const decltype(tuple.GetByIdx<I>())&>)
			{
				lambda(tuple.GetByIdxAsRef<I>());
			}
			else
			{
				lambda(tuple.GetByIdxAsRef<I>(), I);
			}

			if constexpr (I + 1 != tuple.Count)
			{
				ForEachItemImpl<TLambda, I + 1, TTypes...>::ForEachItem(lambda, tuple);
			}
		}
	};

	template<typename TLambda, typename ... TTypes, SizeT ... Idxs>
	struct ForEachImpl<TLambda, Tuple<TTypes...>, typename TypeTraits::Indexator<Idxs...>> {
		ForceInline constexpr static void ForEach(TLambda&& lambda, Tuple<TTypes...>& tuple) {
			ForEachItemImpl<TLambda, 0, TTypes...>::ForEachItem(lambda, tuple);
		}

		ForceInline constexpr static void ForEach(TLambda&& lambda, const Tuple<TTypes...>& tuple) {
			ForEachItemImpl<TLambda, 0, TTypes...>::ForEachItem(lambda, tuple);
		}
	};

	template<typename TLambda, SizeT I, typename ... TTypes>
	struct SumItemImpl {
		ForceInline constexpr static void SumItem(TLambda& lambda, Tuple<TTypes...>& tuple) {
			if constexpr (I + 1 != tuple.Count)
			{
				if constexpr (Concepts::IsInvockable<TLambda, decltype(tuple.GetByIdx<I>())&>)
				{
					return lambda(tuple.GetByIdxAsRef<I>()) + SumItemImpl<TLambda, I + 1, TTypes...>::SumItem(lambda, tuple);
				}
				else
				{
					return lambda(tuple.GetByIdxAsRef<I>(), I) + SumItemImpl<TLambda, I + 1, TTypes...>::SumItem(lambda, tuple);
				}
			}
			else
			{
				if constexpr (Concepts::IsInvockable<TLambda, decltype(tuple.GetByIdx<I>())&>)
				{
					return lambda(tuple.GetByIdxAsRef<I>());
				}
				else
				{
					return lambda(tuple.GetByIdxAsRef<I>(), I);
				}
			}
		}

		ForceInline constexpr static decltype(auto) SumItem(TLambda& lambda, const Tuple<TTypes...>& tuple) {
			if constexpr (I + 1 != tuple.Count)
			{
				if constexpr (Concepts::IsInvockable<TLambda, const decltype(tuple.GetByIdx<I>())&>)
				{
					return lambda(tuple.GetByIdxAsRef<I>()) + SumItemImpl<TLambda, I + 1, TTypes...>::SumItem(lambda, tuple);
				}
				else
				{
					return lambda(tuple.GetByIdxAsRef<I>(), I) + SumItemImpl<TLambda, I + 1, TTypes...>::SumItem(lambda, tuple);
				}
			}
			else
			{
				if constexpr (Concepts::IsInvockable<TLambda, const decltype(tuple.GetByIdx<I>())&>)
				{
					return lambda(tuple.GetByIdxAsRef<I>());
				}
				else
				{
					return lambda(tuple.GetByIdxAsRef<I>(), I);
				}
			}
		}
	};

	template<typename TLambda, typename TTuple, typename TIndexator>
	struct SumImpl;

	template<typename TLambda, typename ... TTypes, SizeT ... Idxs>
	struct SumImpl<TLambda, Tuple<TTypes...>, typename TypeTraits::Indexator<Idxs...>> {
		ForceInline constexpr static decltype(auto) Sum(TLambda&& lambda, Tuple<TTypes...>& tuple) {
			return SumItemImpl<TLambda, 0, TTypes...>::SumItem(lambda, tuple);
		}

		ForceInline constexpr static decltype(auto) Sum(TLambda&& lambda, const Tuple<TTypes...>& tuple) {
			return SumItemImpl<TLambda, 0, TTypes...>::SumItem(lambda, tuple);
		}
	};

	template<typename TLambda, typename TTuple, typename TIndexator>
	struct ForEachTypeImpl;

	template<typename TLambda, SizeT I, typename ... TTypes>
	struct ForEachItemTypeImpl {
		ForceInline constexpr static void ForEachItem(TLambda& lambda) {
			using T = typename Tuple<TTypes...>::template ItemType<I>;
			if constexpr (Concepts::IsInvockableTemplateLambda<TLambda, T>)
			{
				lambda.template operator() < T > ();
			}
			else if constexpr (Concepts::IsInvockableTemplateLambdaWithTemplateIndex<TLambda, T, I>)
			{
				lambda.template operator() < T, I > ();
			}
			else if constexpr (Concepts::IsInvockableTemplateLambda<TLambda, T, SizeT>)
			{
				lambda.template operator() < T > (I);
			}
			else
			{
				static_assert(
					!(Concepts::IsInvockableTemplateLambda<TLambda, T> ||
						Concepts::IsInvockableTemplateLambdaWithTemplateIndex<TLambda, T, I> ||
						Concepts::IsInvockableTemplateLambda<TLambda, T, SizeT>)

					);
				__assume(0);
			}

			if constexpr (I + 1 != Tuple<TTypes...>::Count)
			{
				ForEachItemTypeImpl<TLambda, I + 1, TTypes...>::ForEachItem(lambda);
			}
		}
	};

	template<typename TLambda, typename ... TTypes, SizeT ... Idxs>
	struct ForEachTypeImpl<TLambda, Tuple<TTypes...>, typename TypeTraits::Indexator<Idxs...>> {
		ForceInline constexpr static void ForEach(TLambda&& lambda) {
			ForEachItemTypeImpl<TLambda, 0, TTypes...>::ForEachItem(lambda);
		}
	};

	template<typename TLambda, SizeT I, typename ... TTypes>
	struct FirstItemImpl {
		ForceInline constexpr static SizeT FirstItem() {
			using T = typename Tuple<TTypes...>::template ItemType<I>;

			if constexpr (TLambda().template operator() < T > ())
			{
				return I;
			}
			else if constexpr (I + 1 != Tuple<TTypes...>::Count)
			{
				return FirstItemImpl<TLambda, I + 1, TTypes...>::FirstItem();
			}
			else
			{
				return Math::Max<SizeT>();
			}
		}

		ForceInline constexpr static SizeT FirstItem(TLambda&& lambda, const Tuple<TTypes...>& tuple) {
			using T = typename Tuple<TTypes...>::template ItemType<I>;

			if (lambda.template operator() < T > (tuple.GetByIdx<I>()))
			{
				return I;
			}
			else if constexpr (I + 1 != Tuple<TTypes...>::Count)
			{
				return FirstItemImpl<TLambda, I + 1, TTypes...>::FirstItem(MoveRValue(lambda), tuple);
			}
			else
			{
				return Math::Max<SizeT>();
			}
		}
	};

	template<typename TLambda, typename TTuple, typename TIndexator>
	struct FirstImpl;

	template<typename TLambda, typename ... TTypes, SizeT ... Idxs>
	struct FirstImpl<TLambda, Tuple<TTypes...>, typename TypeTraits::Indexator<Idxs...>> {
		ForceInline constexpr static SizeT First() {
			return FirstItemImpl<TLambda, 0, TTypes...>::FirstItem();
		}

		ForceInline constexpr static SizeT First(TLambda&& lambda, const Tuple<TTypes...>& tuple) {
			return FirstItemImpl<TLambda, 0, TTypes...>::FirstItem(MoveRValue(lambda), tuple);
		}
	};

	template<typename TTuple, typename TIndexator>
	struct ReverseImpl;

	template<typename ... TTypes, SizeT ... Idxs>
	struct ReverseImpl<Tuple<TTypes...>, typename TypeTraits::Indexator<Idxs...>> {
		ForceInline constexpr static auto Reverse(const Tuple<TTypes...>& tuple) {
			return Tuple{ tuple.GetByIdx<Idxs>() ... };
		}
	};
}

export namespace Cardinal::Containers
{
	/// <summary>
	/// The cortege
	/// </summary>
	/// <typeparam name="...TTypes">The cortege type</typeparam>
	template<typename ... TTypes>
	class Tuple : LINQ::ITupleGenericBase {
	private:
		using TV2Leafs = typename Details::V2Leafs<typename TypeTraits::Indexer<sizeof...(TTypes)>::Type, TTypes...>;

	private:
		TV2Leafs leafs;

	public:
		/// <summary>
		/// The element type by index
		/// </summary>
		template<SizeT Idx>
		using ItemType = typename TV2Leafs::template TypeByIdx<Idx>;

	public:
		/// <summary>
		/// A number of elements
		/// </summary>
		static constexpr SizeT Count = TV2Leafs::Count;

	public:
		/// <summary>
		/// An empty constructor
		/// </summary>
		constexpr Tuple() requires (Concepts::IsDefaultConstructible<TTypes> && ...) : leafs() {}

	public:
		/// <summary>
		/// A constructor of elements
		/// </summary>
		constexpr Tuple(TTypes... data) : leafs(ForwardReference<TTypes>(data)...) {}

	public:
		/// <summary>
		/// Copying constructor
		/// </summary>
		/// <param name="tuple">Original</param>
		constexpr Tuple(const Tuple& tuple) : leafs(tuple.leafs) {}

		/// <summary>
		/// Moving constructor
		/// </summary>
		/// <param name="tuple">Original</param>
		constexpr Tuple(Tuple&& tuple) : leafs(MoveRValue(tuple.leafs)) {}

	private:
		template<typename T, SizeT...Idxs>
		ForceInline constexpr Tuple<TTypes..., T> AddImpl(T&& newItem, UnusedData TypeTraits::TypeTag<TypeTraits::Indexator<Idxs...>>)&& {
			return { ((TTypes) this->MoveByIdx<(sizeof...(TTypes) - 1) - Idxs>())..., newItem };
		}

		template<SizeT ... Idxs>
		using DiscardedTuple = Tuple<ItemType<(sizeof...(TTypes) - 2) - Idxs>...>;

		template<SizeT...Idxs>
		ForceInline DiscardedTuple<Idxs...> constexpr DiscardImpl(UnusedData TypeTraits::TypeTag<TypeTraits::Indexator<Idxs...>>)&& {
			using TTuple = DiscardedTuple<Idxs...>;
			constexpr SizeT N = (sizeof...(TTypes)) - 2;
			return TTuple{ this->MoveByIdx<N - Idxs>()... };
		}

	public:
		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="tuple">Original</param>
		/// <returns>A reference to a current object</returns>
		ForceInline constexpr Tuple& operator=(const Tuple& tuple) {
			this->~Tuple();
			this->Tuple::Tuple(tuple);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="tuple">Original</param>
		/// <returns>A reference to a current object</returns>
		ForceInline constexpr Tuple& operator=(Tuple&& tuple) {
			this->~Tuple();
			this->Tuple::Tuple(MoveRValue(tuple));
			return *this;
		}

	public:
		/// <summary>
		/// Returns the element by its index
		/// </summary>
		/// <typeparam name="Idx">The index of element from zero</typeparam>
		/// <returns>The copy of element</returns>
		template<SizeT Idx>
			requires (sizeof...(TTypes) > Idx)
		constexpr decltype(auto) GetByIdx() {
			return leafs.GetByIdx<Idx>();
		}

		/// <summary>
		/// Returns the element by its index, it disappears in cortege
		/// </summary>
		/// <typeparam name="Idx">The index of element from zero</typeparam>
		/// <returns>The original of element, it disappears in cortege</returns>
		template<SizeT Idx>
			requires (sizeof...(TTypes) > Idx)
		ForceInline constexpr decltype(auto) MoveByIdx() {
			return leafs.MoveByIdx<Idx>();
		}

		/// <summary>
		/// Returns the element by its index
		/// </summary>
		/// <typeparam name="Idx">The index of element from zero</typeparam>
		/// <returns>The copy of element</returns>
		template<SizeT Idx>
			requires (sizeof...(TTypes) > Idx)
		ForceInline constexpr decltype(auto) GetByIdx() const {
			return leafs.GetByIdx<Idx>();
		}

		/// <summary>
		/// Returns the reference to an element by index
		/// </summary>
		/// <typeparam name="Idx">The index of element from zero</typeparam>
		/// <returns> A reference to element</returns>
		template<SizeT Idx>
			requires (sizeof...(TTypes) > Idx)
		ForceInline constexpr decltype(auto) GetByIdxAsRef() const {
			return leafs.GetByIdxAsRef<Idx>();
		}

		/// <summary>
		/// Returns the reference to an element by index
		/// </summary>
		/// <typeparam name="Idx">The index of element from zero</typeparam>
		/// <returns> A constant reference to element</returns>
		template<SizeT Idx>
			requires (sizeof...(TTypes) > Idx)
		ForceInline constexpr decltype(auto) GetByIdxAsRef() {
			return leafs.GetByIdxAsRef<Idx>();
		}

	public:
		/// <summary>
		/// Calls the "F" functor with cortege arguments
		/// </summary>
		/// <param name = "f">The functor which is need to be called</param>
		/// <typeparam name="F">The functor type</typeparam>
		/// <returns>The functor result if it exists</returns>
		template<typename F>
		ForceInline constexpr decltype(auto) CallWithArgsFromTuple(F&& f) const {
			using TExecutor = typename Details::Executor<F, Tuple<TTypes...>, typename TypeTraits::Indexer<sizeof...(TTypes)>::template Type>;

			if constexpr (TypeTraits::IsSameV<TypeTraits::InvockableResult<F, TTypes...>, void>)
			{
				TExecutor::Execute(ForwardReference<F>(f), *this);
			}
			else
			{
				return TExecutor::Execute(ForwardReference<F>(f), *this);
			}
		}

		/// <summary>
		/// Calls the "F" functor with cortege arguments
		/// </summary>
		/// <param name = "f">The functor which is need to be called</param>
		/// <typeparam name="F">The functor type</typeparam>
		/// <returns>The functor result if it exists</returns>
		template<typename F>
		ForceInline constexpr decltype(auto) CallWithArgsFromTuple(F&& f) {
			using TExecutor = typename Details::Executor<
				F,
				Tuple<TTypes...>,
				typename TypeTraits::Indexer<sizeof...(TTypes)>::template Type>;

			if constexpr (TypeTraits::IsSameV<TypeTraits::InvockableResult<F, TTypes...>, void>)
			{
				TExecutor::Execute(ForwardReference<F>(f), *this);
			}
			else
			{
				return TExecutor::Execute(ForwardReference<F>(f), *this);
			}
		}

		/// <summary>
		/// Calling the chosen method with argumentd from tuple
		/// </summary>
		/// <typeparam name="T">Object type</typeparam>
		/// <typeparam name="F">Method type</typeparam>
		/// <param name="obj">Object</param>
		/// <param name="f">Method</param>
		/// <returns>Method result</returns>
		template<typename T, typename F>
		constexpr decltype(auto) CallMethodWithArgsFromTuple(T& obj, F&& f) {
			using TExecutor = typename Details::MethodExecutor<
				T,
				F,
				Tuple<TTypes...>,
				typename TypeTraits::Indexer<sizeof...(TTypes)>::template Type>;

			if constexpr (TypeTraits::IsSameV<decltype(TExecutor::Execute(obj, ForwardReference<F&&>(f), *this)), void>)
			{
				TExecutor::Execute(obj, ForwardReference<F&&>(f), *this);
			}
			else
			{
				return TExecutor::Execute(obj, ForwardReference<F&&>(f), *this);
			}
		}

	public:
		/// <summary>
		/// Converts the one contege into another using lambda converter
		/// </summary>
		/// <typeparam name="TLambda">The lambda converter type</typeparam>
		/// <param name="lambda">The lambda converter</param>
		/// <returns>The new cortege with the copy of elements</returns>
		template<typename TLambda>
		ForceInline constexpr decltype(auto) ConvertTo(TLambda&& lambda) const {
			using Convertor = typename Details::Convertor<
				TLambda,
				Tuple<TTypes...>,
				typename TypeTraits::Indexer<sizeof...(TTypes)>::template Type>;

			return Convertor::Convert(MoveRValue(lambda), *this);
		}

		/// <summary>
		/// Converts the one contege into another using lambda converter
		/// </summary>
		/// <typeparam name="TLambda">The lambda converter type</typeparam>
		/// <param name="lambda">The lambda converter</param>
		/// <returns>The new cortege with the copy of elements</returns>
		template<typename TLambda>
		ForceInline constexpr decltype(auto) ConvertTo(TLambda&& lambda) {
			using Convertor = typename Details::Convertor<
				TLambda,
				Tuple<TTypes...>,
				typename TypeTraits::Indexer<sizeof...(TTypes)>::template Type>;

			return Convertor::Convert(MoveRValue(lambda), *this);
		}

		/// <summary>
		/// Reads all the cortege elements and calls the lambda for them
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">The lambda</param>
		/// <returns>Returns the reference to the given object</returns>
		template<typename TLambda>
		ForceInline constexpr const Tuple& ForEach(TLambda&& lambda) const& {
			using ForEach = typename Details::ForEachImpl<
				TLambda,
				Tuple<TTypes...>,
				typename TypeTraits::Indexer<sizeof...(TTypes)>::template Type>;

			ForEach::ForEach(MoveRValue(lambda), *this);

			return *this;
		}

		/// <summary>
		/// Reads all the cortege elements and calls the lambda for them
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">The lambda</param>
		/// <returns>Returns the reference to the given object</returns>
		template<typename TLambda>
		ForceInline constexpr Tuple& ForEach(TLambda&& lambda)& {
			using ForEach = typename Details::ForEachImpl<
				TLambda,
				Tuple<TTypes...>,
				typename TypeTraits::Indexer<sizeof...(TTypes)>::template Type>;

			ForEach::ForEach(MoveRValue(lambda), *this);

			return *this;
		}

		/// <summary>
		/// Reads all the cortege elements and calls the lambda for them
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">The lambda</param>
		/// <returns>Returns the reference to the given object</returns>
		template<typename TLambda>
		ForceInline constexpr Tuple&& ForEach(TLambda&& lambda)&& {
			using ForEach = typename Details::ForEachImpl<
				TLambda,
				Tuple<TTypes...>,
				typename TypeTraits::Indexer<sizeof...(TTypes)>::template Type>;

			ForEach::ForEach(MoveRValue(lambda), *this);

			return *this;
		}

		/// <summary>
		/// Sums all the elements using lambda
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">Lambda object</param>
		/// <returns>Sum result</returns>
		template<typename TLambda>
		ForceInline constexpr decltype(auto) Sum(TLambda&& lambda) const {
			using Sum = typename Details::SumImpl<
				TLambda,
				Tuple<TTypes...>,
				typename TypeTraits::Indexer<sizeof...(TTypes)>::template Type>;

			return Sum::Sum(MoveRValue(lambda), *this);
		}

		/// <summary>
		/// Calls the lambda for every type
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">Lambda object</param>
		template<typename TLambda>
		ForceInline static constexpr void ForEachType(TLambda&& lambda) {
			using ForEachType = typename Details::ForEachTypeImpl<
				TLambda,
				Tuple<TTypes...>,
				typename TypeTraits::Indexer<sizeof...(TTypes)>::template Type>;

			ForEachType::ForEach(MoveRValue(lambda));
		}

		/// <summary>
		/// Returns the index of first type which satisfies the lambda condition 
		/// or -1 if that kind of element does not exist
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		template<typename TLambda>
		ForceInline constexpr static SizeT FirstType() {
			using FirstImpl = typename Details::FirstImpl<
				TLambda,
				Tuple<TTypes...>,
				typename TypeTraits::Indexer<sizeof...(TTypes)>::template Type>;

			return FirstImpl::First();
		}

		/// <summary>
		/// Returns the index of first element which satisfies the lambda condition 
		/// or -1 if that kind of element does not exist
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">Lambda object</param>
		template<typename TLambda>
		ForceInline constexpr SizeT FirstItem(TLambda&& lambda) const {
			using FirstImpl = typename Details::FirstImpl<
				TLambda,
				Tuple<TTypes...>,
				typename TypeTraits::Indexer<sizeof...(TTypes)>::template Type>;

			return FirstImpl::First(MoveRValue(lambda), *this);
		}

		/// <summary>
		/// Reverse items in tuple
		/// </summary>
		/// <returns>Reversed tuple</returns>
		ForceInline constexpr auto Reverse() const {
			using ReverseImpl = typename Details::ReverseImpl<
				Tuple<TTypes...>,
				typename TypeTraits::Indexer<sizeof...(TTypes)>::template Type>;

			return ReverseImpl::Reverse(*this);
		}

		/// <summary>
		/// Item addition to tuple
		/// </summary>
		/// <typeparam name="T">Item type</typeparam>
		/// <param name="newItem">New item</param>
		/// <returns>Tuple with new set of items</returns>
		template<typename T>
		ForceInline constexpr Tuple<TTypes..., T> Add(const T& newItem) const& {
			using Indexator = typename TypeTraits::Indexer<sizeof...(TTypes)>::template Type;
			auto tuple = *this;
			return MoveRValue(tuple).AddImpl<T>(MoveRValue(T(newItem)), TypeTraits::TypeTag<Indexator>{});
		}

		/// <summary>
		/// Item addition to tuple
		/// </summary>
		/// <typeparam name="T">Item type</typeparam>
		/// <param name="newItem">New item</param>
		/// <returns>Tuple with new set of items</returns>
		template<typename T>
		ForceInline constexpr Tuple<TTypes..., T> Add(T&& newItem) const& {
			using Indexator = typename TypeTraits::Indexer<sizeof...(TTypes)>::template Type;
			auto tuple = *this;
			return MoveRValue(tuple).AddImpl<T>(MoveRValue(newItem), TypeTraits::TypeTag<Indexator>{});
		}

		/// <summary>
		/// Item addition to tuple
		/// </summary>
		/// <typeparam name="T">Item type</typeparam>
		/// <param name="newItem">New item</param>
		/// <returns>Tuple with new set of items</returns>
		template<typename T>
		ForceInline constexpr Tuple<TTypes..., T> Add(const T& newItem)&& {
			using Indexator = typename TypeTraits::Indexer<sizeof...(TTypes)>::template Type;
			return MoveRValue(*this).AddImpl<T>(MoveRValue(T(newItem)), TypeTraits::TypeTag<Indexator>{});
		}

		/// <summary>
		/// Item addition to tuple
		/// </summary>
		/// <typeparam name="T">Item type</typeparam>
		/// <param name="newItem">New item</param>
		/// <returns>Tuple with new set of items</returns>
		template<typename T>
		ForceInline constexpr Tuple<TTypes..., T> Add(T&& newItem)&& {
			using Indexator = typename TypeTraits::Indexer<sizeof...(TTypes)>::template Type;
			return MoveRValue(*this).AddImpl<T>(MoveRValue(newItem), TypeTraits::TypeTag<Indexator>{});
		}

		/// <summary>
		/// Addition operator to tuple object
		/// </summary>
		/// <typeparam name="T">Item type</typeparam>
		/// <param name="item">New item</param>
		/// <returns>Tuple with new set of items</returns>
		template<typename T>
		constexpr auto operator+(T& item) const& {
			return Add<T>(T(item));
		}

		/// <summary>
		/// Addition operator to tuple object
		/// </summary>
		/// <typeparam name="T">Item type</typeparam>
		/// <param name="item">New item</param>
		/// <returns>Tuple with new set of items</returns>
		template<typename T>
		constexpr auto operator+(const T& item) const& {
			return Add<T>(T(item));
		}

		/// <summary>
		/// Addition operator to tuple object
		/// </summary>
		/// <typeparam name="T">Item type</typeparam>
		/// <param name="item">New item</param>
		/// <returns>Tuple with new set of items</returns>
		template<typename T>
		constexpr auto operator+(T&& item) const& {
			return Add<T>(MoveRValue(item));
		}

		/// <summary>
		/// Addition operator to tuple object
		/// </summary>
		/// <typeparam name="T">Item type</typeparam>
		/// <param name="item">New item</param>
		/// <returns>Tuple with new set of items</returns>
		template<typename T>
		constexpr auto operator+(T& item)&& {
			return MoveRValue(*this).Add<T>(T(item));
		}

		/// <summary>
		/// Addition operator to tuple object
		/// </summary>
		/// <typeparam name="T">Item type</typeparam>
		/// <param name="item">New item</param>
		/// <returns>Tuple with new set of items</returns>
		template<typename T>
		constexpr auto operator+(const T& item)&& {
			return MoveRValue(*this).Add<T>(item);
		}

		/// <summary>
		/// Addition operator to tuple object
		/// </summary>
		/// <typeparam name="T">Item type</typeparam>
		/// <param name="item">New item</param>
		/// <returns>Tuple with new set of items</returns>
		template<typename T>
		constexpr auto operator+(T&& item)&& {
			return MoveRValue(*this).Add<T>(MoveRValue(item));
		}

		/// <summary>
		/// Discard last object and return new tuple with rest items
		/// </summary>
		/// <returns>Tuple with rest items</returns>
		constexpr auto DiscardLastItem()& {
			return Tuple(*this).DiscardLastItem();
		}

		/// <summary>
		/// Discard last object and return new tuple with rest items
		/// </summary>
		/// <returns>Tuple with rest items</returns>
		constexpr auto DiscardLastItem() const& {
			return Tuple(*this).DiscardLastItem();
		}

		/// <summary>
		/// Discard last object and return new tuple with rest items
		/// </summary>
		/// <returns>Tuple with rest items</returns>
		constexpr auto DiscardLastItem()&& {
			using Indexator = typename TypeTraits::Indexer<(sizeof...(TTypes)) - 1>::Type;
			return MoveRValue(*this).DiscardImpl(TypeTraits::TypeTag<Indexator>{});
		}

		/// <summary>
		/// Discard last object and return new tuple with rest items
		/// </summary>
		/// <returns>Tuple with rest items</returns>
		constexpr auto DiscardLastItem() const&& {
			return Tuple(*this).DiscardLastItem();
		}
	};

	/// <summary>
	/// Empty cortege
	/// </summary>
	template<>
	class Tuple<> : LINQ::ITupleGenericBase {
	public:
		/// <summary>
		/// An empty constructor
		/// </summary>
		ForceInline constexpr Tuple() {}

		/// <summary>
		/// Copying constructor
		/// </summary>
		/// <param name="orig">Original</param>
		ForceInline constexpr Tuple(const Tuple& orig) = default;

		/// <summary>
		/// Moving constructor
		/// </summary>
		/// <param name="orig">Original</param>
		ForceInline constexpr Tuple(Tuple&& orig) = default;

	public:
		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="orig">Original</param>
		/// <returns>A reference to a current object</returns>
		ForceInline constexpr Tuple& operator=(const Tuple& orig) = default;

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="orig">Original</param>
		/// <returns>A reference to a current object</returns>
		ForceInline constexpr Tuple& operator=(Tuple&& orig) = default;

	public:
		/// <summary>
		/// A number of elements
		/// </summary>
		static constexpr SizeT Count = 0;

	public:
		/// <summary>
		/// Calls the "F" functor with cortege arguments
		/// </summary>
		/// <param name = "f">The functor which is need to be called</param>
		/// <typeparam name="F">The functor type</typeparam>
		/// <returns>The functor result if it exists</returns>
		template<typename F>
		ForceInline constexpr decltype(auto) CallWithArgsFromTuple(F&& f) {
			if constexpr (TypeTraits::IsSameV<TypeTraits::InvockableResult<F>, void>)
			{
				f();
			}
			else
			{
				return f();
			}
		}

		/// <summary>
		/// Calls the "F" functor with cortege arguments
		/// </summary>
		/// <param name = "f">The functor which is need to be called</param>
		/// <typeparam name="F">The functor type</typeparam>
		/// <returns>The functor result if it exists</returns>
		template<typename F>
		ForceInline constexpr decltype(auto) CallWithArgsFromTuple(F&& f) const {
			if constexpr (TypeTraits::IsSameV<TypeTraits::InvockableResult<F>, void>)
			{
				f();
			}
			else
			{
				return f();
			}
		}

		/// <summary>
		/// Calls the lambda for every type
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">Lambda object</param>
		template<typename TLambda>
		ForceInline static constexpr void ForEachType(TLambda&& lambda) {}

		/// <summary>
		/// Reads all the cortege elements and calls the lambda for them
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">The lambda</param>
		/// <returns>Returns the reference to the given object</returns>
		template<typename TLambda>
		ForceInline constexpr const Tuple& ForEach(TLambda&& lambda) const& {
			return *this;
		}

		/// <summary>
		/// Reads all the cortege elements and calls the lambda for them
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">The lambda</param>
		/// <returns>Returns the reference to the given object</returns>
		template<typename TLambda>
		ForceInline constexpr Tuple& ForEach(TLambda&& lambda)& {
			return *this;
		}


		/// <summary>
		/// Reads all the cortege elements and calls the lambda for them
		/// </summary>
		/// <typeparam name="TLambda">Lambda type</typeparam>
		/// <param name="lambda">The lambda</param>
		/// <returns>Returns the reference to the given object</returns>
		template<typename TLambda>
		ForceInline constexpr Tuple&& ForEach(TLambda&& lambda)&& {
			return MoveRValue(*this);
		}
	};

	template<typename ... TTypes>
	Tuple(TTypes... data)->Tuple<TTypes...>;
}

export namespace Cardinal::TypeTraits
{
	template<typename T>
	struct IsTupleT : LogicFalse {};

	template<typename ... Types>
	struct IsTupleT<Containers::Tuple<Types...>> : LogicTrue {};

	template<typename T>
	constexpr bool IsTupleV = IsTupleT<T>::State;
}

export namespace Cardinal::Concepts
{
	template<typename T>
	concept Tuple = TypeTraits::IsTupleV<T>;
}

export namespace std
{
	template<typename ... TTypes>
	using Tuple = Cardinal::Containers::Tuple<TTypes...>;

#pragma region Tuple element
	template <size_t _Index, class _Tuple>
	struct tuple_element;

	template <size_t TI, typename ... TTypes>
	struct tuple_element<TI, Tuple<TTypes...>> {
		using ItemType = typename Tuple<TTypes...>::template ItemType<TI>;
		using type = ItemType;
	};

	template <size_t TI, typename ... TTypes>
	struct tuple_element<TI, const Tuple<TTypes...>> {
		using ItemType = typename Tuple<TTypes...>::template ItemType<TI>;
		using ConstItemType = Cardinal::TypeTraits::AddConstV<ItemType>;
		using type = ConstItemType;
	};

	template <size_t TI, typename ... TTypes>
	struct tuple_element<TI, volatile Tuple<TTypes...>> {
		using ItemType = typename Tuple<TTypes...>::template ItemType<TI>;
		using VolatileItemType = Cardinal::TypeTraits::AddVolatileV<ItemType>;
		using type = VolatileItemType;
	};

	template <size_t TI, typename ... TTypes>
	struct tuple_element<TI, const volatile Tuple<TTypes...>> {
		using ItemType = typename Tuple<TTypes...>::template ItemType<TI>;
		using CVItemType = Cardinal::TypeTraits::AddCvV<ItemType>;
		using type = CVItemType;
	};

	template <size_t TI, typename ... TTypes>
	struct tuple_element<TI, Tuple<TTypes...>&> {
		using ItemType = typename Tuple<TTypes...>::template ItemType<TI>;
		using type = typename Cardinal::TypeTraits::AddReference<ItemType>::Lvalue;
	};

	template <size_t TI, typename ... TTypes>
	struct tuple_element<TI, const Tuple<TTypes...>&> {
		using ItemType = typename Tuple<TTypes...>::template ItemType<TI>;
		using ConstItemType = Cardinal::TypeTraits::AddConstV<ItemType>;
		using type = typename Cardinal::TypeTraits::AddReference<ConstItemType>::Lvalue;
	};

	template <size_t TI, typename ... TTypes>
	struct tuple_element<TI, volatile Tuple<TTypes...>&> {
		using ItemType = typename Tuple<TTypes...>::template ItemType<TI>;
		using VolatileItemType = Cardinal::TypeTraits::AddVolatileV<ItemType>;
		using type = typename Cardinal::TypeTraits::AddReference<VolatileItemType>::Lvalue;
	};

	template <size_t TI, typename ... TTypes>
	struct tuple_element<TI, const volatile Tuple<TTypes...>&> {
		using ItemType = typename Tuple<TTypes...>::template ItemType<TI>;
		using CVItemType = Cardinal::TypeTraits::AddCvV<ItemType>;
		using type = typename Cardinal::TypeTraits::AddReference<CVItemType>::Lvalue;
	};

	template <size_t TI, typename ... TTypes>
	struct tuple_element<TI, Tuple<TTypes...>&&> {
		using ItemType = typename Tuple<TTypes...>::template ItemType<TI>;
		using type = typename Cardinal::TypeTraits::AddReference<ItemType>::Rvalue;
	};

	template <size_t TI, typename ... TTypes>
	struct tuple_element<TI, const Tuple<TTypes...>&&> {
		using ItemType = typename Tuple<TTypes...>::template ItemType<TI>;
		using ConstItemType = Cardinal::TypeTraits::AddConstV<ItemType>;
		using type = typename Cardinal::TypeTraits::AddReference<ConstItemType>::Rvalue;
	};

	template <size_t TI, typename ... TTypes>
	struct tuple_element<TI, volatile Tuple<TTypes...>&&> {
		using ItemType = typename Tuple<TTypes...>::template ItemType<TI>;
		using VolatileItemType = Cardinal::TypeTraits::AddVolatileV<ItemType>;
		using type = typename Cardinal::TypeTraits::AddReference<VolatileItemType>::Rvalue;
	};

	template <size_t TI, typename ... TTypes>
	struct tuple_element<TI, const volatile Tuple<TTypes...>&&> {
		using ItemType = typename Tuple<TTypes...>::template ItemType<TI>;
		using CVItemType = Cardinal::TypeTraits::AddCvV<ItemType>;
		using type = typename Cardinal::TypeTraits::AddReference<CVItemType>::Rvalue;
	};
#pragma endregion


#pragma region Get item
	template <size_t TI, class... TType>
	ForceInline constexpr tuple_element<TI, Tuple<TType...>&>::type get(Tuple<TType...>& tuple) {
		return tuple.GetByIdxAsRef<TI>();
	}

	template <size_t TI, class... TType>
	ForceInline constexpr tuple_element<TI, const Tuple<TType...>&>::type get(const Tuple<TType...>& tuple) {
		return tuple.GetByIdxAsRef<TI>();
	}

	template <size_t TI, class... TType>
	ForceInline constexpr tuple_element<TI, Tuple<TType...>&&>::type get(Tuple<TType...>&& tuple) {
		return Cardinal::MoveRValue(tuple.MoveByIdx<TI>());
	}

	template <size_t TI, class... TType>
	ForceInline constexpr tuple_element<TI, const Tuple<TType...>&&>::type get(const Tuple<TType...>&& tuple) {
		return Cardinal::MoveRValue(tuple.MoveByIdx<TI>());
	}
#pragma endregion

#pragma region Tuple size
	template<class T>
	struct tuple_size;

	template<typename ... TTypes>
	struct tuple_size<Tuple<TTypes...>> {

		static constexpr size_t value = sizeof...(TTypes);

		ForceInline constexpr operator size_t() const {
			return value;
		}

		ForceInline constexpr size_t operator()() const {
			return value;
		}

		using value_type = size_t;
	};

	template<typename ... TTypes>
	struct tuple_size<const Tuple<TTypes...>> {

		static constexpr size_t value = sizeof...(TTypes);

		ForceInline constexpr operator size_t() const {
			return value;
		}

		ForceInline constexpr size_t operator()() const {
			return value;
		}

		using value_type = size_t;
	};

	template<typename ... TTypes>
	struct tuple_size<const volatile Tuple<TTypes...>> {

		static constexpr size_t value = sizeof...(TTypes);

		ForceInline constexpr operator size_t() const {
			return value;
		}

		ForceInline constexpr size_t operator()() const {
			return value;
		}

		using value_type = size_t;
	};

	template<typename ... TTypes>
	struct tuple_size<Tuple<TTypes...>&> {

		static constexpr size_t value = sizeof...(TTypes);

		ForceInline constexpr operator size_t() const {
			return value;
		}

		ForceInline constexpr size_t operator()() const {
			return value;
		}

		using value_type = size_t;
	};

	template<typename ... TTypes>
	struct tuple_size<const Tuple<TTypes...>&> {

		static constexpr size_t value = sizeof...(TTypes);

		ForceInline constexpr operator size_t() const {
			return value;
		}

		ForceInline constexpr size_t operator()() const {
			return value;
		}

		using value_type = size_t;
	};

	template<typename ... TTypes>
	struct tuple_size<const volatile Tuple<TTypes...>&> {

		static constexpr size_t value = sizeof...(TTypes);

		ForceInline constexpr operator size_t() const {
			return value;
		}

		ForceInline constexpr size_t operator()() const {
			return value;
		}

		using value_type = size_t;
	};

	template<typename ... TTypes>
	struct tuple_size<Tuple<TTypes...>&&> {

		static constexpr size_t value = sizeof...(TTypes);

		ForceInline constexpr operator size_t() const {
			return value;
		}

		ForceInline constexpr size_t operator()() const {
			return value;
		}

		using value_type = size_t;
	};

	template<typename ... TTypes>
	struct tuple_size<const Tuple<TTypes...>&&> {

		static constexpr size_t value = sizeof...(TTypes);

		ForceInline constexpr operator size_t() const {
			return value;
		}

		ForceInline constexpr size_t operator()() const {
			return value;
		}

		using value_type = size_t;
	};

	template<typename ... TTypes>
	struct tuple_size<const volatile Tuple<TTypes...>&&> {

		static constexpr size_t value = sizeof...(TTypes);

		ForceInline constexpr operator size_t() const {
			return value;
		}

		ForceInline constexpr size_t operator()() const {
			return value;
		}

		using value_type = size_t;
	};
#pragma endregion
}