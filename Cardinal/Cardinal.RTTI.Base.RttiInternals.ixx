export module Cardinal.RTTI:Base.RttiInternals;

export import :Base.MetaData;

namespace Cardinal::RTTI::Internals
{
	/// <summary>
	/// A class with internal RTTI values optimizations
	/// </summary>
	class RttiInternals {
	public:
		RttiInternals() = delete;
		RttiInternals(const RttiInternals&) = delete;
		RttiInternals(RttiInternals&&) = delete;

	private:
		using ValidIdsT = Containers::List<Containers::Pair<ConstString, UniversalTypeIdentificator>>;

		static void FixRttiRecordFullTypeId(const Record::RttiTypeRecord& r, const Containers::Array<Record::RttiTypeRecord> records, SizeT idx);
		static void FixRttiRecordFields(const Record::RttiTypeRecord& r, const ValidIdsT& validTypeids);
		static void FixRttiRecordConstructors(const Record::RttiTypeRecord& r, const ValidIdsT& validTypeids);
		static void FixRttiRecordInterfaces(const Record::RttiTypeRecord& r, const ValidIdsT& validTypeids);
		static void FixRttiRecordPropertyGetters(const Record::RttiProperty& p, const ValidIdsT& validTypeids);
		static void FixRttiRecordPropertySetters(const Record::RttiProperty& r, const ValidIdsT& validTypeids);
		static void FixRttiRecordProperties(const Record::RttiTypeRecord& r, const ValidIdsT& validTypeids);
		static void FixRttiRecordMethods(const Record::RttiTypeRecord& r, const ValidIdsT& validTypeids);

		static void FixRttiRecords();

	private:
		friend void __FixUpRttiRecords();

	private:
		static Record::RttiTypeRecord BuildRTTITypeRecord(const MetaData::MetaData* data, bool isValidCxxTypeId, SizeT imageBase);

	private:
		friend Record::RttiTypeRecord __BuildRTTITypeRecord(const MetaData::MetaData* data, bool isValidCxxTypeId, SizeT imageBase);

	private:
		template<typename THeap>
		friend class Record::BaseRttiField;

		template<typename THeap>
		friend class Record::BaseRttiArgumentRecord;

		template<typename THeap>
		friend class Record::BaseRttiIntefaceRecord;

		template<typename THeap>
		friend class Record::BaseRttiConstructorRecord;

		template<typename THeap>
		friend class Record::BaseRttiMethodRecord;

		template<typename THeap>
		friend class Record::BaseRttiProperty;

		template<typename THeap>
		friend class Record::BaseRttiTypeRecord;
	};
}