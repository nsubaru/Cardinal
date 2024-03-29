export module Cardinal.Exceptions:KeyAllreadyExistException;

import :Base;
import :AllreadyExistException;

export namespace Cardinal
{
	/// <summary>
	/// A key already exists exception
	/// </summary>
	class KeyAllreadyExistException : public AllreadyExistException {
	public:
		ForceInline KeyAllreadyExistException(const KeyAllreadyExistException& orig) : AllreadyExistException(orig) {}
		ForceInline KeyAllreadyExistException(KeyAllreadyExistException&& from) : AllreadyExistException(MoveRValue(from)) {}
		ForceInline KeyAllreadyExistException& operator=(const KeyAllreadyExistException& from) { this->~KeyAllreadyExistException(); this->KeyAllreadyExistException::KeyAllreadyExistException(from); return *this; }
		ForceInline KeyAllreadyExistException& operator=(KeyAllreadyExistException&& from) { this->~KeyAllreadyExistException(); this->KeyAllreadyExistException::KeyAllreadyExistException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'FunctName'>The function name</param>
		KeyAllreadyExistException(SourceLocation sourceLocation)
			:AllreadyExistException(sourceLocation, L"A key already exists") {
		}

	public:
		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>A "smart" pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new KeyAllreadyExistException(*this));
		}

	public:
		ForceInline virtual ~KeyAllreadyExistException() {}
	};
}