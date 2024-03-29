export module Cardinal.RTTI:Exceptions.GenericRttiException;

export import :Exceptions.Base;

export namespace Cardinal::RTTI::Exceptions
{
	/// <summary>
	/// Base exception for all rtti exceptions
	/// </summary>
	class GenericRttiException : public Exception {
	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		/// <param Name = 'Type'>The type which DI cannot found</param>
		GenericRttiException(SourceLocation sourceLocation, ConstString message)
			:Exception(sourceLocation, message) {
		}

		GenericRttiException(const GenericRttiException& from) : Exception(from) {}
		GenericRttiException(GenericRttiException&& from) : Exception(from) {}

	public:
		ForceInline virtual ~GenericRttiException() {}
	};
}