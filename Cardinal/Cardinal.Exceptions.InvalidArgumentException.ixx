export module Cardinal.Exceptions:InvalidArgumentException;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// Invalid argument
	/// </summary>
	class InvalidArgumentException final : public Exception {
	private:
		/// <summary>
		/// Argument name
		/// </summary>
		ConstString ArgNameValue;

		/// <summary>
		/// Additional information
		/// </summary>
		ConstString DescriptionValue;

	public:
		ForceInline InvalidArgumentException(const InvalidArgumentException& orig) :
			Exception(orig),
			ArgNameValue(orig.ArgNameValue),
			DescriptionValue(orig.DescriptionValue) {
		}

		ForceInline InvalidArgumentException(InvalidArgumentException&& from) :
			Exception(from),
			ArgNameValue(from.ArgNameValue),
			DescriptionValue(from.DescriptionValue) {
		}

		ForceInline InvalidArgumentException& operator=(const InvalidArgumentException& from) { this->~InvalidArgumentException(); this->InvalidArgumentException::InvalidArgumentException(from); return *this; }
		ForceInline InvalidArgumentException& operator=(InvalidArgumentException&& from) { this->~InvalidArgumentException(); this->InvalidArgumentException::InvalidArgumentException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		/// <param name = 'ArgName'>Argument name</param>
		/// <param name = 'Description'>Additional information</param>
		InvalidArgumentException(SourceLocation sourceLocation, const ConstString& ArgName, const ConstString& Description) :
			Exception(sourceLocation, L"Invalid argument"), ArgNameValue(ArgName), DescriptionValue(Description) {
		}

	public:
		/// <summary>
		/// Returns the argument name
		/// </summary>
		ForceInline const ConstString& GetArgName() { return ArgNameValue; }

		/// <summary>
		/// Returns the additional information
		/// </summary>
		ForceInline const ConstString& GetDescription() { return DescriptionValue; }

	public:
		/// <summary>
		/// Argument name
		/// </summary>
		ConstString Property(ArgNameValue) ArgumentPosition;

		/// <summary>
		/// Additional information
		/// </summary>
		ConstString Property(GetDescription) Description;

	public:
		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override final {
			return ExceptionString::Combine({
				ExceptionString(exceptionName),
				ExceptionString(L". Invalid "), ArgNameValue,
				ExceptionString(L" argument. Additional description: "),
				ExceptionString(DescriptionValue), ExceptionString(L". "),
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new InvalidArgumentException(*this));
		}

	public:
		ForceInline virtual ~InvalidArgumentException() {}
	};
}