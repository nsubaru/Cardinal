export module Cardinal.Threading:Exceptions.DeadlockExpectedException;

export import :Exceptions.Base;

export namespace Cardinal::Threading
{
	/// <summary>
	/// Mutual blocking
	/// </summary>
	class DeadlockExpectedException final : public Exception {
	public:
		/// <summary>
		/// Copying constructor
		/// </summary>
		/// <param name="orig">Original</param>
		ForceInline DeadlockExpectedException(const DeadlockExpectedException& orig) : Exception(orig) {}

		/// <summary>
		/// Moving constructor
		/// </summary>
		/// <param name="from">Original</param>
		ForceInline DeadlockExpectedException(DeadlockExpectedException&& from) : Exception(MoveRValue(from)) {}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="from">Original</param>
		/// <returns>A reference to a current object</returns>
		ForceInline DeadlockExpectedException& operator=(const DeadlockExpectedException& from) {
			this->~DeadlockExpectedException();
			this->DeadlockExpectedException::DeadlockExpectedException(from);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="from">Original</param>
		/// <returns>A reference to a current object</returns>
		ForceInline DeadlockExpectedException& operator=(DeadlockExpectedException&& from) {
			this->~DeadlockExpectedException();
			this->DeadlockExpectedException::DeadlockExpectedException(MoveRValue(from));
			return *this;
		}

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = "File">The file in which the exception was generated</param>
		/// <param name = "FunctName">The function name</param>
		/// <param name = "Line">The line number in which the exception was generated</param>
		DeadlockExpectedException(SourceLocation sourceLocation)
			: Exception(sourceLocation, L"Mutual blocking") {
		};

	public:
		/// <summary>
		/// Text representation of exception information
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
			return ExceptionHolderType(new DeadlockExpectedException(*this));
		}

	public:
		ForceInline virtual ~DeadlockExpectedException() override {}
	};
}