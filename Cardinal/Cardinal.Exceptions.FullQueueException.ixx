export module Cardinal.Exceptions:FullQueueException;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// Queue overflow exception
	/// </summary>
	class FullQueueException : public Exception {
	private:
		/// <summary>
		/// Maximal allowed queue size
		/// </summary>
		SizeT maxSize;

	public:
		ForceInline FullQueueException(const FullQueueException& orig) : Exception(orig), maxSize(orig.maxSize) {}
		ForceInline FullQueueException(FullQueueException&& from) : Exception(MoveRValue(from)), maxSize(from.maxSize) {}
		ForceInline FullQueueException& operator=(const FullQueueException& from) { this->~FullQueueException(); this->FullQueueException::FullQueueException(from); return *this; }
		ForceInline FullQueueException& operator=(FullQueueException&& from) { this->~FullQueueException(); this->FullQueueException::FullQueueException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		/// <param name = 'maxSize'>Maximal allowed queue size</param>
		FullQueueException(SourceLocation sourceLocation, SizeT maxSize)
			: Exception(sourceLocation, L"Maximal queue size exceeded "), maxSize(maxSize) {
		}

	public:
		/// <summary>
		/// "Getter" for the maximal queue size
		/// </summary>
		ForceInline SizeT GetMaxSize() const {
			return maxSize;
		}

	public:
		/// <summary>
		/// Maximal allowed queue size
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
			return ExceptionHolderType(new FullQueueException(*this));
		}

	public:
		ForceInline virtual ~FullQueueException() {}
	};
}