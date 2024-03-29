export module Cardinal.RTTI:Section.Type;

export import :Section.Base;

export namespace Cardinal::RTTI
{
	/// <summary>
	/// This type contain raw internal rtti section
	/// <remarks>
	/// <para>RTTI section in executable have name .type and have next structure:</para>
	/// <para> .type$a  - Magic number (<c>Cardinal::RTTI::SectionMagicNumber</c> variable)</para>
	/// <para> .type$b  - Section header (have all neaded information and have type <c>Cardinal::RTTI::SectionHeaderType</c></para>
	/// <para> .type$fa - Start of pointer subsection (this subsection store pointers to all type records)</para>
	/// <para> .type$fz - End of pointer subsection</para>
	/// <para> .type$va - Start of variable subsection (this subsection store type records of internal type <c>Cardinal::RTTI::RttiRecord::MetaData::MetaData</c>)</para>
	/// <para> .type$vz - End of variable subsection</para>
	/// <para> .type$z  - End of rtti section</para>
	/// <para> 
	/// For iterating over all rtti records this class iterate over pointer subsection and using adresses of meta data records build RTTITypeRecord 
	/// Before and during iterating this class convert absolut pointers adresses to current one
	/// </para>
	/// </remarks>
	/// </summary>
	class RTTISection : public IClonable<RTTISection>, public IDisposable {
	private:
		struct Data {
			Containers::Blob rawData;

			SizeT headerOffset;
			SizeT* fa;
			SizeT fCount;
			SizeT offset;
			Boolean direction;

			mutable Containers::Array<RTTI::Record::BaseRttiTypeRecord<Memory::Heap>> registeredRecords;

			RefCountT RefCount;

			Data(const Containers::Blob& rawData) :
				headerOffset(0),
				fa(nullptr),
				fCount(0),
				offset(0),
				direction(false),
				rawData(rawData),
				registeredRecords(),
				RefCount(1) {
			}

			Data(Containers::Blob&& rawData) :
				headerOffset(0),
				fa(nullptr),
				fCount(0),
				offset(0),
				direction(false),
				rawData(MoveRValue(rawData)),
				registeredRecords(),
				RefCount(1) {
			}
		};

	private:
		Data* This;

	private:
		void FixupAddreses();

	protected:
		virtual void Dispose() override {};

	private:
		RTTISection(const Containers::Blob& rawData) :
			This(new Data(rawData)) { FixupAddreses(); }

		RTTISection(Containers::Blob&& rawData) :
			This(new Data(MoveRValue(rawData))) { FixupAddreses(); }

		void LoadAllRegisteredTypes() const;

	public:
		RTTISection() : This(nullptr) {}

	public:
		/// <summary>
		/// Return RttiTypeRecord from current section
		/// This records have some limitation during that they don't have 
		/// </summary>
		/// <returns></returns>
		const Containers::Array<RTTI::Record::BaseRttiTypeRecord<Memory::Heap>>& GetAllRegisteredTypes() const {
			if (This->registeredRecords.Count == 0)
				LoadAllRegisteredTypes();

			return This->registeredRecords;
		}

	public:
		/// <summary>
		/// Check is object valid
		/// </summary>
		/// <returns>Logic variable</returns>
		Boolean GetIsValid() const { return This == nullptr; }

		/// <summary>
		/// Check is object valid
		/// </summary>
		Boolean Property(GetIsValid) IsValid;

	public:
		/// <summary>
		/// Return kernel version with which this section was builded
		/// </summary>
		/// <returns>KernelVersion struct</returns>
		Settings::KernelVersionT GetKernelVersion() const;

		/// <summary>
		/// Kernel version with which this section was builded
		/// </summary>
		Settings::KernelVersionT Property(GetKernelVersion) KernelVersion;

	public:
		/// <summary>
		/// Clone object(create copy of data array)
		/// </summary>
		/// <returns>Cloned object</returns>
		virtual RTTISection Clone() const {
			if (This == nullptr)
				return RTTISection();

			return RTTISection(This->rawData);
		}

	public:
		/// <summary>
		/// Load RTTI section from raw blob
		/// </summary>
		/// <param name="data">Blob with rtti section</param>
		/// <returns>Section object</returns>
		static RTTISection LoadFromBlob(Containers::Blob&& blob);

	public:
		RefCountClass(RTTISection, This);

	private:
		friend void LoadSystemSection();
		friend void LoadSystemSectionBeforeFixups();

		template<typename T>
		friend struct Cardinal::Serialization::SerializationHandler;
	};

	/// <summary>
	/// Return all registered RTTI records
	/// </summary>
	/// <returns>Array of registered RTTI records</returns>
	const Containers::Array<RTTI::Record::BaseRttiTypeRecord<Memory::Heap>>& GetAllRegisteredTypes();

	/// <summary>
	/// Return copy of current RTTI PE section after all fixups
	/// </summary>
	/// <returns>RTTISection with current RTTI PE section</returns>
	RTTISection GetCurrentRttiSection();

	/// <summary>
	/// Return copy of current RTTI PE section before all fixups
	/// </summary>
	/// <returns>RTTISection with current RTTI PE section</returns>
	RTTISection GetCurrentRttiSectionWithoutFixes();
}

namespace Cardinal::CTTI::TypeTraits::Details
{
	template<bool TIsConst, bool TIsVolatile>
	struct CTTITypeDataImpl<RTTI::RTTISection, TIsConst, TIsVolatile> :
		BaseCTTITypeDataImpl<RTTI::RTTISection, EType::MetaData, true, TIsConst, TIsVolatile> {
		using RawStorage = Containers::Blob;
	};
}
