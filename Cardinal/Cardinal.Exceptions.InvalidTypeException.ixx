export module Cardinal.Exceptions:InvalidTypeException;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// Invalid type exception
	/// </summary>
	class InvalidTypeException final : public Exception {
	private:
		const TypeInfo& requiredType;
		const TypeInfo& actuallyType;

	public:
		ForceInline InvalidTypeException(const InvalidTypeException& orig) : Exception(orig), requiredType(orig.requiredType), actuallyType(orig.actuallyType) {}
		ForceInline InvalidTypeException(InvalidTypeException&& from) : Exception(MoveRValue(from)), requiredType(from.requiredType), actuallyType(from.actuallyType) {}
		ForceInline InvalidTypeException& operator=(const InvalidTypeException& from) { this->~InvalidTypeException(); this->InvalidTypeException::InvalidTypeException(from); return *this; }
		ForceInline InvalidTypeException& operator=(InvalidTypeException&& from) { this->~InvalidTypeException(); this->InvalidTypeException::InvalidTypeException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		/// <param name = 'RequireType'>The information about expected type</param>
		/// <param name = 'ActuallyType'>The information about actual type</param>
		InvalidTypeException(SourceLocation sourceLocation, const TypeInfo& RequiredType, const TypeInfo& ActuallyType)
			: Exception(sourceLocation, L"Wrong type"),
			requiredType(RequiredType), actuallyType(ActuallyType) {
		}

	public:
		/// <summary>
		/// The information about expected type
		/// </summary>
		ForceInline const TypeInfo& GetRequiredType() const { return requiredType; }

		/// <summary>
		/// The information about actual type
		/// </summary>
		ForceInline const TypeInfo& GetActuallyType() const { return actuallyType; }

	public:
		/// <summary>
		/// The information about expected type
		/// </summary>
		const TypeInfo Property(GetRequiredType) RequiredType;

		/// <summary>
		/// The information about actual type
		/// </summary>
		const TypeInfo Property(GetActuallyType) ActuallyType;

	public:
		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override final {
			Unsafe(
				return ExceptionString::Combine({
					ExceptionString(exceptionName),
					ExceptionString(L". The expected type: "), ExceptionString(const_cast<AChar*>(RequiredType.GetCxxName())),
					ExceptionString(L". The actual type: "), ExceptionString(const_cast<AChar*>(ActuallyType.GetCxxName())),
					ExceptionString(L". "),
					location.toString(),
					(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
					}));
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new InvalidTypeException(*this));
		}

	public:
		ForceInline virtual ~InvalidTypeException() {}
	};
}