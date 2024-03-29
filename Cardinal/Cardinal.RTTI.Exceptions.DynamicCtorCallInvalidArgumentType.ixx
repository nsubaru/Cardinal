export module Cardinal.RTTI:Exceptions.DynamicCtorCallInvalidArgumentType;

export import :Exceptions.Base;
export import :Exceptions.DynamicCtorCallException;

export namespace Cardinal::RTTI::Exceptions
{
	class DynamicCtorCallInvalidTargetException : public DynamicCtorCallException {
	public:
		ForceInline DynamicCtorCallInvalidTargetException(const DynamicCtorCallInvalidTargetException& orig)
			: DynamicCtorCallException(orig), actualType(orig.actualType), expectedlType(orig.expectedlType) {}

		ForceInline DynamicCtorCallInvalidTargetException(DynamicCtorCallInvalidTargetException&& from)
			: DynamicCtorCallException(MoveRValue(from)), actualType(from.actualType), expectedlType(from.expectedlType) {}

		ForceInline DynamicCtorCallInvalidTargetException& operator=(const DynamicCtorCallInvalidTargetException& from) {
			this->~DynamicCtorCallInvalidTargetException();
			this->DynamicCtorCallInvalidTargetException::DynamicCtorCallInvalidTargetException(from);
			return *this;
		}

		ForceInline DynamicCtorCallInvalidTargetException& operator=(DynamicCtorCallInvalidTargetException&& from) {
			this->~DynamicCtorCallInvalidTargetException();
			this->DynamicCtorCallInvalidTargetException::DynamicCtorCallInvalidTargetException(MoveRValue(from));
			return *this;
		}

	private:
		Record::RttiTypeRecord actualType;
		Record::RttiTypeRecord expectedlType;
		SizeT position;

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param Name = 'target'>Target method record</param>
		DynamicCtorCallInvalidTargetException(
			SourceLocation sourceLocation,
			Record::RttiConstructorRecord target,
			Record::RttiTypeRecord actualType,
			Record::RttiTypeRecord expectedlType,
			SizeT position
		) :
			DynamicCtorCallException(sourceLocation, L"Dynamic call invalid target object type", target),
			actualType(actualType),
			expectedlType(expectedlType),
			position(position) {
		}

	public:
		/// <summary>
		/// Return actual type
		/// </summary>
		Record::RttiTypeRecord  GetActualType() const { return actualType; }

		/// <summary>
		/// Return argument position
		/// </summary>
		SizeT GetPosition() const { return position; }

	public:
		/// <summary>
		/// Actual type
		/// </summary>
		Record::RttiTypeRecord  Property(GetActualType) ActualType;

		/// <summary>
		/// Argument position
		/// </summary>
		SizeT Property(GetPosition) Position;

	public:
		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override final {
			return ExceptionString::Combine({
				ExceptionString(exceptionName), ExceptionString(L". "),
				ExceptionString(L"Durring calling ctor for type: "), target.Type.TypeName,
				ExceptionString(L"Expected type: "), ExceptionString(expectedlType.TypeName), ExceptionString(L". "),
				ExceptionString(L"Actual type: "), ExceptionString(actualType.TypeName), ExceptionString(L". "),
				ExceptionString(L"On position: "), ExceptionString::toString(position), ExceptionString(L". "),
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new DynamicCtorCallInvalidTargetException(*this));
		}

	public:
		ForceInline virtual ~DynamicCtorCallInvalidTargetException() {}
	};
}