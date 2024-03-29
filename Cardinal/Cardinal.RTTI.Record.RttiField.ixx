export module Cardinal.RTTI:Record.RttiField;

export import :Record.Base;
export import :Record.RttiIterator;

export namespace Cardinal::RTTI::Record
{
	/// <summary>
	/// A base RTTI field class
	/// </summary>
	/// <typeparam name="THeap">Heap type</typeparam>
	template<typename THeap>
	class BaseRttiField final : public Memory::Allocator<THeap> {
		BaseRttiIterator<THeap, BaseRttiField> it;
	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="it">Iterator</param>
		BaseRttiField(BaseRttiIterator<THeap, BaseRttiField> it) : it(it) {}

	private:
		ForceInline const MetaData::FieldMetaData* GetRecord() const { return (const MetaData::FieldMetaData*) it.item; }

	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="from">Source</param>
		BaseRttiField(const BaseRttiField& from) : it(from.it) {}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="from">RValue reference to a source</param>
		BaseRttiField(BaseRttiField&& from) : it(MoveRValue(from.it)) {}

		/// <summary>
		/// Assignment operator
		/// </summary>
		/// <param name="from">Source</param>
		/// <returns>Assigned value</returns>
		BaseRttiField& operator=(const BaseRttiField& from) {
			this->~BaseRttiField();
			this->BaseRttiField::BaseRttiField(from);
			return *this;
		}

		/// <summary>
		/// Assignment operator
		/// </summary>
		/// <param name="from">RValue reference to a source</param>
		/// <returns>Assigned value</returns>
		BaseRttiField& operator=(BaseRttiField&& from) {
			this->~BaseRttiField();
			this->BaseRttiField::BaseRttiField(MoveRValue(from));
			return *this;
		}

		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~BaseRttiField() {
			it.~BaseRttiIterator();
		}

	public:
		/// <summary>
		/// Receiving the field record with type ID
		/// </summary>
		/// <returns>Record with type ID</returns>
		SizeT GetTypeId() const {
			return ((SizeT) GetRecord()->TypeId.TypeId) + it.imageBase;
		}

		/// <summary>
		/// Receiving the field record with full type ID
		/// </summary>
		/// <returns>Record with type ID</returns>
		UniversalTypeIdentificator GetFullTypeId() const {
			return GetRecord()->TypeId;
		}

		/// <summary>
		/// Receiving the field record with type class
		/// </summary>
		/// <returns>Record with type class</returns>
		CTTI::EType GetTypeClass() const {
			return GetRecord()->TypeClass;
		}

		/// <summary>
		/// Receiving the field record with binary size
		/// </summary>
		/// <returns>Record with binary size</returns>
		SizeT GetBinarySize() const {
			return GetRecord()->MetaDataForDynamic.BinarySize;
		}

		/// <summary>
		/// Receiving the field offset
		/// </summary>
		/// <returns>Field offset</returns>
		SizeT GetFieldOffset() const {
			return GetRecord()->Offset;
		}

		/// <summary>
		/// Receiving the field record with type modifiers
		/// </summary>
		/// <returns>Record with type modifiers</returns>
		TypeModificator GetTypeModificators() const {
			return GetRecord()->TypeModificators;
		}

		/// <summary>
		/// Receiving the record with field name
		/// </summary>
		/// <returns>Record with field name</returns>
		ConstString GetFieldName() const {
			Unsafe(return ConstString(GetRecord()->FieldName.Text, GetRecord()->FieldName.Size)); //-V612
		}

		/// <summary>
		/// Receiving the field record with type name
		/// </summary>
		/// <returns>Record with type name</returns>
		ConstString GetTypeName() const {
			Unsafe( //-V612
				auto pTypeName = (const MetaData::TextMetaData*) (
					((Byte*) GetRecord()) +
					StructOffsetOfMember(MetaData::FieldMetaData, FieldName.Text) +
					(GetRecord()->FieldName.Size * sizeof(Char)));

			return ConstString(pTypeName->Text, pTypeName->Size);
			);
		}

		/// <summary>
		/// Receiving the field record with type information
		/// </summary>
		/// <returns>Record with type information</returns>
		CxxTypeInfoT GetTypeInfo() const {
			if (it.isValidCxxTypeId)
			{
				auto f = (CxxTypeInfoT(*)())(GetRecord()->TypeId.TypeId + it.imageBase);
				return f();
			}
			else
			{
				return nullptr;
			}
		}

		/// <summary>
		/// Receiving the field type record
		/// </summary>
		/// <returns>Fielf type record</returns>
		BaseRttiTypeRecord<THeap> GetFieldTypeRecord() const;

	public:
		/// <summary>
		/// A property for receiving the field record with type ID
		/// </summary>
		SizeT Property(GetTypeId) TypeId;

		/// <summary>
		/// A property for receiving the field record with full type ID
		/// </summary>
		UniversalTypeIdentificator Property(GetFullTypeId) FullTypeId;

		/// <summary>
		/// A property for receiving the field record with type class
		/// </summary>
		CTTI::EType Property(GetTypeClass) TypeClass;

		/// <summary>
		/// A property for receiving the field record with binary size
		/// </summary>
		SizeT Property(GetBinarySize) BinarySize;

		/// <summary>
		/// Field offset
		/// </summary>
		SizeT Property(GetFieldOffset) FieldOffset;

		/// <summary>
		/// A property for receiving the field record with type modifiers
		/// </summary>
		TypeModificator Property(GetTypeModificators) TypeModificators;

		/// <summary>
		/// A property for receiving the field record with type name
		/// </summary>
		ConstString Property(GetTypeName) TypeName;

		/// <summary>
		/// A property for receiving the record with field name
		/// </summary>
		ConstString Property(GetFieldName) FieldName;

		/// <summary>
		/// A property for receiving the field record with type information
		/// </summary>
		CxxTypeInfoT Property(GetTypeInfo) TypeInfo;

		/// <summary>
		/// A property for receiving the field type record
		/// </summary>
		BaseRttiTypeRecord<THeap> Property(GetFieldTypeRecord) FieldTypeRecord;

	private:
		friend class Internals::RttiInternals;

	public:
		template<typename>
		friend class BaseTraveler;
	};
}