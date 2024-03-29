export module Cardinal.Threading:Exceptions.InvalidThreadException;

export import :Exceptions.Base;

export namespace Cardinal::Threading
{
	/// <summary>
	/// Non existent thread or it is not managed through "Core"
	/// </summary>
	class InvalidThreadException final : public Exception {
	private:
		/// <summary>
		/// ID of non existent thread
		/// </summary>
		ID tid;

	public:
		/// <summary>
		/// Copying constructor
		/// </summary>
		/// <param name="orig">Original</param>
		ForceInline InvalidThreadException(const InvalidThreadException& orig) : Exception(orig), tid(orig.tid) {}

		/// <summary>
		/// Moving constructor
		/// </summary>
		/// <param name="from">Original</param>
		ForceInline InvalidThreadException(InvalidThreadException&& from) : Exception(MoveRValue(from)), tid(from.tid) {}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="from">Original</param>
		/// <returns>A reference to a current object</returns>
		ForceInline InvalidThreadException& operator=(const InvalidThreadException& from) {
			this->~InvalidThreadException();
			this->InvalidThreadException::InvalidThreadException(from);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="from">Original</param>
		/// <returns>A reference to a current object</returns>
		ForceInline InvalidThreadException& operator=(InvalidThreadException&& from) {
			this->~InvalidThreadException();
			this->InvalidThreadException::InvalidThreadException(MoveRValue(from));
			return *this;
		}

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = "File">The file in which the exception was generated</param>
		/// <param name = "FunctName">The function name</param>
		/// <param name = "Line">The line number in which the exception was generated</param>
		/// <param name = "TID">ID of non existent thread</param>
		InvalidThreadException(SourceLocation sourceLocation, ID TID)
			: Exception(sourceLocation, L"Cannot work with a thread that is not registered in runtime"), tid(TID) {
		};

	public:
		/// <summary>
		/// Returns the ID of non existent thread
		/// </summary>
		/// <returns>ID of non existent thread</returns>
		ForceInline ID GetTID() const { return tid; }

		/// <summary>
		/// ID of non existent thread
		/// </summary>
		ID Property(GetTID) TID;

	public:
		/// <summary>
		/// Text representation of exception information в помилках
		/// </summary>
		virtual ExceptionString toString() const override {
			return ExceptionString::Combine({
				ExceptionString(this->exceptionName),
				ExceptionString(L". Thread not fount with ID "), ExceptionString::toString(tid), ExceptionString(L". "),
				Location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new InvalidThreadException(*this));
		}

	public:
		ForceInline virtual ~InvalidThreadException() override {};
	};
}