export module Cardinal.Exceptions:BadPureCall;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// Impossible to call the virtual function
	/// </summary>
	class BadPureCall final : public Exception {
	public:
		ForceInline BadPureCall(const BadPureCall& orig) : Exception(orig) {}
		ForceInline BadPureCall(BadPureCall&& from) : Exception(MoveRValue(from)) {}
		ForceInline BadPureCall& operator=(const BadPureCall& from) { this->~BadPureCall(); this->BadPureCall::BadPureCall(from); return *this; }
		ForceInline BadPureCall& operator=(BadPureCall&& from) { this->~BadPureCall(); this->BadPureCall::BadPureCall(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		BadPureCall(SourceLocation sourceLocation)
			:Exception(sourceLocation, L"Impossible to call the virtual function") {
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
			return ExceptionHolderType(new BadPureCall(*this));
		}

	public:
		ForceInline virtual ~BadPureCall() {}
	};
}