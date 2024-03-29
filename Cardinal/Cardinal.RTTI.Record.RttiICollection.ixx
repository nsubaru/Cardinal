export module Cardinal.RTTI:Record.RttiICollection;

export import :Record.Base;
export import :Record.RttiIterator;

export namespace Cardinal::RTTI::Record
{
	/// <summary>
	/// A base RTTI field class
	/// </summary>
	/// <typeparam name="THeap">Heap type</typeparam>
	/// <typeparam name="T"></typeparam>
	template<typename THeap, typename T>
	class BaseRttiCollection final :
		public Memory::Allocator<THeap>,
		public Containers::LINQ::IReadOnlyCollection<BaseRttiCollection, THeap, T, false, true, false> {
	private:
		const MetaData::Collection* base;
		SizeT imageBase;
		bool isValidCxxTypeId;

	protected:
		BaseRttiCollection(const MetaData::Collection* base, bool isValidCxxTypeId, SizeT imageBase)
			: base(base), isValidCxxTypeId(isValidCxxTypeId), imageBase(imageBase) {}

	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="from">Source</param>
		BaseRttiCollection(const BaseRttiCollection& from)
			: base(from.base), isValidCxxTypeId(from.isValidCxxTypeId), imageBase(from.imageBase) {}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="from">RValue reference to a source</param>
		BaseRttiCollection(BaseRttiCollection&& from)
			: base(from.base), isValidCxxTypeId(from.isValidCxxTypeId), imageBase(from.imageBase) {}

		/// <summary>
		/// Assignment operator
		/// </summary>
		/// <param name="from">Source</param>
		/// <returns>Assigned value</returns>
		BaseRttiCollection& operator=(const BaseRttiCollection& from) {
			this->~BaseRttiCollection();
			this->BaseRttiCollection::BaseRttiCollection(from);
			return *this;
		}

		/// <summary>
		/// Assignment operator
		/// </summary>
		/// <param name="from">RValue reference to a source</param>
		/// <returns>Assigned value</returns>
		BaseRttiCollection& operator=(BaseRttiCollection&& from) {
			this->~BaseRttiCollection();
			this->BaseRttiCollection::BaseRttiCollection(MoveRValue(from));
			return *this;
		}

		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~BaseRttiCollection() {
			base = nullptr;
			imageBase = 0;
			isValidCxxTypeId = false;
		}

	public:
		/// <summary>
		/// Recieving the count of collection items
		/// </summary>
		/// <returns>Count of collection items</returns>
		SizeT GetCount() const { return base == nullptr ? 0 : base->Count; }

		/// <summary>
		/// Receiving the beginning of collection
		/// </summary>
		/// <returns>Beginning of collection</returns>
		BaseRttiIterator<THeap, T> begin() const {
			if (base != nullptr && base->Count != 0)
			{
				return RttiIterator<T>(static_cast<const void*>(((Byte*) base) + base->OffsetToFirstItem), 0, base->Count, imageBase, isValidCxxTypeId);
			}
			else
			{
				return RttiIterator<T>(nullptr, 0, 0, 0, isValidCxxTypeId);
			}
		}

		/// <summary>
		/// Receiving the end of collection
		/// </summary>
		/// <returns>End of collection</returns>
		constexpr NullptrT end() const { return nullptr; }

		/// <summary>
		/// Receiving the pointer to a chosen item in collection
		/// </summary>
		/// <param name="index">Chosen index of item</param>
		/// <returns>Pointer to a chosen item in collection</returns>
		T At(SizeT index) {
			if (index >= base->Count)
			{
				Throw(NullPointException(SourcePosition));
			}
			else
			{
				auto it = begin();
				for (SizeT i = 0; i < index; i++)
				{
					++it;
				}
				return *it;
			}
		}

	public:
		/// <summary>
		/// A property for receiving the count of collection items
		/// </summary>
		SizeT Property(GetCount) Count;

	protected:
		template<typename TLambda>
		ForceInline BaseRttiCollection& ForEachImpl(TLambda&& lambda) {
			if constexpr (Concepts::IsInvockable<TLambda, const T&>)
			{
				for (auto it = this->begin(); it != nullptr; ++it)
				{
					auto v = *it;
					lambda(v);
				}
				return *this;
			}
			else if constexpr (Concepts::IsInvockable<TLambda, const T&, SizeT>)
			{
				SizeT i = 0;
				for (auto it = this->begin(); it != nullptr; ++it, i++)
				{
					auto v = *it;
					lambda(v, i);
				}
				return *this;
			}
			else
			{
				static_assert(Concepts::IsInvockable<TLambda, const T&> ||
					Concepts::IsInvockable<TLambda, const T&, SizeT>);
			}
		}

		template<typename TLambda>
		ForceInline const BaseRttiCollection& ForEachImpl(TLambda&& lambda) const {
			if constexpr (Concepts::IsInvockable<TLambda, const T&>)
			{
				for (auto it = this->begin(); it != nullptr; ++it)
				{
					auto v = *it;
					lambda(v);
				}
				return *this;
			}
			else if constexpr (Concepts::IsInvockable<TLambda, const T&, SizeT>)
			{
				SizeT i = 0;
				for (auto it = this->begin(); it != nullptr; ++it, i++)
				{
					auto v = *it;
					lambda(v, i);
				}
				return *this;
			}
			else
			{
				static_assert(Concepts::IsInvockable<TLambda, const T&> ||
					Concepts::IsInvockable<TLambda, const T&, SizeT>);
			}
		}

		template<typename U, typename TLambda>
		ForceInline U AggregateImpl(TLambda&& lambda, const U& value) const {
			U result = value;
			U& data = result;
			for (auto it = this->begin(); it != nullptr; ++it)
			{
				if constexpr (
					Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&> ||
					Concepts::IsInvockableWithResult<TLambda, U, U&, const T&>)
				{
					auto v = *it;
					data = lambda(data, v);
				}
				else
				{
					auto v = *it;
					lambda(data, v);
				}
			}
			return MoveRValue(result);
		}

		template<typename U, typename TLambda>
		ForceInline U AggregateImpl(TLambda&& lambda, U&& value) const {
			for (auto it = this->begin(); it != nullptr; ++it)
			{
				if constexpr (
					Concepts::IsInvockableWithResult<TLambda, U&, U&, const T&> ||
					Concepts::IsInvockableWithResult<TLambda, U, U&, const T&>)
				{
					auto v = *it;
					value = lambda(value, v);
				}
				else
				{
					auto v = *it;
					lambda(value, v);
				}
			}
			return MoveRValue(value);
		}

		ForceInline bool AnyImpl() const { return base->Count > 0; }

		template<typename TLambda>
		ForceInline bool AnyImpl(TLambda&& lambda) const {
			for (auto it = this->begin(); it != nullptr; ++it)
			{
				auto v = *it;
				if (lambda(v))
					return true;
			}
			return false;
		}

		template<typename TLambda>
		ForceInline bool AllImpl(TLambda&& lambda) const {
			for (auto it = this->begin(); it != nullptr; ++it)
			{
				auto v = *it;
				if (!lambda(v))
					return false;
			}
			return true;
		}

		template<typename TLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, T&& item) const {
			if (GetCount() == 0)
				return MoveRValue(item);

			for (auto it = this->begin(); it != nullptr; ++it)
				if (auto v = *it; lambda(v))
					return v;

			return MoveRValue(item);
		}

		template<typename TLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, T&& item) const {
			if (GetCount() == 0)
				return MoveRValue(item);

			auto item = *this->begin();
			SizeT Index = -1, i = 0;
			for (auto it = this->begin(); it != nullptr; ++it, i++)
				if (auto v = *it; lambda(v))
					item = v;

			if (Index != -1)
				return item;

			return MoveRValue(item);
		}

		template<typename TLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, const T& item) const {
			if (GetCount() == 0)
				return item;

			for (auto it = this->begin(); it != nullptr; ++it)
				if (auto v = *it; lambda(v))
					return v;

			return item;
		}

		template<typename TLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, const T& item) const {
			if (GetCount() == 0)
				return item;

			auto item = *this->begin();
			SizeT Index = -1, i = 0;
			for (auto it = this->begin(); it != nullptr; ++it, i++)
				if (auto v = *it; lambda(v))
					item = v;

			if (Index != -1)
				return item;

			return item;
		}

		template<typename TLambda, typename TGetterLambda>
		ForceInline T FirstOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			if (GetCount() == 0)
				return otherLambda();

			for (auto it = this->begin(); it != nullptr; ++it)
				if (auto v = *it; lambda(v))
					return v;

			return otherLambda();
		}

		template<typename TLambda, typename TGetterLambda>
		ForceInline T LastOrImpl(TLambda&& lambda, TGetterLambda&& otherLambda) const {
			if (GetCount() == 0)
				return otherLambda();

			auto item = *this->begin();
			SizeT Index = -1, i = 0;
			for (auto it = this->begin(); it != nullptr; ++it, i++)
				if (auto v = *it; lambda(v))
					item = v;

			if (Index != -1)
				return item;

			return otherLambda();
		}

		template<typename TLambda>
		SizeT GetCountIfImpl(TLambda&& lambda) const {
			SizeT selectedCount = 0;

			for (auto it = this->begin(); it != nullptr; ++it)
				if (auto v = *it; lambda(v))
					selectedCount++;

			return selectedCount;
		}

		template<typename TLambda>
		ForceInline T FirstImpl(TLambda&& lambda) const {
			if (GetCount() == 0)
				Throw(NotFoundException(SourcePosition));

			for (auto it = this->begin(); it != nullptr; ++it)
				if (auto v = *it; lambda(v))
					return v;

			Throw(NotFoundException(SourcePosition));
		}

		template<typename TLambda>
		ForceInline T LastImpl(TLambda&& lambda) const {
			if (GetCount() == 0)
				Throw(NotFoundException(SourcePosition));

			auto res = this->begin();
			for (auto it = this->begin(); it != nullptr; ++it)
				if (auto v = *it; lambda(v))
					res = it;

			if (res == nullptr)
				Throw(NotFoundException(SourcePosition));

			return res;
		}

	private:
		template<typename UHeap>
		friend class BaseRttiTypeRecord;

		friend T;

	private:
		template<template<typename THeap, typename T> typename TReadOnlyCollection, typename THeap, typename T, bool TIsDynamic, bool TIsConcurencySuport, bool IsCollectionCanReturnRef>
		friend class Containers::LINQ::IReadOnlyCollection;

		template<template<typename, typename> typename, typename, typename, Boolean, Boolean, Boolean, Boolean>
		friend class Containers::LINQ::ICollection;

		template<template<typename, SizeT, typename> typename TCollection, typename UHeap, typename U, SizeT TSize, Boolean TIsConcurencySuport>
		friend class Containers::LINQ::IFixedCollectionBase;

		template<template<typename, SizeT, typename> typename TCollection, typename THeap, typename T, SizeT TSize, Boolean TIsConcurencySuport>
		friend class Containers::LINQ::IFixedCollection;

		template<typename U>
		friend struct Serialization::SerializationHandler;

		template<typename>
		friend class BaseTraveler;
	};
}