namespace Cardinal.Serialization;

/// <summary>
/// Cardinal serializator interface
/// </summary>
public interface ICardinalSerializator
{
    /// <summary>
    /// Serializes data into binnary string
    /// </summary>
    /// <param name="data">Data to serialize</param>
    /// <param name="serializationStream">Target stream</param>
    public void Serialize(dynamic data, Stream serializationStream);

    /// <summary>
    /// Deserializes data from binnary string
    /// </summary>
    /// <param name="data">Deserialized data target container</param>
    /// <param name="serializationStream">Data stream</param>
    public void Deserialize(ref dynamic data, Stream serializationStream);

    /// <summary>
    /// Returns list of supprted types
    /// </summary>
    /// <returns>List of types</returns>
    public List<TypeRecord> GetSupportedTypes();
}
