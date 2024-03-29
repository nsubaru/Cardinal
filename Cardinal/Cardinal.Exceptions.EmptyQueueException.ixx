export module Cardinal.Exceptions:EmptyQueueException;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// Empty queue
	/// </summary>
	class EmptyQueueException : public Exception {
	public:
		ForceInline EmptyQueueException(const EmptyQueueException& orig) : Exception(orig) {}
		ForceInline EmptyQueueException(EmptyQueueException&& from) : Exception(MoveRValue(from)) {}
		ForceInline EmptyQueueException& operator=(const EmptyQueueException& from) { this->~EmptyQueueException(); this->EmptyQueueException::EmptyQueueException(from); return *this; }
		ForceInline EmptyQueueException& operator=(EmptyQueueException&& from) { this->~EmptyQueueException(); this->EmptyQueueException::EmptyQueueException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		EmptyQueueException(SourceLocation sourceLocation)
			: Exception(sourceLocation, L"Trying to remove items from an empty queue.") {
		}

	public:
		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override {
			return ExceptionString::Combine({
				ExceptionString(exceptionName),
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new EmptyQueueException(*this));
		}

	public:
		ForceInline virtual ~EmptyQueueException() {}
	};
}