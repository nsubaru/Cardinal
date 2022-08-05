using Cardinal.CTTI;

namespace Cardinal.FS;

/// <summary>
/// Volume attributes
/// </summary>
[SerializableStruct("Cardinal::FS::VolumeAttributes")]
public struct VolumeAttributes
{
	/// <summary>
	/// Volume flags
	/// </summary>
	[SerializableStructField("Flags", "Cardinal::SizeT")]
	public UInt64 Flags { get; set; }
};
