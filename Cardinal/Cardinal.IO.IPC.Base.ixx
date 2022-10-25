export module Cardinal.IO:IPC.Base;

export import :Base;
export import :Exceptions;

export namespace Cardinal::IO::IPC
{
	/// <summary>
	/// Generic "IPC" exception
	/// </summary>
	class IpcException : public IoException {
	public:
		ForceInline IpcException(const IpcException& orig) : IoException(orig) {}
		ForceInline IpcException(IpcException&& from) : IoException(MoveRValue(from)) {}
		ForceInline IpcException& operator=(const IpcException& from) { this->~IpcException(); this->IpcException::IpcException(from); return *this; }
		ForceInline IpcException& operator=(IpcException&& from) { this->~IpcException(); this->IpcException::IpcException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = "File">The file in which the exception was generated</param>
		/// <param name = "FunctName">The function name</param>
		/// <param name = "Line">The line number in which the exception was generated</param>
		IpcException(SourceLocation sourceLocation)
			: IoException(sourceLocation, L"General IPC exception") {}

		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = "File">The file in which the exception was generated</param>
		/// <param name = "FunctName">The function name</param>
		/// <param name = "Line">The line number in which the exception was generated</param>
		/// <param name = "Msg">Exception description</param>
		IpcException(SourceLocation sourceLocation, const ConstString& Msg)
			: IoException(sourceLocation, Msg) {}

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
			return ExceptionHolderType(new IpcException(*this));
		}

	public:
		virtual ~IpcException() {}
	};
}