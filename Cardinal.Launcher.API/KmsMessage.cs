using System.Runtime.InteropServices;
using System.Text;

namespace Cardinal.Launcher.API;

/// <summary>
/// KMS message structure
/// </summary>
[StructLayout(LayoutKind.Sequential)]
public struct KmsMessage
{
    /// <summary>
    /// Message header
    /// </summary>
    private KmsMessageHeader messageHeader;

    /// <summary>
    /// Mesage data
    /// </summary>
    public byte[] data;

    /// <summary>
    /// Message type
    /// </summary>
    public EMessageType MessageType
    {
        get { return messageHeader.MessageType; }
        set { messageHeader.MessageType = value; }
    }

    /// <summary>
    /// Command type
    /// </summary>
    public ECommandType CommandType
    {
        get { return messageHeader.CommandType; }
        set { messageHeader.CommandType = value; }
    }

    /// <summary>
    /// Datagram type
    /// </summary>
    public EDatagramType DatagramType
    {
        get { return messageHeader.DatagramType; }
        set { messageHeader.DatagramType = value; }
    }

    /// <summary>
    /// Log message level
    /// </summary>
    public ELogLevel LogLevel
    {
        get { return messageHeader.LogLevel; }
        set { messageHeader.LogLevel = value; }
    }

    /// <summary>
    /// Message data
    /// </summary>
    public byte[] Data
    {
        get { return data; }
        set { data = value; }
    }

    public KmsMessage(byte[] data)
    {
        messageHeader = new KmsMessageHeader(data);

        this.data = data;
    }

    public KmsMessage(string message)
    {
        var fullData = Encoding.Unicode.GetBytes(message);

        messageHeader = new KmsMessageHeader(fullData);

        var data = new byte[fullData.Length - Marshal.SizeOf(messageHeader)];
        fullData.CopyTo(data, Marshal.SizeOf(messageHeader));

        this.data = data;
    }

    public KmsMessage(KmsMessageHeader messageHeader, byte[] data)
    {
        this.messageHeader = messageHeader;

        this.data = data;
    }

    public KmsMessage(KmsMessageHeader messageHeader, string message)
    {
        this.messageHeader = messageHeader;

        data = Encoding.Unicode.GetBytes(message);
    }

    public KmsMessageHeader GetMessageHeader()
    {
        return messageHeader;
    }

    /// <summary>
    /// Return structure as byte array.
    /// </summary>
    public byte[] ToBytes()
    {
        int headSize = Marshal.SizeOf(messageHeader);
        byte[] bytes = new byte[data.Length + headSize];

        IntPtr headPtr = Marshal.AllocHGlobal(headSize);
        Marshal.StructureToPtr(messageHeader, headPtr, true);
        Marshal.Copy(headPtr, bytes, 0, headSize);
        Marshal.FreeHGlobal(headPtr);

        data.CopyTo(bytes, headSize);

        return bytes;
    }

    /// <summary>
    /// Returns structure as string
    /// </summary>
    public String StrToString() => Encoding.Unicode.GetString(ToBytes());

    /// <summary>
    /// Returns data as string
    /// </summary>
    public String GetDataAsString() => Encoding.Unicode.GetString(data);
}