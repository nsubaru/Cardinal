export module Cardinal.RTTI:Record.Object.Method;

export import :Record.Object.Base;
export import :Record.Object.Argument;

export namespace Cardinal::RTTI::Record
{
	/// <summary>
	/// A base RTTI method record class
	/// </summary>
	/// <typeparam name="THeap">Heap type</typeparam>
	template<typename THeap>
	class BaseRttiMethodRecord final : public Memory::Allocator<THeap> {
	private:
		BaseRttiIterator<THeap, BaseRttiMethodRecord> it;

	private:
		BaseRttiMethodRecord(BaseRttiIterator<THeap, BaseRttiMethodRecord> it) : it(it) {}

	private:
		ForceInline const MetaData::MethodMetaData* GetRecord() const { return (const MetaData::MethodMetaData*) it.item; }

	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="from">Source</param>
		BaseRttiMethodRecord(const BaseRttiMethodRecord& from) : it(from.it) {}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="from">RValue reference to a source</param>
		BaseRttiMethodRecord(BaseRttiMethodRecord&& from) : it(MoveRValue(from.it)) {}

		/// <summary>
		/// Assignment operator
		/// </summary>
		/// <param name="from">Source</param>
		/// <returns>Assigned value</returns>
		BaseRttiMethodRecord& operator=(const BaseRttiMethodRecord& from) {
			this->~BaseRttiMethodRecord();
			this->BaseRttiMethodRecord::BaseRttiMethodRecord(from);
			return *this;
		}

		/// <summary>
		/// Assignment operator
		/// </summary>
		/// <param name="from">RValue reference to a source</param>
		/// <returns>Assigned value</returns>
		BaseRttiMethodRecord& operator=(BaseRttiMethodRecord&& from) {
			this->~BaseRttiMethodRecord();
			this->BaseRttiMethodRecord::BaseRttiMethodRecord(MoveRValue(from));
			return *this;
		}

		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~BaseRttiMethodRecord() {
			it.~BaseRttiIterator();
		}

	public:
		/// <summary>
		/// Receiving the method CV-ref mofifiers
		/// </summary>
		/// <returns>Method CV-ref mofifiers</returns>
		EnumFlags<ECvRefModificators> GetMethodCVRefModificators() const {
			return GetRecord()->MethodCVRefModificators;
		}

		/// <summary>
		/// Receiving the method name
		/// </summary>
		/// <returns>Method name</returns>
		ConstString GetName() const {
			Unsafe(return ConstString(GetRecord()->MethodName.Text, GetRecord()->MethodName.Size));
		}

		/// <summary>
		/// Receiving the method arguments
		/// </summary>
		/// <returns>Method arguments</returns>
		BaseRttiCollection<THeap, BaseRttiArgumentRecord<THeap>> GetArguments() const {
			auto colletionBase = (MetaData::Collection*) (((const Byte*) GetRecord()) + GetRecord()->MethodArgsOffset);

			return BaseRttiArgumentRecord<THeap>::AsArgumentCollection(colletionBase, it.isValidCxxTypeId, it.imageBase);
		}

	public:
		/// <summary>
		/// A property for receiving the method CV-ref mofifiers
		/// </summary>
		EnumFlags<ECvRefModificators> Property(GetMethodCVRefModificators) MethodCVRefModificators;

		/// <summary>
		/// A property for receiving the method name
		/// </summary>
		ConstString Property(GetName) Name;

		/// <summary>
		/// A property for receiving the method arguments
		/// </summary>
		BaseRttiCollection<THeap, BaseRttiArgumentRecord<THeap>> Property(GetArguments) Arguments;

	private:
		friend class Internals::RttiInternals;

		template<typename THeap>
		friend class BaseRttiProperty;

	private:
		static BaseRttiCollection<THeap, BaseRttiMethodRecord> AsMethodCollection(const MetaData::Collection* record, bool isValidCxxTypeId, SizeT imageBase) {
			return BaseRttiCollection<THeap, BaseRttiMethodRecord>(record, isValidCxxTypeId, imageBase);
		}

	public:
		friend class BaseRttiIterator<THeap, BaseRttiMethodRecord>;

		friend class Dynamic;

		template<typename>
		friend class BaseTraveler;
	};
}