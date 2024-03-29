export module Cardinal.Exceptions:InvalidFormatException;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// Invalid format exception
	/// </summary>
	class InvalidFormatException : public Exception {
	public:
		ForceInline InvalidFormatException(const InvalidFormatException& orig) : Exception(orig) {}
		ForceInline InvalidFormatException(InvalidFormatException&& from) : Exception(MoveRValue(from)) {}
		ForceInline InvalidFormatException& operator=(const InvalidFormatException& from) { this->~InvalidFormatException(); this->InvalidFormatException::InvalidFormatException(from); return *this; }
		ForceInline InvalidFormatException& operator=(InvalidFormatException&& from) { this->~InvalidFormatException(); this->InvalidFormatException::InvalidFormatException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		InvalidFormatException(SourceLocation sourceLocation)
			:Exception(sourceLocation, L"Invalid format") {
		}

	public:
		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override final {
			return ExceptionString::Combine({
				ExceptionString(exceptionName), ExceptionString(L". "),
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new InvalidFormatException(*this));
		}

	public:
		ForceInline virtual ~InvalidFormatException() {}
	};
}