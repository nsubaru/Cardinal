export module Cardinal.RTTI:MetaData.Primitives;

export import :MetaData.Base;

export namespace Cardinal::RTTI::MetaData
{
	/// <summary>
	/// Receiving the CTTI metadata for undefined CTTI type
	/// </summary>
	/// <returns>CTTI metadata</returns>
	template<CTTI::Concepts::IsCttiType<CTTI::EType::Undefine> T>
	inline constexpr auto GetCTTIMetaData() {
		return TypeTraits::TypeTag<void>();
	}

	/// <summary>
	/// Receiving the CTTI metadata for logic CTTI type
	/// </summary>
	/// <returns>CTTI metadata</returns>
	template<CTTI::Concepts::IsLogicCttiType T>
	inline constexpr auto GetCTTIMetaData() {
		return TypeTraits::TypeTag<void>();
	}

	/// <summary>
	/// Receiving the CTTI metadata for numeric CTTI type
	/// </summary>
	/// <returns>CTTI metadata</returns>
	template<CTTI::Concepts::IsNumericCttiType T>
	inline constexpr auto GetCTTIMetaData() {
#pragma pack(push, 1)
		struct MetaData {
			Boolean IsSigned;
		};
#pragma pack(pop)

		return MetaData{
			.IsSigned = CTTI::TypeTraits::CTTITypeMetaData<T>::IsSigned,
		};
	}

	/// <summary>
	/// Receiving the CTTI metadata for float point numer CTTI type
	/// </summary>
	/// <returns>CTTI metadata</returns>
	template<CTTI::Concepts::IsFloatPointNumberCttiType T>
	inline constexpr auto GetCTTIMetaData() {
		return TypeTraits::TypeTag<void>();
	}

	/// <summary>
	/// Receiving the CTTI metadata for character CTTI type
	/// </summary>
	/// <returns>CTTI metadata</returns>
	template<CTTI::Concepts::IsCharacterCttiType T>
	inline constexpr auto GetCTTIMetaData() {
		return TypeTraits::TypeTag<void>();
	}

	/// <summary>
	/// Receiving the CTTI metadata for POD CTTI type
	/// </summary>
	/// <returns>CTTI metadata</returns>
	template<CTTI::Concepts::IsPodCttiType T>
	inline constexpr auto GetCTTIMetaData() {
		return TypeTraits::TypeTag<void>{};
	}

	/// <summary>
	/// Receiving the CTTI metadata for CTTITypeMetaData type
	/// </summary>
	/// <typeparam name="T">CTTI item type</typeparam>
	/// <returns>CTTI metadata</returns>
	template<typename T> requires (CTTI::TypeTraits::CTTITypeMetaData<T>::TypeClass == CTTI::EType::MetaData)
		inline constexpr auto GetCTTIMetaData() {
		return TypeTraits::TypeTag<void>{};
	}
}