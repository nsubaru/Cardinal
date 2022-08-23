//export module Cardinal.RTTI:Dynamic.Serialization;
//
//export import :Dynamic.Type;
//
//export namespace Cardinal::CTTI::TypeTraits::Details
//{
//	template<bool TIsConst, bool TIsVolatile>
//	struct CTTITypeDataImpl<RTTI::Dynamic, TIsConst, TIsVolatile>
//		: BaseCTTITypeDataImpl<RTTI::Dynamic, EType::Dynamic, true, TIsConst, TIsVolatile> {
//	};
//
//	template<>
//	struct IsDynamicT<RTTI::Dynamic> : Core::TypeTraits::LogicTrue {};
//}
//
//namespace Cardinal::Serialization::Details
//{
//	/// <summary>
//	/// Base serialization handler class
//	/// </summary>
//	/// <typeparam name="T">Serialization item type</typeparam>
//	template<typename T> requires Core::CTTI::Concepts::IsDynamicCttiType<T>&& Core::Concepts::IsSame<T, RTTI::Dynamic>
//	struct SerializationHandler<T> {
//		/// <summary>
//		/// Serialization method
//		/// </summary>
//		/// <param name="data">Dynamic serializable data</param>
//		/// <param name="rawStream">A pointer to a stream of data</param>
//		/// <param name="offset">Offset</param>
//		static void SerializeImpl(const RTTI::Dynamic& data, Byte* rawStream, SizeT& offset) {
//			const auto& schema = data.GetMetaDataForDynamicRecordForDynamic();
//
//			if (schema.FSerialize == nullptr)
//				Throw(NullPointException(SourcePosition));
//
//			auto id = data.Type.FullTypeId;
//			SerializationHandler<RTTI::UniversalTypeIdentificator>::SerializeImpl(id, rawStream, offset);
//			schema.FSerialize(rawStream, offset, data.data);
//		}
//
//		/// <summary>
//		/// Deserialization method
//		/// </summary>
//		/// <param name="data">Dynamic deserializable data</param>
//		/// <param name="rawStream">A pointer to a stream of data</param>
//		/// <param name="offset">Offset</param>
//		static void DeserializeImpl(RTTI::Dynamic& data, const Byte* rawStream, SizeT& offset) {
//			RTTI::UniversalTypeIdentificator id;
//			SerializationHandler<RTTI::UniversalTypeIdentificator>::DeserializeImpl(id, rawStream, offset);
//
//			auto record = RTTI::GetAllRegisteredTypes().First(ArrowFunctWithCapture(const& x, x.FullTypeId == id));
//
//			data = RTTI::Dynamic(record, true);
//
//			const auto& schema = data.GetMetaDataForDynamicRecordForDynamic();
//
//			if (schema.FDeserialize == nullptr)
//				Throw(NullPointException(SourcePosition));
//
//			schema.FDeserialize(rawStream, offset, data.data);
//		}
//
//		/// <summary>
//		/// Indicates, is it possible to copy the item without any additional processing
//		/// </summary>
//		/// <returns>"True" if it is possible to copy the item without any additional processing</returns>
//		static constexpr Boolean IsMemCopy() {
//			return false;
//		}
//
//		/// <summary>
//		/// Receiving the data dize
//		/// </summary>
//		/// <param name="data">Dynamic data</param>
//		/// <returns>Data size</returns>
//		static SizeT DataSize(const RTTI::Dynamic& data) {
//			const auto& schema = data.GetMetaDataForDynamicRecordForDynamic();
//			if (schema.FSerializizationSize == nullptr)
//				Throw(NullPointException(SourcePosition));
//
//			auto id = data.Type.FullTypeId;
//			return schema.FSerializizationSize(data.data) + SerializationHandler<RTTI::UniversalTypeIdentificator>::DataSize(id);
//		}
//
//		/// <summary>
//		/// Returns the additional type information
//		/// </summary>
//		/// <returns>Additional type information</returns>
//		static constexpr auto TypeHint() {
//			return
//				Core::Details::CreateTypeStr(L"Dynamic");
//		}
//
//		/// <summary>
//		/// Returns the compile-time size of type
//		/// </summary>
//		/// <returns>Compile-time size of type</returns>
//		static constexpr auto ConstexprSize() {
//			return
//				Core::Details::CreateTypeStr(L"0");
//		}
//
//		/// <summary>
//		/// Returns the runtime information for serialization
//		/// </summary>
//		/// <returns>Runtime information for serialization</returns>
//		static constexpr auto TypeMetaData() {
//			return
//				Core::Details::CreateTypeStr(L"{") +
//				CTTI::TypeName<RTTI::Dynamic>() +
//				Core::Details::CreateTypeStr(L"|") +
//				Core::Details::ToCharStr<static_cast<SizeT>(CTTI::TypeTraits::CTTITypeMetaData<RTTI::Dynamic>::TypeClass)>() +
//				Core::Details::CreateTypeStr(L"|") +
//				Core::Details::ToCharStr<sizeof(RTTI::Dynamic)>() +
//				Core::Details::CreateTypeStr(L"|") +
//				ConstexprSize() +
//				Core::Details::CreateTypeStr(L"|") +
//				TypeHint() +
//				Core::Details::CreateTypeStr(L"|") +
//				SerializationHandler<RTTI::UniversalTypeIdentificator>::TypeMetaData() +
//				Core::Details::CreateTypeStr(L"}");
//		}
//
//		/// <summary>
//		/// Type metadata record getter function
//		/// </summary>
//		/// <typeparam name="THeap">Heap type</typeparam>
//		/// <returns>Type metadata record</returns>
//		template<typename THeap>
//		static BaseTypeMetaDataRecord<THeap> GetTypeMetaDataRecord() {
//			return BaseTypeMetaDataRecord<THeap>(
//				CTTI::TypeName<RTTI::Dynamic>(),
//				CTTI::TypeTraits::CTTITypeMetaData<RTTI::Dynamic>::TypeClass,
//				sizeof(RTTI::Dynamic),
//				0,
//				TypeHint(),
//				{ SerializationHandler<Containers::Blob>::GetTypeMetaDataRecord<THeap>() }
//			);
//		}
//	};
//}