export module Cardinal.CTTI:TypeTraits.String;

export import :Base;
export import :TypeTraits.HeapMetaData;

#pragma warning(push)
#pragma warning(disable: 4996)

export namespace Cardinal::CTTI::TypeTraits
{
	template<typename THeap, bool TIsConst, bool TIsVolatile>
	struct CTTITypeDataImpl<BaseString<THeap>, TIsConst, TIsVolatile> :
		BaseHeapCTTITypeData<BaseString<THeap>, THeap, EType::String, true, TIsConst, TIsVolatile> {
		using Character = UChar;
		using CharacterTypeMetaData = CTTITypeMetaData<UChar>;

		static constexpr auto CodePage = CharacterTypeMetaData::CodePage;
	};
}

#pragma warning(pop)