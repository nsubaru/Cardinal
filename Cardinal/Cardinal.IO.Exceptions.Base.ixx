export module Cardinal.IO:Exceptions.Base;

export import :Base;

export namespace Cardinal::IO
{
	/// <summary>
	/// Generic "I/O" exception
	/// </summary>
	class IoException : public Exception {
	public:
		ForceInline IoException(const IoException& orig) : Exception(orig) {}
		ForceInline IoException(IoException&& from) : Exception(MoveRValue(from)) {}
		ForceInline IoException& operator=(const IoException& from) { this->~IoException(); this->IoException::IoException(from); return *this; }
		ForceInline IoException& operator=(IoException&& from) { this->~IoException(); this->IoException::IoException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		IoException(SourceLocation sourceLocation)
			: Exception(sourceLocation, L"General I/O exception") {}

		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		/// <param name = 'Msg'>Exception description</param>
		IoException(SourceLocation sourceLocation, const ConstString& Msg)
			: Exception(sourceLocation, Msg) {}

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
		virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new IoException(*this));
		}

	public:
		virtual ~IoException() override {}
	};
}