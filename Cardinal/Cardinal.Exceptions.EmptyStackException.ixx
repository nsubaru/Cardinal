export module Cardinal.Exceptions:EmptyStackException;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// Empty stack exception
	/// </summary>
	class EmptyStackException : public Exception {
	public:
		ForceInline EmptyStackException(const EmptyStackException& orig) : Exception(orig) {}
		ForceInline EmptyStackException(EmptyStackException&& from) : Exception(MoveRValue(from)) {}
		ForceInline EmptyStackException& operator=(const EmptyStackException& from) { this->~EmptyStackException(); this->EmptyStackException::EmptyStackException(from); return *this; }
		ForceInline EmptyStackException& operator=(EmptyStackException&& from) { this->~EmptyStackException(); this->EmptyStackException::EmptyStackException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		EmptyStackException(SourceLocation sourceLocation)
			: Exception(sourceLocation, L"Trying to remove items from an empty stack.") {
		}

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
			return ExceptionHolderType(new EmptyStackException(*this));
		}

	public:
		ForceInline virtual ~EmptyStackException() {}
	};
}