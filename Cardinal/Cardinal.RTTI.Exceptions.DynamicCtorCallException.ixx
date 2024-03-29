export module Cardinal.RTTI:Exceptions.DynamicCtorCallException;

export import :Exceptions.Base;
export import :Exceptions.GenericRttiException;

export namespace Cardinal::RTTI::Exceptions
{
	class DynamicCtorCallException : public GenericRttiException {
	protected:
		Record::RttiConstructorRecord target;
	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'message'>Exception description</param>
		/// <param Name = 'target'>Constructor rtti record</param>
		DynamicCtorCallException(SourceLocation sourceLocation, ConstString message, Record::RttiConstructorRecord target)
			:GenericRttiException(sourceLocation, message), target(target) {
		}

		DynamicCtorCallException(const DynamicCtorCallException& from) : GenericRttiException(from), target(from.target) {}
		DynamicCtorCallException(DynamicCtorCallException&& from) : GenericRttiException(from), target(from.target) {}

	public:
		/// <summary>
		/// Return ctor rtti record durring dynamic call which exception was threw
		/// </summary>
		/// <returns></returns>
		Record::RttiConstructorRecord GetTarget() const { return target; }

		/// <summary>
		/// Ctor rtti record durring dynamic call which exception was threw
		/// </summary>
		Record::RttiConstructorRecord Property(GetTarget) Target;

	public:
		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override {
			return ExceptionString::Combine({
				ExceptionString(exceptionName), ExceptionString(L". "),
				ExceptionString(L"Durring calling ctor for: "), target.Type.TypeName,
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new DynamicCtorCallException(*this));
		}

	public:
		ForceInline virtual ~DynamicCtorCallException() {}
	};
}