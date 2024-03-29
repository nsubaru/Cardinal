export module Cardinal.RTTI:Exceptions.DynamicCallInvalidArgumentsCount;

export import :Exceptions.Base;
export import :Exceptions.DynamicCallException;

export namespace Cardinal::RTTI::Exceptions
{
	/// <summary>
	/// Dynamic call invalid arguments count exception
	/// </summary>
	class DynamicCallInvalidArgumentsCountException : public DynamicCallException {
	public:
		ForceInline DynamicCallInvalidArgumentsCountException(const DynamicCallInvalidArgumentsCountException& orig)
			: DynamicCallException(orig), actualCount(orig.actualCount) {}

		ForceInline DynamicCallInvalidArgumentsCountException(DynamicCallInvalidArgumentsCountException&& from)
			: DynamicCallException(MoveRValue(from)), actualCount(from.actualCount) {}

		ForceInline DynamicCallInvalidArgumentsCountException& operator=(const DynamicCallInvalidArgumentsCountException& from) {
			this->~DynamicCallInvalidArgumentsCountException();
			this->DynamicCallInvalidArgumentsCountException::DynamicCallInvalidArgumentsCountException(from);
			return *this;
		}

		ForceInline DynamicCallInvalidArgumentsCountException& operator=(DynamicCallInvalidArgumentsCountException&& from) {
			this->~DynamicCallInvalidArgumentsCountException();
			this->DynamicCallInvalidArgumentsCountException::DynamicCallInvalidArgumentsCountException(MoveRValue(from));
			return *this;
		}

	private:
		SizeT actualCount;

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param Name = 'target'>Target method record</param>
		/// <param Name = 'actualCount'>Actual count of arguments in method</param>
		DynamicCallInvalidArgumentsCountException(
			SourceLocation sourceLocation,
			Record::RttiMethodRecord target,
			SizeT actualCount
		) :
			DynamicCallException(sourceLocation, L"Dynamic call invalid arguments count", target),
			actualCount(actualCount) {
		}

	public:
		/// <summary>
		/// Return actual count
		/// </summary>
		SizeT GetActualCount() const { return actualCount; }

		/// <summary>
		/// Actual count
		/// </summary>
		SizeT Property(GetActualType) ActualCount;

	public:
		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override final {
			return ExceptionString::Combine({
				ExceptionString(exceptionName), ExceptionString(L". "),
				ExceptionString(L"Durring calling: "), target.Name,
				ExceptionString(L"Expected count: "), ExceptionString::toString(target.Arguments.Count), ExceptionString(L". "),
				ExceptionString(L"Actual count: "), ExceptionString::toString(actualCount), ExceptionString(L". "),
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new DynamicCallInvalidArgumentsCountException(*this));
		}

	public:
		ForceInline virtual ~DynamicCallInvalidArgumentsCountException() {}
	};
}