export module Cardinal.Exceptions:NullPointException;

import :Base;

export namespace Cardinal
{
	/// <summary>
	/// The memory block access exception
	/// <para>Can be three variants</para>
	/// <para> - Read	- an attempt to read at a non-existent address </para>
	/// <para> - Write	- an attempt to write at a non-existent address </para>
	/// <para> - Execute - an attempt to execute the command at a non-existent address </para>
	/// <para>Therefore the exception contains the information about access type and address which is tried to use</para>
	/// </summary>
	class NullPointException final : public Exception {
	public:
		/// <summary>
		/// Possible exception variants
		/// </summary>
		enum class Types : UInt8 { Read, Write, Execute };

	private:
		/// <summary>
		/// The address which was triet to use
		/// </summary>
		SizeT Addr;

		/// <summary>
		/// The access type which generates the exception
		/// </summary>
		Types Type;

	public:
		ForceInline NullPointException(const NullPointException& from) : Exception(from), Addr(from.Addr), Type(from.Type) {}
		ForceInline NullPointException(NullPointException&& from) : Exception(MoveRValue(from)), Addr(from.Addr), Type(from.Type) {}
		ForceInline NullPointException& operator=(const NullPointException& from) { this->~NullPointException(); this->NullPointException::NullPointException(from); return *this; }
		ForceInline NullPointException& operator=(NullPointException&& from) { this->~NullPointException(); this->NullPointException::NullPointException(MoveRValue(from)); return *this; }

	public:
		/// <summary>
		/// The exception constructor
		/// </summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		/// <param name = 'Addr'>The memory block address which generates the exception</param>
		/// <param mame = 'Type'>The access type to a block which generates the exception</param>
		NullPointException(SourceLocation sourceLocation, SizeT Addr, Types Type)
			:Exception(sourceLocation, L"The memory access error"), Addr(Addr), Type(Type) {
		}

		/// <summary>The exception constructor</summary>
		/// <param name = 'sourceLocation'>The file and line number in which the exception was generated</param>
		NullPointException(SourceLocation sourceLocation)
			:Exception(sourceLocation, L"The memory access error"), Addr(0x0), Type(Types::Read) {
		}

	public:
		/// <summary>
		/// The memory block address which generates the exception
		/// </summary>
		ForceInline SizeT GetMemoryAddress() const { return Addr; }

		/// <summary>
		/// The access type to a block which generates the exception
		/// </summary>
		ForceInline Types GetAccessType() const { return Type; }

	public:
		/// <summary>
		/// Text representation of exception information
		/// </summary>
		virtual ExceptionString toString() const override final {
			return ExceptionString::Combine({
				ExceptionString(exceptionName),
				ExceptionString(L" for the address "), ExceptionString::UInt64asHex(GetMemoryAddress()),
				ExceptionString(L" when trying "),
				((GetAccessType() == Types::Read) ? ExceptionString(L"read. ") :
				(GetAccessType() == Types::Write) ? ExceptionString(L"write. ")
													: ExceptionString(L"execution. ")),
				location.toString(),
				(innerException == nullptr) ? ExceptionString(L". ") : (L". Also there is a nested exception: " + InnerException->toString())
				});
		}

		/// <summary>
		/// Returns the copy of the exception object
		/// </summary>
		/// <returns>Smart pointer to an exception object</returns>
		ForceInline virtual ExceptionHolderType Clone() const override {
			return ExceptionHolderType(new NullPointException(*this));
		}

	public:
		ForceInline virtual ~NullPointException() {}
	};
}