export module Cardinal.Serialization:Collection;

export import :Core;

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

export namespace Cardinal::Serialization
{
	template<typename T>
	constexpr bool IsSerializableString = CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass == CTTI::EType::String
		&& CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable;

	template<template<class THeap> typename TString, typename TStringHeap>
		requires IsSerializableString<TString<TStringHeap>>
	struct SerializationHandler<TString<TStringHeap>> {
		using CharacterType = typename CTTI::TypeTraits::CTTITypeMetaData<TString<TStringHeap>>::Character;

		static void SerializeImpl(const TString<TStringHeap>& data, Byte* rawStream, SizeT& offset) {
			SizeT Count = data.Size.Bytes;
			SerializationHandler<SizeT>::SerializeImpl(Count, rawStream, offset);
			Memory::Copy(reinterpret_cast<const Byte*>(const_cast<const Char*>(static_cast<const ConstCharStr>(data))),
				rawStream + offset,
				Count);
			offset += Count;
		}

		static constexpr Boolean IsMemCopy() {
			return false;
		}

		static constexpr SizeT DataSize(const TString<TStringHeap>& data) {
			return data.Size.Bytes + sizeof(SizeT);
		}

		static constexpr auto TypeHint() {
			return
				CTTI::TypeTraits::CTTITypeMetaData<TString<TStringHeap>>::CodePage.Resize<
				CTTI::TypeTraits::CTTITypeMetaData<TString<TStringHeap>>::CodePage.Count - 1
				>();
		}

		static constexpr auto ConstexprSize() {
			return
				TypeTraits::CreateTypeStr(L"0");
		}

		static constexpr auto TypeMetaData() {
			return
				TypeTraits::CreateTypeStr(L"{") +
				CTTI::TypeName<TString<TStringHeap>>() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeTraits::ToCharStr<static_cast<SizeT>(CTTI::TypeTraits::CTTITypeMetaData<TString<TStringHeap>>::TypeClass)>() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeTraits::ToCharStr<sizeof(TString<TStringHeap>)>() +
				TypeTraits::CreateTypeStr(L"|") +
				ConstexprSize() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeHint() +
				TypeTraits::CreateTypeStr(L"|") +
				SerializationHandler<CharacterType>::TypeMetaData() +
				TypeTraits::CreateTypeStr(L"}");
		}

		static void DeserializeImpl(TString<TStringHeap>& data, const Byte* rawStream, SizeT& offset) {
			SizeT Count = 0;
			SerializationHandler<SizeT>::DeserializeImpl(Count, rawStream, offset);
			data = TString<TStringHeap>(reinterpret_cast<const CharStr>(const_cast<Byte*>(rawStream + offset)), (Count - sizeof(CharacterType)) / sizeof(CharacterType));
			offset += Count;
		}

		template<typename THeap>
		static BaseTypeMetaDataRecord<THeap> GetTypeMetaDataRecord() {
			return TypeMetaDataRecord(
				CTTI::TypeName<TString<TStringHeap>>(),
				CTTI::TypeTraits::CTTITypeMetaData<TString<TStringHeap>>::TypeClass,
				sizeof(TString<TStringHeap>),
				0,
				TypeHint(),
				{ SerializationHandler<CharacterType>::template GetTypeMetaDataRecord<THeap>() }
			);
		}
	};

	template<typename T>
	constexpr bool IsSerializablePair = CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass == CTTI::EType::Pair
		&& CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable;

	template<template<class THeap, typename TKey, typename TValue> typename TPair, typename TPairHeap, typename TKey, typename TValue>
		requires IsSerializablePair<TPair<TPairHeap, TKey, TValue>>
	struct SerializationHandler<TPair<TPairHeap, TKey, TValue>> {
		static void SerializeImpl(const TPair<TPairHeap, TKey, TValue>& data, Byte* rawStream, SizeT& offset) {
			SerializationHandler<TKey>::SerializeImpl(data.Key, rawStream, offset);
			SerializationHandler<TValue>::SerializeImpl(data.Value, rawStream, offset);
		}

		static constexpr Boolean IsMemCopy() {
			return false;
		}

		static constexpr SizeT DataSize(const TPair<TPairHeap, TKey, TValue>& data) {
			return SerializationHandler<TKey>::DataSize(data.Key) +
				SerializationHandler<TValue>::DataSize(data.Value);
		}

		static constexpr auto TypeHint() {
			return TypeTraits::CreateTypeStr(L"");
		}

		static constexpr auto ConstexprSize() {
			return
				TypeTraits::CreateTypeStr(L"0");
		}

		static constexpr auto TypeMetaData() {
			return
				TypeTraits::CreateTypeStr(L"{") +
				CTTI::TypeName<TPair<TPairHeap, TKey, TValue>>() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeTraits::ToCharStr<static_cast<SizeT>(CTTI::TypeTraits::CTTITypeMetaData<TPair<TPairHeap, TKey, TValue>>::TypeClass)>() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeTraits::ToCharStr<sizeof(TPair<TPairHeap, TKey, TValue>)>() +
				TypeTraits::CreateTypeStr(L"|") +
				ConstexprSize() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeHint() +
				TypeTraits::CreateTypeStr(L"|") +
				SerializationHandler<TKey>::TypeMetaData() +
				SerializationHandler<TValue>::TypeMetaData() +
				TypeTraits::CreateTypeStr(L"}");
		}

		static void DeserializeImpl(TPair<TPairHeap, TKey, TValue>& data, const Byte* rawStream, SizeT& offset) {
			TKey key{};
			TValue value{};
			SerializationHandler<TKey>::DeserializeImpl(key, rawStream, offset);
			SerializationHandler<TValue>::DeserializeImpl(value, rawStream, offset);
			data = TPair<TPairHeap, TKey, TValue>(MoveRValue(key), MoveRValue(value));
		}

		template<typename THeap>
		static BaseTypeMetaDataRecord<THeap> GetTypeMetaDataRecord() {
			return BaseTypeMetaDataRecord<THeap>(
				CTTI::TypeName<TPair<TPairHeap, TKey, TValue>>(),
				CTTI::TypeTraits::CTTITypeMetaData<TPair<TPairHeap, TKey, TValue>>::TypeClass,
				sizeof(TPair<TPairHeap, TKey, TValue>),
				0,
				TypeHint(),
				{ SerializationHandler<TKey>::template GetTypeMetaDataRecord<THeap>(), SerializationHandler<TValue>::template GetTypeMetaDataRecord<THeap>() }
			);
		}
	};

	template<typename T>
	constexpr bool IsSerializableCollection = CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass == CTTI::EType::Collection
		&& CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable;

	template<template<typename TCollectionHeap, typename T> typename TCollection, typename TCollectionHeap, typename T>
		requires IsSerializableCollection<TCollection<TCollectionHeap, T>>
	struct SerializationHandler<TCollection<TCollectionHeap, T>> {
		using TCollectionClass = TCollection<TCollectionHeap, T>;

		static void SerializeImpl(const TCollectionClass& data, Byte* rawStream, SizeT& offset) {
			SerializationHandler<SizeT>::SerializeImpl(data.Count, rawStream, offset);
			data.ForEach([&](const T& elem) mutable InlineLambda
				{
					SerializationHandler<T>::SerializeImpl(elem, rawStream, offset);
				});
		}

		static constexpr Boolean IsMemCopy() {
			return false;
		}

		static constexpr SizeT DataSize(const TCollectionClass& data) {
			if constexpr (SerializationHandler<T>::IsMemCopy())
			{
				return (data.Count * sizeof(T)) + sizeof(SizeT);
			}
			else
			{
				return data.Aggregate(
					[&] (const SizeT& size, const T& item) -> SizeT InlineLambda {
						return size + SerializationHandler<T>::DataSize(item);
					}, 0Sz)
					+ sizeof(SizeT);
			}
		}

		static constexpr auto TypeHint() {
			return
				TypeTraits::CreateTypeStr(L"");
		}

		static constexpr auto ConstexprSize() {
			return
				TypeTraits::CreateTypeStr(L"0");
		}

		static constexpr auto TypeMetaData() {
			return
				TypeTraits::CreateTypeStr(L"{") +
				CTTI::TypeName<TCollectionClass>() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeTraits::ToCharStr<static_cast<SizeT>(CTTI::TypeTraits::CTTITypeMetaData<TCollectionClass>::TypeClass)>() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeTraits::ToCharStr<sizeof(TCollectionClass)>() +
				TypeTraits::CreateTypeStr(L"|") +
				ConstexprSize() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeHint() +
				TypeTraits::CreateTypeStr(L"|") +
				SerializationHandler<T>::TypeMetaData() +
				TypeTraits::CreateTypeStr(L"}");
		}

		static void DeserializeImpl(TCollectionClass& data, const Byte* rawStream, SizeT& offset) {
			if constexpr (CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable)
			{
				SizeT Count = 0;
				SerializationHandler<SizeT>::DeserializeImpl(Count, rawStream, offset);

				if constexpr (!TCollectionClass::IsDynamic)
				{
					data = TCollectionClass(Count);

					for (SizeT i = 0; i < Count; i++)
					{
						T tmp = T();
						SerializationHandler<T>::DeserializeImpl(tmp, rawStream, offset);
						data.UnsafeSet(MoveRValue(tmp), i);
					}
				}
				else
				{
					data = TCollectionClass();

					for (SizeT i = 0; i < Count; i++)
					{
						T tmp;
						SerializationHandler<T>::DeserializeImpl(tmp, rawStream, offset);
						data.UnsafeAdd(MoveRValue(tmp));
					}
				}
			}
			else
			{
				Throw(NullPointException(SourcePosition));
			}
		}

		template<typename THeap>
		static BaseTypeMetaDataRecord<THeap> GetTypeMetaDataRecord() {
			return BaseTypeMetaDataRecord<THeap>(
				CTTI::TypeName<TCollectionClass>(),
				CTTI::TypeTraits::CTTITypeMetaData<TCollectionClass>::TypeClass,
				sizeof(TCollectionClass),
				0,
				TypeHint(),
				{ SerializationHandler<T>::template GetTypeMetaDataRecord<THeap>() }
			);
		}
	};

	template<typename T>
	constexpr bool IsSerializableFixedCollection = CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass == CTTI::EType::FixedCollection
		&& CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable;

	template<template<typename TCollectionHeap, SizeT TSize, typename T> typename TFixedCollection, typename TCollectionHeap, SizeT TSize, typename T>
		requires IsSerializableFixedCollection<TFixedCollection<TCollectionHeap, TSize, T>>
	struct SerializationHandler<TFixedCollection<TCollectionHeap, TSize, T>> {
		using TCollectionClass = TFixedCollection<TCollectionHeap, TSize, T>;

		static void SerializeImpl(const TCollectionClass& data, Byte* rawStream, SizeT& offset) {
			SizeT size = data.Count;
			SerializationHandler<SizeT>::SerializeImpl(size, rawStream, offset);

			data.ForEach([&] (const T& elem) mutable {
				SerializationHandler<T>::SerializeImpl(elem, rawStream, offset);
				});
		}

		static constexpr Boolean IsMemCopy() {
			return false;
		}

		static constexpr SizeT DataSize(const TCollectionClass& data) {
			if constexpr (SerializationHandler<T>::IsMemCopy())
			{
				return (data.Count * sizeof(T)) + sizeof(SizeT);
			}
			else
			{
				return data.Aggregate([&](SizeT& size, const T& item)->SizeT & InlineLambda
					{
						return size += SerializationHandler<T>::DataSize(item);
					}, 0ui64) + sizeof(SizeT);
			}
		}

		static constexpr auto TypeHint() {
			return
				TypeTraits::ToCharStr<TSize>();
		}

		static constexpr auto ConstexprSize() {
			return
				TypeTraits::CreateTypeStr(L"0");
		}

		static constexpr auto TypeMetaData() {
			return
				TypeTraits::CreateTypeStr(L"{") +
				CTTI::TypeName<TCollectionClass>() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeTraits::ToCharStr<static_cast<SizeT>(CTTI::TypeTraits::CTTITypeMetaData<TCollectionClass>::TypeClass)>() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeTraits::ToCharStr<sizeof(TCollectionClass)>() +
				TypeTraits::CreateTypeStr(L"|") +
				ConstexprSize() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeHint() +
				TypeTraits::CreateTypeStr(L"|") +
				SerializationHandler<T>::TypeMetaData() +
				TypeTraits::CreateTypeStr(L"}");
		}

		static void DeserializeImpl(TCollectionClass& data, const Byte* rawStream, SizeT& offset) {
			if constexpr (CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable)
			{
				SizeT Count = 0;
				SerializationHandler<SizeT>::DeserializeImpl(Count, rawStream, offset);

				data = TCollectionClass();

				for (SizeT i = 0; i < Count; i++)
				{
					T tmp = T();
					SerializationHandler<T>::DeserializeImpl(tmp, rawStream, offset);
					data.UnsafeSet(MoveRValue(tmp), i);
				}
			}
			else
			{
				Throw(NullPointException(SourcePosition));
			}
		}

		template<typename THeap>
		static BaseTypeMetaDataRecord<THeap> GetTypeMetaDataRecord() {
			return BaseTypeMetaDataRecord<THeap>(
				CTTI::TypeName<TCollectionClass>(),
				CTTI::TypeTraits::CTTITypeMetaData<TCollectionClass>::TypeClass,
				sizeof(TCollectionClass),
				0,
				TypeHint(),
				{ SerializationHandler<T>::template GetTypeMetaDataRecord<THeap>() }
			);
		}
	};

	template<typename T>
	constexpr bool IsSerializableAssociableCollection = CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass == CTTI::EType::AssociableCollection
		&& CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable;

	template<template<typename TCollectionHeap, typename TKey, typename TValue> typename TAssociableCollection, typename TCollectionHeap, typename TKey, typename TValue>
		requires (IsSerializableAssociableCollection<TAssociableCollection<TCollectionHeap, TKey, TValue>> &&
	!TypeTraits::Collection::IsDictionary<TAssociableCollection<TCollectionHeap, TKey, TValue>>)
		struct SerializationHandler<TAssociableCollection<TCollectionHeap, TKey, TValue>> {
		using TCollectionClass = TAssociableCollection<TCollectionHeap, TKey, TValue>;
		using T = typename TCollectionClass::DataType;

		static void SerializeImpl(const TCollectionClass& data, Byte* rawStream, SizeT& offset) {
			SerializationHandler<SizeT>::SerializeImpl(data.Count, rawStream, offset);
			data.ForEach([&](const T& elem) mutable InlineLambda
				{
					SerializationHandler<T>::SerializeImpl(elem, rawStream, offset);
				});
		}

		static constexpr Boolean IsMemCopy() {
			return false;
		}

		static constexpr SizeT DataSize(const TCollectionClass& data) {
			if constexpr (SerializationHandler<T>::IsMemCopy())
			{
				return (data.Count * sizeof(T)) + sizeof(SizeT);
			}
			else
			{
				SizeT size = sizeof(T);
				data.ForEach([&](const T& item) InlineLambda{
					size += SerializationHandler<T>::DataSize(item);
					});
				return size;
			}
		}

		static constexpr auto TypeHint() {
			return
				TypeTraits::CreateTypeStr(L"");
		}

		static constexpr auto ConstexprSize() {
			return
				TypeTraits::CreateTypeStr(L"0");
		}

		static constexpr auto TypeMetaData() {
			return
				TypeTraits::CreateTypeStr(L"{") +
				CTTI::TypeName<TCollectionClass>() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeTraits::ToCharStr<static_cast<SizeT>(CTTI::TypeTraits::CTTITypeMetaData<TCollectionClass>::TypeClass)>() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeTraits::ToCharStr<sizeof(TCollectionClass)>() +
				TypeTraits::CreateTypeStr(L"|") +
				ConstexprSize() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeHint() +
				TypeTraits::CreateTypeStr(L"|") +
				SerializationHandler<T>::TypeMetaData() +
				TypeTraits::CreateTypeStr(L"}");
		}

		static void DeserializeImpl(TCollectionClass& data, const Byte* rawStream, SizeT& offset) {
			if constexpr (CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable)
			{
				SizeT Count = 0;
				SerializationHandler<SizeT>::DeserializeImpl(Count, rawStream, offset);

				data = TCollectionClass();

				for (SizeT i = 0; i < Count; i++)
				{
					T tmp = T();
					SerializationHandler<T>::DeserializeImpl(tmp, rawStream, offset);
					data.UnsafeAdd(MoveRValue(tmp));
				}
			}
			else
			{
				Throw(NullPointException(SourcePosition));
			}
		}

		template<typename THeap>
		static BaseTypeMetaDataRecord<THeap> GetTypeMetaDataRecord() {
			return BaseTypeMetaDataRecord<THeap>(
				CTTI::TypeName<TCollectionClass>(),
				CTTI::TypeTraits::CTTITypeMetaData<TCollectionClass>::TypeClass,
				sizeof(TCollectionClass),
				0,
				TypeHint(),
				{ SerializationHandler<T>::template GetTypeMetaDataRecord<THeap>() }
			);
		}
	};

	template<
		template<typename TCollectionHeap, typename TKey, typename TValue, Containers::TCharacterComparator CharacterComparator> typename TAssociableCollection,
		typename TCollectionHeap,
		typename TKey,
		typename TValue,
		Containers::TCharacterComparator CharacterComparator>
		requires (IsSerializableAssociableCollection<TAssociableCollection<TCollectionHeap, TKey, TValue, CharacterComparator>>&&
	TypeTraits::Collection::IsDictionary<TAssociableCollection<TCollectionHeap, TKey, TValue, CharacterComparator>>)
		struct SerializationHandler<TAssociableCollection<TCollectionHeap, TKey, TValue, CharacterComparator>> {
		using TCollectionClass = TAssociableCollection<TCollectionHeap, TKey, TValue, CharacterComparator>;
		using T = typename TCollectionClass::DataType;

		static void SerializeImpl(const TCollectionClass& data, Byte* rawStream, SizeT& offset) {
			SerializationHandler<SizeT>::SerializeImpl(data.Count, rawStream, offset);
			data.ForEach([&](const T& elem) mutable InlineLambda
				{
					SerializationHandler<T>::SerializeImpl(elem, rawStream, offset);
				});
		}

		static constexpr Boolean IsMemCopy() {
			return false;
		}

		static constexpr SizeT DataSize(const TCollectionClass& data) {
			if constexpr (SerializationHandler<T>::IsMemCopy())
			{
				return (data.Count * sizeof(T)) + sizeof(SizeT);
			}
			else
			{
				SizeT size = sizeof(T);
				data.ForEach([&](const T& item) InlineLambda{
					size += SerializationHandler<T>::DataSize(item);
					});
				return size;
			}
		}

		static constexpr auto TypeHint() {
			return
				TypeTraits::CreateTypeStr(L"[IsCaseSensitiveCharacterComparing - ") +
				TypeTraits::BoolToCharStr<CTTI::TypeTraits::CTTITypeMetaData<TCollectionClass>::IsCaseSensitiveCharacterComparing>() +
				TypeTraits::CreateTypeStr(L"][IsCaseSensitiveCharacterComparing - ") +
				TypeTraits::BoolToCharStr<CTTI::TypeTraits::CTTITypeMetaData<TCollectionClass>::IsCaseInsensitiveCharacterComparing>() +
				TypeTraits::CreateTypeStr(L"]");
		}

		static constexpr auto ConstexprSize() {
			return
				TypeTraits::CreateTypeStr(L"0");
		}

		static constexpr auto TypeMetaData() {
			return
				TypeTraits::CreateTypeStr(L"{") +
				CTTI::TypeName<TCollectionClass>() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeTraits::ToCharStr<static_cast<SizeT>(CTTI::TypeTraits::CTTITypeMetaData<TCollectionClass>::TypeClass)>() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeTraits::ToCharStr<sizeof(TCollectionClass)>() +
				TypeTraits::CreateTypeStr(L"|") +
				ConstexprSize() +
				TypeTraits::CreateTypeStr(L"|") +
				TypeHint() +
				TypeTraits::CreateTypeStr(L"|") +
				SerializationHandler<T>::TypeMetaData() +
				TypeTraits::CreateTypeStr(L"}");
		}

		static void DeserializeImpl(TCollectionClass& data, const Byte* rawStream, SizeT& offset) {
			if constexpr (CTTI::TypeTraits::CTTITypeMetaData<T>::IsSerializable)
			{
				SizeT Count = 0;
				SerializationHandler<SizeT>::DeserializeImpl(Count, rawStream, offset);

				data = TCollectionClass();

				for (SizeT i = 0; i < Count; i++)
				{
					T tmp = T();
					SerializationHandler<T>::DeserializeImpl(tmp, rawStream, offset);
					data.UnsafeAdd(MoveRValue(tmp));
				}
			}
			else
			{
				Throw(NullPointException(SourcePosition));
			}
		}

		template<typename THeap>
		static BaseTypeMetaDataRecord<THeap> GetTypeMetaDataRecord() {
			return BaseTypeMetaDataRecord<THeap>(
				CTTI::TypeName<TCollectionClass>(),
				CTTI::TypeTraits::CTTITypeMetaData<TCollectionClass>::TypeClass,
				sizeof(TCollectionClass),
				0,
				TypeHint(),
				{ SerializationHandler<T>::template GetTypeMetaDataRecord<THeap>() }
			);
		}
	};
}

#pragma warning(pop)