export module Cardinal.KMS:Console;

import :Base;

import Cardinal.KMS.Keyboard;
import Cardinal.Threading;

export namespace Cardinal::KMS
{
	/// <summary>
	/// Console object for native KMS
	/// </summary>
	class Console {
	public:
		/// <summary>
		/// Full message size
		/// </summary>
		static constexpr SizeT MessageSize = 64 * 1024;

		/// <summary>
		/// Text buffer size
		/// </summary>
		static constexpr SizeT BufferSize = API::TextMessageSize<MessageSize>;

		/// <summary>
		/// New line transition
		/// </summary>
		static constexpr Char NextLine[] = L"\r\n";

		/// <summary>
		/// List size of last used saved commands
		/// </summary>
		static constexpr SizeT CmdHistoryLen = 20;

		/// <summary>
		/// Message text buffer type
		/// </summary>
		using LineBufferT = Char[MessageSize / sizeof(Char)];

	private:
		struct CmdHistoryEntity {
			LineBufferT cmd;
			SizeT len;
		};

		struct Data {
			Keyboard::Keyboard keyboard;

			CmdHistoryEntity CmdHistoryEntities[CmdHistoryLen];

			CmdHistoryEntity currentEntity;

			LineBufferT currentLine;
			LineBufferT backspaces;
			SizeT cmdEntityIndex = 0;
			SizeT cmdEntitiesCount = 0;

			Threading::SRWLock kbdReadLock;
			Threading::SRWLock kbdReadPrintLock;
			volatile LineBufferT* currentBuffer;
			volatile SizeT currentBufferSize;

			volatile API::ECommandType	commandType;
			volatile API::EDatagramType datagramType;
			volatile API::ELogLevel		logLevel;
			volatile API::EMessageType	messageType;

			SizeT linesCount;

			RefCountT RefCount;

			Data(const Keyboard::Keyboard& keyboard) :
				Data(Keyboard::Keyboard(keyboard)) {}

			Data(Keyboard::Keyboard&& keyboard) :
				keyboard(MoveRValue(keyboard)),
				currentEntity{ 0 },
				CmdHistoryEntities{ 0 },
				cmdEntitiesCount{ 0 },
				cmdEntityIndex{ 0 },
				kbdReadLock(),
				kbdReadPrintLock(),
				currentBuffer(nullptr),
				linesCount(0),
				RefCount(1)
			{
#pragma loop(hint_parallel(16))
				for (SizeT i = 0; i < MessageSize / sizeof(Char); i++)
				{
					backspaces[i] = L'\b';
					currentLine[i] = L'\0';
				}
			}
		};

	private:
		Data* This;

	private:
		void Copy(LineBufferT& source, LineBufferT& dest) {
			for (SizeT i = 0; i < MessageSize / sizeof(wchar_t); i++)
			{
				dest[i] = source[i];
			}
		}

		void ReadDataLine(LineBufferT& buffer, bool hideInput);

	protected:
		virtual void Dispose() {}

	private:
		void RequestPressAnyKeyIfNeed();

	public:
		/// <summary>
		/// Keyboard object copying constructor
		/// </summary>
		/// <param name="keyboard">Keyboard object</param>
		Console(const Keyboard::Keyboard& keyboard) : This(new Data(keyboard)) {}

		/// <summary>
		/// Keyboard object moving constructor
		/// </summary>
		/// <param name="keyboard">Keyboard object</param>
		Console(Keyboard::Keyboard&& keyboard) : This(new Data(MoveRValue(keyboard))) {}

	public:
		/// <summary>
		/// Reads the keyboard
		/// </summary>
		/// <param name="buffer">Data buffer</param>
		void ReadLine(LineBufferT& buffer);

		/// <summary>
		/// Display text on screen and move cursor to next line
		/// </summary>
		/// <param name="buffer">Data buffer</param>
		void WriteLine(const LineBufferT& buffer);

		/// <summary>
		/// Display multi line message on screen with auto scrolling
		/// </summary>
		/// <param name="buffer">Data buffer</param>
		void WriteMultiLineMessage(const LineBufferT& message);

		/// <summary>
		/// Saves printed commands
		/// </summary>
		/// <param name="buffer">Data buffer</param>
		/// <param name="len">Number of symbols</param>
		void StoreComand(LineBufferT& buffer, SizeT cmdLen);

		/// <summary>
		/// Returns last command in list
		/// </summary>
		/// <param name="buffer">Data buffer</param>
		/// <param name="offset">Command index</param>
		/// <param name="cmdLen">Number of symbols in command</param>
		void GetPreviousCommand(LineBufferT& buffer, SizeT& offset, volatile SizeT& cmdLen);

		/// <summary>
		/// Returns next command in list
		/// </summary>
		/// <param name="buffer">Data buffer</param>
		/// <param name="offset">Command index</param>
		/// <param name="cmdLen">Number of symbols in command</param>
		void GetNextCommand(LineBufferT& buffer, SizeT& offset, volatile SizeT& cmdLen);

		/// <summary>
		/// Displays text on loading screen
		/// </summary>
		/// <param name="buffer">Displayed text</param>
		void DisplayToBootScreen(const Char* buffer);

		/// <summary>
		/// Waiting for pressing any key
		/// </summary>
		void WaitForAnyKeyDurringReadCode() {
			This->keyboard.WaitForAnyKeyDurringReadCode();
		}

	public:
		/// <summary>
		/// Sets new state of last command type field
		/// </summary>
		/// <param name="newData">New state</param>
		void SetLastCommandType(API::ECommandType newData) {
			Interlocked::Store(This->commandType, newData);
		};

		/// <summary>
		/// Sets new state of last datagram type field
		/// </summary>
		/// <param name="newData">New state</param>
		void SetLastDatagramType(API::EDatagramType newData) {
			Interlocked::Store(This->datagramType, newData);
		};

		/// <summary>
		/// Sets new state of last log level field
		/// </summary>
		/// <param name="newData">New state</param>
		void SetLastLogLevel(API::ELogLevel newData) {
			Interlocked::Store(This->logLevel, newData);
		};

		/// <summary>
		/// Sets new state of last message type field
		/// </summary>
		/// <param name="newData">New state</param>
		void SetLastMessageType(API::EMessageType newData) {
			Interlocked::Store(This->messageType, newData);
		};

		/// <summary>
		/// Returns last stored command type
		/// </summary>
		/// <returns>Last stored command type</returns>
		API::ECommandType GetLastCommandType() { return Interlocked::Load(This->commandType); }

		/// <summary>
		/// Returns last stored datagram type
		/// </summary>
		/// <returns>Last stored datagram type</returns>
		API::EDatagramType GetLastDatagramType() { return Interlocked::Load(This->datagramType); }

		/// <summary>
		/// Returns last stored log level
		/// </summary>
		/// <returns>Last stored log level</returns>
		API::ELogLevel GetLastLogLevel() { return Interlocked::Load(This->logLevel); }

		/// <summary>
		/// Returns last stored message type
		/// </summary>
		/// <returns>Last stored message type</returns>
		API::EMessageType GetLastMessageType() { return (API::EMessageType)Interlocked::Load(This->messageType); }

	public:
		RefCountClass(Console, This)
	};
}