export module Cardinal.Exceptions:AllreadyExistException;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// Already exists exception
	/// </summary>
	class AllreadyExistException : public Exception {
	public:
		ForceInline AllreadyExistException(const AllreadyExistException& orig) : Exception(orig) {}
		ForceInline AllreadyExistException(AllreadyExistException&& from) : Exception(MoveRValue(from)) {}
		ForceInline AllreadyExistException& operator=(const AllreadyExistException& from) { this->~AllreadyExistException(); this->AllreadyExistException::AllreadyExistException(from); return *this; }
		ForceInline AllreadyExistException& operator=(AllreadyExistException&& from) { this->~AllreadyExistException(); this->AllreadyExistException::AllreadyExistException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		AllreadyExistException(SourceLocation sourceLocation)
			:Exception(sourceLocation, L"Already exists") {
		}

		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		AllreadyExistException(SourceLocation sourceLocation, const ConstString& Msg)
			:Exception(sourceLocation, Msg) {
		}

	public:
		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override {
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
			return ExceptionHolderType(new AllreadyExistException(*this));
		}

	public:
		ForceInline virtual ~AllreadyExistException() {}
	};
}