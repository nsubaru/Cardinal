export module Cardinal.Exceptions:InvalidHandleUsage;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// Using the invalid descriptor
	/// </summary>
	class InvalidHandleUsage final : public Exception {
	public:
		ForceInline InvalidHandleUsage(const InvalidHandleUsage& orig) : Exception(orig) {}
		ForceInline InvalidHandleUsage(InvalidHandleUsage&& from) : Exception(MoveRValue(from)) {}
		ForceInline InvalidHandleUsage& operator=(const InvalidHandleUsage& from) { this->~InvalidHandleUsage(); this->InvalidHandleUsage::InvalidHandleUsage(from); return *this; }
		ForceInline InvalidHandleUsage& operator=(InvalidHandleUsage&& from) { this->~InvalidHandleUsage(); this->InvalidHandleUsage::InvalidHandleUsage(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		InvalidHandleUsage(SourceLocation sourceLocation)
			:Exception(sourceLocation, L"Using the invalid descriptor") {
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
			return ExceptionHolderType(new InvalidHandleUsage(*this));
		}

	public:
		ForceInline virtual ~InvalidHandleUsage() {}
	};
}