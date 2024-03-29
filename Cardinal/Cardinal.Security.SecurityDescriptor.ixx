export module Cardinal.Security.SecurityDescriptor;

export import :Base;
export import :Sid;
export import :Ace;
export import :MandatoryLabel;
export import :WellKnownSids;

export import :ProcessRights;

export namespace Cardinal::Security
{
	/// <summary>
	/// Security descriptor object interface
	/// </summary>
	VirtualClass ISecurityDescriptor{
	protected:
		SensitiveUniquePtr<void> GenerateNtDescriptorImpl(
			const Sid & owner,
			const Sid & ownerGroup,
			Containers::Array<GenericAce> && dacl,
			Containers::Array<GenericAce> && sacl,
			Functionality::Optional<IntegrityLevel>&level);

	public:
		virtual ~ISecurityDescriptor() = 0 {}
	};

	template<typename T>
	class SecurityDescriptorBuilder;

	/// <summary>
	/// Security descriptor object creation
	/// </summary>
	/// <typeparam name="T">The enumeration type of possible rights on an object</typeparam>
	template<typename T>
	class SecurityDescriptor final :
		public IDisposable,
		protected Memory::Allocator<Memory::SensitiveDataHeap>,
		protected ISecurityDescriptor {
	private:
		struct Data {
			Sid owner;
			Sid ownerGroup;
			Containers::Array<Ace<T>> dacl;
			Containers::Array<Ace<T>> sacl;
			Functionality::Optional<IntegrityLevel> mandatoryLabel;
			SensitiveUniquePtr<void> handle;
			RefCountT RefCount;

			Data(
				Sid&& owner,
				Sid&& ownerGroup,
				Containers::Array<Ace<T>>&& dacl,
				Containers::Array<Ace<T>>&& sacl,
				IntegrityLevel label
			) :
				owner(MoveRValue(owner)),
				ownerGroup(MoveRValue(ownerGroup)),
				dacl(MoveRValue(dacl)),
				sacl(MoveRValue(sacl)),
				mandatoryLabel(label),
				handle(),
				RefCount(1) {
			}
		};

	private:
		Data* This;

	protected:
		void Dispose() override {}

	public:
		/// <summary>
		/// Security descriptor creation constructor
		/// </summary>
		/// <param name="owner">Owner</param>
		/// <param name="ownerGroup">Group of owners</param>
		/// <param name="dacl">Discretionary Access Control List</param>
		/// <param name="sacl">System Access Control List</param>
		/// <param name="label">Integrity level</param>
		SecurityDescriptor(
			Sid&& owner,
			Sid&& ownerGroup,
			Containers::Array<Ace<T>>&& dacl,
			Containers::Array<Ace<T>>&& sacl,
			IntegrityLevel label
		) :
			This(new Data(
				MoveRValue(owner),
				MoveRValue(ownerGroup),
				MoveRValue(dacl),
				MoveRValue(sacl),
				label)
			) {
			This->handle = GenerateNtDescriptorImpl(
				This->owner,
				This->ownerGroup,
				This->dacl.CopyTo<Containers::Array>(ArrowFunct(const& x, GenericAce(x))),
				This->sacl.CopyTo<Containers::Array>(ArrowFunct(const& x, GenericAce(x))),
				This->mandatoryLabel);
		}

	public:
		RefCountClass(SecurityDescriptor, This);
	};

	/// <summary>
	/// Security descriptor object constructor
	/// </summary>
	/// <typeparam name="T">The enumeration type of possible rights on an object</typeparam>
	template<typename T>
	class SecurityDescriptorBuilder {
	private:
		Sid owner;
		Sid ownerGroup;
		Containers::Array<Ace<T>> dacl;
		Containers::Array<Ace<T>> sacl;
		Functionality::Optional<IntegrityLevel> mandatoryLabel;

	public:
		/// <summary>
		/// An empty constructor
		/// </summary>
		SecurityDescriptorBuilder() = default;

		/// <summary>
		/// Moving constructor
		/// </summary>
		/// <param name="from">Original object</param>
		SecurityDescriptorBuilder(SecurityDescriptorBuilder&& from) :
			owner(MoveRValue(from.owner)),
			ownerGroup(MoveRValue(from.ownerGroup)),
			dacl(MoveRValue(from.dacl)),
			sacl(MoveRValue(from.sacl)),
			mandatoryLabel(MoveRValue(from.mandatoryLabel)) {
		}

	public:
		SecurityDescriptorBuilder(const SecurityDescriptorBuilder&) = delete;
		SecurityDescriptorBuilder& operator=(const SecurityDescriptorBuilder&) = delete;

	public:
		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="from">Original object</param>
		SecurityDescriptorBuilder& operator=(SecurityDescriptorBuilder&& from) {
			this->~SecurityDescriptorBuilder();
			this->SecurityDescriptorBuilder::SecurityDescriptorBuilder(MoveRValue(from));
			return *this;
		}

	public:
		/// <summary>
		/// Owner name setting
		/// </summary>
		/// <param name="owner">"RValue" reference to owner</param>
		/// <returns>"RValue" reference to an object</returns>
		SecurityDescriptorBuilder&& SetOwner(Sid&& owner)&& {
			this->owner = MoveRValue(owner);
			return MoveRValue(*this);
		}
		/// <summary>
		/// Owner name setting
		/// </summary>
		/// <param name="owner">"LValue" reference to owner</param>
		/// <returns>"RValue" reference to an object</returns>
		SecurityDescriptorBuilder&& SetOwner(const Sid& owner)&& {
			this->owner = owner;
			return MoveRValue(*this);
		}

		/// <summary>
		/// Owner group name setting
		/// </summary>
		/// <param name="ownerGroup">"RValue" reference to owner group</param>
		/// <returns>"RValue" reference to an object</returns>
		SecurityDescriptorBuilder&& SetOwnerGroup(Sid&& ownerGroup)&& {
			this->ownerGroup = MoveRValue(ownerGroup);
			return MoveRValue(*this);
		}

		/// <summary>
		/// Owner group name setting
		/// </summary>
		/// <param name="ownerGroup">"RValue" reference to owner group</param>
		/// <returns>"RValue" reference to an object</returns>
		SecurityDescriptorBuilder&& SetOwnerGroup(const Sid& ownerGroup)&& {
			this->ownerGroup = ownerGroup;
			return MoveRValue(*this);
		}

		/// <summary>
		/// DACL setting
		/// </summary>
		/// <param name="dacl">"RValue" reference to DACL</param>
		/// <returns>"RValue" reference to an object</returns>
		SecurityDescriptorBuilder&& SetDacl(Containers::Array<Ace<T>>&& dacl)&& {
			this->dacl = MoveRValue(dacl);
			return MoveRValue(*this);
		}

		/// <summary>
		/// DACL setting
		/// </summary>
		/// <param name="dacl">"LValue" reference to DACL</param>
		/// <returns>"RValue" reference to an object</returns>
		SecurityDescriptorBuilder&& SetDacl(const Containers::Array<Ace<T>>& dacl)&& {
			this->dacl = dacl;
			return MoveRValue(*this);
		}

		/// <summary>
		/// SACL setting
		/// </summary>
		/// <param name="sacl">"RValue" reference to SACL</param>
		/// <returns>"RValue" reference to an object</returns>
		SecurityDescriptorBuilder&& SetSacl(Containers::Array<Ace<T>>&& sacl)&& {
			this->sacl = MoveRValue(sacl);
			return MoveRValue(*this);
		}

		/// <summary>
		/// SACL setting
		/// </summary>
		/// <param name="sacl">"LValue" reference to SACL</param>
		/// <returns>"RValue" reference to an object</returns>
		SecurityDescriptorBuilder&& SetSacl(const Containers::Array<Ace<T>>& sacl)&& {
			this->sacl = sacl;
			return MoveRValue(*this);
		}

		/// <summary>
		/// Integrity level setting
		/// </summary>
		/// <param name="mandatoryLabel">"RValue" reference to integrity level</param>
		/// <returns>"RValue" reference to an object</returns>
		SecurityDescriptorBuilder&& SetMandatoryLabel(
			Functionality::Optional<IntegrityLevel>&& mandatoryLabel)&& {
			this->mandatoryLabel = MoveRValue(mandatoryLabel);
			return MoveRValue(*this);
		}

		/// <summary>
		/// Integrity level setting
		/// </summary>
		/// <param name="mandatoryLabel">"LValue" reference to integrity level</param>
		/// <returns>"RValue" reference to an object</returns>
		SecurityDescriptorBuilder&& SetMandatoryLabel(
			const Functionality::Optional<IntegrityLevel>& mandatoryLabel)&& {
			this->mandatoryLabel = mandatoryLabel;
			return MoveRValue(*this);
		}

	public:
		/// <summary>
		/// Security descriptor creation
		/// </summary>
		/// <returns>Security descriptor</returns>
		SecurityDescriptor<T> Build()&& {
			if (!this->owner.IsValid && ownerGroup.IsValid)
				Throw(NullPointException(SourcePosition));

			return SecurityDescriptor<T>(
				new typename SecurityDescriptor<T>::Data(
					MoveRValue(owner),
					MoveRValue(ownerGroup),
					MoveRValue(dacl),
					MoveRValue(sacl),
					MoveRValue(mandatoryLabel)));
		}
	};
}