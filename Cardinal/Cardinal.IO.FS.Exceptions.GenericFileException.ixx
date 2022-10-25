export module Cardinal.IO:FS.Exceptions.GenericFileException;

export import :FS.Exceptions.Base;
export import :FS.Exceptions.FsIoException;

export namespace Cardinal::IO::FS
{
	/// <summary>
	/// Generic file exception
	/// </summary>
	class GenericFileException : public FsIoException {
	private:
		ExceptionString fileName;

	public:
		ForceInline GenericFileException(const GenericFileException& orig) : FsIoException(orig), fileName(orig.fileName) {}
		ForceInline GenericFileException(GenericFileException&& from) : FsIoException(MoveRValue(from)), fileName(MoveRValue(from.fileName)) {}
		ForceInline GenericFileException& operator=(const GenericFileException& from) { this->~GenericFileException(); this->GenericFileException::GenericFileException(from); return *this; }
		ForceInline GenericFileException& operator=(GenericFileException&& from) { this->~GenericFileException(); this->GenericFileException::GenericFileException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// Exception generation
		/// </summary>
		GenericFileException(SourceLocation sourceLocation, ExceptionString&& fileName, ConstString&& ErrorName)
			: fileName(fileName), FsIoException(sourceLocation, ErrorName) {}

	public:
		virtual ExceptionString toString() const override {
			return ExceptionString::Combine({
				ExceptionString(L"File exception with message: "), ExceptionString(exceptionName), ExceptionString(L". "),
				ExceptionString(L"Occurs while processing the file: \""), fileName, ExceptionString(L"\""),
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L".") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new GenericFileException(*this));
		}

	public:
		ForceInline virtual ~GenericFileException() override {}
	};
}