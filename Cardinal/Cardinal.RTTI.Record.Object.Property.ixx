export module Cardinal.RTTI:Record.Object.Property;

export import :Record.Object.Base;
export import :Record.Object.Method;

export namespace Cardinal::RTTI::Record
{
	/// <summary>
	/// A base RTTI property record class
	/// </summary>
	/// <typeparam name="THeap">Heap type</typeparam>
	template<typename THeap>
	class BaseRttiProperty final : public Memory::Allocator<THeap> {
	private:
		BaseRttiIterator<THeap, BaseRttiProperty> it;

	protected:
		BaseRttiProperty(BaseRttiIterator<THeap, BaseRttiProperty> it) : it(it) {}

	private:
		const MetaData::PropertyRecord* GetRecord() const { return (const MetaData::PropertyRecord*) it.item; }

	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="from">Source</param>
		BaseRttiProperty(const BaseRttiProperty& from) : it(from.it) {}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="from">RValue reference to a source</param>
		BaseRttiProperty(BaseRttiProperty&& from) : it(MoveRValue(from.it)) {}

		/// <summary>
		/// Assignment operator
		/// </summary>
		/// <param name="from">Source</param>
		/// <returns>Assigned value</returns>
		BaseRttiProperty& operator=(const BaseRttiProperty& from) {
			this->~BaseRttiProperty();
			this->BaseRttiProperty::BaseRttiProperty(from);
			return *this;
		}

		/// <summary>
		/// Assignment operator
		/// </summary>
		/// <param name="from">RValue reference to a source</param>
		/// <returns>Assigned value</returns>
		BaseRttiProperty& operator=(BaseRttiProperty&& from) {
			this->~BaseRttiProperty();
			this->BaseRttiProperty::BaseRttiProperty(MoveRValue(from));
			return *this;
		}

		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~BaseRttiProperty() {
			it.~BaseRttiIterator();
		}

	public:
		/// <summary>
		/// Receiving the property record with type ID
		/// </summary>
		/// <returns>Record with type ID</returns>
		SizeT GetTypeId() const {
			return ((SizeT) GetRecord()->PropertyType.TypeId.TypeId) + it.imageBase;
		}

		/// <summary>
		/// Receiving the property record with full type ID
		/// </summary>
		/// <returns>Record with full type ID</returns>
		UniversalTypeIdentificator GetFullTypeId() const {
			return GetRecord()->PropertyType.TypeId;
		}

		/// <summary>
		/// Receiving the property record with type class
		/// </summary>
		/// <returns>Record with type class</returns>
		CTTI::EType GetTypeClass() const {
			return GetRecord()->PropertyType.TypeClass;
		}

		/// <summary>
		/// Receiving the property record with binary size
		/// </summary>
		/// <returns>Record with binary size</returns>
		SizeT GetBinarySize() const {
			return GetRecord()->PropertyType.MetaDataForDynamic.BinarySize;
		}

		/// <summary>
		/// Receiving the property record with type modifiers
		/// </summary>
		/// <returns>Record with type modifiers</returns>
		TypeModificator GetTypeModificators() const {
			return GetRecord()->PropertyType.TypeModificators;
		}

		/// <summary>
		/// Receiving the property record with name
		/// </summary>
		/// <returns>Property name</returns>
		ConstString GetName() const {
			Unsafe(return ConstString(GetRecord()->PropertyType.FieldName.Text, GetRecord()->PropertyType.FieldName.Size));
		}

		/// <summary>
		/// Receiving the property record with type name
		/// </summary>
		/// <returns>Property type name</returns>
		ConstString GetTypeName() const {
			Unsafe(
				auto pTypeName = (const MetaData::TextMetaData*) (
					((Byte*) GetRecord()) +
					StructOffsetOfMember(MetaData::PropertyRecord, PropertyType.FieldName.Text) +
					(GetRecord()->PropertyType.FieldName.Size * sizeof(Char)));

			return ConstString(pTypeName->Text, pTypeName->Size);
			);
		}

		/// <summary>
		/// Receiving the property record with getters
		/// </summary>
		/// <returns>Record with getters</returns>
		BaseRttiCollection<THeap, BaseRttiMethodRecord<THeap>> GetGetters() const {
			auto base = (const MetaData::Collection*) (((const Byte*) GetRecord()) + GetRecord()->OffsetToGetters);

			return BaseRttiMethodRecord<THeap>::AsMethodCollection(base, it.isValidCxxTypeId, it.imageBase);
		}

		/// <summary>
		/// Receiving the property record with setters
		/// </summary>
		/// <returns>Record with setters</returns>
		BaseRttiCollection<THeap, BaseRttiMethodRecord<THeap>> GetSetters() const {
			auto base = (const MetaData::Collection*) (((const Byte*) GetRecord()) + GetRecord()->OffsetToSetters);

			return BaseRttiMethodRecord<THeap>::AsMethodCollection(base, it.isValidCxxTypeId, it.imageBase);
		}

		/// <summary>
		/// Receiving the property record with type information
		/// </summary>
		/// <returns>Record with type information</returns>
		CxxTypeInfoT GetTypeInfo() const {
			if (it.isValidCxxTypeId)
			{
				auto f = (CxxTypeInfoT(*)())(GetRecord()->PropertyType.TypeId.TypeId + it.imageBase);
				return f();
			}
			else
			{
				return nullptr;
			}
		}

		/// <summary>
		/// Indicates if the property is rw
		/// </summary>
		/// <returns>"True" if the record is rw</returns>
		Boolean GetIsRw() const {
			auto gettersRw = (const MetaData::Collection*) (((const Byte*) GetRecord()) + GetRecord()->OffsetToGetters);
			auto settersRw = (const MetaData::Collection*) (((const Byte*) GetRecord()) + GetRecord()->OffsetToSetters);

			return gettersRw->Count > 0 && settersRw->Count > 0;
		}

		/// <summary>
		/// Receiving the property type record
		/// </summary>
		/// <returns>Property type record</returns>
		BaseRttiTypeRecord<THeap> GetPropertyTypeRecord() const;

	public:
		/// <summary>
		/// A property for receiving the argument record with type ID
		/// </summary>
		SizeT Property(GetTypeId) TypeId;

		/// <summary>
		/// A property for receiving the argument record with full type ID
		/// </summary>
		UniversalTypeIdentificator Property(GetFullTypeId) FullTypeId;

		/// <summary>
		/// A property for receiving the argument record with type class
		/// </summary>
		CTTI::EType Property(GetTypeClass) TypeClass;

		/// <summary>
		/// A property for receiving the argument record with binary size
		/// </summary>
		SizeT Property(GetBinarySize) BinarySize;

		/// <summary>
		/// A property for receiving the argument record with type modifiers
		/// </summary>
		TypeModificator Property(GetTypeModificators) TypeModificators;

		/// <summary>
		/// A property for receiving the argument record with type name
		/// </summary>
		ConstString Property(GetTypeName) TypeName;

		/// <summary>
		/// A property for receiving the argument record with name
		/// </summary>
		ConstString Property(GetName) Name;

		/// <summary>
		/// A property for receiving the argument record with getters
		/// </summary>
		BaseRttiCollection<THeap, BaseRttiMethodRecord<THeap>> Property(GetGetters) Getters;

		/// <summary>
		/// A property for receiving the argument record with setters
		/// </summary>
		BaseRttiCollection<THeap, BaseRttiMethodRecord<THeap>> Property(GetSetters) Setters;

		/// <summary>
		/// A property for receiving the argument record with type info
		/// </summary>
		CxxTypeInfoT Property(GetTypeInfo) TypeInfo;

		/// <summary>
		/// A property with indicating if the property is RW
		/// </summary>
		Boolean Property(GetIsRw) IsRw;

		/// <summary>
		/// A property for receiving the argument type record
		/// </summary>
		BaseRttiTypeRecord<THeap> Property(GetPropertyTypeRecord) PropertyType;

	private:
		friend class Internals::RttiInternals;

	public:
		template<typename UHeap, typename T>
		friend class BaseRttiIterator;

		friend class Dynamic;

		template<typename>
		friend class BaseTraveler;
	};
}