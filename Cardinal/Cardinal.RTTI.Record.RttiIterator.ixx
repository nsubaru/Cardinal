export module Cardinal.RTTI:Record.RttiIterator;

export import :Record.Base;

export namespace Cardinal::RTTI::Record
{
	template<typename THeap, typename TIterator>
	class BaseRttiCollection;

	/// <summary>
	/// A base iterator class
	/// </summary>
	/// <typeparam name="THeap">Heap type</typeparam>
	/// <typeparam name="T">Item type</typeparam>
	template<typename THeap, typename T>
	class BaseRttiIterator {
	private:
		const MetaData::ItemBase* item;
		SizeT idx;
		SizeT count;
		bool isValidCxxTypeId;
		SizeT imageBase;

	protected:
		BaseRttiIterator(const void* item, SizeT idx, SizeT count, SizeT imageBase, bool isValidCxxTypeId)
			: item((const MetaData::ItemBase*) item), idx(idx), count(count), imageBase(imageBase), isValidCxxTypeId(isValidCxxTypeId) {}

	public:
		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="from">Source</param>
		BaseRttiIterator(const BaseRttiIterator& from)
			: item(from.item), idx(from.idx), count(from.count), imageBase(from.imageBase), isValidCxxTypeId(from.isValidCxxTypeId) {}

		/// <summary>
		/// Constructor
		/// </summary>
		/// <param name="from">RValue reference to a source</param>
		BaseRttiIterator(BaseRttiIterator&& from)
			: item(from.item), idx(from.idx), count(from.count), imageBase(from.imageBase), isValidCxxTypeId(from.isValidCxxTypeId) {}

	public:
		/// <summary>
		/// Denomination operator
		/// </summary>
		/// <returns>Denomination result</returns>
		T operator*() const {
			if (item == nullptr || idx == count)
			{
				Throw(IndexOutOfRange(SourcePosition, idx + 1, count));
			}

			return T(*this);
		}

		/// <summary>
		/// Incrementation operator
		/// </summary>
		/// <returns>Incrementation result</returns>
		BaseRttiIterator& operator++() {
			if (item == nullptr || idx == count)
			{
				Throw(IndexOutOfRange(SourcePosition, idx + 1, count));
			}

			idx++;

			if (idx == count)
			{
				item = nullptr;

			}
			else
			{
				item = (decltype(item)) (((const Byte*) item) + item->SizeOfItem);
			}

			return *this;
		}

		/// <summary>
		/// Equality operator
		/// </summary>
		/// <returns>Equality result</returns>
		bool operator==(NullptrT) { return item == nullptr || idx == count; }

		/// <summary>
		/// Inequality operator
		/// </summary>
		/// <returns>Inequality result</returns>
		bool operator!=(NullptrT) { return !(*this == nullptr); }

		/// <summary>
		/// Equality operator
		/// </summary>
		/// <param name="other">RTTI iterator</param>
		/// <returns>Equality result</returns>
		bool operator==(const BaseRttiIterator& other) {
			if (other == nullptr || *this == nullptr)
				return other == nullptr && *this == nullptr;

			return this->item == other.item && this->idx == other.idx && this->count == other.count;
		}

		/// <summary>
		/// Inequality operator
		/// </summary>
		/// <param name="other">RTTI iterator</param>
		/// <returns>Inequality result</returns>
		bool operator!=(const BaseRttiIterator& other) {
			return !(*this == other);
		}

	public:
		friend T;

		template<typename THeap, typename T>
		friend class BaseRttiCollection;

		template<typename>
		friend class BaseTraveler;

	private:
		friend class Internals::RttiInternals;
	};
}