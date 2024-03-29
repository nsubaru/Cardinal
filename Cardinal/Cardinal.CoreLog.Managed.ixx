export module Cardinal.CoreLog.Managed;

export import Cardinal.CoreLog;
export import Cardinal.Containers.NonConcurency.Array;
export import Cardinal.Containers.NonConcurency.List;
export import Cardinal.String;
export import Cardinal.Exceptions;

namespace Cardinal
{
	InternalApiCall Boolean ToCoreLog(const Char* value, SizeT lenght, const Exception& ex);

	/// <summary>
	/// The complex message output into logs (every element of array - new line)
	/// </summary>
	/// <param name = "lines">The lines list</param>
	/// <returns>"<c>True</c>" if logging was successful</returns>
	bool ToCoreLog(const Containers::Array<ExceptionString>& lines);

	/// <summary>
	/// The line output into logs with the message
	/// </summary>
	/// <param name = "msg">The message</param>
	/// <param name = "Heap"The heap for the memory allocator</param>
	/// <returns>"<c>True</c>" if logging was successful</returns>
	template<typename Heap>
	ForceInline bool ToCoreLog(const BaseString<Heap>& msg) {
#pragma warning(push)
#pragma warning(disable: 4996)
		return ToCoreLog(static_cast<ConstCharStr>(msg), msg.Lenght);
#pragma warning(pop)
	}

#pragma warning(push)
#pragma warning(disable: 4996)

	/// <summary>
	/// A message record for exceptions
	/// </summary>
	/// <param name = "Msg">The custom message</param>
	/// <param name = "Base"The heap for the memory allocator</param>
	/// <param name = "ex">The exception object</param>
	/// <returns>"<c>True</c>" if logging was successful</returns>
	template<typename THeap>
	ForceInline bool ToCoreLog(const BaseString<THeap>& msg, const Exception& ex) {
		return Cardinal::ToCoreLog({ msg, ex.toString() });
	}

	/// <summary>
	/// A message record for exceptions
	/// </summary>
	/// <param name = "Msg">The custom message</param>
	/// <param name = "Lenght">The message size</param>
	/// <param name = "ex">The exception object</param>
	/// <returns>"<c>True</c>" if logging was successful</returns>
	template<SizeT Lenght>
	ForceInline bool ToCoreLog(const wchar_t(&msg)[Lenght], const Exception& ex) {
		return Cardinal::ToCoreLog({ msg, ex.toString() });
	}
#pragma warning(pop)
}