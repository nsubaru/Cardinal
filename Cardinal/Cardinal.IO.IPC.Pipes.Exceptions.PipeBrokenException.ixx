export module Cardinal.IO:IPC.Pipes.Exceptions.PipeBrokenException;

export import :IPC.Base;

export namespace Cardinal::IO::IPC::Pipes
{
	/// <summary>
	/// The channel has disconnected from the other side
	/// </summary>
	class PipeBrokenException final : public IpcException {
	public:
		ForceInline PipeBrokenException(const PipeBrokenException& orig) : IpcException(orig) {}
		ForceInline PipeBrokenException(PipeBrokenException&& from) : IpcException(MoveRValue(from)) {}
		ForceInline PipeBrokenException& operator=(const PipeBrokenException& from) { this->~PipeBrokenException(); this->PipeBrokenException::PipeBrokenException(from); return *this; }
		ForceInline PipeBrokenException& operator=(PipeBrokenException&& from) { this->~PipeBrokenException(); this->PipeBrokenException::PipeBrokenException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = "File">The file in which the exception was generated</param>
		/// <param name = "FunctName">The function name</param>
		/// <param name = "Line">The line number in which the exception was generated</param>
		PipeBrokenException(SourceLocation sourceLocation)
			: IpcException(sourceLocation, L"Pipe transport is broken") {}

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
			return ExceptionHolderType(new PipeBrokenException(*this));
		}

	public:
		ForceInline virtual ~PipeBrokenException() override {}
	};
}