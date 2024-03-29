export module Cardinal.RTTI:Exceptions.DynamicCallInvalidTargetException;

export import :Exceptions.Base;
export import :Exceptions.DynamicCallException;

export namespace Cardinal::RTTI::Exceptions
{
	/// <summary>
	/// Dynamic call invalid target exception
	/// </summary>
	class DynamicCallInvalidTargetException : public DynamicCallException {
	public:
		ForceInline DynamicCallInvalidTargetException(const DynamicCallInvalidTargetException& orig)
			: DynamicCallException(orig), actualType(orig.actualType), expectedlType(orig.expectedlType) {}

		ForceInline DynamicCallInvalidTargetException(DynamicCallInvalidTargetException&& from)
			: DynamicCallException(MoveRValue(from)), actualType(from.actualType), expectedlType(from.expectedlType) {}

		ForceInline DynamicCallInvalidTargetException& operator=(const DynamicCallInvalidTargetException& from) {
			this->~DynamicCallInvalidTargetException();
			this->DynamicCallInvalidTargetException::DynamicCallInvalidTargetException(from);
			return *this;
		}

		ForceInline DynamicCallInvalidTargetException& operator=(DynamicCallInvalidTargetException&& from) {
			this->~DynamicCallInvalidTargetException();
			this->DynamicCallInvalidTargetException::DynamicCallInvalidTargetException(MoveRValue(from));
			return *this;
		}

	private:
		Record::BaseRttiTypeRecord<Memory::Heap> actualType;
		Record::BaseRttiTypeRecord<Memory::Heap> expectedlType;

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param Name = 'target'>Target method record</param>
		DynamicCallInvalidTargetException(
			SourceLocation sourceLocation,
			Record::BaseRttiMethodRecord<Memory::Heap> target,
			Record::BaseRttiTypeRecord<Memory::Heap> actualType,
			Record::BaseRttiTypeRecord<Memory::Heap> expectedlType
		) :
			DynamicCallException(sourceLocation, L"Dynamic call invalid target object type", target),
			actualType(actualType),
			expectedlType(expectedlType) {
		}

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
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new DynamicCallInvalidTargetException(*this));
		}

	public:
		ForceInline virtual ~DynamicCallInvalidTargetException() {}
	};
}