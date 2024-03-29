export module Cardinal.Security.SecurityDescriptor:MandatoryLabel;

import :Base;
import :Ace;

export namespace Cardinal::Security
{
	/// <summary>
	/// Integrity level mask
	/// </summary>
	enum EIntegrityLevelMask {
		NoWriteUp = 0b0001,
		NoReadUp = 0b0010,
		NoExecuteUp = 0b0100
	};

	using IntegrityLevelMask = EnumFlags<EIntegrityLevelMask>;

	/// <summary>
	/// Mandatory marking of integrity level
	/// </summary>
	enum class EMandatoryLabel : UInt16 {
		Untrusted = 0x0000,
		Low = 0x1000,
		Medium = 0x2000,
		MediumPlus = 0x2100,
		High = 0x3000,
		System = 0x4000,
		Protected = 0x5000,
		SecureProcess = 0x7000
	};

	EnumCttiDefinition(EMandatoryLabel,
		EMandatoryLabel::Untrusted,
		EMandatoryLabel::Low,
		EMandatoryLabel::Medium,
		EMandatoryLabel::MediumPlus,
		EMandatoryLabel::High,
		EMandatoryLabel::System,
		EMandatoryLabel::Protected,
		EMandatoryLabel::SecureProcess
	);

	/// <summary>
	/// Integrity level object creation
	/// </summary>
	class IntegrityLevel {
	private:
		UInt16 level;
		IntegrityLevelMask mask;
		EnumFlags<EAceInherits> inherits;

	public:
		/// <summary>
		/// Integrity level object creation constructor
		/// </summary>
		/// <param name="level">Integrity level</param>
		/// <param name="mask">Integrity level mask</param>
		/// <param name="inherits">Integrity level inheritance</param>
		IntegrityLevel(UInt16 level, IntegrityLevelMask mask, EnumFlags<EAceInherits> inherits)
			: level(level), mask(mask), inherits(inherits) {}

		/// <summary>
		/// Integrity level object creation constructor of a certain value
		/// </summary>
		/// <param name="level">Integrity level type</param>
		/// <param name="mask">Integrity level mask</param>
		/// <param name="inherits">Integrity level inheritance</param>
		IntegrityLevel(EMandatoryLabel level, IntegrityLevelMask mask, EnumFlags<EAceInherits> inherits)
			: level(static_cast<UInt16>(level)), mask(mask), inherits(inherits) {}

	public:
		/// <summary>
		/// Returns the integrity level object
		/// </summary>
		UInt16 GetValue() const { return level; }

		/// <summary>
		/// Returns the integrity level mask object
		/// </summary>
		IntegrityLevelMask GetMask() const { return mask; }

		/// <summary>
		/// Returns the integrity level inheritance object
		/// </summary>
		EnumFlags<EAceInherits> GetInherits() const { return inherits; }

	public:
		/// <summary>
		/// Integrity level
		/// </summary>
		UInt16 Property(GetValue) Level;

		/// <summary>
		/// Integrity level mask
		/// </summary>
		IntegrityLevelMask Property(GetMask) Mask;

		/// <summary>
		/// Integrity level inheritance
		/// </summary>
		EnumFlags<EAceInherits> Property(GetInherits) Inherits;
	};
}