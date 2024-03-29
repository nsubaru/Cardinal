export module Cardinal.Exceptions:BadCast;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// Impossible to transform the type
	/// </summary>
	class BadCast final : public Exception {
	public:
		ForceInline BadCast(const BadCast& orig) : Exception(orig) {}
		ForceInline BadCast(BadCast&& from) : Exception(MoveRValue(from)) {}
		ForceInline BadCast& operator=(const BadCast& from) { this->~BadCast(); this->BadCast::BadCast(from); return *this; }
		ForceInline BadCast& operator=(BadCast&& from) { this->~BadCast(); this->BadCast::BadCast(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		BadCast(SourceLocation sourceLocation)
			:Exception(sourceLocation, L"Impossible to transform the type") {
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
			return ExceptionHolderType(new BadCast(*this));
		}

	public:
		ForceInline virtual ~BadCast() {}
	};
}