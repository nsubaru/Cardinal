export module Cardinal.RTTI:Record.Object.Constructor;

export import :Record.Object.Base;
export import :Record.Object.Argument;

export namespace Cardinal::RTTI::Record
{
	/// <summary>
	/// A base RTTI constructor record class
	/// </summary>
	/// <typeparam name="THeap">Heap type</typeparam>
	template<typename THeap>
	class BaseRttiConstructorRecord final : public Memory::Allocator<THeap> {
	private:
		BaseRttiIterator<THeap, BaseRttiConstructorRecord> it;

	private:
		BaseRttiConstructorRecord(BaseRttiIterator<THeap, BaseRttiConstructorRecord> it) : it(it) {}

	private:
		ForceInline const MetaData::Constructor* GetRecord() const { return (const MetaData::Constructor*) it.item; }

	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="from">Source</param>
		BaseRttiConstructorRecord(const BaseRttiConstructorRecord& from) : it(from.it) {}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="from">RValue reference to a source</param>
		BaseRttiConstructorRecord(BaseRttiConstructorRecord&& from) : it(MoveRValue(from.it)) {}

		/// <summary>
		/// Assignment operator
		/// </summary>
		/// <param name="from">Source</param>
		/// <returns>Assigned value</returns>
		BaseRttiConstructorRecord& operator=(const BaseRttiConstructorRecord& from) {
			this->~BaseRttiConstructorRecord();
			this->BaseRttiConstructorRecord::BaseRttiConstructorRecord(from);
			return *this;
		}

		/// <summary>
		/// Assignment operator
		/// </summary>
		/// <param name="from">RValue reference to a source</param>
		/// <returns>Assigned value</returns>
		BaseRttiConstructorRecord& operator=(BaseRttiConstructorRecord&& from) {
			this->~BaseRttiConstructorRecord();
			this->BaseRttiConstructorRecord::BaseRttiConstructorRecord(MoveRValue(from));
			return *this;
		}

		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~BaseRttiConstructorRecord() {
			it.~BaseRttiIterator();
		}

		/// <summary>
		/// Return type record of this ctor record
		/// </summary>
		BaseRttiTypeRecord<THeap> GetTypeRecord() const;

	public:
		/// <summary>
		/// Receiving the constructor record with arguments
		/// </summary>
		/// <returns>Record with arguments</returns>
		BaseRttiCollection<THeap, BaseRttiArgumentRecord<THeap>> GetArguments() const {
			auto colletionBase = (MetaData::Collection*) (((Byte*) GetRecord()) + GetRecord()->OffsetToArgs);
			return BaseRttiArgumentRecord<THeap>::AsArgumentCollection(colletionBase, it.isValidCxxTypeId, it.imageBase);
		}

	public:
		/// <summary>
		/// A property for receiving the constructor record with arguments
		/// </summary>
		BaseRttiCollection<THeap, BaseRttiArgumentRecord<THeap>> Property(GetArguments) Arguments;

		/// <summary>
		/// Type record of this ctor record
		/// </summary>
		BaseRttiTypeRecord<THeap> Property(GetTypeRecord) Type;

	public:
		friend class BaseRttiIterator<THeap, BaseRttiConstructorRecord>;

	private:
		friend class Internals::RttiInternals;

	public:
		friend class Dynamic;

		template<typename>
		friend class BaseTraveler;
	};
}