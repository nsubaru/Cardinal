export module Cardinal.CoreLog;

export import Cardinal.Core;

namespace Cardinal
{
	InternalApiCall Boolean ToCoreLog(const Char* value, SizeT lenght);

	/// <summary>
	/// Record the static message into core logs
	/// </summary>
	template<SizeT Lenght>
	ForceInline Boolean ToCoreLog(StaticTextRef<Lenght> msg) {
#pragma warning(push)
#pragma warning(disable: 4996)
		return ToCoreLog(msg, Lenght);
#pragma warning(pop)
	}
}