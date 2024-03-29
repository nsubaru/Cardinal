export module Cardinal.Exceptions:InternalError;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// Kernel internal exception
	/// </summary>
	class InternalError final : public Exception {
	public:
		ForceInline InternalError(const InternalError& orig) : Exception(orig) {}
		ForceInline InternalError(InternalError&& from) : Exception(MoveRValue(from)) {}
		ForceInline InternalError& operator=(const InternalError& from) { this->~InternalError(); this->InternalError::InternalError(from); return *this; }
		ForceInline InternalError& operator=(InternalError&& from) { this->~InternalError(); this->InternalError::InternalError(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		ForceInline InternalError(SourceLocation sourceLocation)
			:Exception(sourceLocation, L"Kernel internal exception") {
		}

	public:
		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override final {
			return ExceptionString::Combine({
				ExceptionString(exceptionName), ExceptionString(L". "),
				location.toString(), ExceptionString(L". ")
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new InternalError(*this));
		}

	public:
		ForceInline virtual ~InternalError() {}
	};
}