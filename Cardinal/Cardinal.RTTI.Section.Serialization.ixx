export module Cardinal.RTTI:Section.Serialization;

export import :Section.Type;

export namespace Cardinal::Serialization
{
	/// <summary>
	/// A base serialization handler structure
	/// </summary>
	template<>
	struct SerializationHandler<RTTI::RTTISection> {
		/// <summary>
		/// Serialization implementstion
		/// </summary>
		/// <param name="data">Data</param>
		/// <param name="rawStream">Raw stream</param>
		/// <param name="offset">Offset</param>
		static void SerializeImpl(const RTTI::RTTISection& data, Byte* rawStream, SizeT& offset) {
			SerializationHandler<Containers::Blob>::SerializeImpl(data.This->rawData, rawStream, offset);
		}

		/// <summary>
		/// Deserialization implementstion
		/// </summary>
		/// <param name="data">Data</param>
		/// <param name="rawStream">Raw stream</param>
		/// <param name="offset">Offset</param>
		static void DeserializeImpl(RTTI::RTTISection& data, const Byte* rawStream, SizeT& offset) {
			auto blob = Containers::Blob();
			SerializationHandler<Containers::Blob>::DeserializeImpl(blob, rawStream, offset);
			data = RTTI::RTTISection(MoveRValue(blob));
		}

		/// <summary>
		/// Indicates, is it possible to copy the item without any additional processing
		/// </summary>
		/// <returns>"True" if it is possible to copy the item without any additional processing</returns>
		static constexpr Boolean IsMemCopy() {
			return false;
		}

		/// <summary>
		/// Receiving the data dize
		/// </summary>
		/// <param name="data">RTTI section data</param>
		/// <returns>Data size</returns>
		static constexpr SizeT DataSize(const RTTI::RTTISection& data) {
			return SerializationHandler<Containers::Blob>::DataSize(data.This->rawData);
		}

		/// <summary>
		/// Returns the additional type information
		/// </summary>
		/// <returns>Additional type information</returns>
		static constexpr auto TypeHint() {
			return
				TypeTraits::CreateTypeStr(L"RAW");
		}

		/// <summary>
		/// Returns the compile-time size of type
		/// </summary>
		/// <returns>Compile-time size of type</returns>
		static constexpr auto ConstexprSize() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"0");
		}

		/// <summary>
		/// Returns the runtime information for serialization
		/// </summary>
		/// <returns>Runtime information for serialization</returns>
		static constexpr auto TypeMetaData() {
			return
				Cardinal::TypeTraits::CreateTypeStr(L"{") +
				CTTI::TypeName<RTTI::RTTISection>() +
				Cardinal::TypeTraits::CreateTypeStr(L"|") +
				Cardinal::TypeTraits::ToCharStr<static_cast<SizeT>(CTTI::TypeTraits::CTTITypeMetaData<RTTI::RTTISection>::TypeClass)>() +
				Cardinal::TypeTraits::CreateTypeStr(L"|") +
				Cardinal::TypeTraits::ToCharStr<sizeof(RTTI::RTTISection)>() +
				Cardinal::TypeTraits::CreateTypeStr(L"|") +
				ConstexprSize() +
				Cardinal::TypeTraits::CreateTypeStr(L"|") +
				TypeHint() +
				Cardinal::TypeTraits::CreateTypeStr(L"|") +
				SerializationHandler<Containers::Blob>::TypeMetaData() +
				Cardinal::TypeTraits::CreateTypeStr(L"}");
		}

		/// <summary>
		/// Receiving the type metadata record
		/// </summary>
		/// <typeparam name="THeap">Heap type</typeparam>
		/// <returns>Type metadata record</returns>
		template<typename THeap>
		static BaseTypeMetaDataRecord<THeap> GetTypeMetaDataRecord() {
			return BaseTypeMetaDataRecord<THeap>(
				CTTI::TypeName<RTTI::RTTISection>(),
				CTTI::TypeTraits::CTTITypeMetaData<RTTI::RTTISection>::TypeClass,
				sizeof(RTTI::RTTISection),
				0,
				TypeHint(),
				{ SerializationHandler<Containers::Blob>::GetTypeMetaDataRecord<THeap>() }
			);
		}
	};
}