export module Cardinal.Exceptions:NtException;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// NT function exception
	/// </summary>
	class NtException : public Exception {
	private:
		/// <summary>
		/// "NTSTATUS code" of operation
		/// </summary>
		Int32 statusCode;

	public:
		ForceInline NtException(const NtException& orig) : Exception(orig), statusCode(orig.statusCode) {}
		ForceInline NtException(NtException&& from) : Exception(MoveRValue(from)), statusCode(from.statusCode) {}
		ForceInline NtException& operator=(const NtException& from) { this->~NtException(); this->NtException::NtException(from); return *this; }
		ForceInline NtException& operator=(NtException&& from) { this->~NtException(); this->NtException::NtException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		/// <param name = 'StatusCode'>Exception code</param>
		NtException(SourceLocation sourceLocation, Int32 StatusCode)
			: Exception(sourceLocation, L"NT system call exception"), statusCode(StatusCode) {
		}

		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		/// <param name = 'StatusCode'>Exception code</param>
		NtException(SourceLocation sourceLocation, Int32 StatusCode, const ConstString& Name)
			: Exception(sourceLocation, Name), statusCode(StatusCode) {}

	public:
		/// <summary>
		/// Returns the exception code
		/// </summary>
		/// <returns>Exception code</returns>
		ForceInline Int32 GetCode() const { return statusCode; }

		/// <summary>
		/// Exception code
		/// </summary>
		Int32 Property(GetCode) Code;

	private:
		static ExceptionString GetStatusCodeMsgByCode(Int32 code);

	public:
		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override {
			return ExceptionString::Combine({
				ExceptionString(exceptionName),
				ExceptionString(L". The system call returns the code "),
				BaseString<Memory::ExceptionHeap>::Int32asHex(statusCode), ExceptionString(L". "),
				GetStatusCodeMsgByCode(statusCode), L" ",
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new NtException(*this));
		}

	public:
		ForceInline virtual ~NtException() {}
	};
}