using Cardinal.CTTI;

namespace Cardinal.Time;

/// <summary>
/// Time class, optimized for quick arithmetic
/// </summary>
[SerializableStruct("Cardinal::Time::DateTime")]
public struct DateTime
{
    [SerializableStructField("Ticks", "Cardinal::SizeT")]
    public UInt64 Ticks { get; set; }
}
