export module Cardinal.Threading:UnifiedThreadPool.Alpc;

export import :UnifiedThreadPool.Base;

#pragma warning(push)
#pragma warning(disable: 4996)

export namespace Cardinal
{
	namespace IO::IPC::Alpc
	{
		class IAlpcBase;
		class AlpcContext;

		class AlpcServer;
		class AlpcServerInstance;
		class AlpcClient;

		template<typename T>
		using AlpcTask = Threading::BaseTask<T, AlpcContext>;

		using IAlpcTask = Threading::IPayloadBaseTask<AlpcContext>;

		/// <summary>
		/// Base ALPC callback interface
		/// </summary>
		class IAlpcObjectBase : public IDisposable {
		public:
			/// <summary>
			/// Callback error processor
			/// </summary>
			/// <param name="statusCode">Error status code</param>
			virtual void OnError(Int32 statusCode) = 0 {}

			virtual ~IAlpcObjectBase() /*= 0*/ {}; //Ref -> IThreadable
		};
	}

	namespace Threading
	{
		/// <summary>
		/// ALPC task in tak pool
		/// </summary>
		class AlpcPoolJobContainer : public IDisposable {
		private:
			struct Data {
				Handle alpcHandle;
				String taskName;
				ImplSpecificT ownerPool;
				ImplSpecificT alpcObj;
				Containers::ConcurencyList<AlpcPoolJobContainer>::Iterator poolQueueRef;
				IO::IPC::Alpc::IAlpcTask* task;
				void* callbackObject;
				TypeInfo* callbackObjectType;
				Threading::ContextsBlock context;

				Handle hEvent;

				RefCountT RefCount;
				Data(
					Handle alpcHandle,
					String taskName,
					ImplSpecificT ownerPool,
					void* callbackObject,
					TypeInfo* callbackObjectType,
					Threading::ContextsBlock context
				) :
					alpcHandle(alpcHandle),
					taskName(taskName),
					ownerPool(ownerPool),
					callbackObject(callbackObject),
					callbackObjectType(callbackObjectType),
					context(context),
					RefCount(1) {
				}
			};

			Data* This;

		private:
			AlpcPoolJobContainer(Handle alpcHandle, String taskName, ImplSpecificT ownerPool, void* callbackObject, TypeInfo* callbackObjectType)
				: This(new Data(alpcHandle, taskName, ownerPool, callbackObject, callbackObjectType, GetCurrentContextForAlpc())) {};

			ForceInline AlpcPoolJobContainer(Data*&& This) : This(MoveRValue(This)) {}

			ForceInline AlpcPoolJobContainer(const Data* This) : This((Data*) This) { if (This != nullptr)++this->This->RefCount; }

		private:
			Threading::ContextsBlock GetCurrentContextForAlpc();

			friend Threadables::ThreadableRef RegisterThreadable(AlpcPoolJobContainer& thread, const SynchronizeEvent& event, ImplSpecificT callbackInstance, ContextsBlock& contexts);

			friend bool IsUnifiedThreadPoolTerminate(AlpcPoolJobContainer& task);

			friend SRWLock& GetUnifiedPoolLock(AlpcPoolJobContainer& task);

			friend void UnRegisterThreadable(AlpcPoolJobContainer& thread, Threadables::ThreadableRef&& ref);

			friend void UnregisterAlpcTaskFromAlpcTasksList(AlpcPoolJobContainer& alpcTaskObj);

		protected:
			virtual void Dispose();

		public:
			/// <summary>
			/// Returns assigned object handle
			/// </summary>
			/// <returns>Object handle</returns>
			Handle GetHandle() { return This->alpcHandle; };

			/// <summary>
			/// Returns reference to assigned object
			/// </summary>
			/// <returns>Object reference</returns>
			SmartPtr<IO::IPC::Alpc::IAlpcBase> GetCallback();

			/// <summary>
			/// Returns the task context
			/// </summary>
			/// <returns>Task context object</returns>
			Threading::ContextsBlock& GetContext() { return This->context; };

			/// <summary>
			/// Returns task
			/// </summary>
			/// <returns>Task object</returns>
			IO::IPC::Alpc::IAlpcTask* GetTask() { return This->task; };

		public:
			/// <summary>
			/// Returns the task name
			/// </summary>
			/// <returns>Task name</returns>
			ForceInline const String& GetName() const { return This->taskName; }

			/// <summary>
			/// Sets the new task name
			/// </summary>
			/// <param name="newTaskName">New task name</param>
			void SetName(String&& newTaskName) { This->taskName = newTaskName; };

			/// <summary>
			/// Sets the new task name
			/// </summary>
			/// <param name="newTaskName">New task name</param>
			ForceInline void SetName(const String& newTaskName) { SetName(String(newTaskName)); }

		public:
			/// <summary>
			/// Task name
			/// </summary>
			String Property(GetName, SetName) Name;

			/// <summary>
			/// Returns the task pool which processes the timer
			/// </summary>
			/// <returns>Task pool which processes the timer</returns>
			UnifiedThreadPool GetOwnerPool() const;

			/// <summary>
			/// Task pool which processes the timer
			/// </summary>
			UnifiedThreadPool Property(OwnerPool) OwnerPool;

		public:
			friend class Threadables::ThreadableUnifiedThreadPoolAlpcTask;

			friend class IO::IPC::Alpc::IAlpcBase;

			friend class IO::IPC::Alpc::AlpcServer;

			friend AlpcPoolJobContainer GetPoolContainerFromNtContext(RawMemBlk ntContext);

		public:
			RefCountClass(AlpcPoolJobContainer, This);
		};
	}

	namespace IO::IPC::Alpc
	{
		/// <summary>
		/// Closed ALPC object report
		/// </summary>
		struct AlpcReport {
			/// <summary>
			/// Last transfered data size
			/// </summary>
			SizeT DataSize = 0;

			/// <summary>
			/// Pointer to last transfered data 
			/// </summary>
			SmartPtr<Containers::Blob> DataPtr;
		};

		/// <summary>
		/// ALPC message information
		/// </summary>
		struct RecievedMessageDetails {
			/// <summary>
			/// Recieved message id
			/// </summary>
			UInt32 MessageId = 0;

			/// <summary>
			/// Is message requires replying
			/// </summary>
			Boolean IsReply = false;

			/// <summary>
			/// Is reply finished
			/// </summary>
			Boolean ReplyEnd = false;

			/// <summary>
			/// Size of recieved message data
			/// </summary>
			SizeT DataSize = 0;

			/// <summary>
			/// Recieved message data pointer
			/// </summary>
			RawMemBlk DataPtr = nullptr;
		};

		/// <summary>
		/// ALPC message type
		/// </summary>
		enum class AlpcMessageType : UInt16 {
			/// <summary>
			/// Disconnect message with some information
			/// </summary>
			DisconnectWithReport = 0x0005,

			/// <summary>
			/// Connection request approved
			/// </summary>
			ConnectReplyPositive = 0x000b,

			/// <summary>
			/// Disconnect message
			/// </summary>
			Disconnect = 0x000c,

			/// <summary>
			/// Datagram message
			/// </summary>
			IncommingMessageDatagram = 0x0003,

			/// <summary>
			/// Message which require replying
			/// </summary>
			IncommingMessageWithReply = 0x2001,

			/// <summary>
			/// Reply message which end replying
			/// </summary>
			IncommingMessageReplyRelease = 0x0002,

			/// <summary>
			/// Connection request
			/// </summary>
			Connect = 0x200a,

			/// <summary>
			/// Connection request rejected
			/// </summary>
			ConnectReplyNegative = 0xffff
		};

		/// <summary>
		/// Max possible ALPC message object size (MaxMessageSize - sizeof(PORT_MESSAGE) - awailable as message data)
		/// </summary>
		constexpr SizeT MaxMessageSize = ((SizeT) 64Kb) - 1;

		/// <summary>
		/// Acceptable ALPC message type
		/// </summary>
		template<typename T>
		concept AlpcMessageDataType =
			(Concepts::IsPod<T> && (sizeof(T) < MaxMessageSize)) ||
			(TypeTraits::Collection::IsArray<T> && (Concepts::IsPod<typename T::DataType> && (sizeof(typename T::DataType) < MaxMessageSize)));

		/// <summary>
		/// Base ALPC object object
		/// </summary>
		class IAlpcBase : public IDisposable {
		protected:
			template<AlpcMessageDataType T>
			static Containers::Tuple<RawMemBlk, Memory::DataSizeT> GetDataPtrAndSize(const T& data) {
				if constexpr (TypeTraits::Collection::IsArray<T>)
				{
					using DataType = typename T::DataType;
					return { static_cast<RawMemBlk>(const_cast<DataType*>(static_cast<const DataType* const>(data))), Memory::DataSizeT(sizeof(DataType) * data.Count) };
				}
				else
				{
					return { (RawMemBlk) data, Memory::DataSizeT(sizeof(T)) };
				}
			}

			static Threading::AlpcPoolJobContainer AllocAlpcInPool(const String& taskName,
				Cardinal::Handle alpcHandle,
				Threading::UnifiedThreadPool& ownerPool,
				void* callbackObject,
				TypeInfo* callbackObjectType);

			static void RemoveAlpcFromPool(Threading::AlpcPoolJobContainer poolTask);

			virtual void TriggerServiceMessageProccessor(RecievedMessageDetails messageDetails, AlpcMessageType msgType) = 0;

			virtual Handle GetHandle() = 0;

			virtual IAlpcObjectBase& GetCallback() = 0;

			friend void ProccessAlpcMessage(SmartPtr<IAlpcBase> object);

			friend void SendError(SmartPtr<IAlpcBase> obj, Int32 code);

			virtual bool IsServer() = 0;

		public:
			virtual ~IAlpcBase() /*= 0*/ {}; //Ref -> IThreadable
		};
	}
}

#pragma warning(pop)