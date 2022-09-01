using System.Runtime.InteropServices;
using System.Text;

namespace Cardinal.Launcher.API;

/// <summary>
/// KMS message header
/// </summary>
[StructLayout(LayoutKind.Explicit)]
public struct KmsMessageHeader
{
    /// <summary>
    /// Message type
    /// </summary>
    [FieldOffset(0)]
    private EMessageType messageType;

    /// <summary>
    /// Command type
    /// </summary>
    [FieldOffset(1)]
    private ECommandType commandType;

    /// <summary>
    /// Datagram type
    /// </summary>
    [FieldOffset(2)]
    private EDatagramType datagramType;

    /// <summary>
    /// Log message level
    /// </summary>
    [FieldOffset(3)]
    private ELogLevel logLevel;

    /// <summary>
    /// Message type
    /// </summary>
    public EMessageType MessageType
    {
        get { return messageType; }
        set { messageType = value; }
    }

    /// <summary>
    /// Command type
    /// </summary>
    public ECommandType CommandType
    {
        get { return commandType; }
        set { commandType = value; }
    }

    /// <summary>
    /// Datagram type
    /// </summary>
    public EDatagramType DatagramType
    {
        get { return datagramType; }
        set { datagramType = value; }
    }

    /// <summary>
    /// Log message level
    /// </summary>
    public ELogLevel LogLevel
    {
        get { return logLevel; }
        set { logLevel = value; }
    }

    public KmsMessageHeader(EMessageType messageType, ECommandType commandType, EDatagramType datagramType, ELogLevel logLevel)
    {
        this.messageType = messageType;
        this.commandType = commandType;
        this.datagramType = datagramType;
        this.logLevel = logLevel;
    }

    public KmsMessageHeader(byte[] data)
    {
        messageType = (EMessageType)data[0];
        commandType = (ECommandType)data[1];
        datagramType = (EDatagramType)data[2];
        logLevel = (ELogLevel)data[3];
    }

    public KmsMessageHeader(string message)
    {
        byte[] data = Encoding.Default.GetBytes(message);
        messageType = (EMessageType)data[0];
        commandType = (ECommandType)data[1];
        datagramType = (EDatagramType)data[2];
        logLevel = (ELogLevel)data[3];
    }

    /// <summary>
    /// Convert structure to byte array
    /// </summary>
    /// <returns></returns>
    public byte[] ToBytes()
    {
        var size = Marshal.SizeOf(this);
        byte[] bytes = new byte[size];

        IntPtr Ptr = Marshal.AllocHGlobal(size);
        Marshal.StructureToPtr(this, Ptr, true);
        Marshal.Copy(Ptr, bytes, 0, size);
        Marshal.FreeHGlobal(Ptr);

        return bytes;
    }
}