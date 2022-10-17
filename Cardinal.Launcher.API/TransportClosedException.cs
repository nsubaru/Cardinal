namespace Cardinal.Launcher.API;

/// <summary>
/// KMS transport closed exception
/// </summary>
public class TransportClosedException : Exception
{
    public TransportClosedException() : base() { }
    public TransportClosedException(string message) : base(message) { }
    public TransportClosedException(string message, System.Exception inner) : base(message, inner) { }
}
