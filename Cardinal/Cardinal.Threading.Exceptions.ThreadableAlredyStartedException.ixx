export module Cardinal.Threading:Exceptions.ThreadableAlredyStartedException;

export import :Exceptions.Base;

export namespace Cardinal::Threading
{
	/// <summary>
	/// A launch of non launched object
	/// </summary>
	class ThreadableAlredyStartedException final : public Exception {
	public:
		/// <summary>
		/// Copying constructor
		/// </summary>
		/// <param name="orig">Original</param>
		ForceInline ThreadableAlredyStartedException(const ThreadableAlredyStartedException& orig) : Exception(orig) {}

		/// <summary>
		/// Moving constructor
		/// </summary>
		/// <param name="from">Original</param>
		ForceInline ThreadableAlredyStartedException(ThreadableAlredyStartedException&& from) : Exception(MoveRValue(from)) {}

		/// <summary>
		/// Copying operator
		/// </summary>
		/// <param name="from">Original</param>
		/// <returns>A reference to a current object</returns>
		ForceInline ThreadableAlredyStartedException& operator=(const ThreadableAlredyStartedException& from) {
			this->~ThreadableAlredyStartedException();
			this->ThreadableAlredyStartedException::ThreadableAlredyStartedException(from);
			return *this;
		}

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="from">Original</param>
		/// <returns>A reference to a current object</returns>
		ForceInline ThreadableAlredyStartedException& operator=(ThreadableAlredyStartedException&& from) {
			this->~ThreadableAlredyStartedException();
			this->ThreadableAlredyStartedException::ThreadableAlredyStartedException(MoveRValue(from));
			return *this;
		}

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = "File">The file in which the exception was generated</param>
		/// <param name = "FunctName">The function name</param>
		/// <param name = "Line">The line number in which the exception was generated</param>
		ThreadableAlredyStartedException(SourceLocation sourceLocation)
			: Exception(sourceLocation, L"A launch of non launched object") {
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
			return ExceptionHolderType(new ThreadableAlredyStartedException(*this));
		}

	public:
		ForceInline virtual ~ThreadableAlredyStartedException() override {}
	};
}