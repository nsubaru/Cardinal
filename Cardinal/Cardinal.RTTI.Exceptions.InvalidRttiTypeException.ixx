export module Cardinal.RTTI:Exceptions.InvalidRttiTypeException;

export import :Exceptions.Base;
export import :Exceptions.GenericRttiException;

export namespace Cardinal::RTTI::Exceptions
{
	///<summary>
	/// Invalid RTTI type exception
	/// </summary>
	class InvalidRttiTypeException final : public GenericRttiException {
	public:
		ForceInline InvalidRttiTypeException(const InvalidRttiTypeException& orig)
			: GenericRttiException(orig), actual(orig.actual), expected(orig.expected) {}

		ForceInline InvalidRttiTypeException(InvalidRttiTypeException&& from)
			: GenericRttiException(MoveRValue(from)), actual(from.actual), expected(from.expected) {}

		ForceInline InvalidRttiTypeException& operator=(const InvalidRttiTypeException& from) {
			this->~InvalidRttiTypeException();
			this->InvalidRttiTypeException::InvalidRttiTypeException(from);
			return *this;
		}

		ForceInline InvalidRttiTypeException& operator=(InvalidRttiTypeException&& from) {
			this->~InvalidRttiTypeException();
			this->InvalidRttiTypeException::InvalidRttiTypeException(MoveRValue(from));
			return *this;
		}

	private:
		Cardinal::RTTI::Record::BaseRttiTypeRecord<Memory::Heap> actual;
		Cardinal::RTTI::Record::BaseRttiTypeRecord<Memory::Heap> expected;

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		/// <param Name = 'Type'>The type which DI cannot found</param>
		InvalidRttiTypeException(
			SourceLocation sourceLocation,
			Cardinal::RTTI::Record::BaseRttiTypeRecord<Memory::Heap> actual,
			Cardinal::RTTI::Record::BaseRttiTypeRecord<Memory::Heap> expected
		) :
			GenericRttiException(sourceLocation, L"Invalid rttti type"),
			actual(actual),
			expected(expected) {
		}

	public:
		/// <summary>
		/// Return actual type
		/// </summary>
		Record::RttiTypeRecord GetActualType() const { return actual; }

		/// <summary>
		/// Return expected type
		/// </summary>
		Record::RttiTypeRecord GetExpectedType() const { return expected; }

	public:
		/// <summary>
		/// Actual type
		/// </summary>
		Record::RttiTypeRecord Property(GetActualType) ActualType;

		/// <summary>
		/// Expected type
		/// </summary>
		Record::RttiTypeRecord Property(GetExpectedType) ExpectedType;

	public:
		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override final {
			return ExceptionString::Combine({
				ExceptionString(exceptionName), ExceptionString(L". "),
				ExceptionString(L"Actual type: "), actual.TypeName,
				ExceptionString(L"Expected type: "), expected.TypeName,
				ExceptionString(L". "), location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". There is also a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new InvalidRttiTypeException(*this));
		}

	public:
		ForceInline virtual ~InvalidRttiTypeException() {}
	};
}