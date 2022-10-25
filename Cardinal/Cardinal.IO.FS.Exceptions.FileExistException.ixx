export module Cardinal.IO:FS.Exceptions.FileExistException;

export import :FS.Exceptions.Base;
export import :FS.Exceptions.GenericFileException;

export namespace Cardinal::IO::FS
{
	/// <summary>
	/// File exists exception
	/// </summary>
	class FileExistException : public GenericFileException {
	public:
		ForceInline FileExistException(const FileExistException& orig) : GenericFileException(orig) {}
		ForceInline FileExistException(FileExistException&& from) : GenericFileException(MoveRValue(from)) {}
		ForceInline FileExistException& operator=(const FileExistException& from) { this->~FileExistException(); this->FileExistException::FileExistException(from); return *this; }
		ForceInline FileExistException& operator=(FileExistException&& from) { this->~FileExistException(); this->FileExistException::FileExistException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// Exception generation
		/// </summary>
		FileExistException(SourceLocation sourceLocation, ExceptionString&& fileName)
			: GenericFileException(sourceLocation, MoveRValue(fileName), L"File already exists") {}

	public:
		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new FileExistException(*this));
		}

	public:
		ForceInline virtual ~FileExistException() override {}
	};
}