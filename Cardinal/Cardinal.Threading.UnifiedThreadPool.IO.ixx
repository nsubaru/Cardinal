export module Cardinal.Threading:UnifiedThreadPool.IO;

export import :UnifiedThreadPool.Base;
export import Cardinal.Containers.TypeTraits;

#pragma warning(push)
#pragma warning(disable: 4996)

namespace Cardinal::IO
{
	class IAsyncIOBase;
}

export namespace Cardinal::Threading
	{
		/// <summary>
		/// I/O object of task pool
		/// </summary>
		class UnifiedThreadPoolIoTask final : public IDisposable {
		public:
			friend class IO::IAsyncIOBase;

		private:
			struct Data {
				Handle hFile;
				Containers::ConcurencyList<UnifiedThreadPoolIoTask>::Iterator poolRef;
				ImplSpecificT ownerPool;
				ImplSpecificT hIoPoolObj;
				String ioTaskName;

				RefCountT RefCount;

				ForceInline Data(Handle hFile, ImplSpecificT ownerPool)
					: hFile(hFile),
					poolRef(),
					ownerPool(ownerPool),
					hIoPoolObj(nullptr),
					ioTaskName("I\\O worker"),
					RefCount(1) {
				}
			};

		private:
			Data* This;

		protected:
			friend void DestroyIoPoolObj(UnifiedThreadPoolIoTask* task);

		protected:
			friend Threadables::ThreadableRef RegisterThreadable(
				const UnifiedThreadPoolIoTask& task,
				const SynchronizeEvent& event,
				ImplSpecificT instance,
				ContextsBlock& contexts);

			friend void UnRegisterThreadable(const UnifiedThreadPoolIoTask& task, Threadables::ThreadableRef&& ref);
			friend SRWLock& GetUnifiedPoolLock(UnifiedThreadPoolIoTask& task);

		protected:
			virtual void Dispose() override;

		protected:
			ForceInline UnifiedThreadPoolIoTask(Data* This) : This(MoveRValue(This)) {}

		public:
			/// <summary>
			/// Returns the task name
			/// </summary>
			/// <returns>Task name</returns>
			ForceInline const String& GetName() const { return This->ioTaskName; }

			/// <summary>
			/// Sets the new task name
			/// </summary>
			/// <param name="newName">New task name</param>
			ForceInline void SetName(const String& newName) { This->ioTaskName = newName; }

			/// <summary>
			/// Sets the new task name
			/// </summary>
			/// <param name="newName">New task name</param>
			ForceInline void SetName(String&& newName) { This->ioTaskName = MoveRValue(newName); }

			/// <summary>
			/// Returns the task pool which processes the timer
			/// </summary>
			/// <returns>Task pool which processes the timer</returns>
			UnifiedThreadPool GetOwnerPool() const;

		public:
			/// <summary>
			/// Task name
			/// </summary>
			String Property(GetName, SetName) Name;

		public:
			RefCountClass(UnifiedThreadPoolIoTask, This);

		public:
			friend class Threadables::ThreadableUnifiedThreadPoolIoTask;
		};
	}

export namespace Cardinal::IO
	{
		/// <summary>
		/// Callback type
		/// </summary>
		enum class ECallbackType {
			/// <summary>
			/// Reading
			/// </summary>
			Read,

			/// <summary>
			/// riting
			/// </summary>
			Write,

			/// <summary>
			/// Connecting
			/// </summary>
			Connect
		};

		EnumCttiDefinition(ECallbackType,
			ECallbackType::Read,
			ECallbackType::Write,
			ECallbackType::Connect);

		class IRP;

		/// <summary>
		/// The task object type for I/O tasks
		/// </summary>
		/// <typeparam name="T">The result type</typeparam>
		template<typename T>
		using IOTask = Cardinal::Threading::BaseTask<T, IRP>;

		/// <summary>
		/// The interface object type of I/O task
		/// </summary>
		using IIOTask = Cardinal::Threading::IPayloadBaseTask<IRP>;

		/// <summary>
		/// Base package class of I/O operation
		/// </summary>
		class IRP final : public IDisposable, public Threading::ITaskPayload<IRP, true> {
		public:
			friend class IAsyncIOBase;

			using IrpInit = void(*)(IIOTask& task, IRP& irp, SmartPtr<IAsyncIOBase>&);
			using IrpEnd = void(*)(IIOTask& task, IRP& irp, SmartPtr<IAsyncIOBase>&, Int32& status);

		private:
			// IO_STATUS_BLOCK
			Byte Reserved[16 + sizeof(SizeT)];

			Threading::ContextsBlock contexts;

			volatile bool isCompleted;

			Threading::UnifiedThreadPoolIoTask poolTask;

			SmartPtr<IAsyncIOBase> irpTarget;

			ImplSpecificT requestSpecific;

			IrpInit irpInit;
			IrpEnd irpEnd;

		private:
			friend void MarkIrpCompleted(IRP* irp);

			static Threading::ContextsBlock GetCurrentContextForIo();

			friend Threading::ContextsBlock& GetContext(IRP* irp);

		protected:
			virtual void Dispose() override {}

		protected:
			ForceInline IRP(
				const Threading::UnifiedThreadPoolIoTask& poolTask,
				SmartPtr<IAsyncIOBase>&& irpTarget,
				IrpInit irpInit,
				IrpEnd irpEnd,
				ImplSpecificT requestSpecific
			) :
				Reserved{},
				contexts(GetCurrentContextForIo()),
				isCompleted(false),
				poolTask(poolTask),
				irpTarget(irpTarget),
				irpInit(irpInit),
				irpEnd(irpEnd),
				requestSpecific(requestSpecific) {
			}

		public:
			/// <summary>
			/// Standard copying constructor
			/// </summary>
			/// <param name="orig">Original</param>
			IRP(const IRP& orig) = default;

			/// <summary>
			/// Standard moving constructor
			/// </summary>
			/// <param name="orig">Original</param>
			IRP(IRP&& orig) = default;

		public:
			/// <summary>
			/// Operation cancelling
			/// </summary>
			void CancelIo();

		public:
			/// <summary>
			/// Receiving the task from the thread pool
			/// </summary>
			/// <returns>Task from the thread pool</returns>
			Cardinal::Threading::UnifiedThreadPoolIoTask& GetTask() { return poolTask; }

		public:
			/// <summary>
			/// Returns whether the operation has already been completed
			/// </summary>
			/// <returns>"True" if the operation has already been completed</returns>
			ForceInline Boolean GetIsCompleted() const { return isCompleted; }

			/// <summary>
			/// "True" if the operation has already been completed
			/// </summary>
			Boolean Property(GetIsCompleted) IsCompleted;

		public:
			/// <summary>
			/// Returns packet-specific I/O data
			/// </summary>
			ForceInline ImplSpecificT GetRequestSpecificData() {
				return requestSpecific;
			}

			/// <summary>
			/// Packet-specific I/O data
			/// </summary>
			ImplSpecificT Property(GetRequestSpecificData) RequestSpecificData;

		public:
			/// <summary>
			/// Returns the address to the additional data
			/// </summary>
			/// <returns>Address to the additional data</returns>
			ImplSpecificT GetImplSpecificData() {
				return Reserved;
			}

		private:
			void InitImpl(IIOTask& task) {
				irpInit(task, *this, this->irpTarget);
			}

			void EndImpl(IIOTask& task, Int32& status) {
				irpEnd(task, *this, this->irpTarget, status);
			}

		private:
			friend void CallEndTaskIfRequired(IIOTask& task, Int32& status);

		private:
			friend class Threading::ITaskPayload<IRP, true>;

		public:
			[[msvc::noop_dtor]] virtual ~IRP() override {};
		};

		/// <summary>
		/// Base interface of asynchronous I/O object
		/// </summary>
		class IAsyncIOBase : virtual public IDisposable {
		protected:
			friend class IRP;

		protected:
			// Registering I/O object in thread pool
			static Cardinal::Threading::UnifiedThreadPoolIoTask RegisterNtIoObjectInThreadPool(const Threading::UnifiedThreadPool& pool, Handle hFileObj);

			// Remove registration I/O object in thread pool
			static void UnRegisterNtIoObjectInThreadPool(Cardinal::Threading::UnifiedThreadPoolIoTask task);

			// Return ptr to IO_STATUS_BLOCK
			static ImplSpecificT GetReservedPlaceInIrp(IRP* irp);

			static IRP MakeIrp(
				const Threading::UnifiedThreadPoolIoTask& poolTask,
				SmartPtr<IAsyncIOBase>&& irpTarget,
				IRP::IrpInit irpInit,
				IRP::IrpEnd irpEnd,
				ImplSpecificT requestSpecific
			) {
				return IRP(poolTask, MoveRValue(irpTarget), irpInit, irpEnd, requestSpecific);
			}

			// Send notify to thread pool
			static void NotifyIoOperation(const Threading::UnifiedThreadPoolIoTask& ptpIo);

			// Cancel notification of thread pool
			static void CancelNotifyIoOperation(const Threading::UnifiedThreadPoolIoTask& ptpIo);

			template<typename T>
			static Containers::Tuple<RawMemBlk, Memory::DataSizeT> GetRawBufferAndSize(IOTask<T>& task) {
				T& data = task.GetTaskDataBuffer();

				if constexpr (TypeTraits::Collection::IsArray<T>)
				{
					if constexpr (TypeTraits::IsPodV<typename T::DataType>)
					{
						using DataType = typename T::DataType;
						return { static_cast<RawMemBlk>(const_cast<DataType*>(static_cast<const DataType* const>(data))), Memory::DataSizeT(sizeof(DataType) * data.Count) };
					}
					else
					{
						static_assert(!TypeTraits::IsPodV<typename T::DataType>);
						__assume(0);
					}
				}
				else if constexpr (TypeTraits::Collection::IsString<T>)
				{
					using DataType = typename T::DataType;
					return { static_cast<RawMemBlk>(const_cast<DataType*>(static_cast<const DataType* const>(data))), data.Size };
				}
				else if constexpr (TypeTraits::IsPodV<T>)
				{
					return { static_cast<RawMemBlk>(&data), Memory::DataSizeT(sizeof(data)) };
				}
				else
				{
					static_assert(!TypeTraits::IsPodV<T> && !TypeTraits::Collection::IsString<T>);
					__assume(0);
				}
			}

		protected:
			// Receive NT object handle
			virtual Handle GetHandle() = 0;

		protected:
			virtual const Cardinal::Threading::UnifiedThreadPoolIoTask& GetIoPoolTask() const = 0;

		public:
			/// <summary>
			/// Checking if object is valid
			/// </summary>
			/// <returns>"True" if the object allows I/O operations</returns>
			virtual bool GetIsValid() const = 0;

		public:
			/// <summary>
			/// Destructor
			/// </summary>
			virtual ~IAsyncIOBase() override /*= 0*/ {}; //Ref -> IThreadable
		};

		/// <summary>
		/// The asynchronous I/O object interface
		/// </summary>
		class IBaseAsyncIO : virtual public IAsyncIOBase {
		public:
			/// <summary>
			/// Checking if object is valid
			/// </summary>
			/// <returns>"True" if the object allows I/O operations</returns>
			virtual bool GetIsValid() const override = 0;

		public:
			/// <summary>
			/// Destructor
			/// </summary>
			virtual ~IBaseAsyncIO() override /*= 0*/ {}; //Ref -> IThreadable
		};

		/// <summary>
		/// The asynchronous object connection interface
		/// </summary>
		/// <typeparam name="TAsyncObject">The connection object type</typeparam>
		template<typename TAsyncObject>
		class IBaseConnectableAsyncIO : virtual public IAsyncIOBase {
		public:
			/// <summary>
			/// Asynchronous connection
			/// </summary>
			/// <returns>The asynchronous I/O task object</returns>
			virtual IOTask<TAsyncObject> AsyncConnect() = 0;

		public:
			/// <summary>
			/// Checking if object is valid
			/// </summary>
			/// <returns>"True" if the object allows I/O operations</returns>
			virtual bool GetIsValid() const override = 0;

		public:
			/// <summary>
			/// Destructor
			/// </summary>
			virtual ~IBaseConnectableAsyncIO() override = 0 {};
		};

		/// <summary>
		/// The asynchronous I/O object interface with positional reading
		/// </summary>
		class IBasePositionAsyncIO : virtual public IAsyncIOBase {
		public:
			/// <summary>
			/// Checking if object is valid
			/// </summary>
			/// <returns>"True" if the object allows I/O operations</returns>
			virtual bool GetIsValid() const override = 0;

		public:
			/// <summary>
			/// Destructor
			/// </summary>
			virtual ~IBasePositionAsyncIO() override /*= 0*/ {}; //Ref -> IThreadable
		};
	}

#pragma warning(pop)