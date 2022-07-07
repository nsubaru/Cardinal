module;

#ifndef CARDINAL_NT_LAUNCHER_DEFAULT_PIPE_NAME
#define CARDINAL_NT_LAUNCHER_DEFAULT_PIPE_NAME KmsGenericIoPipe
#endif // CARDINAL_NT_LAUNCHER_DEFAULT_PIPE_NAME

export module Cardinal.KMS:Shell;

import :Base;
import :Console;

export namespace Cardinal::KMS
{
	class Shell {
	public:
		/// <summary>
		/// Using "LineBufferT" buffer
		/// </summary>
		using TBuffer = Console::LineBufferT;

		/// <summary>
		/// Text buffer size
		/// </summary>
		static constexpr auto BufferSize = Console::BufferSize;

		/// <summary>
		/// Full message size
		/// </summary>
		static constexpr auto MessageSize = Console::MessageSize;

		/// <summary>
		/// Generic IO named pipe name
		/// </summary>
		static constexpr Core::Char GenericIoPipeName[] =  L"\\Device\\NamedPipe\\" __TOSTRING(CARDINAL_NT_LAUNCHER_DEFAULT_PIPE_NAME);

		/// <summary>
		/// Calculation time in seconds
		/// </summary>
		static constexpr Core::Int64 SecondIntNtTime = 1000LL * 10000LL;

	private:
		struct IO {
			Core::Handle hGenericIoPipe;
			Core::Handle hGenericIoPipeEventWrite;
			Core::Handle hGenericIoPipeEventRead;
		};

		struct Data {
			IO io;
			Core::Handle hKernel;
			Core::Int32 status;

			volatile API::ECommandType commandType;
			volatile API::EDatagramType datagramType;
			volatile API::ELogLevel logLevel;
			volatile API::EMessageType messageType;

			Core::RefCountT RefCount;

			Data() :
				io({ nullptr, nullptr }),
				hKernel(nullptr),
				commandType(),
				datagramType(),
				logLevel(),
				messageType(),
				status(0),
				RefCount(1)
			{}
		};

	private:
		Data* This;

	private:
		void Dispose();

	private:
		IO CreateIoPipe();

		void Sleep(Core::Int64 seconds);

		void ProccesNewMessageType(API::MessageHeader& dest);

	public:
		/// <summary>
		/// Kernel interaction object constructor
		/// </summary>
		/// <param name="console">Console object</param>
		explicit Shell(Console& console);

	public:
		/// <summary>
		/// Waiting for connection constructor
		/// </summary>
		/// <param name="timeoutInMiliseconds">Timeout in milliseconds</param>
		bool WaitForConnection(Core::Int64 timeoutInMiliseconds);

		/// <summary>
		/// Reading constructor
		/// </summary>
		/// <param name="buffer">Data buffer</param>
		bool Read(TBuffer& buffer);

		/// <summary>
		/// Writing constructor
		/// </summary>
		/// <param name="buffer">Data buffer</param>
		bool Write(TBuffer& buffer);

		/// <summary>
		/// Init the input and output pipes
		/// </summary>
		/// <param name="pipeInName">Input pipe name</param>
		/// <param name="pipeOutName">Output pipe name</param>
		bool SetPipes(const KMS::Shell::TBuffer& pipeGenericName);

		/// <summary>
		/// Init default pipes
		/// </summary>
		void SetDefaultPipes() {
			This->io = CreateIoPipe();
		}

	public:
		/// <summary>
		/// Returns last stored command type
		/// </summary>
		/// <returns>Last stored command type</returns>
		API::ECommandType GetLastCommandType() { return Core::Interlocked::Load(This->commandType); }

		/// <summary>
		/// Returns last stored datagram type
		/// </summary>
		/// <returns>Last stored datagram type</returns>
		API::EDatagramType GetLastDatagramType() { return Core::Interlocked::Load(This->datagramType); }

		/// <summary>
		/// Returns last stored log level
		/// </summary>
		/// <returns>Last stored log level</returns>
		API::ELogLevel GetLastLogLevel() { return Core::Interlocked::Load(This->logLevel); }

		/// <summary>
		/// Returns last stored message type
		/// </summary>
		/// <returns>Last stored message type</returns>
		API::EMessageType GetLastMessageType() { return (API::EMessageType)Core::Interlocked::Load(This->messageType); }

		/// <summary>
		/// Operation status getting constructor
		/// </summary>
		Core::Int32 GetStatus() const { return This->status; }

	public:
		RefCountClass(Shell, This);
	};
}