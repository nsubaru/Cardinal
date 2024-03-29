export module Cardinal.Exceptions:NotImplementedException;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// Not implemented exception
	/// </summary>
	class NotImplementedException final : public Exception {
	public:
		ForceInline NotImplementedException(const NotImplementedException& orig) : Exception(orig) {}
		ForceInline NotImplementedException(NotImplementedException&& from) : Exception(MoveRValue(from)) {}
		ForceInline NotImplementedException& operator=(const NotImplementedException& from) { this->~NotImplementedException(); this->NotImplementedException::NotImplementedException(from); return *this; }
		ForceInline NotImplementedException& operator=(NotImplementedException&& from) { this->~NotImplementedException(); this->NotImplementedException::NotImplementedException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		NotImplementedException(SourceLocation sourceLocation)
			:Exception(sourceLocation, L"Function\\Method is not implemented") {
		}

		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		/// <param name = 'Msg'>Exception message</param>
		NotImplementedException(SourceLocation sourceLocation, const ConstString& Msg)
			:Exception(sourceLocation, Msg) {}

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
		/// <returns>Smart pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new NotImplementedException(*this));
		}

	public:
		ForceInline virtual ~NotImplementedException() {}
	};
}