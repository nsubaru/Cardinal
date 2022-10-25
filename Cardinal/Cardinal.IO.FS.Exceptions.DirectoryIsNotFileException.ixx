export module Cardinal.IO:FS.Exceptions.DirectoryIsNotFileException;

export import :FS.Exceptions.Base;
export import :FS.Exceptions.GenericFileException;

export namespace Cardinal::IO::FS
{
	/// <summary>
	/// Attempt to open the directory as a file exception
	/// </summary>
	class DirectoryIsNotFileException : public GenericFileException {
	public:
		ForceInline DirectoryIsNotFileException(const DirectoryIsNotFileException& orig) : GenericFileException(orig) {}
		ForceInline DirectoryIsNotFileException(DirectoryIsNotFileException&& from) : GenericFileException(MoveRValue(from)) {}
		ForceInline DirectoryIsNotFileException& operator=(const DirectoryIsNotFileException& from) { this->~DirectoryIsNotFileException(); this->DirectoryIsNotFileException::DirectoryIsNotFileException(from); return *this; }
		ForceInline DirectoryIsNotFileException& operator=(DirectoryIsNotFileException&& from) { this->~DirectoryIsNotFileException(); this->DirectoryIsNotFileException::DirectoryIsNotFileException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// Exception generation
		/// </summary>
		DirectoryIsNotFileException(SourceLocation sourceLocation, ExceptionString&& fileName)
			: GenericFileException(sourceLocation, MoveRValue(fileName), L"Unable to open the directory as a file") {}

	public:
		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new DirectoryIsNotFileException(*this));
		}

	public:
		ForceInline virtual ~DirectoryIsNotFileException() override {}
	};
}