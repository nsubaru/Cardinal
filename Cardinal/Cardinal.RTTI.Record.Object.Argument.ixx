export module Cardinal.RTTI:Record.Object.Argument;

export import :Record.Object.Base;

export namespace Cardinal::RTTI::Record
{
	/// <summary>
	/// A base RTTI argument record class
	/// </summary>
	/// <typeparam name="THeap">Heap type</typeparam>
	template<typename THeap>
	class BaseRttiArgumentRecord final : public Memory::Allocator<THeap> {
	private:
		BaseRttiIterator<THeap, BaseRttiArgumentRecord> it;

	private:
		BaseRttiArgumentRecord(BaseRttiIterator<THeap, BaseRttiArgumentRecord> it) : it(it) {}

	private:
		ForceInline const MetaData::ArgumentMetaData* GetRecord() const { return (const MetaData::ArgumentMetaData*) it.item; }

	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="from">Source</param>
		BaseRttiArgumentRecord(const BaseRttiArgumentRecord& from) : it(from.it) {}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="from">RValue reference to a source</param>
		BaseRttiArgumentRecord(BaseRttiArgumentRecord&& from) : it(MoveRValue(from.it)) {}

		/// <summary>
		/// Assignment operator
		/// </summary>
		/// <param name="from">Source</param>
		/// <returns>Assigned value</returns>
		BaseRttiArgumentRecord& operator=(const BaseRttiArgumentRecord& from) {
			this->~BaseRttiArgumentRecord();
			this->BaseRttiArgumentRecord::BaseRttiArgumentRecord(from);
			return *this;
		}

		/// <summary>
		/// Assignment operator
		/// </summary>
		/// <param name="from">RValue reference to a source</param>
		/// <returns>Assigned value</returns>
		BaseRttiArgumentRecord& operator=(BaseRttiArgumentRecord&& from) {
			this->~BaseRttiArgumentRecord();
			this->BaseRttiArgumentRecord::BaseRttiArgumentRecord(MoveRValue(from));
			return *this;
		}

		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~BaseRttiArgumentRecord() {
			it.~BaseRttiIterator();
		}

	public:
		/// <summary>
		/// Receiving the argument record with type class
		/// </summary>
		/// <returns>Record with type class</returns>
		EType GetTypeClass() const {
			return (EType) GetRecord()->TypeClass;
		}

		/// <summary>
		/// Receiving the argument record with type ID
		/// </summary>
		/// <returns>Record with type ID</returns>
		SizeT GetTypeId() const {
			return ((SizeT) GetRecord()->TypeId.TypeId) + it.imageBase;
		}

		/// <summary>
		/// Receiving the argument record with full type ID
		/// </summary>
		/// <returns>Record with full type ID</returns>
		UniversalTypeIdentificator GetFullTypeId() const {
			return GetRecord()->TypeId;
		}

		/// <summary>
		/// Receiving the argument record with type name
		/// </summary>
		/// <returns>Record with type name</returns>
		ConstString GetTypeName() const {
			Unsafe(return ConstString(GetRecord()->TypeName.Text, GetRecord()->TypeName.Size)); //-V612
		}

		/// <summary>
		/// Receiving the argument record with flags
		/// </summary>
		/// <returns>Record with flags</returns>
		EnumFlags<ECvRefModificators> GetFlags() const {
			return GetRecord()->Flags;
		}

		/// <summary>
		/// Receiving the argument record with binary size
		/// </summary>
		/// <returns>Record with binary size</returns>
		SizeT GetBinarySize() const {
			return GetRecord()->MetaDataForDynamic.BinarySize;
		}

		/// <summary>
		/// Indicates if the argument record is serializable
		/// </summary>
		/// <returns>"True" if the record is serializable</returns>
		Boolean GetIsSerializable() const {
			return (Boolean) GetRecord()->IsSerializable;
		}

		/// <summary>
		/// Receiving the argument record with type information
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

		BaseRttiTypeRecord<THeap> GetTypeRecord() const;

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
		/// A property for receiving the argument record with flags
		/// </summary>
		EnumFlags<ECvRefModificators> Property(GetFlags) Flags;

		/// <summary>
		/// A property for receiving the argument record with type name
		/// </summary>
		ConstString Property(GetTypeName) TypeName;

		/// <summary>
		/// A property for receiving the argument record with type info
		/// </summary>
		CxxTypeInfoT Property(GetTypeInfo) TypeInfo;

		/// <summary>
		/// A property for RTTI type record
		/// </summary>
		BaseRttiTypeRecord<THeap> Property(GetTypeRecord) Type;

	private:
		template<typename THeap>
		friend class BaseRttiMethodRecord;

		template<typename THeap>
		friend class BaseRttiConstructorRecord;

	private:
		static BaseRttiCollection<THeap, BaseRttiArgumentRecord> AsArgumentCollection(const MetaData::Collection* record, bool isValidCxxTypeId, SizeT imageBase) {
			return BaseRttiCollection<THeap, BaseRttiArgumentRecord>(record, isValidCxxTypeId, imageBase);
		}

	private:
		friend class Internals::RttiInternals;

	public:
		friend class BaseRttiIterator<THeap, BaseRttiArgumentRecord>;
		friend class Dynamic;

		template<typename>
		friend class BaseTraveler;
	};
}