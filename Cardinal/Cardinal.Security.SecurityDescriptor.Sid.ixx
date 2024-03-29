export module Cardinal.Security.SecurityDescriptor:Sid;

import :Base;
import :WellKnownSids;

export namespace Cardinal::Security
{
	/// <summary>
	/// Security identifier object creation
	/// </summary>
	class Sid final : public IDisposable, protected Memory::Allocator<Memory::SensitiveDataHeap> {
	public:
		struct IdentifierAuthority {
			union {
				Byte data[6];
				SizeT value;
			};

			/// <summary>
			/// An empty constructor
			/// </summary>
			IdentifierAuthority() : value(0) {}

			/// <summary>
			/// Authority identifier constructor
			/// </summary>
			/// <param name="value">Authority identifier value</param>
			IdentifierAuthority(SizeT value) : value(value) {}
		};

	private:
		struct Data {
			IdentifierAuthority identifierAuthority;
			Containers::SensitiveArray<UInt64> value;
			RefCountT RefCount;

			Data(
				const Containers::SensitiveArray<UInt64>& value,
				IdentifierAuthority identifierAuthority
			) : identifierAuthority(identifierAuthority),
				value(value),
				RefCount(1) {
			}

			Data(
				Containers::SensitiveArray<UInt64>&& value,
				IdentifierAuthority identifierAuthority
			) : identifierAuthority(identifierAuthority),
				value(MoveRValue(value)),
				RefCount(1) {
			}
		};

	private:
		Data* This;

	protected:
		void Dispose() override {}

	public:
		/// <summary>
		/// An empty constructor
		/// </summary>
		Sid() : This(nullptr) {}

		/// <summary>
		/// SID creation constructor
		/// </summary>
		/// <param name="identifierAuthority">Authority level</param>
		/// <param name="value">Array of subidentifiers</param>
		Sid(
			IdentifierAuthority identifierAuthority,
			const Containers::SensitiveArray<UInt64>& value
		) :
			This(new Data(value, identifierAuthority)) {
		}

		/// <summary>
		/// SID creation constructor
		/// </summary>
		/// <param name="identifierAuthority">Authority level</param>
		/// <param name="value">"RValue" reference to array of subidentifiers</param>
		Sid(
			IdentifierAuthority identifierAuthority,
			Containers::SensitiveArray<UInt64>&& value
		) :
			This(new Data(MoveRValue(value), identifierAuthority)) {
		}

	public:
		/// <summary>
		/// Returns the SID object
		/// </summary>
		bool GetIsValid() const { return This != nullptr; }

		/// <summary>
		/// Returns the constant reference in aary of subidentifiers object
		/// </summary>
		const Containers::SensitiveArray<UInt64>& GetValue() const {
			if (!IsValid)
			{
				Throw(NullPointException(SourcePosition));
			}
			return This->value;
		}

		/// <summary>
		/// Returns the authority level object
		/// </summary>
		IdentifierAuthority GetAuthority() const {
			if (!IsValid)
			{
				Throw(NullPointException(SourcePosition));
			}
			return This->identifierAuthority;
		}

	public:
		/// <summary>
		/// SID state
		/// </summary>
		bool Property(GetIsValid) IsValid;

		/// <summary>
		/// Array of subidentifiers
		/// </summary>
		Containers::SensitiveArray<UInt64> Property(GetValue) Value;

		/// <summary>
		/// SID
		/// </summary>
		IdentifierAuthority Property(GetAuthority) Authority;

	public:
		/// <summary>
		/// String convertion into a SID
		/// </summary>
		/// <param name="sid">SID</param>
		/// <returns>SID</returns>
		static Sid ConvertStringToSid(const SensitiveString& sid) {
			if (sid[0] != L'S')
				Throw(InvalidFormatException(SourcePosition));

			auto items = sid.Split(L'-', 1);
			auto authority = items.GetRoot()->toInt<UInt64>();
			items.GetRoot().Remove();
			auto arr = items.CopyTo<Containers::SensitiveArray>(
				[] (const SensitiveString& item) { return item.toInt<UInt64>(); });

			return Sid(authority, MoveRValue(arr));
		}

	public:
		RefCountClass(Sid, This);
	};
}