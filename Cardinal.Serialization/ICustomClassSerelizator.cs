namespace Cardinal.Serialization;

/// <summary>
/// Interface for custom class serelization/deserelization.
/// </summary>
public interface ICustomClassSerelizator
{
    /// <summary>
    /// Custom class .Net CLR type
    /// </summary>
    Type ClrType { get; set; }

    /// <summary>
    /// Custom class name in the Cardinal kernel
    /// </summary>
    string TypeName { get; set; }

    /// <summary>
    /// Full type Id
    /// </summary>
    Guid FullTypeId { get; set; }

    /// <summary>
    /// Serializes data into binnary string
    /// </summary>
    /// <param name="data">Data to serialize</param>
    /// <param name="serializationStream">Target stream</param>
    void Serialize(dynamic data, Stream serializationStream);

    /// <summary>
    /// Deserializes data from binnary string
    /// </summary>
    /// <param name="serializationStream">Data stream</param>
    /// <returns>Deserialized object</returns>
    object Deserialize(Stream serializationStream);
}

