export module Cardinal.Exceptions:InteruptException;

import :Base;

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 4251)

export namespace Cardinal
{
	/// <summary>
	/// Interrupted operation exception
	/// </summary>
	class InteruptException final : protected Exception {
	public:
		ForceInline InteruptException(const InteruptException& orig) : Exception(orig) {}
		ForceInline InteruptException(InteruptException&& from) : Exception(MoveRValue(from)) {}
		ForceInline InteruptException& operator=(const InteruptException& from) { this->~InteruptException(); this->InteruptException::InteruptException(from); return *this; }
		ForceInline InteruptException& operator=(InteruptException&& from) { this->~InteruptException(); this->InteruptException::InteruptException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		InteruptException(SourceLocation sourceLocation)
			: Exception(sourceLocation, L"The operation is interrupted") {
		};

	public:
		/// <summary>
		/// Returns the exception generation location
		/// </summary>
		ForceInline const ExeptionLocation& GetLocation() const {
			return location;
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
			return ExceptionHolderType(new InteruptException(*this));
		}

	public:
		ForceInline virtual ~InteruptException() {}

	private:
		friend Exception* GetBaseForInteruptException(InteruptException* pEx);
	};
}

namespace Cardinal
{
	Exception* GetBaseForInteruptException(InteruptException* pEx);
}

#pragma warning(pop)