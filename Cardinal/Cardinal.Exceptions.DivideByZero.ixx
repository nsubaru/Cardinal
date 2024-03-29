export module Cardinal.Exceptions:DivideByZero;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// The division by zero
	/// </summary>
	class DivideByZero final : public Exception {
	public:
		ForceInline DivideByZero(const DivideByZero& from) :Exception(from) {}
		ForceInline DivideByZero(DivideByZero&& from) : Exception(MoveRValue(from)) {}
		ForceInline DivideByZero& operator=(const DivideByZero& from) { this->~DivideByZero(); this->DivideByZero::DivideByZero(from); return *this; }
		ForceInline DivideByZero& operator=(DivideByZero&& from) { this->~DivideByZero(); this->DivideByZero::DivideByZero(MoveRValue(from)); return *this; }

	public:
		/// <summary>The exception constructor</summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		DivideByZero(SourceLocation sourceLocation)
			:Exception(sourceLocation, L"The division by zero") {
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
			return ExceptionHolderType(new DivideByZero(*this));
		}

	public:
		ForceInline virtual ~DivideByZero() {};
	};
}