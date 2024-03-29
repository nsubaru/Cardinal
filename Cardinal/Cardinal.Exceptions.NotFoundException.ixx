export module Cardinal.Exceptions:NotFoundException;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// Not found exception 
	/// </summary>
	class NotFoundException : public Exception {
	public:
		ForceInline NotFoundException(const NotFoundException& orig) : Exception(orig) {}
		ForceInline NotFoundException(NotFoundException&& from) : Exception(MoveRValue(from)) {}
		ForceInline NotFoundException& operator=(const NotFoundException& from) { this->~NotFoundException(); this->NotFoundException::NotFoundException(from); return *this; }
		ForceInline NotFoundException& operator=(NotFoundException&& from) { this->~NotFoundException(); this->NotFoundException::NotFoundException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		NotFoundException(SourceLocation sourceLocation)
			:Exception(sourceLocation, L"Not found") {
		}

		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		/// <param name = "Msg">The message</param>
		NotFoundException(SourceLocation sourceLocation, const ConstString& Msg)
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
		/// <returns>Smart pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new NotFoundException(*this));
		}

	public:
		ForceInline virtual ~NotFoundException() {}
	};
}
