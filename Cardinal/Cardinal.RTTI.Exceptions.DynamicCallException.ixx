export module Cardinal.RTTI:Exceptions.DynamicCallException;

export import :Exceptions.Base;
export import :Exceptions.GenericRttiException;

export namespace Cardinal::RTTI::Exceptions
{
	/// <summary>
	/// Dynamic call exception
	/// </summary>
	class DynamicCallException : public GenericRttiException {
	protected:
		Record::RttiMethodRecord target;

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'message'>Exception description</param>
		/// <param Name = 'target'>Method rtti record</param>
		DynamicCallException(SourceLocation sourceLocation, ConstString message, Record::RttiMethodRecord target)
			:GenericRttiException(sourceLocation, message), target(target) {
		}

		DynamicCallException(const DynamicCallException& from) : GenericRttiException(from), target(from.target) {}
		DynamicCallException(DynamicCallException&& from) : GenericRttiException(from), target(from.target) {}

	public:
		/// <summary>
		/// Return the method RTTI record during dynamic call, which exception was thrown
		/// </summary>
		Record::RttiMethodRecord GetTarget() const { return target; }

		/// <summary>
		/// The method RTTI record during dynamic call, which exception was thrown
		/// </summary>
		Record::RttiMethodRecord Property(GetTarget) Target;

	public:
		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override {
			return ExceptionString::Combine({
				ExceptionString(exceptionName), ExceptionString(L". "),
				ExceptionString(L"Durring calling: "), target.Name,
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new DynamicCallException(*this));
		}

	public:
		ForceInline virtual ~DynamicCallException() {}
	};
}