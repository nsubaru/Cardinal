export module Cardinal.IO:FS.Exceptions.FileModeNoSupportedByOperationException;

export import :FS.Exceptions.Base;
export import :FS.Exceptions.GenericFileException;

export namespace Cardinal::IO::FS
{
	/// <summary>
	/// File opening mode with selected operation exception
	/// </summary>
	class FileModeNoSupportedByOperationException : public GenericFileException {
	public:
		ForceInline FileModeNoSupportedByOperationException(const FileModeNoSupportedByOperationException& orig) : GenericFileException(orig) {}
		ForceInline FileModeNoSupportedByOperationException(FileModeNoSupportedByOperationException&& from) : GenericFileException(MoveRValue(from)) {}
		ForceInline FileModeNoSupportedByOperationException& operator=(const FileModeNoSupportedByOperationException& from) { this->~FileModeNoSupportedByOperationException(); this->FileModeNoSupportedByOperationException::FileModeNoSupportedByOperationException(from); return *this; }
		ForceInline FileModeNoSupportedByOperationException& operator=(FileModeNoSupportedByOperationException&& from) { this->~FileModeNoSupportedByOperationException(); this->FileModeNoSupportedByOperationException::FileModeNoSupportedByOperationException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// Exception generation
		/// </summary>
		FileModeNoSupportedByOperationException(SourceLocation sourceLocation, ExceptionString&& fileName)
			: GenericFileException(sourceLocation, MoveRValue(fileName), L"The operation is not available in the current file mode") {}

	public:
		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new FileModeNoSupportedByOperationException(*this));
		}

	public:
		ForceInline virtual ~FileModeNoSupportedByOperationException() override {}
	};
}