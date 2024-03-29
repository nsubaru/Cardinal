export module Cardinal.Threading:UnifiedThreadPool.Pool;

export import :UnifiedThreadPool.Base;
export import :UnifiedThreadPool.Job;
export import :UnifiedThreadPool.ScheduledTask;
export import :UnifiedThreadPool.IO;
export import :UnifiedThreadPool.Alpc;

export namespace Cardinal::Threading
{
	/// <summary>
	/// Thread pool
	/// </summary>
	class UnifiedThreadPool final : public IDisposable {
	private:
		friend class UnifiedThreadPoolJob;
		friend class UnifiedThreadPoolScheduledTask;
		friend class UnifiedThreadPoolScheduledTaskBuilder;
		friend class UnifiedThreadPoolIoTask;
		friend class AlpcPoolJobContainer;
		friend class IO::IPC::Alpc::IAlpcBase;

		friend class UnifiedThreadPoolBuilder;

		friend class IO::IAsyncIOBase;

	private:
		struct PoolConfigureation {
			ImplSpecificT CallbackEnviroment = nullptr;
			ImplSpecificT ClenupGroup = nullptr;
			EPoolThreadsPriority Priority = EPoolThreadsPriority::Medium;
			SRWLock PoolLock = {};
		};

		struct ImplementationSpecificData {
			ImplSpecificT PoolHandle;

			PoolConfigureation JobConfiguration;
			PoolConfigureation TaskConfiguration;
			PoolConfigureation IrpConfiguration;
			PoolConfigureation EventsConfiguration;
			PoolConfigureation SleepsConfiguration;
			PoolConfigureation AlpcConfiguration;
		};

		struct Data {
			SizeT MinThreadCount;
			SizeT MaxThreadCount;

			Containers::ConcurencyList<UnifiedThreadPoolJob> JobsQueue;
			Containers::ConcurencyList<UnifiedThreadPoolScheduledTask> TaskQueue;
			Containers::ConcurencyList<UnifiedThreadPoolIoTask> IrpQueue;
			Containers::ConcurencyList<AlpcPoolJobContainer> AlpcQueue;

			String threadPoolName;

			volatile Boolean isPoolTermination;

			ImplementationSpecificData handlesData;

			RefCountT RefCount;

			Data(
				SizeT minThreadCount,
				SizeT maxThreadCount,
				String&& threadPoolName,
				ImplementationSpecificData&& handlesData
			) :
				MinThreadCount(minThreadCount),
				MaxThreadCount(maxThreadCount),
				JobsQueue(),
				TaskQueue(),
				IrpQueue(),
				threadPoolName(MoveRValue(threadPoolName)),
				handlesData(MoveRValue(handlesData)),
				isPoolTermination(false),
				RefCount(0) {
			}
		};

	private:
		Data* This;

	protected:
		virtual void Dispose() override;

	private:
		static void DestroyPoolObject(ImplSpecificT PoolHandle);
		static void DestroyConfiguration(PoolConfigureation& poolConfiguration);

	private:
		friend bool IsUnifiedThreadPoolTerminate(UnifiedThreadPoolJob* jobObj);
		friend bool IsUnifiedThreadPoolTerminate(UnifiedThreadPoolScheduledTask* task);
		friend bool IsUnifiedThreadPoolTerminate(AlpcPoolJobContainer& jobObj);
		friend SRWLock& GetUnifiedPoolLock(UnifiedThreadPoolJob* jobObj);
		friend SRWLock& GetUnifiedPoolLock(UnifiedThreadPoolScheduledTask* task);
		friend SRWLock& GetUnifiedPoolLock(UnifiedThreadPoolIoTask& task);
		friend SRWLock& GetUnifiedPoolLock(AlpcPoolJobContainer& task);

		friend void UnregisterAlpcTaskFromAlpcTasksList(AlpcPoolJobContainer& alpcTaskObj);
		friend void UnregisterJobFromJobList(UnifiedThreadPoolJob* jobObj);
		friend void UnRegisterTaskFromPoolQueue(UnifiedThreadPoolScheduledTask* thread);
		friend ImplSpecificT GetEventsTpEnviroments(UnifiedThreadPool& pool);
		friend ImplSpecificT GetDelaysTpEnviroments(UnifiedThreadPool& pool);

		friend void TerminatePoolExecuter();
		friend void InsertIntoTerminateThreadQueue(UnifiedThreadPool::Data* This);
		friend Containers::ConcurencyDynamicQueue<UnifiedThreadPool::Data*>& GetTerminateThreadQueue();

		friend void DisposeThreadPoolData(UnifiedThreadPool::Data* This);
		friend void DisposeThreadPool(UnifiedThreadPool*& pool);
	private:
		ForceInline UnifiedThreadPool(Data*& This) :This(This) { ++This->RefCount; }

	public:
		/// <summary>
		/// Starts the tasks in thread pool
		/// </summary>
		/// <param name="jobName">Task name</param>
		/// <param name="action">Task delegate</param>
		/// <returns>Thread pool task object</returns>
		UnifiedThreadPoolJob PostJob(String&& jobName, Delegate&& action);

		/// <summary>
		/// Starts the unnamed task in thread pool
		/// </summary>
		/// <param name="action">Task delegate</param>
		/// <returns>Thread pool task object</returns>
		ForceInline UnifiedThreadPoolJob PostJob(Delegate&& action) {
			return PostJob(String(), MoveRValue(action));
		}

		/// <summary>
		/// Starts the tasks in thread pool
		/// </summary>
		/// <param name="jobName">Task name</param>
		/// <param name="action">Task delegate</param>
		/// <returns>Thread pool task object</returns>
		ForceInline UnifiedThreadPoolJob PostJob(const String& jobName, Delegate&& action) {
			return PostJob(String(jobName), MoveRValue(action));
		}

	public:
		/// <summary>
		/// Starts the unnamed timer
		/// </summary>
		/// <param name="callback">Timer callback</param>
		/// <returns>Timer construction object</returns>
		UnifiedThreadPoolScheduledTaskBuilder SheduleJob(SmartPtr<IUnifiedThreadPoolShedulerCallback>&& callback);

		/// <summary>
		/// Timer starting
		/// </summary>
		/// <param name="taskName">Timer name</param>
		/// <param name="callback">Callback</param>
		/// <returns>Timer construction object</returns>
		UnifiedThreadPoolScheduledTaskBuilder SheduleJob(String&& taskName, SmartPtr<IUnifiedThreadPoolShedulerCallback>&& callback);

		/// <summary>
		/// Timer starting
		/// </summary>
		/// <param name="taskName">Timer name</param>
		/// <param name="callback">Callback</param>
		/// <returns>Timer construction object</returns>
		UnifiedThreadPoolScheduledTaskBuilder SheduleJob(const String& taskName, SmartPtr<IUnifiedThreadPoolShedulerCallback>&& callback);

	public:
		/// <summary>
		/// Returns the number of registered tasks in pool
		/// </summary>
		/// <returns>Number of registered tasks in pool</returns>
		ForceInline SizeT GetJobsCount() const { return This->JobsQueue.Count; }

		/// <summary>
		/// Returns the number of registered timers in pool
		/// </summary>
		/// <returns>Number of registered timers in pool</returns>
		ForceInline SizeT GetScheduledTasksCount() const { return This->TaskQueue.Count; }

		/// <summary>
		/// Returns the number of registered I/O objects in pool
		/// </summary>
		/// <returns>Number of registered I/O objects in pool</returns>
		ForceInline SizeT GetIORequestsCount() const { return This->IrpQueue.Count; }

		/// <summary>
		/// Returns the number of registered ALPC objects in pool
		/// </summary>
		/// <returns>Number of registered ALPC objects in pool</returns>
		ForceInline SizeT GetAlpcRequestsCount() const { return This->AlpcQueue.Count; }

	public:
		/// <summary>
		/// Returns the task stack
		/// </summary>
		/// <returns>Task stack in a list view</returns>
		Containers::List<UnifiedThreadPoolJob> GetJobsQueue() const { return This->JobsQueue.CopyTo<Containers::List>(); }

		/// <summary>
		/// Returns the list of timers
		/// </summary>
		/// <returns>List of timers</returns>
		Containers::List<UnifiedThreadPoolScheduledTask> GetScheduledTaskList() const { return This->TaskQueue.CopyTo<Containers::List>(); }

		/// <summary>
		/// Returns the list of I/O objects
		/// </summary>
		/// <returns>List of I/O objects</returns>
		Containers::List<UnifiedThreadPoolIoTask> GetAsyncIOList() const { return This->IrpQueue.CopyTo<Containers::List>(); }

		/// <summary>
		/// Returns the list of ALPC objects
		/// </summary>
		/// <returns>List of ALPC objects</returns>
		Containers::List<AlpcPoolJobContainer> GetAsyncAlpcList() const { return This->AlpcQueue.CopyTo<Containers::List>(); }

	public:
		/// <summary>
		/// Number of registered tasks in pool
		/// </summary>
		SizeT Property(GetJobsCount) JobsCount;

		/// <summary>
		/// Number of registered timers in pool
		/// </summary>
		SizeT Property(GetScheduledTasksCount) ScheduledTasksCount;

		/// <summary>
		/// Number of registered I/O objects in pool
		/// </summary>
		SizeT Property(GetIORequestsCount) IORequestsCount;

		/// <summary>
		/// Number of registered ALPC objects in pool
		/// </summary>
		SizeT Property(GetAlpcRequestsCount) AlpcRequestsCount;

	public:
		/// <summary>
		/// Returns the task pool name
		/// </summary>
		/// <returns>Task pool name</returns>
		ForceInline const String& GetName() const { return This->threadPoolName; }

		/// <summary>
		/// Sets the new task pool name
		/// </summary>
		/// <param name="newName">New task pool name</param>
		ForceInline void SetName(const String& newName) { This->threadPoolName = newName; }

		/// <summary>
		/// Sets the new task pool name
		/// </summary>
		/// <param name="newName">New task pool name</param>
		ForceInline void SetName(String&& newName) { This->threadPoolName = MoveRValue(newName); }

	public:
		/// <summary>
		/// Task pool name
		/// </summary>
		String Property(GetName, SetName) Name;

	public:
		/// <summary>
		/// Returns the task priority in thread pool
		/// </summary>
		/// <returns>Task priority in thread pool</returns>
		ForceInline EPoolThreadsPriority GetJobsPriority() const { return This->handlesData.JobConfiguration.Priority; }

		/// <summary>
		/// Returns the timers proirity in thread pool
		/// </summary>
		/// <returns>Proirity of timers in thread pool</returns>
		ForceInline EPoolThreadsPriority GetScheduledTasksPriority() const { return This->handlesData.TaskConfiguration.Priority; }

		/// <summary>
		/// Returns the informing priority about completing the I/O operations in thread pools
		/// </summary>
		/// <returns>Informing priority about completing the I/O operations in thread pools</returns>
		ForceInline EPoolThreadsPriority GetIORequestsPriority() const { return This->handlesData.IrpConfiguration.Priority; }

		/// <summary>
		/// Returns the informing priority about completing the I/O operations in thread pools
		/// </summary>
		/// <returns>Informing priority about completing the ALPC operations in thread pools</returns>
		ForceInline EPoolThreadsPriority GetAlpcRequestsPriority() const { return This->handlesData.AlpcConfiguration.Priority; }

	public:
		/// <summary>
		/// Task priority in thread pool
		/// </summary>
		EPoolThreadsPriority Property(GetJobsPriority) JobsPriority;

		/// <summary>
		/// Timers priority in thread pool
		/// </summary>
		EPoolThreadsPriority Property(GetScheduledTasksPriority) ScheduledTasksPriority;

		/// <summary>
		/// Informing priority about completing the I/O operations in thread pools 
		/// </summary>
		EPoolThreadsPriority Property(GetIORequestsPriority) IORequestsPriority;

		/// <summary>
		/// Informing priority about completing the ALPC operations in thread pools 
		/// </summary>
		EPoolThreadsPriority Property(GetAlpcRequestsPriority) AlpcRequestsPriority;

	public:
		/// <summary>
		/// Returns the maximal number of threads in pool
		/// </summary>
		/// <returns>Maximal number of threads in pool</returns>
		ForceInline UInt64 GetMaxThreadCount() const { return This->MaxThreadCount; }

		/// <summary>
		/// Returns the minimal number of threads in pool
		/// </summary>
		/// <returns>Minimal number of threads in pool</returns>
		ForceInline UInt64 GetMinThreadCount() const { return This->MinThreadCount; }

	public:
		/// <summary>
		/// Maximal number of threads in pool
		/// </summary>
		UInt64 Property(GetMaxThreadCount) MaxThreadCount;

		/// <summary>
		/// Minimal number of threads in pool
		/// </summary>
		UInt64 Property(GetMinThreadCount) MinThreadCount;

	public:
		RefCountClass(UnifiedThreadPool, This);
	};
}