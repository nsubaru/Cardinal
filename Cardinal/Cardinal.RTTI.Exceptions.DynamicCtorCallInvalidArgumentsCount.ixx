export module Cardinal.RTTI:Exceptions.DynamicCtorCallInvalidArgumentsCount;

export import :Exceptions.Base;
export import :Exceptions.DynamicCtorCallException;

export namespace Cardinal::RTTI::Exceptions
{
	class DynamicCtorCallInvalidArgumentsCountException : public DynamicCtorCallException {
	public:
		ForceInline DynamicCtorCallInvalidArgumentsCountException(const DynamicCtorCallInvalidArgumentsCountException& orig)
			: DynamicCtorCallException(orig), actualCount(orig.actualCount) {}

		ForceInline DynamicCtorCallInvalidArgumentsCountException(DynamicCtorCallInvalidArgumentsCountException&& from)
			: DynamicCtorCallException(MoveRValue(from)), actualCount(from.actualCount) {}

		ForceInline DynamicCtorCallInvalidArgumentsCountException& operator=(const DynamicCtorCallInvalidArgumentsCountException& from) {
			this->~DynamicCtorCallInvalidArgumentsCountException();
			this->DynamicCtorCallInvalidArgumentsCountException::DynamicCtorCallInvalidArgumentsCountException(from);
			return *this;
		}

		ForceInline DynamicCtorCallInvalidArgumentsCountException& operator=(DynamicCtorCallInvalidArgumentsCountException&& from) {
			this->~DynamicCtorCallInvalidArgumentsCountException();
			this->DynamicCtorCallInvalidArgumentsCountException::DynamicCtorCallInvalidArgumentsCountException(MoveRValue(from));
			return *this;
		}

	private:
		SizeT actualCount;

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param Name = 'target'>Target ctor record</param>
		/// <param Name = 'actualCount'>Actual count of arguments in method</param>
		DynamicCtorCallInvalidArgumentsCountException(
			SourceLocation sourceLocation,
			Record::RttiConstructorRecord target,
			SizeT actualCount
		) :
			DynamicCtorCallException(sourceLocation, L"Dynamic call invalid arguments count", target),
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
				ExceptionString(L"Durring calling ctor for type: "), target.Type.TypeName,
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
			return ExceptionHolderType(new DynamicCtorCallInvalidArgumentsCountException(*this));
		}

	public:
		ForceInline virtual ~DynamicCtorCallInvalidArgumentsCountException() {}
	};
}