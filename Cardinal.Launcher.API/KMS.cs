using System.Diagnostics;
using System.IO.Pipes;
using System.Runtime.InteropServices;
using System.Text;

namespace Cardinal.Launcher.API;

/// <summary>
/// KMS transport implementation
/// </summary>
public class KMS : IDisposable
{
    /// <summary>
    /// Rem kernel process object
    /// </summary>
    public Process KernelProcess { get; private set; }

    /// <summary>
    /// Rem kernel process main thread
    /// </summary>
#pragma warning disable CA1416 // Validate platform compatibility
    public ProcessThread KernelProcessMainThread => KernelProcess.Threads.Cast<ProcessThread>().OrderBy(item => item.StartTime).First();
#pragma warning restore CA1416 // Validate platform compatibility

    private bool disposedValue;
    private NamedPipeServerStream GenericIOPipe;

    [DllImport("Rem.KMS.Starter.dll", CharSet = CharSet.Unicode)]
    private static extern ulong StartRemKernel(String path);

    private static string BytesToStringConverted(byte[] bytes)
    {
        using var stream = new MemoryStream(bytes);
        using var streamReader = new StreamReader(stream, Encoding.Unicode);

        return streamReader.ReadToEnd();
    }

    private void InitPipeDataTransport(string genericIOPipeName)
    {
#pragma warning disable CA1416 // Validate platform compatibility
        GenericIOPipe = new NamedPipeServerStream(genericIOPipeName, PipeDirection.InOut, 254, PipeTransmissionMode.Message, PipeOptions.Asynchronous);
#pragma warning restore CA1416 // Validate platform compatibility
    }

    /// <summary>
    /// Constructor with path to Rem, notification when process close and pipe name, create new Rem kernel process
    /// </summary>
    /// <param name="excecutablePath">Path Rem executable</param>
    /// <param name="onProcessExited">Event handler witch will call when Rem process exit</param>
    /// <param name="kmsGenericIOPipeName">KMS transport pipe name</param>
    /// <param name="onlyCreate">If true, than process will create without running code</param>
    /// <exception cref="ArgumentException"></exception>
    public KMS(string excecutablePath, EventHandler onProcessExited, string kmsGenericIOPipeName, bool onlyCreate = false)
    {
        InitPipeDataTransport(kmsGenericIOPipeName);

        if (GenericIOPipe == null)
        {
            throw new NullReferenceException();
        }

        var processId = StartRemKernel(excecutablePath);

        if (processId == 0)
        {
            throw new ArgumentException("Invalid path passed");
        }

        KernelProcess = Process.GetProcesses().First(process => process.Id == (int)processId);

        KernelProcess.EnableRaisingEvents = true;
        KernelProcess.Exited += onProcessExited;

        if (!onlyCreate)
        {
            KernelProcess.Resume();
        }

        KernelProcess.Refresh();
    }

    /// <summary>
    /// Constructor with notification when process close and pipe name, connect to exist Rem process
    /// </summary>
    /// <param name="onProcessExited">Event handler witch will call when Rem process exit</param>
    /// <param name="kmsGenericIOPipeName">KMS transport pipe name</param>
    /// <exception cref="ArgumentException"></exception>
    public KMS(EventHandler onProcessExited, string kmsGenericIOPipeName)
    {
        InitPipeDataTransport(kmsGenericIOPipeName);

        if (GenericIOPipe == null)
        {
            throw new NullReferenceException();
        }

        KernelProcess = Process.GetProcesses().First(pr => pr.ProcessName.Contains("Natsuki Rem"));

        KernelProcess.EnableRaisingEvents = true;
        KernelProcess.Exited += onProcessExited;

        KernelProcess.Refresh();
    }

    /// <summary>
    /// Perform KMS message receive
    /// </summary>
    /// <param name="maxMsgSize">Full kms message size</param>
    /// <returns></returns>
    /// <exception cref="TransportClosed">Throw when transport disconect durring operation</exception>
    public KmsMessage Read(int maxMsgSize = 16 * 1024)
    {
        if (!GenericIOPipe.IsConnected)
        {
            GenericIOPipe.WaitForConnection();
        }

        byte[] res = new byte[maxMsgSize];
        GenericIOPipe.Read(res, 0, maxMsgSize);

        if (!GenericIOPipe.IsConnected)
        {
            throw new TransportClosedException();
        }

        KmsMessageHeader head = new KmsMessageHeader(res);
        byte[] text = res.Skip(4).ToArray();

        String ResStr = BytesToStringConverted(text);
        ResStr = ResStr.TrimEnd('\0');

        KmsMessage message = new KmsMessage(head, ResStr);
        return message;
    }

    /// <summary>
    /// Perform async KMS message receive
    /// </summary>
    /// <param name="maxMsgSize">Full kms message size</param>
    /// <returns>Task with full kms message</returns>
    /// <exception cref="TransportClosed">Throw when transport disconect durring operation</exception>
    public async Task<KmsMessage> ReadAsync(int maxMsgSize = 16 * 1024)
    {
        if (!GenericIOPipe.IsConnected)
        {
            GenericIOPipe.WaitForConnection();
        }

        byte[] res = new byte[maxMsgSize];
        await GenericIOPipe.ReadAsync(res, 0, maxMsgSize);

        if (!GenericIOPipe.IsConnected)
        {
            throw new TransportClosedException();
        }

        KmsMessageHeader head = new KmsMessageHeader(res);
        byte[] text = res.Skip(4).ToArray();

        String ResStr = BytesToStringConverted(text);
        ResStr = ResStr.TrimEnd('\0');

        KmsMessage message = new KmsMessage(head, ResStr);
        return message;
    }

    /// <summary>
    /// Perform KMS message send
    /// </summary>
    /// <param name="message">Full KMS message</param>
    /// <exception cref="TransportClosed">Throw when transport disconect durring operation</exception>
    public void Write(KmsMessage message)
    {
        try
        {
            if (!GenericIOPipe.IsConnected)
            {
                GenericIOPipe.WaitForConnection();
            }

            byte[] data = message.ToBytes();
            GenericIOPipe.Write(data, 0, data.Length);

            if (!GenericIOPipe.IsConnected)
            {
                throw new TransportClosedException();
            }
        }
        catch (IOException)
        {
            throw new TransportClosedException();
        }
    }

    /// <summary>
    /// Perform async KMS message send
    /// </summary>
    /// <param name="message">Full KMS message</param>
    /// <returns>Awaitable task</returns>
    /// <exception cref="TransportClosed">Throw when transport disconect durring operation</exception>
    public async Task WriteAsync(KmsMessage message)
    {
        try
        {
            if (!GenericIOPipe.IsConnected)
            {
                GenericIOPipe.WaitForConnection();
            }

            byte[] data = message.ToBytes();
            await GenericIOPipe.WriteAsync(data, 0, data.Length);

            if (!GenericIOPipe.IsConnected)
            {
                throw new TransportClosedException();
            }
        }
        catch (IOException)
        {
            throw new TransportClosedException();
        }
    }

    /// <summary>
    /// Flush transport buffers
    /// </summary>
    public void Flush()
    {
        GenericIOPipe.Flush();
    }

    /// <summary>
    /// Waiting for connection from Rem
    /// </summary>
    public void WaitForConnection()
    {
        GenericIOPipe.WaitForConnection();
    }

    /// <summary>
    /// Waiting for connection from Rem
    /// </summary>
    public async Task WaitForConnectionAsync()
    {
        await GenericIOPipe.WaitForConnectionAsync();
    }

    /// <summary>
    /// True if transport valid
    /// </summary>
    public bool IsValid => GenericIOPipe.IsConnected;

    /// <summary>
    /// Perform resume Rem process
    /// </summary>
    public void ResumeProcess()
    {
        KernelProcess.Resume();
        KernelProcess.Refresh();
    }

    /// <summary>
    /// Disposing object
    /// </summary>
    public void Dispose()
    {
        Dispose(disposing: true);
        GC.SuppressFinalize(this);
    }

    protected virtual void Dispose(bool disposing)
    {
        if (!disposedValue)
        {
            if (disposing)
            {
                ((IDisposable)KernelProcess).Dispose();
                ((IDisposable)GenericIOPipe).Dispose();
                ((IDisposable)GenericIOPipe).Dispose();
            }

            disposedValue = true;
        }
    }
}