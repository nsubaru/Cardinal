export module Cardinal.RTTI:Record;

export import :Record.Base;
export import :Record.RttiField;
export import :Record.RttiIterator;
export import :Record.RttiICollection;
export import :Record.Object;
export import :Record.RttiTypeRecord;

export namespace Cardinal::RTTI::Record
{
	/// <summary>
	/// Receiving the RTTI type record
	/// </summary>
	/// <typeparam name="T">Item type</typeparam>
	/// <returnsRTTI type record></returns>
	template<typename T>
	inline BaseRttiTypeRecord<Memory::Heap> GetRttiTypeRecord() {
		return BaseRttiTypeRecord<Memory::Heap>(MetaData::GetTypeRecord<T>(), true, GetPeImageBase());
	}

	/// <summary>
	/// Receiving the RTTI type record
	/// </summary>
	/// <typeparam name="UHeap">Heap type</typeparam>
	/// <typeparam name="T">Item type</typeparam>
	/// <returnsRTTI type record></returns>
	template<typename UHeap, typename T>
	inline BaseRttiTypeRecord<UHeap> GetRttiTypeRecord() {
		return BaseRttiTypeRecord<UHeap>(MetaData::GetTypeRecord<T>(), true, GetPeImageBase());
	}
}