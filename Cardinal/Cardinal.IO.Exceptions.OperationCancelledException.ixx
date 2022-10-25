export module Cardinal.IO:Exceptions.OperationCancelledException;

export import :Exceptions.Base;

export namespace Cardinal::IO
{
	/// <summary>
	/// Operation cancelled exception
	/// </summary>
	class OperationCancelledException : public IoException {
	public:
		ForceInline OperationCancelledException(const OperationCancelledException& orig) : IoException(orig) {}
		ForceInline OperationCancelledException(OperationCancelledException&& from) : IoException(MoveRValue(from)) {}
		ForceInline OperationCancelledException& operator=(const OperationCancelledException& from) { this->~OperationCancelledException(); this->OperationCancelledException::OperationCancelledException(from); return *this; }
		ForceInline OperationCancelledException& operator=(OperationCancelledException&& from) { this->~OperationCancelledException(); this->OperationCancelledException::OperationCancelledException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// Exception generation
		/// </summary>
		OperationCancelledException(SourceLocation sourceLocation)
			: IoException(sourceLocation, L"Operation was cancelled") {}

	public:
		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new OperationCancelledException(*this));
		}

	public:
		ForceInline virtual ~OperationCancelledException() override {}
	};
}