export module Cardinal.Exceptions:BadException;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// Invalid exception type
	/// </summary>
	class BadException final : public Exception {
	public:
		ForceInline BadException(const BadException& orig) : Exception(orig) {}
		ForceInline BadException(BadException&& from) : Exception(MoveRValue(from)) {}
		ForceInline BadException& operator=(const BadException& from) { this->~BadException(); this->BadException::BadException(from); return *this; }
		ForceInline BadException& operator=(BadException&& from) { this->~BadException(); this->BadException::BadException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		BadException(SourceLocation sourceLocation)
			:Exception(sourceLocation, L"Invalid exception type") {
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
			return ExceptionHolderType(new BadException(*this));
		}

	public:
		ForceInline virtual ~BadException() {}
	};
}