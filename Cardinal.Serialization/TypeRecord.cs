using Cardinal.RTTI.Record;

namespace Cardinal.Serialization;

/// <summary>
/// Type record
/// </summary>
public struct TypeRecord
{
    /// <summary>
    /// Type name
    /// </summary>
    public string TypeName { get; set; }

    /// <summary>
    /// Type serialization schema
    /// </summary>
    public string SerializationSchema { get; set; }

    /// <summary>
    /// .Net CLR type
    /// </summary>
    public Type ClrType { get; set; }

    /// <summary>
    /// Type class in the Cardinal kernel
    /// </summary>
    public CTTI.ETypes Type { get; set; }

    /// <summary>
    /// Full type Id
    /// </summary>
    public Guid FullTypeId { get; set; }

    /// <summary>
    /// Rtti record of type
    /// </summary>
    public RttiTypeRecord RttiType { get; set; }
}