export module Cardinal.Security.SecurityDescriptor:Ace;

import :Base;
import :Sid;

export namespace Cardinal::Security
{
	/// <summary>
	/// Access control type record
	/// </summary>
	enum class EAceType : UInt8 {
		Allow, Deny, Invalid
	};

	/// <summary>
	/// Access control record inherit
	/// </summary>
	enum class EAceInherits : UInt8 {
		ObjectInheritAce = 0x1,
		ContainerInheritAce = 0x2,
		NoPropagateInheritAce = 0x4,
		InheritOnlyAce = 0x8,
		InheritedAce = 0x10,
		ValidInheritFlags = 0x1F,
		Invalid
	};

	template<typename T>
	class AceBuilder;
			
	/// <summary>
	/// Access control record object creation
	/// </summary>
	/// <typeparam name="T">The enumeration type of possible rights on an object</typeparam>
	template<typename T>
	class Ace final : public IDisposable, public Memory::Allocator<Memory::SensitiveDataHeap> {
	public:
		using Rights = AccessRights<T>;

	private:
		struct Data {
			Sid sid;
			EAceType type;
			EnumFlags<EAceInherits> inherits;
			Rights rights;
			RefCountT RefCount;

			Data(Sid&& sid, EAceType type, EnumFlags<EAceInherits> inherits, Rights rights) :
				sid(MoveRValue(sid)), type(type), inherits(inherits), rights(rights), RefCount(1) {}
		};

	private:
		Data* This;

	private:
		Ace(Data* This) : This(This) {}

	protected:
		void Dispose() override {}

	public:
		/// <summary>
		/// An empty constructor
		/// </summary>
		/// <typeparam name="T">The enumeration type of possible rights on an object</typeparam>
		Ace() : This(nullptr) {}

	public:
		/// <summary>
		/// Set of rights addition operator to record
		/// </summary>
		Ace& operator|=(Rights rights) {
			This->rights |= rights;
			return *this;
		}

		/// <summary>
		/// Rights addition operator to record
		/// </summary>
		Ace& operator|=(T right) {
			This->rights |= right;
			return *this;
		}

	public:
		/// <summary>
		/// Returns the security identifier object
		/// </summary>
		const Sid& GetAceSid() const { return This->sid; }

		/// <summary>
		/// Returns the type object of access control record
		/// </summary>
		EAceType GetAceType() const { return This->type; }

		/// <summary>
		/// Returns the rights object of access control record
		/// </summary>
		Rights GetAceRights() const { return This->rights; }

		/// <summary>
		/// Returns the inherits object of access control record
		/// </summary>
		EnumFlags<EAceInherits> GetAceInherits() const { return This->inherits; }

	public:
		/// <summary>
		/// Subject security identifier
		/// </summary>
		Sid Property(GetAceSid) AceSid;

		/// <summary>
		/// Access control type record
		/// </summary>
		EAceType Property(GetAceType) AceType;

		/// <summary>
		/// Access control rights type
		/// </summary>
		Rights Property(GetAceRights) AceRights;

		/// <summary>
		/// Access control rights type
		/// </summary>
		EnumFlags<EAceInherits> Property(GetAceInherits) AceInherits;

	public:
		RefCountClass(Ace, This);

	public:
		template<typename T>
		friend class AceBuilder;
	};

	/// <summary>
	/// Constructor of access control record object
	/// </summary>
	/// <typeparam name="T">The enumeration type of possible rights on an object</typeparam>
	template<typename T>
	class AceBuilder {
	private:
		Sid sid;
		EAceType type;
		EnumFlags<EAceInherits> inherits;
		AccessRights<T> rights;

	public:
		/// <summary>
		/// An empty constructor
		/// </summary>
		AceBuilder() = default;

		/// <summary>
		/// Moving constructor
		/// </summary>
		/// <param name="from">Original object</param>
		AceBuilder(AceBuilder&& from) : 
			sid(MoveRValue(from.sid)), 
			type(from.type),
			inherits(from.inherits),
			rights(from.rights)
		{}

	public:
		AceBuilder(const AceBuilder&) = delete;
		AceBuilder& operator=(const AceBuilder&) = delete;

	public:
		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="from">Original object</param>
		AceBuilder& operator=(AceBuilder&& from) {
			this->~AceBuilder();
			this->AceBuilder::AceBuilder(MoveRValue(from));
			return *this;
		}

	public:
		/// <summary>
		/// Subject security identifier setting
		/// </summary>
		/// <param name="sid">"RValue" reference to a security identifier</param>
		/// <returns>"RValue" reference to an object</returns>
		AceBuilder&& SetSid(Sid&& sid)&& {
			this->sid = MoveRValue(sid);
			return MoveRValue(*this);
		}

		/// <summary>
		/// Subject security identifier setting
		/// </summary>
		/// <param name="sid">"LValue" reference to a security identifier</param>
		/// <returns>"RValue" reference to an object</returns>
		AceBuilder&& SetSid(const Sid& sid)&& {
			this->sid = sid;
			return MoveRValue(*this);
		}

		/// <summary>
		/// Access control of type record setting
		/// </summary>
		/// <param name="type">Access control type record</param>
		/// <returns>"RValue" reference to an object</returns>
		AceBuilder&& SetType(EAceType type)&& {
			this->type = type;
			return MoveRValue(*this);
		}

		/// <summary>
		/// Access control of rights record setting
		/// </summary>
		/// <param name="rights">Access control of rights record</param>
		/// <returns>"RValue" reference to an object</returns>
		AceBuilder&& SetRights(AccessRights<T> rights)&& {
			this->rights = rights;
			return MoveRValue(*this);
		}

		/// <summary>
		/// Access control inherit record setting
		/// </summary>
		/// <param name="inherits">Access control record inherit</param>
		/// <returns>"RValue" reference to an object</returns>
		AceBuilder&& SetInherits(EnumFlags<EAceInherits> inherits)&& {
			this->inherits = inherits;
			return MoveRValue(*this);
		}

	public:
		/// <summary>
		/// Access control record creation
		/// </summary>
		Ace<T> Build()&& {
			if (!this->sid.IsValid)
				Throw(NullPointException(SourcePosition));

			return Ace<T>(new typename Ace<T>::Data(MoveRValue(sid), type, inherits, rights));
		}
	};

	/// <summary>
	/// General access control record object creation
	/// </summary>
	class GenericAce final {
	private:
		Sid sid;
		EAceType type;
		UInt64 rights;
		EnumFlags<EAceInherits> inherits;

	public:
		/// <summary>
		/// An empty constructor, initializes the invalid record
		/// </summary>
		GenericAce() : sid(),
			type(EAceType::Invalid),
			rights(0),
			inherits(EnumFlags<EAceInherits>(EAceInherits::Invalid)){}

		/// <summary>
		/// A constructor with typed access control record
		/// </summary>
		/// <typeparam name="T">The enumeration type of rights</typeparam>
		template<typename T> 
		GenericAce(const Ace<T>& ace) :
			sid(ace.AceSid),
			type(ace.AceType),
			rights(ace.AceRights.Value) {}

	public:
		/// <summary>
		/// Returns the object of general security identifier
		/// </summary>
		const Sid& GetGenericAceSid() const { return this->sid; }

		/// <summary>
		/// Returns the type object of general security identifier
		/// </summary>
		EAceType GetGenericAceType() const { return this->type; }

		/// <summary>
		/// Returns the rights object of general security identifier
		/// </summary>
		UInt64 GetGenericAceRights() const { return this->rights; }

		/// <summary>
		/// Returns the inherits object of general security identifier
		/// </summary>
		EnumFlags<EAceInherits> GetAceInherits() const { return this->inherits; }

	public:
		/// <summary>
		/// The object of general security identifier
		/// </summary>
		Sid Property(GetGenericAceSid) GenericAceSid;

		/// <summary>
		/// The type object of general security identifier
		/// </summary>
		EAceType Property(GetGenericAceType) GenericAceType;

		/// <summary>
		/// The rights object of generic access control record
		/// </summary>
		UInt64 Property(GetGenericAceRights) GenericAceRights;

		/// <summary>
		/// The inherits object of general security identifier
		/// </summary>
		EnumFlags<EAceInherits> Property(GetAceInherits) GenericAceInherits;
	};
}