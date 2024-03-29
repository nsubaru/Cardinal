export module Cardinal.RTTI:Exceptions.DynamicCallInvalidArgumentType;

export import :Exceptions.Base;
export import :Exceptions.DynamicCallException;

export namespace Cardinal::RTTI::Exceptions
{
	/// <summary>
	/// Dynamic call invalid arguments type exception
	/// </summary>
	class DynamicCallInvalidArgumentTypeException : public DynamicCallException {
	public:
		ForceInline DynamicCallInvalidArgumentTypeException(const DynamicCallInvalidArgumentTypeException& orig)
			: DynamicCallException(orig), actualType(orig.actualType), expectedlType(orig.expectedlType), index(orig.index) {}

		ForceInline DynamicCallInvalidArgumentTypeException(DynamicCallInvalidArgumentTypeException&& from)
			: DynamicCallException(MoveRValue(from)), actualType(from.actualType), expectedlType(from.expectedlType), index(from.index) {}

		ForceInline DynamicCallInvalidArgumentTypeException& operator=(const DynamicCallInvalidArgumentTypeException& from) {
			this->~DynamicCallInvalidArgumentTypeException();
			this->DynamicCallInvalidArgumentTypeException::DynamicCallInvalidArgumentTypeException(from);
			return *this;
		}

		ForceInline DynamicCallInvalidArgumentTypeException& operator=(DynamicCallInvalidArgumentTypeException&& from) {
			this->~DynamicCallInvalidArgumentTypeException();
			this->DynamicCallInvalidArgumentTypeException::DynamicCallInvalidArgumentTypeException(MoveRValue(from));
			return *this;
		}

	private:
		Cardinal::RTTI::Record::BaseRttiTypeRecord<Memory::Heap> actualType;
		Cardinal::RTTI::Record::BaseRttiTypeRecord<Memory::Heap> expectedlType;
		SizeT index;

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param Name = 'target'>Target method record</param>
		/// <param Name = 'actualCount'>Actual count of arguments in method</param>
		DynamicCallInvalidArgumentTypeException(
			SourceLocation sourceLocation,
			Cardinal::RTTI::Record::BaseRttiMethodRecord<Memory::Heap> target,
			Cardinal::RTTI::Record::BaseRttiTypeRecord<Memory::Heap> actualType,
			Cardinal::RTTI::Record::BaseRttiTypeRecord<Memory::Heap> expectedlType,
			SizeT index
		) :
			DynamicCallException(sourceLocation, L"Dynamic call invalid type on specific position", target),
			actualType(actualType),
			expectedlType(expectedlType),
			index(index) {
		}

	public:
		/// <summary>
		/// Return argument index
		/// </summary>
		SizeT GetIndex() const { return index; }

		/// <summary>
		/// Argument index
		/// </summary>
		SizeT Property(GetIndex) Index;

	public:
		/// <summary>
		/// Return actual type
		/// </summary>
		Record::RttiTypeRecord  GetActualType() const { return actualType; }

		/// <summary>
		/// Actual type
		/// </summary>
		Record::RttiTypeRecord  Property(GetActualType) ActualCount;

	public:
		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override final {
			return ExceptionString::Combine({
				ExceptionString(exceptionName), ExceptionString(L". "),
				ExceptionString(L"Durring calling: "), target.Name,
				ExceptionString(L"Expected type: "), ExceptionString(expectedlType.TypeName), ExceptionString(L". "),
				ExceptionString(L"Actual type: "), ExceptionString(actualType.TypeName), ExceptionString(L". "),
				ExceptionString(L"At position: "), ExceptionString::toString(index), ExceptionString(L". "),
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new DynamicCallInvalidArgumentTypeException(*this));
		}

	public:
		ForceInline virtual ~DynamicCallInvalidArgumentTypeException() {}
	};
}