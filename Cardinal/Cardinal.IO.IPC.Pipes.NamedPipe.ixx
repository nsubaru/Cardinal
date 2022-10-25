export module Cardinal.IO:IPC.Pipes.NamedPipe;

export import :IPC.Pipes.Base;
export import :IPC.Pipes.Exceptions;

export namespace Cardinal::IO::IPC::Pipes
{
	/// <summary>
	/// Pipe interface
	/// </summary>
	class INamedPipe : public IDisposable {
	public:
		/// <summary>
		/// Reads the data from channel in buffer
		/// </summary>
		/// <param name="data">Data buffer</param>
		virtual void ReadRaw(Containers::Blob& data) = 0;

		/// <summary>
		/// Writing the data from channel in buffer
		/// </summary>
		/// <param name="rawData">Data buffer</param>
		virtual void WriteRawData(const Containers::Blob& rawData) = 0;

		/// <summary>
		/// Indicates whether pipe is valid
		/// </summary>
		/// <returns>Logical value "true" (if pipe is valid)</returns>
		virtual Boolean GetIsValid() const = 0;

		/// <summary>
		/// Closes the pipe if it is valid (else do nothing)
		/// </summary>
		virtual void Close() = 0;

		/// <summary>
		/// Waiting for closing the pipe
		/// </summary>
		virtual void WaitClosing() const = 0;

		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~INamedPipe() override {};
	};

	/// <summary>
	/// Named pipe client
	/// </summary>
	class NamedPipeClient final : public INamedPipe {
	private:
		struct Data {
			PipeMode mode;
			Boolean isNoCache;
			Handle pipe;
			Threading::SRWLock inputLock;
			Threading::SRWLock ouputLock;
			String name;
			RefCountT RefCount;

			Data(
				Handle pipe,
				Boolean isNoCache,
				PipeMode mode,
				String&& name
			) :
				mode(mode),
				isNoCache(isNoCache),
				pipe(pipe),
				inputLock(),
				ouputLock(),
				name(MoveRValue(name)),
				RefCount(1) {
			};
		};

		Data* This;

	private:
		NamedPipeClient(Data*&& This) :This(This) {};

	protected:
		virtual void Dispose() override;

	public:
		/// <summary>
		/// An empty constructor (creates invalid pipe)
		/// </summary>
		NamedPipeClient() :This(nullptr) {}

	public:
		/// <summary>
		/// Reads the data from channel in buffer
		/// </summary>
		/// <param name="data">Data buffer</param>
		virtual void ReadRaw(Containers::Blob& data) override;

		/// <summary>
		/// Writing the data from channel in buffer
		/// </summary>
		/// <param name="data">Data buffer</param>
		virtual void WriteRawData(const Containers::Blob& data) override;

		/// <summary>
		/// Closes the pipe if it is valid (else do nothing)
		/// </summary>
		virtual void Close() override;

		/// <summary>
		/// Waiting for closing the pipe 
		/// </summary>
		virtual void WaitClosing() const override;

		/// <summary>
		/// Waiting for buffer clearing
		/// </summary>
		void Flush();

	public:
		/// <summary>
		/// Indicates whether pipe is valid
		/// </summary>
		/// <returns>Logical value "true" (if pipe is valid)</returns>
		virtual Boolean GetIsValid() const override { return This != nullptr && This->pipe != nullptr; }

		/// <summary>
		/// Returns the pipe name
		/// </summary>
		/// <returns>A string with pipe name</returns>
		const String& GetName() const { return This->name; }

		/// <summary>
		/// Pipe mode
		/// </summary>
		/// <returns>Current pipe mode</returns>
		PipeMode GetSupportedMode() const { return This->mode; }

		/// <summary>
		/// Indicates whether to transfer data to this instance without buffering
		/// </summary>
		/// <returns>Logical value which indicates whether to transfer data to this instance without buffering</returns>
		Boolean GetIsNoCache() const { return This->isNoCache; }

	public:
		/// <summary>
		/// Indicates whether pipe is valid
		/// </summary>
		Boolean Property(GetIsValid) IsValid;

		/// <summary>
		/// Pipe name
		/// </summary>
		String Property(GetName) Name;

		/// <summary>
		/// Pipe work mode
		/// </summary>
		PipeMode Property(GetSupportedMode) SupportedMode;

		/// <summary>
		/// Logical value which indicates whether to transfer data to this instance without buffering
		/// </summary>
		Boolean Property(GetIsNoCache) IsNoCache;

	public:
		friend NamedPipeClient ConnectPipe(const String& name, PipeMode mode, bool isNoCache);

	public:
		RefCountClass(NamedPipeClient, This)
	};

	class NamedPipeServerInstance;

	/// <summary>
	/// Named pipe server
	/// </summary>
	class NamedPipeServer final : public IDisposable {
	private:
		struct Data {
			PipeMode mode;
			Boolean isNoCache;
			UInt32 maxInstanceCount;
			Containers::ConcurencyList<NamedPipeServerInstance> instances;
			String name;

			RefCountT RefCount;

			Data(
				PipeMode mode,
				Boolean isNoCache,
				UInt32 maxInstanceCount,
				String name
			) :
				mode(mode),
				isNoCache(isNoCache),
				maxInstanceCount(maxInstanceCount),
				name(name),
				instances(),
				RefCount(1) {
			};
		};

		Data* This;

	private:
		NamedPipeServer(Data*&& This) :This(This) {};

	private:
		Handle CreateServerPipeHandle(bool isNoCache);

	protected:
		virtual void Dispose() override;

	public:
		/// <summary>
		/// An empty cannel constructor (initializes invalid pipe)
		/// </summary>
		NamedPipeServer() :This(nullptr) {};

		/// <summary>
		/// Waits and joins to the client, returning connection instance
		/// </summary>
		/// <returns>The pipe connection instance with the other side</returns>
		NamedPipeServerInstance Connect();

		/// <summary>
		/// Waits and joins to the client, returning connection instance
		/// </summary>
		/// <param name="isNoCache">Indicates whether to transfer data to this instance without buffering</param>
		/// <returns>The pipe connection instance with the other side</returns>
		NamedPipeServerInstance Connect(Boolean isNoCache);

		/// <summary>
		/// Closes all connections with the clients
		/// </summary>
		void CloseAllInstance();

		/// <summary>
		/// Closes all connections and destroys the pipe
		/// </summary>
		void DestroyPipe();

	public:
		/// <summary>
		/// Indicates whether pipe is valid
		/// </summary>
		/// <returns>Logical value "true" (if pipe is valid)</returns>
		bool GetIsValid() const { return This != nullptr && This->instances.Count <= This->maxInstanceCount; }

		/// <summary>
		/// Returns the number of active connections
		/// </summary>
		/// <returns>A number which indicated the number of active connections</returns>
		SizeT GetInstanceCount() const { return This == nullptr ? 0 : This->instances.Count; }

		/// <summary>
		/// Returns the pipe name
		/// </summary>
		/// <returns>A string with pipe name</returns>
		const String& GetName() const { return This->name; }

		/// <summary>
		/// Pipe mode
		/// </summary>
		/// <returns>Current pipe mode</returns>
		PipeMode GetSupportedMode() const { return This->mode; }

		/// <summary>
		/// Indicates whether to transfer data to this instance without buffering
		/// </summary>
		/// <returns>Logical value which indicates whether to transfer data to this instance without buffering</returns>
		Boolean GetIsNoCache() const { return This->isNoCache; }

	public:
		/// <summary>
		/// Indicates whether pipe is valid
		/// </summary>
		Boolean Property(GetIsValid) IsValid;

		/// <summary>
		/// A number of active connections
		/// </summary>
		SizeT Property(GetInstanceCount) InstanceCount;

		/// <summary>
		/// Pipe name
		/// </summary>
		String Property(GetName) Name;

		/// <summary>
		/// Pipe work mode
		/// </summary>
		PipeMode Property(GetSupportedMode) SupportedMode;

		/// <summary>
		/// A boolean value which indicates if buffers auto-resetting is supported after each operation
		/// </summary>
		Boolean Property(GetIsNoCache) IsNoCache;

	public:
		RefCountClass(NamedPipeServer, This)

	public:
		friend NamedPipeServer CreatePipe(const String& name, UInt8 maxInstanceCount, PipeMode mode, bool isNoCache);
	};

	/// <summary>
	/// Named pipe server instance
	/// </summary>
	class NamedPipeServerInstance final : public INamedPipe {
	private:
		struct Data {
			String pipeName;
			PipeMode mode;
			Boolean isNoCache;
			Handle pipe;
			RefCountT RefCount;
			Containers::ConcurencyList<NamedPipeServerInstance>::Iterator ThisInstanceLink;
			Threading::SRWLock inputLock;
			Threading::SRWLock ouputLock;

			Data(
				String&& pipeName,
				Boolean isNoCache,
				Handle pipe,
				PipeMode mode
			) :
				pipe(pipe),
				isNoCache(isNoCache),
				mode(mode),
				pipeName(MoveRValue(pipeName)),
				RefCount(1),
				inputLock(),
				ouputLock(),
				ThisInstanceLink() {
			}
		};

		Data* This;

	private:
		NamedPipeServerInstance(Data*&& This) :This(This) {};

	protected:
		virtual void Dispose() override;

	public:
		/// <summary>
		/// An empty constructor (initializes invalid pipe)
		/// </summary>
		NamedPipeServerInstance() :This(nullptr) {};

	public:
		/// <summary>
		/// Reads the data from channel in buffer
		/// </summary>
		/// <param name="data">Data buffer</param>
		virtual void ReadRaw(Containers::Blob& data) override final;

		/// <summary>
		/// Data writing in pipe from buffer
		/// </summary>
		/// <param name="data">Data buffer</param>
		virtual void WriteRawData(const Containers::Blob& data) override final;

		/// <summary>
		/// Closes the pipe if it is valid, else do nothing
		/// </summary>
		virtual void Close() override final;

		/// <summary>
		/// Waiting for closing the pipe 
		/// </summary>
		virtual void WaitClosing() const override final;

		/// <summary>
		/// Waiting for buffer clearing
		/// </summary>
		void Flush();

	public:
		/// <summary>
		/// Indicates whether pipe is valid
		/// </summary>
		/// <returns>Boolean value "true" if pipe is valid</returns>
		virtual Boolean GetIsValid() const override final { return This != nullptr && This->pipe != nullptr; }

		/// <summary>
		/// Returns the pipe name
		/// </summary>
		/// <returns>A string with pipe name</returns>
		const String& GetName() const { return This->pipeName; }

		/// <summary>
		/// Pipe mode
		/// </summary>
		/// <returns>Current pipe mode</returns>
		PipeMode GetSupportedMode() const { return This->mode; }

		/// <summary>
		/// Indicates whether to transfer data to this instance without buffering
		/// </summary>
		/// <returns>Logical value which indicates whether to transfer data to this instance without buffering</returns>
		Boolean GetIsNoCache() const { return This->isNoCache; }

	public:
		/// <summary>
		/// Indicates whether pipe is valid
		/// </summary>
		Boolean Property(GetIsValid) IsValid;

		/// <summary>
		/// Pipe name
		/// </summary>
		String Property(GetName) Name;

		/// <summary>
		/// Pipe work mode
		/// </summary>
		PipeMode Property(GetSupportedMode) SupportedMode;

		/// <summary>
		/// Logical value which indicates whether to transfer data to this instance without buffering
		/// </summary>
		Boolean Property(GetIsNoCache) IsNoCache;

	public:
		RefCountClass(NamedPipeServerInstance, This);

	public:
		friend NamedPipeServerInstance NamedPipeServer::Connect(bool isNoCache);
	};

	ForceInline NamedPipeServerInstance NamedPipeServer::Connect() {
		return this->Connect(This->isNoCache);
	}

	/// <summary>
	/// Pipe server creation
	/// </summary>
	/// <param name="name">Pipe name</param>
	/// <param name="maxInstanceCount">Maximal number of pipes connections instances</param>
	/// <param name="mode">Pipe modes</param>
	/// <param name="isNoCache">Indicates whether using the pipe data bufferring by the operating system</param>
	/// <returns>Pipe server object</returns>
	NamedPipeServer CreatePipe(const String& name, UInt8 maxInstanceCount = Math::Max<UInt8>(), PipeMode mode = InOutPipeMode, bool isNoCache = true);

	/// <summary>
	/// Connecting to existing pipe
	/// </summary>
	/// <param name="name">Pipe name</param>
	/// <param name="mode">Pipe mode</param>
	/// <param name="isNoCache">Indicates whether using the pipe data bufferring by the operating system</param>
	/// <returns>Custom pipe connection object</returns>
	NamedPipeClient ConnectPipe(const String& name, PipeMode mode = InOutPipeMode, bool isNoCache = true);
}