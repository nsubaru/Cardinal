export module Cardinal.IO:FS.Exceptions.CantWriteDataAfterEofException;

export import :FS.Exceptions.Base;
export import :FS.Exceptions.GenericFileException;

export namespace Cardinal::IO::FS
{
	/// <summary>
	/// Attempt to write data after end of file exception
	/// </summary>
	class CantWriteDataAfterEofException : public GenericFileException {
	public:
		ForceInline CantWriteDataAfterEofException(const CantWriteDataAfterEofException& orig) : GenericFileException(orig) {}
		ForceInline CantWriteDataAfterEofException(CantWriteDataAfterEofException&& from) : GenericFileException(MoveRValue(from)) {}
		ForceInline CantWriteDataAfterEofException& operator=(const CantWriteDataAfterEofException& from) { this->~CantWriteDataAfterEofException(); this->CantWriteDataAfterEofException::CantWriteDataAfterEofException(from); return *this; }
		ForceInline CantWriteDataAfterEofException& operator=(CantWriteDataAfterEofException&& from) { this->~CantWriteDataAfterEofException(); this->CantWriteDataAfterEofException::CantWriteDataAfterEofException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// Exception generation
		/// </summary>
		CantWriteDataAfterEofException(SourceLocation sourceLocation, ExceptionString&& fileName)
			: GenericFileException(sourceLocation, MoveRValue(fileName), L"Unable to write data after end of file") {}

	public:
		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new CantWriteDataAfterEofException(*this));
		}

	public:
		ForceInline virtual ~CantWriteDataAfterEofException() override {}
	};
}