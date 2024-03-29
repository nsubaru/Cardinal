export module Cardinal.Exceptions:IntegerOverflowException;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// Integer overflow exception
	/// </summary>
	class IntegerOverflowException : public Exception {
	public:
		ForceInline IntegerOverflowException(const IntegerOverflowException& orig) : Exception(orig) {}
		ForceInline IntegerOverflowException(IntegerOverflowException&& from) : Exception(MoveRValue(from)) {}
		ForceInline IntegerOverflowException& operator=(const IntegerOverflowException& from) { this->~IntegerOverflowException(); this->IntegerOverflowException::IntegerOverflowException(from); return *this; }
		ForceInline IntegerOverflowException& operator=(IntegerOverflowException&& from) { this->~IntegerOverflowException(); this->IntegerOverflowException::IntegerOverflowException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		IntegerOverflowException(SourceLocation sourceLocation) :
			Exception(sourceLocation, L"Integer overflow") {}

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
			return ExceptionHolderType(new IntegerOverflowException(*this));
		}

	public:
		ForceInline virtual ~IntegerOverflowException() {}
	};
}