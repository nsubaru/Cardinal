export module Cardinal.IO:FS.Exceptions.CantReadMoreDataThanInExistInFileException;

export import :FS.Exceptions.Base;
export import :FS.Exceptions.GenericFileException;

export namespace Cardinal::IO::FS
{
	/// <summary>
	/// Trying to read more than is in file exception
	/// </summary>
	class CantReadMoreDataThanInExistInFileException : public GenericFileException {
	public:
		ForceInline CantReadMoreDataThanInExistInFileException(const CantReadMoreDataThanInExistInFileException& orig) : GenericFileException(orig) {}
		ForceInline CantReadMoreDataThanInExistInFileException(CantReadMoreDataThanInExistInFileException&& from) : GenericFileException(MoveRValue(from)) {}
		ForceInline CantReadMoreDataThanInExistInFileException& operator=(const CantReadMoreDataThanInExistInFileException& from) { this->~CantReadMoreDataThanInExistInFileException(); this->CantReadMoreDataThanInExistInFileException::CantReadMoreDataThanInExistInFileException(from); return *this; }
		ForceInline CantReadMoreDataThanInExistInFileException& operator=(CantReadMoreDataThanInExistInFileException&& from) { this->~CantReadMoreDataThanInExistInFileException(); this->CantReadMoreDataThanInExistInFileException::CantReadMoreDataThanInExistInFileException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// Exception generation
		/// </summary>
		CantReadMoreDataThanInExistInFileException(SourceLocation sourceLocation, ExceptionString&& fileName)
			: GenericFileException(sourceLocation, MoveRValue(fileName), L"Unable to read more than is in file") {}

	public:
		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new CantReadMoreDataThanInExistInFileException(*this));
		}

	public:
		ForceInline virtual ~CantReadMoreDataThanInExistInFileException() override {}
	};
}