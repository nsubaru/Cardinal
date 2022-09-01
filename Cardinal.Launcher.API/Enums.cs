namespace Cardinal.Launcher.API;

/// <summary>
/// Message type
/// </summary>
public enum EMessageType : byte
{
    Undefined,
    Command,
    Datagram
}

/// <summary>
/// Command type
/// </summary>
public enum ECommandType : byte
{
    Undefined,
    PasswordRequest,
    PasswordAnswer,
    ArgumentRequest,
    ArgumentAnswer,
    CommandRequest
};

/// <summary>
/// Datagram type
/// </summary>
public enum EDatagramType : byte
{
    Undefined,
    Info,
    Log,
    Warning,
    Error
};

/// <summary>
/// Log level
/// </summary>
public enum ELogLevel : byte
{
    Undefined,
    Debug,
    Verbose,
    Warning,
    Error,
    Critical
};