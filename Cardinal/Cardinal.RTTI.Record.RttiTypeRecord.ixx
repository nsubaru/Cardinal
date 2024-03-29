export module Cardinal.RTTI:Record.RttiTypeRecord;

export import :Record.Base;
export import :Record.RttiField;
export import :Record.RttiIterator;
export import :Record.RttiICollection;
export import :Record.Object;

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

export namespace Cardinal::RTTI::Record
{
	/// <summary>
	/// A base RTTI type record class
	/// </summary>
	/// <typeparam name="THeap">Heap type</typeparam>
	template<typename THeap>
	class BaseRttiTypeRecord final : public Memory::Allocator<THeap> {
		const MetaData::MetaData* record;
		SizeT imageBase;
		bool isValidCxxTypeId;
	private:
		constexpr BaseRttiTypeRecord(const MetaData::MetaData* record, bool IsValidCxxTypeId, SizeT imageBase)
			: record(record), isValidCxxTypeId(IsValidCxxTypeId), imageBase(imageBase) {}

	public:
		/// <summary>
		/// Empty constructor
		/// </summary>
		BaseRttiTypeRecord() : record(nullptr), imageBase(0), isValidCxxTypeId(false) {}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="from">Source</param>
		BaseRttiTypeRecord(const BaseRttiTypeRecord& from)
			: record(from.record), imageBase(from.imageBase), isValidCxxTypeId(from.isValidCxxTypeId) {}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="from">RValue reference to a source</param>
		BaseRttiTypeRecord(BaseRttiTypeRecord&& from)
			: record(from.record), imageBase(from.imageBase), isValidCxxTypeId(from.isValidCxxTypeId) {}

		/// <summary>
		/// Assignment operator
		/// </summary>
		/// <param name="from">Source</param>
		/// <returns>Assigned value</returns>
		BaseRttiTypeRecord& operator=(const BaseRttiTypeRecord& from) {
			this->~BaseRttiTypeRecord();
			this->BaseRttiTypeRecord::BaseRttiTypeRecord(from);
			return *this;
		}

		/// <summary>
		/// Assignment operator
		/// </summary>
		/// <param name="from">RValue reference to a source</param>
		/// <returns>Assigned value</returns>
		BaseRttiTypeRecord& operator=(BaseRttiTypeRecord&& from) {
			this->~BaseRttiTypeRecord();
			this->BaseRttiTypeRecord::BaseRttiTypeRecord(MoveRValue(from));
			return *this;
		}

		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~BaseRttiTypeRecord() {
			record = nullptr;
			imageBase = 0;
			isValidCxxTypeId = false;
		}

	public:
		/// <summary>
		/// Indicates if the type record is valid
		/// </summary>
		/// <returns>"True" if the type record is valid</returns>
		constexpr Boolean GetIsValid() const {
			return record != nullptr;
		}

		/// <summary>
		/// Indicates if the C++ type ID is valid
		/// </summary>
		/// <returns>"True" if the C++ type ID is valid</returns>
		constexpr Boolean GetIsValidCxxTypeId() const {
			return IsValidCxxTypeId;
		}

		/// <summary>
		/// Receiving the type record with type ID
		/// </summary>
		/// <returns>Record with type ID</returns>
		constexpr SizeT GetTypeId() const {
			return record->TypeId.TypeId + imageBase;
		}

		/// <summary>
		/// Receiving the type record with full type ID
		/// </summary>
		/// <returns>Record with type ID</returns>
		constexpr UniversalTypeIdentificator GetFullTypeId() const {
			if (record == nullptr)
				return UniversalTypeIdentificator();
			else
				return record->TypeId;
		}

		/// <summary>
		/// Receiving the type record with type class
		/// </summary>
		/// <returns>Record with type class</returns>
		constexpr CTTI::EType GetTypeClass() const {
			return record == nullptr ? CTTI::EType::Undefine : record->TypeClass;
		}

		/// <summary>
		/// Receiving the type record with binary size
		/// </summary>
		/// <returns>Record with binary size</returns>
		constexpr SizeT GetBinarySize() const {
			return record->MetaDataForDynamic.BinarySize;
		}

		/// <summary>
		/// Receiving the type record with type modifiers
		/// </summary>
		/// <returns>Record with type modifiers</returns>
		constexpr TypeModificator GetTypeModificators() const {
			return record->TypeModificators;
		}

		/// <summary>
		/// Receiving the type record with type modifiers
		/// </summary>
		/// <returns>Record with type modifiers</returns>
		ConstString GetTypeName() const {
			Unsafe(return ConstString(record->TypeName.Text, record->TypeName.Size));
		}

		/// <summary>
		/// Receiving the type record with C++ type name
		/// </summary>
		/// <returns>Record with C++ type name</returns>
		ConstString GetCxxTypeName() const {
			Unsafe(
				auto pTypeName = (const MetaData::TextMetaData*) (
					((SizeT) (Byte*) record) +
					StructOffsetOfMember(MetaData::MetaData, TypeName.Text) +
					(record->TypeName.Size * sizeof(Char)));

			return ConstString(pTypeName->Text, pTypeName->Size);
			);
		}

		/// <summary>
		/// Receiving the type record with type information
		/// </summary>
		/// <returns>Record with type information</returns>
		CxxTypeInfoT GetTypeInfo() const {
			if (isValidCxxTypeId)
			{
				auto f = (CxxTypeInfoT(*)())(record->TypeId.TypeId + imageBase);
				return f();
			}
			else
			{
				return nullptr;
			}
		}

		/// <summary>
		/// Receiving the serialization schema for type record
		/// </summary>
		/// <returns>Serialization schema</returns>
		ConstString GetSerializationSchema() const {
			auto pTypeName = (const MetaData::TextMetaData*) (((SizeT) (Byte*) record) + StructOffsetOfMember(MetaData::MetaData, TypeName.Text) + (record->TypeName.Size * sizeof(Char)));

			auto pSchema = (const MetaData::TextMetaData*) (((SizeT) pTypeName->Text) + (pTypeName->Size * sizeof(Char)));

			return ConstString(pSchema->Text, pSchema->Size);
		}

		/// <summary>
		/// Receiving the template type for type record
		/// </summary>
		/// <returns>Template type name</returns>
		ConstString GetTemplateTypeName() const {
			auto pTypeName = (const MetaData::TextMetaData*) (((SizeT) (Byte*) record) + StructOffsetOfMember(MetaData::MetaData, TypeName.Text) + (record->TypeName.Size * sizeof(Char)));

			auto pSchema = (const MetaData::TextMetaData*) (((SizeT) pTypeName->Text) + (pTypeName->Size * sizeof(Char)));

			auto pTemplateTypeName = (const MetaData::TextMetaData*) (((SizeT) pSchema->Text) + (pSchema->Size * sizeof(Char)));

			return ConstString(pTemplateTypeName->Text, pTemplateTypeName->Size);
		}

	public:
		/// <summary>
		/// A property for indicating if the type record is valid
		/// </summary>
		Boolean Property(GetIsValid) IsValid;

		/// <summary>
		/// A property for indicating if the C++ type record is valid
		/// </summary>
		Boolean Property(GetIsValidCxxTypeId) IsValidCxxTypeId;

		/// <summary>
		/// A property for receiving the type record with type ID
		/// </summary>
		SizeT Property(GetTypeId) TypeId;

		/// <summary>
		/// A property for receiving the type record with full type ID
		/// </summary>
		UniversalTypeIdentificator Property(GetFullTypeId) FullTypeId;

		/// <summary>
		/// A property for receiving the type record with type class
		/// </summary>
		CTTI::EType Property(GetTypeClass) TypeClass;

		/// <summary>
		/// A property for receiving the type record with C++ type class
		/// </summary>
		CTTI::EType Property(GetTypeClass) CxxTypeClass;

		/// <summary>
		/// A property for receiving the type record with binary size
		/// </summary>
		SizeT Property(GetBinarySize) BinarySize;

		/// <summary>
		/// A property for receiving the type record with type modifiers
		/// </summary>
		TypeModificator Property(GetTypeModificators) TypeModificators;

		/// <summary>
		/// A property for receiving the type record with type name
		/// </summary>
		ConstString Property(GetTypeName) TypeName;

		/// <summary>
		/// A property for receiving the type record with C++ type name
		/// </summary>
		ConstString Property(GetCxxTypeName) CxxTypeName;

		/// <summary>
		/// A property for receiving the serialization schema of type record
		/// </summary>
		ConstString Property(GetSerializationSchema) SerializationSchema;

		/// <summary>
		/// A property for receiving the template type name of type record
		/// </summary>
		ConstString Property(GetTemplateTypeName) TemplateTypeName;

		/// <summary>
		/// A property for receiving the type record with C++ type information
		/// </summary>
		CxxTypeInfoT Property(GetTypeInfo) CxxTypeInfo;

	private:
		SizeT GetCTTIMetaDataOffset() const {
			return record->OffsetToCttiMetaData;
		}

		SizeT GetItemsCollectionOffset() const {
			return GetCTTIMetaDataOffset() + *Memory::UnSafe::AddOffsetAs<SizeT>(record, GetCTTIMetaDataOffset());
		}

		SizeT GetCustomCttiTypeStrcutSchemaRttiOffset() const {
			auto metaData = Memory::UnSafe::AddOffsetAs<const MetaData::StructCustomTypeMetaData>(record, GetCTTIMetaDataOffset());

			return (metaData->SchemaFormat == ECustomTypeSchemaFormat::StructSchema ||
				metaData->SchemaFormat == ECustomTypeSchemaFormat::RemoteStructSchema)
				? metaData->OffsetToCollection
				: 0;
		}

		SizeT GetFieldsRecordsOffset() const {
			switch (record->TypeClass)
			{
				case EType::Tuple:
				return GetItemsCollectionOffset();

				case EType::Enumeration:
				return GetItemsCollectionOffset();

				case EType::FixedArray:
				return  GetItemsCollectionOffset();

				case EType::Struct:
				return GetItemsCollectionOffset();

				case EType::Union:
				return GetItemsCollectionOffset();

				case EType::Collection:
				return GetItemsCollectionOffset();

				case EType::String:
				case EType::Pointer:
				case EType::Box:
				case EType::Pair:
				return GetItemsCollectionOffset();

				case EType::FixedCollection:
				return GetItemsCollectionOffset();

				case EType::AssociableCollection:
				case EType::Reference:
				return GetItemsCollectionOffset();

				case EType::Custom:
				return GetCustomCttiTypeStrcutSchemaRttiOffset();

				case EType::Object:

				default:
				return 0;
			}
		}

	public:
		/// <summary>
		/// Receiving the fields of type record
		/// </summary>
		/// <returns>Type record fields</returns>
		BaseRttiCollection<THeap, BaseRttiTypeRecord<THeap>> GetFields() const {
			auto offset = GetFieldsRecordsOffset();
			if (offset == 0)
			{
				return BaseRttiFieldsCollection<THeap>((const MetaData::Collection*)nullptr, isValidCxxTypeId, imageBase);
			}
			else
			{
				return BaseRttiFieldsCollection<THeap>((const MetaData::Collection*) ((const Byte*) record + offset), isValidCxxTypeId, imageBase);
			}
		}

		/// <summary>
		/// A property for receiving the fields of type record
		/// </summary>
		BaseRttiCollection<THeap, BaseRttiTypeRecord<THeap>> Property(GetFields) Fields;

	private:
		const MetaData::ObjectMetaData* GetObjectRecord() const {
			return (const MetaData::ObjectMetaData*) (((const Byte*) record) + GetCTTIMetaDataOffset());
		}

		const MetaData::ObjectCustomTypeMetaData* GetCustomTypeRecord() const {
			return (const MetaData::ObjectCustomTypeMetaData*) (((const Byte*) record) + GetCTTIMetaDataOffset());
		}

	public:
		/// <summary>
		/// Receiving the constructors of type record
		/// </summary>
		/// <returns>Type record constructors</returns>
		BaseRttiCollection<THeap, BaseRttiConstructorRecord<THeap>> GetConstructors() const {
			if (record->TypeClass == EType::Object || record->TypeClass == EType::Abstract)
			{
				auto objRecord = GetObjectRecord();

				return BaseRttiConstructorsCollection<THeap>(
					Memory::UnSafe::AddOffsetAs<const MetaData::Collection>(objRecord, objRecord->ConstructorsOffset),
					isValidCxxTypeId,
					imageBase);
			}
			else if (
				record->TypeClass == EType::Custom &&
				(
					GetCustomTypeRecord()->SchemaFormat == ECustomTypeSchemaFormat::ObjectSchema ||
					GetCustomTypeRecord()->SchemaFormat == ECustomTypeSchemaFormat::RemoteObjectSchema
					))
			{
				auto customObjRecord = GetCustomTypeRecord();

				return  BaseRttiConstructorsCollection<THeap>(
					Memory::UnSafe::AddOffsetAs<const MetaData::Collection>(customObjRecord, customObjRecord->ConstructorsOffset),
					isValidCxxTypeId,
					imageBase);
			}
			else
			{
				return BaseRttiConstructorsCollection<THeap>(nullptr, isValidCxxTypeId, imageBase);
			}
		}

		/// <summary>
		/// Receiving the interfaces of type record
		/// </summary>
		/// <returns>Type record interfaces</returns>
		BaseRttiCollection<THeap, BaseRttiIntefaceRecord<THeap>> GetInterfaces() const {
			if (record->TypeClass == EType::Object || record->TypeClass == EType::Abstract)
			{
				auto objRecord = GetObjectRecord();

				return BaseRttiInterfacesCollection<THeap>(
					Memory::UnSafe::AddOffsetAs<const MetaData::Collection>(objRecord, objRecord->InterfacesOffset),
					isValidCxxTypeId,
					imageBase);
			}
			else if (
				record->TypeClass == EType::Custom &&
				(
					GetCustomTypeRecord()->SchemaFormat == ECustomTypeSchemaFormat::ObjectSchema ||
					GetCustomTypeRecord()->SchemaFormat == ECustomTypeSchemaFormat::RemoteObjectSchema
					))
			{
				auto customObjRecord = GetCustomTypeRecord();

				return  BaseRttiInterfacesCollection<THeap>(
					Memory::UnSafe::AddOffsetAs<const MetaData::Collection>(customObjRecord, customObjRecord->InterfacesOffset),
					isValidCxxTypeId,
					imageBase);
			}
			else
			{
				return BaseRttiInterfacesCollection<THeap>(nullptr, isValidCxxTypeId, imageBase);
			}
		}

		/// <summary>
		/// Receiving the properties of type record
		/// </summary>
		/// <returns>Type record properties</returns>
		BaseRttiCollection<THeap, BaseRttiProperty<THeap>> GetProperties() const {
			if (record->TypeClass == EType::Object || record->TypeClass == EType::Abstract)
			{
				auto objRecord = GetObjectRecord();

				return BaseRttiPropertiesCollection<THeap>(
					Memory::UnSafe::AddOffsetAs<const MetaData::Collection>(objRecord, objRecord->PropertiesOffset),
					isValidCxxTypeId,
					imageBase);
			}
			else if (
				record->TypeClass == EType::Custom &&
				(
					GetCustomTypeRecord()->SchemaFormat == ECustomTypeSchemaFormat::ObjectSchema ||
					GetCustomTypeRecord()->SchemaFormat == ECustomTypeSchemaFormat::RemoteObjectSchema
					))
			{
				auto customObjRecord = GetCustomTypeRecord();

				return BaseRttiPropertiesCollection<THeap>(
					Memory::UnSafe::AddOffsetAs<const MetaData::Collection>(customObjRecord, customObjRecord->PropertiesOffset),
					isValidCxxTypeId,
					imageBase);
			}
			else
			{
				return BaseRttiPropertiesCollection<THeap>(nullptr, isValidCxxTypeId, imageBase);
			}
		}

		/// <summary>
		/// Receiving the methods of type record
		/// </summary>
		/// <returns>Type methods fields</returns>
		BaseRttiCollection<THeap, BaseRttiMethodRecord<THeap>> GetMethods() const {
			if (record->TypeClass == EType::Object || record->TypeClass == EType::Abstract)
			{
				auto objRecord = GetObjectRecord();

				return BaseRttiMethodsCollection<THeap>(
					Memory::UnSafe::AddOffsetAs<const MetaData::Collection>(objRecord, objRecord->MethodsOffset),
					isValidCxxTypeId,
					imageBase);
			}
			else if (
				record->TypeClass == EType::Custom &&
				(
					GetCustomTypeRecord()->SchemaFormat == ECustomTypeSchemaFormat::ObjectSchema ||
					GetCustomTypeRecord()->SchemaFormat == ECustomTypeSchemaFormat::RemoteObjectSchema
					))
			{
				auto customObjRecord = GetCustomTypeRecord();

				return BaseRttiMethodsCollection<THeap>(
					Memory::UnSafe::AddOffsetAs<const MetaData::Collection>(customObjRecord, customObjRecord->MethodsOffset),
					isValidCxxTypeId,
					imageBase);
			}
			else
			{
				return BaseRttiMethodsCollection<THeap>(nullptr, isValidCxxTypeId, imageBase);
			}
		}

	public:
		/// <summary>
		/// A property for receiving the constructors of type record
		/// </summary>
		BaseRttiCollection<THeap, BaseRttiConstructorRecord<THeap>> Property(GetConstructors) Constructors;

		/// <summary>
		/// A property for receiving the interfaces of type record
		/// </summary>
		BaseRttiCollection<THeap, BaseRttiIntefaceRecord<THeap>> Property(GetInterfaces) Interfaces;

		/// <summary>
		/// A property for receiving the properties of type record
		/// </summary>
		BaseRttiCollection<THeap, BaseRttiProperty<THeap>> Property(GetProperties) Properties;

		/// <summary>
		/// A property for receiving the methods of type record
		/// </summary>
		BaseRttiCollection<THeap, BaseRttiMethodRecord<THeap>> Property(GetMethods) Methods;

	public:
		/// <summary>
		/// Receive custom CTTI type schema format
		/// For non custom type return ECustomTypeSchemaFormat::Undefined value
		/// </summary>
		/// <returns>One of custom CTTI type schema formats</returns>
		ECustomTypeSchemaFormat GetCustomTypeSchemaFormat() const {
			if (record->TypeClass == EType::Custom)
			{
				auto metaData = Memory::UnSafe::AddOffsetAs<const MetaData::CustomTypeMetaData>(record, GetCTTIMetaDataOffset());

				return metaData->SchemaFormat;
			}
			else
			{
				return ECustomTypeSchemaFormat::Undefined;
			}
		}

	public:
		/// <summary>
		/// A property for receiving custom CTTI type schema format
		/// For non custom type return ECustomTypeSchemaFormat::Undefined value
		/// </summary>
		/// <returns>One of custom CTTI type schema formats</returns>
		ECustomTypeSchemaFormat Property(GetCustomTypeSchemaFormat) CustomTypeSchemaFormat;

	public:
		/// <summary>
		/// Return fixed array or collection items count
		/// </summary>
		/// <returns>Items count</returns>
		SizeT GetArrayItemsCount() const {
			if (this->record->TypeClass == EType::FixedArray || this->record->TypeClass == EType::FixedCollection)
			{
				return Memory::UnSafe::AddOffsetAs<const MetaData::Collection>(this->record, this->record->OffsetToCttiMetaData)->Count;
			}
			else
			{
				return 0;
			}
		}

	public:
		/// <summary>
		/// Fixed array or collection items count
		/// </summary>
		SizeT Property(GetArrayItemsCount) ArrayItemsCount;

	public:
		/// <summary>
		/// Spaceship operator
		/// </summary>
		/// <param name="other">Comparable type record</param>
		/// <returns>Compare result</returns>
		Comparing::StrongOrdering operator<=>(const BaseRttiTypeRecord& other) const {
			auto r = this->record <=> other.record;
			if (r == 0)
				return r;

			if (this->record == nullptr)
				return Comparing::StrongOrdering::less;
			else if (other.record == nullptr)
				return Comparing::StrongOrdering::greater;

			if (isValidCxxTypeId && other.isValidCxxTypeId)
			{
				auto r2 = this->record->TypeId <=> other.record->TypeId;

				return r2;
			}
			else
			{
				return this->CxxTypeName <=> other.CxxTypeName;
			}
		}

		/// <summary>
		/// Spaceship operator
		/// </summary>
		/// <param name="other">Comparable type information field</param>
		/// <returns>Compare result</returns>
		Comparing::StrongOrdering operator<=>(const TypeInfo& other) const {
			if (this->record == nullptr)
				return Comparing::StrongOrdering::less;

			if (isValidCxxTypeId)
			{
				auto typeInfo = this->CxxTypeInfo;
				auto r = typeInfo->HashCode <=> other.HashCode;
				return r;
			}
			else
			{
				Unsafe(
					auto sz = CStr::Size(other.GetCxxName());
				auto otherCxxTypeName = String(other.GetCxxName(), sz);

				return this->CxxTypeName <=> otherCxxTypeName;
				);
			}
		}

		/// <summary>
		/// Spaceship operator
		/// </summary>
		/// <param name="first">First operand</param>
		/// <param name="second">Second operand</param>
		/// <returns>Compare result</returns>
		friend Comparing::StrongOrdering operator<=>(const TypeInfo& first, const BaseRttiTypeRecord& second) {
			if (second.record == nullptr)
				return Comparing::StrongOrdering::greater;

			if (second.isValidCxxTypeId)
			{
				auto typeInfo = second.CxxTypeInfo;
				auto r = first.HashCode <=> typeInfo->HashCode;
				return r;
			}
			else
			{
				Unsafe(
					auto sz = CStr::Size(first.GetCxxName());
				auto otherCxxTypeName = String(first.GetCxxName(), sz);

				return otherCxxTypeName <=> second.CxxTypeName;
				);
			}
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		/// <param name="other">Comparable type record</param>
		/// <returns>Equality result</returns>
		bool operator==(const BaseRttiTypeRecord& other) const { return *this <=> other == 0; }

		/// <summary>
		/// Inequality operator
		/// </summary>
		/// <param name="other">Comparable type record</param>
		/// <returns>Inequality</returns>
		bool operator!=(const BaseRttiTypeRecord& other) const { return *this <=> other != 0; }

		/// <summary>
		/// Equality operator
		/// </summary>
		/// <param name="other">Comparable type information</param>
		/// <returns>Equality result</returns>
		bool operator==(const TypeInfo& other) const {
			return *this <=> other == 0;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		/// <param name="other">Comparable type information</param>
		/// <returns>Inequality</returns>
		bool operator!=(const TypeInfo& other) const {
			return *this <=> other != 0;
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		/// <param name="first">First operand</param>
		/// <param name="second">Second operand</param>
		/// <returns>Equality result</returns>
		friend bool operator==(const TypeInfo& first, const BaseRttiTypeRecord& second) {
			return first <=> second == 0;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		/// <param name="first">First operand</param>
		/// <param name="second">Second operand</param>
		/// <returns>Inequality result</returns>
		friend bool operator!=(const TypeInfo& first, const BaseRttiTypeRecord& second) {
			return first <=> second != 0;
		}

	public:
		template<typename T>
		friend BaseRttiTypeRecord<Memory::Heap> GetRttiTypeRecord();

		template<typename UHeap, typename T>
		friend BaseRttiTypeRecord<UHeap> GetRttiTypeRecord();

	private:
		friend class BaseRttiTypeRecord;

	private:
		template<typename T>
		friend struct MetaData::GetRttiRecordGetter;

		friend class Internals::RttiInternals;

	public:
		friend class Cardinal::RTTI::Dynamic;

		template<typename>
		friend class BaseTraveler;
	};

	/// <summary>
	/// Receiving the field of type record
	/// </summary>
	/// <typeparam name="THeap">Heap type</typeparam>
	/// <returns>Type record field</returns>
	template<typename THeap>
	inline BaseRttiTypeRecord<THeap> Cardinal::RTTI::Record::BaseRttiField<THeap>::GetFieldTypeRecord() const {
		if (!this->it.isValidCxxTypeId)
		{
			Throw(NullPointException(SourcePosition));
		}

		return RTTI::Internals::RttiInternals::BuildRTTITypeRecord(
			this->GetRecord()->MetaDataForDynamic.GetRttiRecord(),
			this->it.isValidCxxTypeId,
			this->it.imageBase);
	}

	/// <summary>
	/// Receiving the property of type record
	/// </summary>
	/// <typeparam name="THeap">Heap type</typeparam>
	/// <returns>Type record property</returns>
	template<typename THeap>
	inline BaseRttiTypeRecord<THeap> Cardinal::RTTI::Record::BaseRttiProperty<THeap>::GetPropertyTypeRecord() const {
		if (!this->it.isValidCxxTypeId)
		{
			Throw(NullPointException(SourcePosition));
		}

		return RTTI::Internals::RttiInternals::BuildRTTITypeRecord(
			this->GetRecord()->PropertyType.MetaDataForDynamic.GetRttiRecord(),
			this->it.isValidCxxTypeId,
			this->it.imageBase);
	}

	/// <summary>
	/// Receiving the type record
	/// </summary>
	/// <typeparam name="THeap">Heap type</typeparam>
	/// <returns>Type record</returns>
	template<typename THeap>
	inline BaseRttiTypeRecord<THeap> Cardinal::RTTI::Record::BaseRttiIntefaceRecord<THeap>::GetTypeRecord() const {
		if (!this->it.isValidCxxTypeId)
		{
			Throw(NullPointException(SourcePosition));
		}

		return RTTI::Internals::RttiInternals::BuildRTTITypeRecord(
			this->GetRecord()->MetaDataForDynamic.GetRttiRecord(),
			this->it.isValidCxxTypeId,
			this->it.imageBase);
	}

	template<typename THeap>
	inline BaseRttiTypeRecord<THeap> Cardinal::RTTI::Record::BaseRttiConstructorRecord<THeap>::GetTypeRecord() const {
		if (!this->it.isValidCxxTypeId)
		{
			Throw(NullPointException(SourcePosition));
		}

		return RTTI::Internals::RttiInternals::BuildRTTITypeRecord(
			this->GetRecord()->GetRttiRecord(),
			this->it.isValidCxxTypeId,
			this->it.imageBase);
	}

	template<typename THeap>
	inline BaseRttiTypeRecord<THeap> Cardinal::RTTI::Record::BaseRttiArgumentRecord<THeap>::GetTypeRecord() const {
		if (!this->it.isValidCxxTypeId)
		{
			Throw(NullPointException(SourcePosition));
		}

		return RTTI::Internals::RttiInternals::BuildRTTITypeRecord(
			this->GetRecord()->MetaDataForDynamic.GetRttiRecord(),
			this->it.isValidCxxTypeId,
			this->it.imageBase);
	}
}

#pragma warning(pop)