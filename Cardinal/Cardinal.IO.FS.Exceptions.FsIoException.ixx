export module Cardinal.IO:FS.Exceptions.FsIoException;

export import :FS.Exceptions.Base;

export namespace Cardinal::IO::FS
{
	/// <summary>
	/// Generic "FS I/O" exception
	/// </summary>
	class FsIoException : public IoException {
	public:
		ForceInline FsIoException(const FsIoException& orig) : IoException(orig) {}
		ForceInline FsIoException(FsIoException&& from) : IoException(MoveRValue(from)) {}
		ForceInline FsIoException& operator=(const FsIoException& from) { this->~FsIoException(); this->FsIoException::FsIoException(from); return *this; }
		ForceInline FsIoException& operator=(FsIoException&& from) { this->~FsIoException(); this->FsIoException::FsIoException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		FsIoException(SourceLocation sourceLocation)
			: IoException(sourceLocation, L"Generic file system I/O exception") {}

		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		/// <param name = 'Msg'>Exception description</param>
		FsIoException(SourceLocation sourceLocation, const ConstString& Msg)
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
			return ExceptionHolderType(new FsIoException(*this));
		}

	public:
		ForceInline virtual ~FsIoException() {}
	};
}
