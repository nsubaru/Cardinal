export module Cardinal.IO:FS.Exceptions.FileIsCoruptedException;

export import :FS.Exceptions.Base;
export import :FS.Exceptions.GenericFileException;

export namespace Cardinal::IO::FS
{
	/// <summary>
	/// File is corrupted exception
	/// </summary>
	class FileIsCoruptedException : public GenericFileException {
	public:
		ForceInline FileIsCoruptedException(const FileIsCoruptedException& orig) : GenericFileException(orig) {}
		ForceInline FileIsCoruptedException(FileIsCoruptedException&& from) : GenericFileException(MoveRValue(from)) {}
		ForceInline FileIsCoruptedException& operator=(const FileIsCoruptedException& from) { this->~FileIsCoruptedException(); this->FileIsCoruptedException::FileIsCoruptedException(from); return *this; }
		ForceInline FileIsCoruptedException& operator=(FileIsCoruptedException&& from) { this->~FileIsCoruptedException(); this->FileIsCoruptedException::FileIsCoruptedException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// Exception generation
		/// </summary>
		FileIsCoruptedException(SourceLocation sourceLocation, ExceptionString&& fileName)
			: GenericFileException(sourceLocation, MoveRValue(fileName), L"File is corrupted, file system check required") {}

	public:
		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new FileIsCoruptedException(*this));
		}

	public:
		ForceInline virtual ~FileIsCoruptedException() override {}
	};
}