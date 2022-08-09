using Cardinal.CTTI;

namespace Cardinal.FS;

/// <summary>
/// Volume information
/// </summary>
[SerializableStruct("Cardinal::FS::VolumeInformation")]
public struct VolumeInformation
{
	/// <summary>
	/// Serial number
	/// </summary>
	[SerializableStructField("SerialNumber", "Cardinal::UInt32")]
	public UInt32 SerialNumber { get; set; }

	/// <summary>
	/// Creation time
	/// </summary>
	[SerializableStructField("CreationTime", "Cardinal::Time::DateTime")]
	public Time.DateTime CreationTime { get; set; }

	/// <summary>
	/// Volume mark
	/// </summary>
	[SerializableStructField("Label", "Cardinal::String")]
	public String Label { get; set; }

	/// <summary>
	/// File system name
	/// </summary>
	[SerializableStructField("FsName", "Cardinal::String")]
	public String FsName { get; set; }

	/// <summary>
	/// Volume size
	/// </summary>
	[SerializableStructField("Size", "Cardinal::FS::VolumeSize")]
	public VolumeSize Size { get; set; }

	/// <summary>
	/// Volume attributes
	/// </summary>
	[SerializableStructField("Attributes", "Cardinal::FS::VolumeAttributes")]
	public VolumeAttributes Attributes { get; set; }
}
