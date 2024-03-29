export module Cardinal.RTTI:Section.Base;

export import :Record;

export namespace Cardinal::RTTI
{
	/// <summary>
	/// Magic number for rtti section
	/// </summary>
	constexpr Byte SectionMagicNumber[] = "CARDINAL_RXX_RTTI";

#pragma pack(push, 1)
	/// <summary>
	/// Header of rtti section
	/// </summary>
	struct SectionHeaderType {
		/// <summary>
		/// Magic number of header same as section magic number
		/// </summary>
		Byte SectionMagicNumber[sizeof(SectionMagicNumber)];

		/// <summary>
		/// Version of kernel with which section was created
		/// </summary>
		Settings::KernelVersionT KernelVersion;

		/// <summary>
		/// Absolut address of section start when they was created
		/// </summary>
		RawMemBlk OriginHeaderAddr;

		/// <summary>
		/// Start of pointer subsection
		/// </summary>
		RawMemBlk FaAddr;

		/// <summary>
		/// End of pointer subsection
		/// </summary>
		RawMemBlk FzAddr;

		/// <summary>
		/// Start of variable subsection
		/// </summary>
		RawMemBlk VaAddr;

		/// <summary>
		/// End of variable subsection
		/// </summary>
		RawMemBlk VzAddr;

		/// <summary>
		/// ExecutableImageBase
		/// </summary>
		RawMemBlk ImageBase;
	};
#pragma pack(pop)
}