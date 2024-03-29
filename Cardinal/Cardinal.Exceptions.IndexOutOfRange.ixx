export module Cardinal.Exceptions:IndexOutOfRange;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// The access to element which is located beyond the range exception
	/// </summary>
	class IndexOutOfRange : public Exception {
	private:
		SizeT Index;
		SizeT MaxIndex;

	public:
		ForceInline IndexOutOfRange(const IndexOutOfRange& orig) : Exception(orig), Index(orig.Index), MaxIndex(orig.MaxIndex) {}
		ForceInline IndexOutOfRange(IndexOutOfRange&& from) : Exception(MoveRValue(from)), Index(from.Index), MaxIndex(from.MaxIndex) {}
		ForceInline IndexOutOfRange& operator=(const IndexOutOfRange& from) { this->~IndexOutOfRange(); this->IndexOutOfRange::IndexOutOfRange(from); return *this; }
		ForceInline IndexOutOfRange& operator=(IndexOutOfRange&& from) { this->~IndexOutOfRange(); this->IndexOutOfRange::IndexOutOfRange(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		/// <param name = 'RequiredIndex'>Index of element which was tried to get</param>
		/// <param name = 'MaxAllowedIndex'>Maximal allowed index</param>
		IndexOutOfRange(SourceLocation sourceLocation, SizeT RequiredIndex, SizeT MaxAllowedIndex)
			: Exception(sourceLocation, L"Out of range"),
			Index(RequiredIndex), MaxIndex(MaxAllowedIndex) {
		}

	public:
		/// <summary>
		/// Requested index
		/// /// </summary>
		ForceInline SizeT GetRequiredIndex() const { return Index; }

		/// <summary>
		/// Maximal allowed index
		/// </summary>
		ForceInline SizeT GetMaxAllowedIndex() const { return MaxIndex; }

	public:
		/// <summary>
		/// Requested index
		/// </summary>
		SizeT Property(GetRequiredIndex) RequiredIndex;

		/// <summary>
		/// Maximal allowed index
		/// </summary>
		SizeT Property(GetMaxAllowedIndex) MaxAllowedIndex;

	public:
		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override final {
			return ExceptionString::Combine({
				ExceptionString(exceptionName),
				ExceptionString(L". During the try to acces the element by index "), ExceptionString::toString(RequiredIndex),
				ExceptionString(L" if maximal allowed index "), ExceptionString::toString(MaxAllowedIndex),
				ExceptionString(L". "),
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>Smart pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new IndexOutOfRange(*this));
		}

	public:
		ForceInline virtual ~IndexOutOfRange() {}
	};
}