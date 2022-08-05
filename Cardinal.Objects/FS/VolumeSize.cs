using Cardinal.CTTI;

namespace Cardinal.FS;

/// <summary>
/// Volume size information
/// </summary>
[SerializableStruct("Cardinal::FS::VolumeSize")]
public struct VolumeSize
{
	/// <summary>
	/// Number of clusters
	/// </summary>
	[SerializableStructField("TotalClusters", "Cardinal::SizeT")]
	public UInt64 TotalClusters { get; set; }
	/// <summary>
	/// Number of used clusters
	/// </summary>
	[SerializableStructField("AllocatedClusters", "Cardinal::SizeT")]
	public UInt64 AllocatedClusters { get; set; }
	/// <summary>
	/// Cluster size in bytes
	/// </summary>
	[SerializableStructField("ClusterSize", "Cardinal::SizeT")]
	public UInt64 ClusterSize { get; set; }
}
