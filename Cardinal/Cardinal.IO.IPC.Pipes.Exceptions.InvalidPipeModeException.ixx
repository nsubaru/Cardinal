export module Cardinal.IO:IPC.Pipes.Exceptions.InvalidPipeModeException;

export import :IPC.Base;

export namespace Cardinal::IO::IPC::Pipes
{
	/// <summary>
	/// Invalid pipe mode exception
	/// </summary>
	class InvalidPipeModeException final : public IpcException {
	public:
		ForceInline InvalidPipeModeException(const InvalidPipeModeException& orig) : IpcException(orig) {}
		ForceInline InvalidPipeModeException(InvalidPipeModeException&& from) : IpcException(MoveRValue(from)) {}
		ForceInline InvalidPipeModeException& operator=(const InvalidPipeModeException& from) { this->~InvalidPipeModeException(); this->InvalidPipeModeException::InvalidPipeModeException(from); return *this; }
		ForceInline InvalidPipeModeException& operator=(InvalidPipeModeException&& from) { this->~InvalidPipeModeException(); this->InvalidPipeModeException::InvalidPipeModeException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = "File">The file in which the exception was generated</param>
		/// <param name = "FunctName">The function name</param>
		/// <param name = "Line">The line number in which the exception was generated</param>
		InvalidPipeModeException(SourceLocation sourceLocation)
			: IpcException(sourceLocation, L"Pipe transport mode is invalid") {}

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
			return ExceptionHolderType(new InvalidPipeModeException(*this));
		}

	public:
		ForceInline virtual ~InvalidPipeModeException() override {}
	};
}