export module Cardinal.IO:IPC.Pipes.Exceptions.InvalidPipeException;

export import :IPC.Base;

export namespace Cardinal::IO::IPC::Pipes
{
	/// <summary>
	/// Invalid pipe exception
	/// </summary>
	class InvalidPipeException final : public IpcException {
	public:
		ForceInline InvalidPipeException(const InvalidPipeException& orig) : IpcException(orig) {}
		ForceInline InvalidPipeException(InvalidPipeException&& from) : IpcException(MoveRValue(from)) {}
		ForceInline InvalidPipeException& operator=(const InvalidPipeException& from) { this->~InvalidPipeException(); this->InvalidPipeException::InvalidPipeException(from); return *this; }
		ForceInline InvalidPipeException& operator=(InvalidPipeException&& from) { this->~InvalidPipeException(); this->InvalidPipeException::InvalidPipeException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = "File">The file in which the exception was generated</param>
		/// <param name = "FunctName">The function name</param>
		/// <param name = "Line">The line number in which the exception was generated</param>
		InvalidPipeException(SourceLocation sourceLocation)
			: IpcException(sourceLocation, L"Pipe transport is invalid") {}

	public:
		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override {
			return ExceptionString::Combine({
				ExceptionString(exceptionName),
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new InvalidPipeException(*this));
		}

	public:
		ForceInline virtual ~InvalidPipeException() override {}
	};
}