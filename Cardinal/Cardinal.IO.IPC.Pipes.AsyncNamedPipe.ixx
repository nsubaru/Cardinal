export module Cardinal.IO:IPC.Pipes.AsyncNamedPipe;
//
//export import :IPC.Pipes.Base;
//export import :IPC.Pipes.Exceptions;
//
//import Cardinal.Threading;
//
//#pragma warning(push)
//// This internal implementation file which can use internal API functions
//#pragma warning(disable: 4996)
//
//export namespace Cardinal::IO::IPC::Pipes
//{
//	class AsyncNamedPipeClient;
//	class AsyncNamedPipeServerInstance;
//	class AsyncNamedPipeServer;
//
//	/// <summary>
//	/// The server connection interface of named pipe
//	/// </summary>
//	class IAsyncNamedPipeServerInstance {
//	public:
//		/// <summary>
//		/// Connection closing
//		/// </summary>
//		virtual void CloseConnection() = 0;
//
//	public:
//		/// <summary>
//		/// Destructor
//		/// </summary>
//		virtual ~IAsyncNamedPipeServerInstance() {}
//	};
//
//	/// <summary>
//	/// Asynchronous pipe interface
//	/// </summary>
//	class IAsyncNamedPipe : public Cardinal::IO::IBaseAsyncIO {
//	public:
//		virtual void CloseConnection() = 0;
//
//		virtual void WaitClosing() const = 0;
//
//		virtual void WaitClosing(Time::TimeSpan timeout) const = 0;
//
//		virtual ~IAsyncNamedPipe() override {};
//	};
//
//	/// <summary>
//	/// Client asynchronous named pipe
//	/// </summary>
//	class AsyncNamedPipeClient final : public IAsyncNamedPipe {
//	private:
//		class BaseAsyncNamedPipeClient final : public IDisposable {
//		protected:
//			Handle hPipe;
//			String pipeName;
//			Boolean isNoCache;
//			PipeMode mode;
//			Cardinal::Threading::UnifiedThreadPoolIoTask* poolTask;
//
//			RefCountT RefCount;
//
//		protected:
//			virtual void Dispose() override;
//
//		protected:
//			BaseAsyncNamedPipeClient(String&& pipeName, PipeMode mode, Boolean isNoCache);
//
//		protected:
//			void InitAsyncRead(IIOTask*&& task, void* buffer, Memory::DataSizeT size, void* ioStatusBlk);
//
//			void InitAsyncWrite(IIOTask*&& task, void* buffer, Memory::DataSizeT size, void* ioStatusBlk);
//
//		protected:
//			void CloseConnection();
//
//			void Flush();
//
//			void WaitClosing() const;
//
//			void WaitClosing(Time::TimeSpan timeout) const;
//
//		protected:
//			bool IsValid() { return hPipe != nullptr; }
//
//		public:
//			friend class AsyncNamedPipeClient;
//		};
//
//	private:
//		BaseAsyncNamedPipeClient* This;
//
//	protected:
//		virtual void Dispose() override {
//			if (This->poolTask != nullptr)
//			{
//				this->UnRegisterNtIoObjectInThreadPool(*This->poolTask);
//				delete This->poolTask;
//				This->poolTask = nullptr;
//			}
//			This->Dispose();
//		}
//
//	protected:
//		virtual Handle GetHandle() override { return This->hPipe; }
//
//	protected:
//		AsyncNamedPipeClient(String&& pipeName, PipeMode mode, Boolean isNoCache, const Cardinal::Threading::UnifiedThreadPool& ownerPool)
//			: This(new BaseAsyncNamedPipeClient(MoveRValue(pipeName), mode, isNoCache)
//			) {
//			This->poolTask = new Cardinal::Threading::UnifiedThreadPoolIoTask(this->RegisterNtIoObjectInThreadPool(ownerPool, This->hPipe));
//		}
//
//	public:
//		/// <summary>
//		/// Asynchronous reading operation initialization
//		/// </summary>
//		/// <param name="data">Data buffer</param>
//		/// <returns>I/O task object</returns>
//		template<typename TDataType = Containers::Blob>
//		IOTask<TDataType> AsyncRead(TDataType&& data) {
//			if (!IsValid)
//				Throw(InvalidHandleUsage(SourcePosition));
//
//			// Verify pipe data transfer mode
//			if (!(This->mode & EPipeMode::In || This->mode & InOutPipeMode))
//				Throw(InvalidPipeException(SourcePosition));
//
//			// Create task object
//			IOTask<TDataType> task = IOTask<TDataType>(
//				// Create irp packet with callbacks for init and end I/O request
//				this->MakeIrp(
//					*This->poolTask,
//					new AsyncNamedPipeClient(*this),
//					[] (IIOTask& task, IRP& irp, SmartPtr<IAsyncIOBase>& ioObject) {
//						IOTask<TDataType>* taskPtr = nullptr;
//
//						// Get request object from interface
//						auto& target = ioObject.As<AsyncNamedPipeClient>();
//
//						try
//						{
//							// Allocate task pointer for thread
//							taskPtr = new IOTask<TDataType>(dynamic_cast<IOTask<TDataType>&>(task));
//
//							// Get buffer and its size
//							auto [buffer, size] = target.GetRawBufferAndSize(*taskPtr);
//
//							// Notify about arrived io operation
//							target.NotifyIoOperation(*target.This->poolTask);
//
//							// Perfom os specific read operations
//							target.This->InitAsyncRead(
//								taskPtr,
//								buffer,
//								size,
//								target.GetReservedPlaceInIrp(&task.GetPayload()));
//						}
//						catch (...)
//						{
//							// If reading is failed cancel notification
//							target.CancelNotifyIoOperation(*target.This->poolTask);
//							// and delete task ptr as it not used
//							delete taskPtr;
//							// and rethrow exception as we don't how handle it
//							ReThrow;
//						}
//					},
//					nullptr,
//						nullptr),
//				MoveRValue(data));
//
//			// Return task object
//			return task;
//		}
//
//		/// <summary>
//		/// Asynchronous writing operation initialization
//		/// </summary>
//		/// <param name="data">Data buffer</param>
//		/// <returns>I/O task object</returns>
//		template<typename TDataType = Containers::Blob>
//		IOTask<TDataType> AsyncWrite(const TDataType&& data) {
//			if (!IsValid)
//				Throw(InvalidHandleUsage(SourcePosition));
//
//			// Verify pipe data transfer mode
//			if (!(This->mode & EPipeMode::Out || This->mode & InOutPipeMode))
//				Throw(InvalidPipeException(SourcePosition));
//
//			// Create task object
//			IOTask<TDataType> task = IOTask<TDataType>(
//				// Create irp packet with callbacks for init and end I/O request
//				this->MakeIrp(
//					*This->poolTask,
//					new AsyncNamedPipeClient(*this),
//					[] (IIOTask& task, IRP& irp, SmartPtr<IAsyncIOBase>& ioObject) {
//						IOTask<TDataType>* taskPtr = nullptr;
//
//						// Get request object from interface
//						auto& target = ioObject.As<AsyncNamedPipeClient>();
//
//						try
//						{
//							// Allocate task pointer for thread
//							taskPtr = new IOTask<TDataType>(dynamic_cast<IOTask<TDataType>&>(task));
//
//							// Get buffer and its size
//							auto [buffer, size] = target.GetRawBufferAndSize(*taskPtr);
//
//							// Notify about arrived io operation
//							target.NotifyIoOperation(*target.This->poolTask);
//
//							// Perfom os specific write operations
//							target.This->InitAsyncWrite(
//								taskPtr,
//								buffer,
//								size,
//								target.GetReservedPlaceInIrp(&task.GetPayload()));
//						}
//						catch (...)
//						{
//							// If reading is failed cancel notification
//							target.CancelNotifyIoOperation(*target.This->poolTask);
//							// and delete task ptr as it not used
//							delete taskPtr;
//							// and rethrow exception as we don't how handle it
//							ReThrow;
//						}
//					},
//					nullptr,
//						nullptr),
//				MoveRValue(data));
//
//			// Return task object
//			return task;
//		}
//
//	public:
//		/// <summary>
//		/// Checking if object is correct
//		/// </summary>
//		/// <returns>"True" if the object allows "I/O" operations</returns>
//		virtual bool GetIsValid() const override { return This->hPipe != nullptr; }
//
//	public:
//		/// <summary>
//		/// Connection closing
//		/// </summary>
//		virtual void CloseConnection() override { This->CloseConnection(); }
//
//		/// <summary>
//		/// Waiting for connection closing
//		/// </summary>
//		virtual void WaitClosing() const override { This->WaitClosing(); }
//
//		/// <summary>
//		/// Waiting for connection closing with maximal allowed waiting time 
//		/// </summary>
//		/// <param name="timeout">Maximal allowed waiting time</param>
//		virtual void WaitClosing(Time::TimeSpan timeout) const override { This->WaitClosing(timeout); }
//
//	public:
//		/// <summary>
//		/// Waiting for reading the data in the other side in synchronous mode
//		/// </summary>
//		void Flush() { This->Flush(); }
//
//	public:
//		/// <summary>
//		/// Returns the pipe name
//		/// </summary>
//		/// <returns>A string with pipe name</returns>
//		const String& GetName() const { return This->pipeName; }
//
//		/// <summary>
//		/// Pipe mode
//		/// </summary>
//		/// <returns>Current pipe mode</returns>
//		PipeMode GetSupportedMode() const { return This->mode; }
//
//		/// <summary>
//		/// Indicates whether to transfer data to this instance without buffering
//		/// </summary>
//		/// <returns>Boolean value which indicates whether to transfer data to this instance without buffering</returns>
//		Boolean GetIsNoCache() const { return This->isNoCache; }
//
//	public:
//		/// <summary>
//		/// Indicates whether pipe is valid
//		/// </summary>
//		Boolean Property(GetIsValid) IsValid;
//
//		/// <summary>
//		/// Pipe name
//		/// </summary>
//		String Property(GetName) Name;
//
//		/// <summary>
//		/// Pipe work mode
//		/// </summary>
//		PipeMode Property(GetSupportedMode) SupportedMode;
//
//		/// <summary>
//		/// Boolean value which indicates whether to transfer data to this instance without buffering
//		/// </summary>
//		Boolean Property(GetIsNoCache) IsNoCache;
//
//	public:
//		/// <summary>
//		/// Returns the reference to task in a pool which processes the operation of this pipe
//		/// </summary>
//		/// <returns>"I/O" operations object in thread pool</returns>
//		virtual const Cardinal::Threading::UnifiedThreadPoolIoTask& GetIoPoolTask() const override {
//			return *This->poolTask;
//		}
//
//	public:
//		friend AsyncNamedPipeClient AsyncConnectPipe(
//			Threading::UnifiedThreadPool&& ownerPool,
//			String&& name,
//			PipeMode mode,
//			Boolean isNoCache);
//
//	public:
//		RefCountClass(AsyncNamedPipeClient, This)
//	};
//
//	/// <summary>
//	/// Server asynchronous named pipe
//	/// </summary>
//	class AsyncNamedPipeServer final : public IDisposable {
//	private:
//		class BaseAsyncNamedPipeServer final : public IDisposable {
//		protected:
//			String pipeName;
//			PipeMode mode;
//			Boolean isNoCache;
//			UInt8 maxConnectionCount;
//			Cardinal::Threading::UnifiedThreadPool pool;
//
//			Cardinal::Containers::ConcurencyList<SmartPtr<IAsyncNamedPipeServerInstance>> instances;
//
//			RefCountT RefCount;
//
//		protected:
//			virtual void Dispose() override;
//
//		protected:
//			BaseAsyncNamedPipeServer(
//				Cardinal::Threading::UnifiedThreadPool&& pool,
//				String&& pipeName,
//				PipeMode mode,
//				Boolean isNoCache,
//				UInt8 maxConnectionCount
//			) :
//				pool(MoveRValue(pool)),
//				pipeName(MoveRValue(pipeName)),
//				mode(mode),
//				isNoCache(isNoCache),
//				maxConnectionCount(maxConnectionCount),
//				instances(),
//				RefCount(1) {
//			}
//
//		protected:
//			Handle CreateServerPipeHandle();
//
//		protected:
//			bool IsValid() const { return instances.Count < maxConnectionCount; }
//
//		protected:
//			void CloseAllInstances();
//
//		public:
//			friend class AsyncNamedPipeServer;
//			friend class AsyncNamedPipeServerInstance;
//		};
//
//	private:
//		BaseAsyncNamedPipeServer* This;
//
//	protected:
//		virtual void Dispose() override {
//			This->Dispose();
//		}
//
//	protected:
//		AsyncNamedPipeServer(
//			Cardinal::Threading::UnifiedThreadPool&& pool,
//			String&& pipeName,
//			PipeMode mode,
//			Boolean isNoCache,
//			UInt8 maxConnectionCount
//		) :
//			This(new BaseAsyncNamedPipeServer(
//				MoveRValue(pool),
//				MoveRValue(pipeName),
//				mode,
//				isNoCache,
//				maxConnectionCount
//			)) {
//		}
//
//	public:
//		/// <summary>
//		/// Asynchronous waiting for new client
//		/// </summary>
//		/// <param name="ownerPool">A pool which will process "I/O/ requests</param>
//		/// <returns>Asynchronous operation object</returns>
//		IOTask<AsyncNamedPipeServerInstance> AsyncConnect();
//
//		/// <summary>
//		/// Closes all active connections with clients
//		/// </summary>
//		void CloseAllInstances() { This->CloseAllInstances(); }
//
//		/// <summary>
//		/// Destroys the pipe with all active connections
//		/// </summary>
//		void DestroyPipe() { Dispose(); }
//
//	public:
//		/// <summary>
//		/// Indicates whether pipe is valid
//		/// </summary>
//		/// <returns>Boolean value "true" (if pipe is valid)</returns>
//		bool GetIsValid() const { return This->IsValid(); }
//
//		/// <summary>
//		/// Returns the number of active connections
//		/// </summary>
//		/// <returns>A number of active connections</returns>
//		SizeT GetInstanceCount() const { return This == nullptr ? 0 : This->instances.Count; }
//
//		/// <summary>
//		/// Returns the pipe name
//		/// </summary>
//		/// <returns>A string with pipe name</returns>
//		const String& GetName() const { return This->pipeName; }
//
//		/// <summary>
//		/// Pipe mode
//		/// </summary>
//		/// <returns>Current pipe mode</returns>
//		PipeMode GetSupportedMode() const { return This->mode; }
//
//		/// <summary>
//		/// Indicates whether to transfer data to this instance without buffering
//		/// </summary>
//		/// <returns>Boolean value which indicates whether to transfer data to this instance without buffering</returns>
//		Boolean GetIsNoCache() const { return This->isNoCache; }
//
//	public:
//		/// <summary>
//		/// Indicates whether pipe is valid
//		/// </summary>
//		Boolean Property(GetIsValid) IsValid;
//
//		/// <summary>
//		/// A number of active connections
//		/// </summary>
//		SizeT Property(GetInstanceCount) InstanceCount;
//
//		/// <summary>
//		/// Pipe name
//		/// </summary>
//		String Property(GetName) Name;
//
//		/// <summary>
//		/// Pipe work mode
//		/// </summary>
//		PipeMode Property(GetSupportedMode) SupportedMode;
//
//		/// <summary>
//		/// A boolean value which indicates if buffers auto-resetting is supported after each operation
//		/// </summary>
//		Boolean Property(GetIsNoCache) IsNoCache;
//
//	public:
//		RefCountClass(AsyncNamedPipeServer, This)
//
//	public:
//		friend AsyncNamedPipeServer CreateAsyncPipe(
//			Cardinal::Threading::UnifiedThreadPool&& ownerPool,
//			String&& name,
//			UInt8 maxInstanceCount,
//			PipeMode mode,
//			Boolean isNoCache);
//
//		friend class AsyncNamedPipeServerInstance;
//	};
//
//	/// <summary>
//	/// Server instance asynchronous named pipe
//	/// </summary>
//	class AsyncNamedPipeServerInstance final :
//		public IAsyncNamedPipe,
//		public IAsyncNamedPipeServerInstance,
//		public IBaseConnectableAsyncIO<AsyncNamedPipeServerInstance> {
//	private:
//		class BaseAsyncNamedPipeServerInstance final : public IDisposable {
//		protected:
//			Handle hPipe;
//			String pipeName;
//			PipeMode mode;
//			Boolean isNoCache;
//			Cardinal::Threading::UnifiedThreadPoolIoTask* poolTask;
//			AsyncNamedPipeServer ownerPipe;
//
//			Containers::ConcurencyList<SmartPtr<IAsyncNamedPipeServerInstance>>::Iterator serverRef;
//
//			RefCountT RefCount;
//
//		protected:
//			virtual void Dispose() override;
//
//		protected:
//			BaseAsyncNamedPipeServerInstance(
//				const AsyncNamedPipeServer& ownerPipe,
//				String&& pipeName,
//				Handle   hPipe,
//				PipeMode mode,
//				Boolean  isNoCache
//			) :
//				hPipe(hPipe),
//				pipeName(MoveRValue(pipeName)),
//				mode(mode),
//				isNoCache(isNoCache),
//				ownerPipe(ownerPipe),
//				poolTask(nullptr),
//				serverRef(),
//				RefCount(1) {
//			}
//
//		protected:
//			void InitAsyncConnect(IIOTask*&& task, void* ioStatusBlk);
//
//		protected:
//			void InitAsyncRead(IIOTask*&& task, void* buffer, Memory::DataSizeT size, void* ioStatusBlk);
//
//			void InitAsyncWrite(IIOTask*&& task, void* buffer, Memory::DataSizeT size, void* ioStatusBlk);
//
//		protected:
//			void CloseConnection();
//
//			void Flush();
//
//			void WaitClosing() const;
//
//			void WaitClosing(Time::TimeSpan timeout) const;
//
//		protected:
//			bool IsValid() { return hPipe != nullptr; }
//
//		public:
//			friend class AsyncNamedPipeServerInstance;
//		};
//
//	private:
//		BaseAsyncNamedPipeServerInstance* This;
//
//	protected:
//		virtual void Dispose() override {
//			if (This->poolTask != nullptr)
//			{
//				this->UnRegisterNtIoObjectInThreadPool(*This->poolTask);
//				delete This->poolTask;
//				This->poolTask = nullptr;
//			}
//			This->Dispose();
//		}
//
//	protected:
//		virtual Handle GetHandle() override {
//			return This->hPipe;
//		}
//
//	protected:
//		AsyncNamedPipeServerInstance(
//			const AsyncNamedPipeServer& ownerPipe,
//			Handle hPipe,
//			const String& name,
//			PipeMode mode,
//			Boolean isNoCache,
//			const Cardinal::Threading::UnifiedThreadPool& threadPool
//		) : This(new BaseAsyncNamedPipeServerInstance(ownerPipe, String(name), hPipe, mode, isNoCache)) {
//			// Register I/O object in thread pool
//			This->poolTask = new Cardinal::Threading::UnifiedThreadPoolIoTask(this->RegisterNtIoObjectInThreadPool(threadPool, This->hPipe));
//		}
//
//	protected:
//		void SetInstaceRef(Cardinal::Containers::ConcurencyList<SmartPtr<IAsyncNamedPipeServerInstance>>::Iterator serverRef) {
//			This->serverRef = serverRef;
//		}
//
//	protected:
//		IOTask<AsyncNamedPipeServerInstance> AsyncConnect() {
//			if (!IsValid)
//				Throw(InvalidHandleUsage(SourcePosition));
//
//			// Create task object
//			IOTask<AsyncNamedPipeServerInstance> task = IOTask<AsyncNamedPipeServerInstance>(
//				// Create irp packet with callbacks for init and end I/O request
//				this->MakeIrp(
//					*This->poolTask,
//					new AsyncNamedPipeServerInstance(*this),
//					[] (IIOTask& task, IRP& irp, SmartPtr<IAsyncIOBase>& ioObject) {
//						IOTask<AsyncNamedPipeServerInstance>* taskPtr = nullptr;
//
//						// Get request object from interface
//						auto& target = ioObject.As<AsyncNamedPipeServerInstance>();
//
//						try
//						{
//							// Allocate task pointer for thread
//							taskPtr = new IOTask<AsyncNamedPipeServerInstance>(dynamic_cast<IOTask<AsyncNamedPipeServerInstance>&>(task));
//
//
//							// Notify about arrived io operation
//							target.NotifyIoOperation(*target.This->poolTask);
//
//							// Perfom os specific connect operations
//							target.This->InitAsyncConnect(MoveRValue(taskPtr), target.GetReservedPlaceInIrp(&irp));
//						}
//						catch (...)
//						{
//							// If reading is failed cancel notification
//							target.CancelNotifyIoOperation(*target.This->poolTask);
//							// and delete task ptr as it not used
//							delete taskPtr;
//							// and rethrow exception as we don't how handle it
//							ReThrow;
//						}
//					},
//					[] (IIOTask& task, IRP& irp, SmartPtr<IAsyncIOBase>& ioObject, Int32& status) {
//						// Get request object from interface
//						auto& target = ioObject.As<AsyncNamedPipeServerInstance>();
//
//						// Store it in list and save ref in object
//						target.SetInstaceRef(target.This->ownerPipe.This->instances.AddLast(new AsyncNamedPipeServerInstance(target)));
//					},
//						nullptr),
//				AsyncNamedPipeServerInstance(*this));
//
//			// Return task object
//			return task;
//		}
//
//	public:
//		/// <summary>
//		/// Asynchronous reading operation initialization
//		/// </summary>
//		/// <param name="data">Data buffer</param>
//		/// <returns>"I/O" task object</returns>
//		template<typename TDataType = Containers::Blob>
//		IOTask<TDataType> AsyncRead(TDataType&& data) {
//			if (!IsValid)
//				Throw(InvalidHandleUsage(SourcePosition));
//
//			// Verify pipe data transfer mode
//			if (!(This->mode & EPipeMode::In || This->mode & InOutPipeMode))
//				Throw(InvalidPipeException(SourcePosition));
//
//			// Create task object
//			IOTask<TDataType> task = IOTask<TDataType>(
//				// Create irp packet with callbacks for init and end I/O request
//				this->MakeIrp(
//					*This->poolTask,
//					new AsyncNamedPipeServerInstance(*this),
//					[] (IIOTask& task, IRP& irp, SmartPtr<IAsyncIOBase>& ioObject) {
//						IOTask<TDataType>* taskPtr = nullptr;
//
//						// Get request object from interface
//						auto& target = ioObject.As<AsyncNamedPipeServerInstance>();
//
//						try
//						{
//							// Allocate task pointer for thread
//							taskPtr = new IOTask<TDataType>(dynamic_cast<IOTask<TDataType>&>(task));
//
//							// Get buffer and its size
//							auto [buffer, size] = target.GetRawBufferAndSize(*taskPtr);
//
//							// Notify about arrived io operation
//							target.NotifyIoOperation(*target.This->poolTask);
//
//							// Perfom os specific read operations
//							target.This->InitAsyncRead(
//								taskPtr,
//								buffer,
//								size,
//								target.GetReservedPlaceInIrp(&task.GetPayload()));
//						}
//						catch (...)
//						{
//							// If reading is failed cancel notification
//							target.CancelNotifyIoOperation(*target.This->poolTask);
//							// and delete task ptr as it not used
//							delete taskPtr;
//							// and rethrow exception as we don't how handle it
//							ReThrow;
//						}
//					},
//					nullptr,
//						nullptr),
//				MoveRValue(data));
//
//			// Return task object
//			return task;
//		}
//
//		/// <summary>
//		/// Asynchronous writing operation initialization
//		/// </summary>
//		/// <param name="data">Data buffer</param>
//		/// <returns>"I/O" task object</returns>
//		template<typename TDataType = Containers::Blob>
//		IOTask<TDataType> AsyncWrite(const TDataType&& data) {
//			if (!IsValid)
//				Throw(InvalidHandleUsage(SourcePosition));
//
//			// Verify pipe data transfer mode
//			if (!(This->mode & EPipeMode::Out || This->mode & InOutPipeMode))
//				Throw(InvalidPipeException(SourcePosition));
//
//			// Create task object
//			IOTask<TDataType> task = IOTask<TDataType>(
//				// Create irp packet with callbacks for init and end I/O request
//				this->MakeIrp(
//					*This->poolTask,
//					new AsyncNamedPipeServerInstance(*this),
//					[] (IIOTask& task, IRP& irp, SmartPtr<IAsyncIOBase>& ioObject) {
//						IOTask<TDataType>* taskPtr = nullptr;
//
//						// Get request object from interface
//						auto& target = ioObject.As<AsyncNamedPipeServerInstance>();
//
//						try
//						{
//							// Allocate task pointer for thread
//							taskPtr = new IOTask<TDataType>(dynamic_cast<IOTask<TDataType>&>(task));
//
//							// Get buffer and its size
//							auto [buffer, size] = target.GetRawBufferAndSize(*taskPtr);
//
//							// Notify about arrived io operation
//							target.NotifyIoOperation(*target.This->poolTask);
//
//							// Perfom os specific write operations
//							target.This->InitAsyncWrite(
//								taskPtr,
//								buffer,
//								size,
//								target.GetReservedPlaceInIrp(&task.GetPayload()));
//						}
//						catch (...)
//						{
//							// If reading is failed cancel notification
//							target.CancelNotifyIoOperation(*target.This->poolTask);
//							// and delete task ptr as it not used
//							delete taskPtr;
//							// and rethrow exception as we don't how handle it
//							ReThrow;
//						}
//					},
//					nullptr,
//						nullptr),
//				MoveRValue(data));
//
//			// Return task object
//			return task;
//		}
//
//	public:
//		/// <summary>
//		/// Waiting for reading the data in the other side in synchronous mode
//		/// </summary>
//		void Flush() { This->Flush(); }
//
//	public:
//		/// <summary>
//		/// Indicates whether pipe is valid
//		/// </summary>
//		/// <returns>Boolean value "true" (if pipe is valid)</returns>
//		virtual bool GetIsValid() const override { return This->IsValid(); }
//
//		/// <summary>
//		/// Returns the pipe name
//		/// </summary>
//		/// <returns>A string with pipe name</returns>
//		const String& GetName() const { return This->pipeName; }
//
//		/// <summary>
//		/// Pipe mode
//		/// </summary>
//		/// <returns>Current pipe mode</returns>
//		PipeMode GetSupportedMode() const { return This->mode; }
//
//		/// <summary>
//		/// Indicates whether to transfer data to this instance without buffering
//		/// </summary>
//		/// <returns>Boolean value which indicates whether to transfer data to this instance without buffering</returns>
//		Boolean GetIsNoCache() const { return This->isNoCache; }
//
//	public:
//		/// <summary>
//		/// Indicates whether pipe is valid
//		/// </summary>
//		Boolean Property(GetIsValid) IsValid;
//
//		/// <summary>
//		/// Pipe name
//		/// </summary>
//		String Property(GetName) Name;
//
//		/// <summary>
//		/// Pipe work mode
//		/// </summary>
//		PipeMode Property(GetSupportedMode) SupportedMode;
//
//		/// <summary>
//		/// Boolean value which indicates whether to transfer data to this instance without buffering
//		/// </summary>
//		Boolean Property(GetIsNoCache) IsNoCache;
//
//	public:
//		/// <summary>
//		/// Connection closing
//		/// </summary>
//		virtual void CloseConnection() override { This->CloseConnection(); }
//
//		/// <summary>
//		/// Waiting for connection closing
//		/// </summary>
//		virtual void WaitClosing() const override { This->WaitClosing(); }
//
//		/// <summary>
//		/// Waiting for connection closing with maximal allowed waiting time 
//		/// </summary>
//		/// <param name="timeout">Maximal allowed waiting time</param>
//		virtual void WaitClosing(Time::TimeSpan timeout) const override { This->WaitClosing(timeout); }
//
//	public:
//		/// <summary>
//		/// Returns the reference to task in a pool which processes the operation of this pipe
//		/// </summary>
//		/// <returns>"I/O" operation object in thread pool</returns>
//		virtual const Threading::UnifiedThreadPoolIoTask& GetIoPoolTask() const override {
//			return *This->poolTask;
//		}
//
//	public:
//		RefCountClass(AsyncNamedPipeServerInstance, This)
//
//	public:
//		friend class AsyncNamedPipeServer;
//	};
//
//	ForceInline IOTask<AsyncNamedPipeServerInstance> AsyncNamedPipeServer::AsyncConnect() {
//		// Create pipe object
//		auto pipe = AsyncNamedPipeServerInstance(
//			*this,
//			This->CreateServerPipeHandle(),
//			This->pipeName,
//			This->mode,
//			This->isNoCache,
//			This->pool);
//
//		// Call pipe async connect
//		return pipe.AsyncConnect();
//	}
//
//#pragma region CreateAsyncPipe
//	/// <summary>
//	/// Creating the server of named pipes
//	/// </summary>
//	/// <param name="ownerPool">A pool which will process "I/O" operations</param>
//	/// <param name="name">Pipe name</param>
//	/// <param name="maxInstanceCount">Maximal number of clients</param>
//	/// <param name="mode">Data transfer mode</param>
//	/// <param name="isNoCache">If "true" then the bufferless work mode will be used</param>
//	/// <returns>The server object of named pipes</returns>
//	ForceInline AsyncNamedPipeServer CreateAsyncPipe(
//		Cardinal::Threading::UnifiedThreadPool&& ownerPool,
//		String&& name,
//		UInt8 maxInstanceCount = Math::Max<UInt8>(),
//		PipeMode mode = InOutPipeMode,
//		Boolean isNoCache = true
//	) {
//		return AsyncNamedPipeServer(
//			MoveRValue(ownerPool),
//			MoveRValue(name),
//			mode,
//			isNoCache,
//			maxInstanceCount
//		);
//	}
//
//	/// <summary>
//	/// Creating the server of named pipes
//	/// </summary>
//	/// <param name="ownerPool">A pool which will process "I/O" operations</param>
//	/// <param name="name">Pipe name</param>
//	/// <param name="maxInstanceCount">Maximal number of clients</param>
//	/// <param name="mode">Data transfer mode</param>
//	/// <param name="isNoCache">If "true" then the bufferless work mode will be used</param>
//	/// <returns>The server object of named pipes</returns>
//	ForceInline AsyncNamedPipeServer CreateAsyncPipe(
//		const Cardinal::Threading::UnifiedThreadPool& ownerPool,
//		const String& name,
//		UInt8 maxInstanceCount = Math::Max<UInt8>(),
//		PipeMode mode = InOutPipeMode,
//		Boolean isNoCache = true) {
//		return CreateAsyncPipe(
//			Threading::UnifiedThreadPool(ownerPool),
//			String(name),
//			maxInstanceCount,
//			mode,
//			isNoCache
//		);
//	}
//
//	/// <summary>
//	/// Creating the server of named pipes
//	/// </summary>
//	/// <param name="ownerPool">A pool which will process "I/O" operations</param>
//	/// <param name="name">Pipe name</param>
//	/// <param name="maxInstanceCount">Maximal number of clients</param>
//	/// <param name="mode">Data transfer mode</param>
//	/// <param name="isNoCache">If "true" then the bufferless work mode will be used</param>
//	/// <returns>The server object of named pipes</returns>
//	ForceInline AsyncNamedPipeServer CreateAsyncPipe(
//		Cardinal::Threading::UnifiedThreadPool&& ownerPool,
//		const String& name,
//		UInt8 maxInstanceCount = Math::Max<UInt8>(),
//		PipeMode mode = InOutPipeMode,
//		Boolean isNoCache = true) {
//		return CreateAsyncPipe(
//			MoveRValue(ownerPool),
//			String(name),
//			maxInstanceCount,
//			mode,
//			isNoCache
//		);
//	}
//
//	/// <summary>
//	/// Creating the server of named pipes
//	/// </summary>
//	/// <param name="ownerPool">A pool which will process "I/O" operations</param>
//	/// <param name="name">Pipe name</param>
//	/// <param name="maxInstanceCount">Maximal number of clients</param>
//	/// <param name="mode">Data transfer mode</param>
//	/// <param name="isNoCache">If "true" then the bufferless work mode will be used</param>
//	/// <returns>The server object of named pipes</returns>
//	ForceInline AsyncNamedPipeServer CreateAsyncPipe(
//		const Cardinal::Threading::UnifiedThreadPool& ownerPool,
//		String&& name,
//		UInt8 maxInstanceCount = Math::Max<UInt8>(),
//		PipeMode mode = InOutPipeMode,
//		Boolean isNoCache = true
//	) {
//		return CreateAsyncPipe(
//			Cardinal::Threading::UnifiedThreadPool(ownerPool),
//			MoveRValue(name),
//			maxInstanceCount,
//			mode,
//			isNoCache
//		);
//	}
//
//	/// <summary>
//	/// Creating the server of named pipes
//	/// </summary>
//	/// <param name="name">Pipe name</param>
//	/// <param name="maxInstanceCount">Maximal number of clients</param>
//	/// <param name="mode">Data transfer mode</param>
//	/// <param name="isNoCache">If "true" then the bufferless work mode will be used</param>
//	/// <returns>The server object of named pipes</returns>
//	ForceInline AsyncNamedPipeServer CreateAsyncPipe(
//		const String& name,
//		UInt8 maxInstanceCount = Math::Max<UInt8>(),
//		PipeMode mode = InOutPipeMode,
//		Boolean isNoCache = true
//	) {
//		return CreateAsyncPipe(
//			Cardinal::Threading::Contexts::GetContext<Threading::ThreadPoolThreadContext>().ThreadPool,
//			String(name),
//			maxInstanceCount,
//			mode,
//			isNoCache
//		);
//	}
//
//	/// <summary>
//	/// Creating the server of named pipes
//	/// </summary>
//	/// <param name="name">Pipe name</param>
//	/// <param name="maxInstanceCount">Maximal number of clients</param>
//	/// <param name="mode">Data transfer mode</param>
//	/// <param name="isNoCache">If "true" then the bufferless work mode will be used</param>
//	/// <returns>The server object of named pipes</returns>
//	ForceInline AsyncNamedPipeServer CreateAsyncPipe(
//		String&& name,
//		UInt8 maxInstanceCount = Math::Max<UInt8>(),
//		PipeMode mode = InOutPipeMode,
//		Boolean isNoCache = true
//	) {
//		return CreateAsyncPipe(
//			Threading::Contexts::GetContext<Threading::ThreadPoolThreadContext>().ThreadPool,
//			MoveRValue(name),
//			maxInstanceCount,
//			mode,
//			isNoCache
//		);
//	}
//#pragma endregion
//
//#pragma region ConnectAsyncPipe
//	/// <summary>
//	/// Asunchronous connection with named pipe
//	/// </summary>
//	/// <param name="ownerPool">A pool which will process "I/O" operations</param>
//	/// <param name="name">Pipe name</param>
//	/// <param name="mode">Data transfer mode</param>
//	/// <param name="isNoCache">If "true" then the bufferless work mode will be used</param>
//	/// <returns>Named pipe object</returns>
//	ForceInline AsyncNamedPipeClient AsyncConnectPipe(
//		Cardinal::Threading::UnifiedThreadPool&& ownerPool,
//		String&& name,
//		PipeMode mode = InOutPipeMode,
//		Boolean isNoCache = true
//	) {
//		return AsyncNamedPipeClient(
//			MoveRValue(name),
//			mode,
//			isNoCache,
//			MoveRValue(ownerPool)
//		);
//	}
//
//	/// <summary>
//	/// Asunchronous connection with named pipe
//	/// </summary>
//	/// <param name="ownerPool">A pool which will process "I/O" operations</param>
//	/// <param name="name">Pipe name</param>
//	/// <param name="mode">Data transfer mode</param>
//	/// <param name="isNoCache">If "true" then the bufferless work mode will be used</param>
//	/// <returns>Named pipe object</returns>
//	ForceInline AsyncNamedPipeClient AsyncConnectPipe(
//		const Cardinal::Threading::UnifiedThreadPool& ownerPool,
//		const String& name,
//		PipeMode mode = InOutPipeMode,
//		Boolean isNoCache = true
//	) {
//		return AsyncConnectPipe(
//			Cardinal::Threading::UnifiedThreadPool(ownerPool),
//			String(name),
//			mode,
//			isNoCache
//		);
//	}
//
//	/// <summary>
//	/// Asunchronous connection with named pipe
//	/// </summary>
//	/// <param name="ownerPool">A pool which will process "I/O" operations</param>
//	/// <param name="name">Pipe name</param>
//	/// <param name="mode">Data transfer mode</param>
//	/// <param name="isNoCache">If "true" then the bufferless work mode will be used</param>
//	/// <returns>Named pipe object</returns>
//	ForceInline AsyncNamedPipeClient AsyncConnectPipe(
//		Cardinal::Threading::UnifiedThreadPool&& ownerPool,
//		const String& name,
//		PipeMode mode = InOutPipeMode,
//		Boolean isNoCache = true
//	) {
//		return AsyncConnectPipe(
//			MoveRValue(ownerPool),
//			String(name),
//			mode,
//			isNoCache
//		);
//	}
//
//	/// <summary>
//	/// Asunchronous connection with named pipe
//	/// </summary>
//	/// <param name="ownerPool">A pool which will process "I/O" operations</param>
//	/// <param name="name">Pipe name</param>
//	/// <param name="mode">Data transfer mode</param>
//	/// <param name="isNoCache">If "true" then the bufferless work mode will be used</param>
//	/// <returns>Named pipe object</returns>
//	ForceInline AsyncNamedPipeClient AsyncConnectPipe(
//		const Cardinal::Threading::UnifiedThreadPool& ownerPool,
//		String&& name,
//		PipeMode mode = InOutPipeMode,
//		Boolean isNoCache = true
//	) {
//		return AsyncConnectPipe(
//			Threading::UnifiedThreadPool(ownerPool),
//			MoveRValue(name),
//			mode,
//			isNoCache
//		);
//	}
//
//	/// <summary>
//	/// Asunchronous connection with named pipe
//	/// </summary>
//	/// <param name="name">Pipe name</param>
//	/// <param name="mode">Data transfer mode</param>
//	/// <param name="isNoCache">If "true" then the bufferless work mode will be used</param>
//	/// <returns>Named pipe object</returns>
//	ForceInline AsyncNamedPipeClient AsyncConnectPipe(
//		const String& name,
//		PipeMode mode = InOutPipeMode,
//		Boolean isNoCache = true
//	) {
//		return AsyncConnectPipe(
//			Cardinal::Threading::Contexts::GetContext<Threading::ThreadPoolThreadContext>().ThreadPool,
//			String(name),
//			mode,
//			isNoCache
//		);
//	}
//
//	/// <summary>
//	/// Asunchronous connection with named pipe
//	/// </summary>
//	/// <param name="name">Pipe name</param>
//	/// <param name="mode">Data transfer mode</param>
//	/// <param name="isNoCache">If "true" then the bufferless work mode will be used</param>
//	/// <returns>Named pipe object</returns>
//	ForceInline AsyncNamedPipeClient AsyncConnectPipe(
//		String&& name,
//		PipeMode mode = InOutPipeMode,
//		Boolean isNoCache = true
//	) {
//		return AsyncConnectPipe(
//			Threading::Contexts::GetContext<Threading::ThreadPoolThreadContext>().ThreadPool,
//			MoveRValue(name),
//			mode,
//			isNoCache
//		);
//	}
//#pragma endregion
//}
//
//#pragma warning(pop)