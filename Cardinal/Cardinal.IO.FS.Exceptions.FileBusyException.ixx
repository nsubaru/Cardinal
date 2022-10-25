export module Cardinal.IO:FS.Exceptions.FileBusyException;

export import :FS.Exceptions.Base;
export import :FS.Exceptions.GenericFileException;

export namespace Cardinal::IO::FS
{
	/// <summary>
	/// File is busy by another process exception
	/// </summary>
	class FileBusyException : public GenericFileException {
	public:
		ForceInline FileBusyException(const FileBusyException& orig) : GenericFileException(orig) {}
		ForceInline FileBusyException(FileBusyException&& from) : GenericFileException(MoveRValue(from)) {}
		ForceInline FileBusyException& operator=(const FileBusyException& from) { this->~FileBusyException(); this->FileBusyException::FileBusyException(from); return *this; }
		ForceInline FileBusyException& operator=(FileBusyException&& from) { this->~FileBusyException(); this->FileBusyException::FileBusyException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// Exception generation
		/// </summary>
		FileBusyException(SourceLocation sourceLocation, ExceptionString&& fileName)
			: GenericFileException(sourceLocation, MoveRValue(fileName), L"File is busy") {}

	public:
		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new FileBusyException(*this));
		}

	public:
		ForceInline virtual ~FileBusyException() override {}
	};
}