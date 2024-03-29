export module Cardinal.Exceptions:FullStackException;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// Stack overflow
	/// </summary>
	class FullStackException : public Exception {
	private:
		/// <summary>
		/// Maximal allowed stack size
		/// </summary>
		SizeT maxSize;

	public:
		ForceInline FullStackException(const FullStackException& orig) : Exception(orig), maxSize(orig.maxSize) {}
		ForceInline FullStackException(FullStackException&& from) : Exception(MoveRValue(from)), maxSize(from.maxSize) {}
		ForceInline FullStackException& operator=(const FullStackException& from) { this->~FullStackException(); this->FullStackException::FullStackException(from); return *this; }
		ForceInline FullStackException& operator=(FullStackException&& from) { this->~FullStackException(); this->FullStackException::FullStackException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		/// <param name = 'maxSize'>Maximal allowed stack size</param>
		FullStackException(SourceLocation sourceLocation, SizeT maxSize)
			: Exception(sourceLocation, L"Maximum stack size is exceeded"), maxSize(maxSize) {
		}

	public:
		/// <summary>
		/// "Getter" for the maximal stack size
		/// </summary>
		ForceInline SizeT GetMaxSize() const {
			return maxSize;
		}

	public:
		/// <summary>
		/// Maximal allowed stack size
		/// </summary>
		SizeT Property(GetMaxSize) MaxSize;

	public:
		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override {
			return ExceptionString::Combine({
				ExceptionString(exceptionName),
				ExceptionString(L". Maximal allowed size - "),
				ExceptionString::toString(maxSize),
				ExceptionString(L". "),
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new FullStackException(*this));
		}

	public:
		ForceInline virtual ~FullStackException() {}
	};
}