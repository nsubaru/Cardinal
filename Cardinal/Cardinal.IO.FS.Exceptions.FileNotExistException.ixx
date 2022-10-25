export module Cardinal.IO:FS.Exceptions.FileNotExistException;

export import :FS.Exceptions.Base;
export import :FS.Exceptions.GenericFileException;

export namespace Cardinal::IO::FS
{
	/// <summary>
	/// File does not exist exception
	/// </summary>
	class FileNotExistException : public GenericFileException {
	public:
		ForceInline FileNotExistException(const FileNotExistException& orig) : GenericFileException(orig) {}
		ForceInline FileNotExistException(FileNotExistException&& from) : GenericFileException(MoveRValue(from)) {}
		ForceInline FileNotExistException& operator=(const FileNotExistException& from) { this->~FileNotExistException(); this->FileNotExistException::FileNotExistException(from); return *this; }
		ForceInline FileNotExistException& operator=(FileNotExistException&& from) { this->~FileNotExistException(); this->FileNotExistException::FileNotExistException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// Exception generation
		/// </summary>
		FileNotExistException(SourceLocation sourceLocation, ExceptionString&& fileName)
			: GenericFileException(sourceLocation, MoveRValue(fileName), L"File not found") {}

	public:
		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new FileNotExistException(*this));
		}

	public:
		ForceInline virtual ~FileNotExistException() override {}
	};
}