export module Cardinal.RTTI:Record.Object.Interface;

export import :Record.Object.Base;

export namespace Cardinal::RTTI::Record
{
	/// <summary>
	/// A base RTTI interface record class
	/// </summary>
	/// <typeparam name="THeap">Heap type</typeparam>
	template<typename THeap>
	class BaseRttiIntefaceRecord final : public Memory::Allocator<THeap> {
	private:
		BaseRttiIterator<THeap, BaseRttiIntefaceRecord> it;

	private:
		BaseRttiIntefaceRecord(BaseRttiIterator<THeap, BaseRttiIntefaceRecord> it) : it(it) {}

	private:
		ForceInline const MetaData::InterfaceRecord* GetRecord() const { return (const MetaData::InterfaceRecord*) it.item; }

	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="from">Source</param>
		BaseRttiIntefaceRecord(const BaseRttiIntefaceRecord& from) : it(from.it) {}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="from">RValue reference to a source</param>
		BaseRttiIntefaceRecord(BaseRttiIntefaceRecord&& from) : it(MoveRValue(from.it)) {}

		/// <summary>
		/// Assignment operator
		/// </summary>
		/// <param name="from">Source</param>
		/// <returns>Assigned value</returns>
		BaseRttiIntefaceRecord& operator=(const BaseRttiIntefaceRecord& from) {
			this->~BaseRttiIntefaceRecord();
			this->BaseRttiIntefaceRecord::BaseRttiIntefaceRecord(from);
			return *this;
		}

		/// <summary>
		/// Assignment operator
		/// </summary>
		/// <param name="from">RValue reference to a source</param>
		/// <returns>Assigned value</returns>
		BaseRttiIntefaceRecord& operator=(BaseRttiIntefaceRecord&& from) {
			this->~BaseRttiIntefaceRecord();
			this->BaseRttiIntefaceRecord::BaseRttiIntefaceRecord(MoveRValue(from));
			return *this;
		}

		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~BaseRttiIntefaceRecord() {
			it.~BaseRttiIterator();
		}

	public:
		/// <summary>
		/// Receiving the interface record with type ID
		/// </summary>
		/// <returns>Record with type ID</returns>
		SizeT GetTypeId() const {
			return ((SizeT) GetRecord()->TypeId.TypeId) + it.imageBase;
		}

		/// <summary>
		/// Receiving the interface record with full type ID
		/// </summary>
		/// <returns>Record with full type ID</returns>
		UniversalTypeIdentificator GetFullTypeId() const {
			return GetRecord()->TypeId;
		}

		/// <summary>
		/// Receiving the interface record with type name
		/// </summary>
		/// <returns>Record with type name</returns>
		ConstString GetTypeName() const {
			Unsafe(return ConstString(GetRecord()->TypeName.Text, GetRecord()->TypeName.Size)); //-V612
		}

		/// <summary>
		/// Receiving the interface record with type information
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
		/// Receiving the interface type record
		/// </summary>
		/// <returns></returns>
		BaseRttiTypeRecord<THeap> GetTypeRecord() const;

	public:
		/// <summary>
		/// A property for receiving the interface record with type ID
		/// </summary>
		SizeT Property(GetTypeId) TypeId;

		/// <summary>
		/// A property for receiving the interface record with full type ID
		/// </summary>
		UniversalTypeIdentificator Property(GetFullTypeId) FullTypeId;

		/// <summary>
		/// A property for receiving the interface record with type name
		/// </summary>
		ConstString Property(GetTypeName) TypeName;

		/// <summary>
		/// A property for receiving the interface record with type information
		/// </summary>
		CxxTypeInfoT Property(GetTypeInfo) TypeInfo;

		/// <summary>
		/// A property for receiving the interface type record
		/// </summary>
		BaseRttiTypeRecord<THeap> Property(GetTypeRecord) Type;

	public:
		friend class BaseRttiIterator<THeap, BaseRttiIntefaceRecord>;
		friend class Dynamic;

	private:
		friend class Internals::RttiInternals;

		template<typename>
		friend class BaseTraveler;
	};
}