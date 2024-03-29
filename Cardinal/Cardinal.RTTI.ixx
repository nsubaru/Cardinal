export module Cardinal.RTTI;

export import :Base;
export import :Record;
export import :Section;
export import :Dynamic;
export import :Exceptions;
export import :MetaData;
export import :Traveler;

export namespace Cardinal::RTTI
{
	export using namespace Cardinal::RTTI::Record;
	export using namespace Cardinal::RTTI::Exceptions;
}

/// <summary>
/// Return type record by type
/// </summary>
/// <typeparam name="T">Type</typeparam>
/// <returns>RTTITypeRecord for required type</returns>
export template<typename T>
inline Cardinal::RTTI::Record::RttiTypeRecord TypeRecord() {
	return Cardinal::RTTI::Record::GetRttiTypeRecord<T>();
}

/// <summary>
/// Return type record by type
/// </summary>
/// <typeparam name="T">Type</typeparam>
/// <returns>RTTITypeRecord for required type</returns>
export template<typename THeap, typename T>
inline Cardinal::RTTI::BaseRttiTypeRecord<THeap> TypeRecord() {
	return Cardinal::RTTI::Record::GetRttiTypeRecord<T>();
}