export module Cardinal.Threading:Exceptions.InvalidThreadObjException;

export import :Exceptions.Base;

export namespace Cardinal::Threading
{
	/// <summary>
	/// Invalid thread object
	/// </summary>
	class InvalidThreadObjException final : public Exception {
	public:
		/// <summary>
		/// Copying constructor
		/// </summary>
		/// <param name="orig">Original</param>
		ForceInline InvalidThreadObjException(const InvalidThreadObjException& orig) : Exception(orig) {}

		/// <summary>
		/// Moving constructor
		/// </summary>
		/// <param name="from">Original</param>
		ForceInline InvalidThreadObjException(InvalidThreadObjException&& from) : Exception(MoveRValue(from)) {}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="from">Original</param>
		/// <returns>A reference to a current object</returns>
		ForceInline InvalidThreadObjException& operator=(const InvalidThreadObjException& from) {
			this->~InvalidThreadObjException();
			this->InvalidThreadObjException::InvalidThreadObjException(from);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="from">Original</param>
		/// <returns>A reference to a current object</returns>
		ForceInline InvalidThreadObjException& operator=(InvalidThreadObjException&& from) {
			this->~InvalidThreadObjException();
			this->InvalidThreadObjException::InvalidThreadObjException(MoveRValue(from));
			return *this;
		}

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = "File">The file in which the exception was generated</param>
		/// <param name = "FunctName">The function name</param>
		/// <param name = "Line">The line number in which the exception was generated</param>
		InvalidThreadObjException(SourceLocation sourceLocation)
			: Exception(sourceLocation, L"Invalid thread object") {
		};

	public:
		/// <summary>
		/// Text representation of exception information в помилках
		/// </summary>
		virtual ExceptionString toString() const override {
			return ExceptionString::Combine({
				ExceptionString(exceptionName), ExceptionString(L". "),
				Location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new InvalidThreadObjException(*this));
		}

	public:
		ForceInline virtual ~InvalidThreadObjException() override {}
	};
}