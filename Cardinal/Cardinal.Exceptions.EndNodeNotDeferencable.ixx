export module Cardinal.Exceptions:EndNodeNotDeferencable;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// Not definned iterator at the end exception
	/// </summary>
	class EndNodeNotDeferencable final : public Exception {
	public:
		ForceInline EndNodeNotDeferencable(const EndNodeNotDeferencable& orig) : Exception(orig) {}
		ForceInline EndNodeNotDeferencable(EndNodeNotDeferencable&& from) : Exception(MoveRValue(from)) {}
		ForceInline EndNodeNotDeferencable& operator=(const EndNodeNotDeferencable& from) { this->~EndNodeNotDeferencable(); this->EndNodeNotDeferencable::EndNodeNotDeferencable(from); return *this; }
		ForceInline EndNodeNotDeferencable& operator=(EndNodeNotDeferencable&& from) { this->~EndNodeNotDeferencable(); this->EndNodeNotDeferencable::EndNodeNotDeferencable(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		EndNodeNotDeferencable(SourceLocation sourceLocation) noexcept
			: Exception(sourceLocation, L"Not definned iterator at the end") {
		};

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
			return ExceptionHolderType(new EndNodeNotDeferencable(*this));
		}

	public:
		ForceInline virtual ~EndNodeNotDeferencable() {}
	};
}