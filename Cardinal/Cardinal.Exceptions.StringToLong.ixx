export module Cardinal.Exceptions:StringToLong;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// String is too long exception
	/// </summary>
	class StringToLong final : public Exception {
	private:
		/// <summary>
		/// Requested size
		/// </summary>
		SizeT requiredSize;

	public:
		ForceInline StringToLong(const StringToLong& orig) : Exception(orig), requiredSize(orig.requiredSize) {}
		ForceInline StringToLong(StringToLong&& from) : Exception(MoveRValue(from)), requiredSize(from.requiredSize) {}
		ForceInline StringToLong& operator=(const StringToLong& from) { this->~StringToLong(); this->StringToLong::StringToLong(from); return *this; }
		ForceInline StringToLong& operator=(StringToLong&& from) { this->~StringToLong(); this->StringToLong::StringToLong(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		/// <param name = 'RequiredSize'>Requested size</param>
		StringToLong(SourceLocation sourceLocation, SizeT RequiredSize) :
			Exception(sourceLocation, L"String is too long"), requiredSize(RequiredSize) {}

	public:
		/// <summary>
		/// Returns the requested size
		/// </summary>
		ForceInline SizeT GetRequiredSize() const { return requiredSize; }

		/// <summary>
		/// Requested size
		/// </summary>
		SizeT Property(GetRequiredSize) RequiredSize;

	public:
		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const final {
			return ExceptionString::Combine({
				ExceptionString(exceptionName),
				ExceptionString(L". Requested size "), ExceptionString::toString(requiredSize),
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new StringToLong(*this));
		}

	public:
		ForceInline virtual ~StringToLong() {}
	};
}