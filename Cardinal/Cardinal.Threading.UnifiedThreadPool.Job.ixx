export module Cardinal.Threading:UnifiedThreadPool.Job;

export import :UnifiedThreadPool.Base;

export namespace Cardinal::Threading
{
	/// <summary>
	/// The task in a task pool
	/// </summary>
	class UnifiedThreadPoolJob final : public IDisposable {
	public:
		friend class UnifiedThreadPool;
		friend class ThreadableUnifiedThreadPoolJob;

	private:
		struct Data {
			ImplSpecificT pOwnerPool;
			String jobName;
			Delegate action;
			ImplSpecificT hWork;
			ImplSpecificT hEvent;

			Containers::ConcurencyList<UnifiedThreadPoolJob>::Iterator poolQueueRef;
			ContextsBlock contexts;

			RefCountT RefCount;

			Data(
				ImplSpecificT   pOwnerPool,
				ContextsBlock&& contexts,
				String&& jobName,
				Delegate&& action,
				ImplSpecificT   hEvent
			)
				: pOwnerPool(pOwnerPool),
				jobName(MoveRValue(jobName)),
				action(MoveRValue(action)),
				hWork(nullptr),
				hEvent(hEvent),
				poolQueueRef(),
				contexts(MoveRValue(contexts)),
				RefCount(1) {
			}
		};

	private:
		Data* This;

	protected:
		virtual void Dispose() override;

	private:
		friend bool IsUnifiedThreadPoolTerminate(UnifiedThreadPoolJob* jobObj);
		friend Threadables::ThreadableRef RegisterThreadable(UnifiedThreadPoolJob* thread, ImplSpecificT callbackInstance);
		friend void UnRegisterThreadable(UnifiedThreadPoolJob* thread, Threadables::ThreadableRef&& ref);
		friend Delegate& GetJobEntryPoint(UnifiedThreadPoolJob* jobObj);

		friend SRWLock& GetUnifiedPoolLock(UnifiedThreadPoolJob* jobObj);
		friend void UnregisterJobFromJobList(UnifiedThreadPoolJob* jobObj);

	private:
		UnifiedThreadPoolJob(Data* This) : This(This) {}

	public:
		/// <summary>
		/// Returns the task name
		/// </summary>
		/// <returns>Task name</returns>
		ForceInline const String& GetName() {
			return This->jobName;
		}

		/// <summary>
		/// Sets the task name
		/// </summary>
		/// <param name="newName">New task name</param>
		ForceInline void SetName(const String& newName) {
			This->jobName = newName;
		}

		/// <summary>
		/// Sets the task name
		/// </summary>
		/// <param name="newName">New task name</param>
		ForceInline void SetName(String&& newName) {
			This->jobName = MoveRValue(newName);
		}

	public:
		/// <summary>
		/// Indicates whether the task has already been completed
		/// </summary>
		/// <returns>"True" if the task has already been completed</returns>
		ForceInline Boolean GetIsAlredyExecuted() const {
			return This->action.GetIsEnded();
		}

		/// <summary>
		/// Indicates whether the task is running
		/// </summary>
		/// <returns>"True" if the task is in progress</returns>
		ForceInline Boolean GetIsStartedExecution() const {
			return This->action.GetIsStarted();
		}

		/// <summary>
		/// Returns the thread pool in which the task is registered
		/// </summary>
		/// <returns>Thread pool object</returns>
		UnifiedThreadPool GetOwnerPool() const;

	public:
		/// <summary>
		/// Task name
		/// </summary>
		String Property(GetName, SetName) Name;

		/// <summary>
		/// Indicates whether the task has already been completed
		/// </summary>
		Boolean Property(GetIsAlredyExecuted) IsAlredyExecuted;

		/// <summary>
		/// Indicates whether the task is running
		/// </summary>
		Boolean Property(GetIsStartedExecution) IsStartedExecution;

	public:
		/// <summary>
		/// Waiting for the task to complete
		/// </summary>
		/// <returns>A reference to a current object</returns>
		UnifiedThreadPoolJob& Join();

		/// <summary>
		/// Waiting for the task to complete with timeout
		/// </summary>
		/// <param name="timeOut">Waiting timeout</param>
		UnifiedThreadPoolJob& Join(Time::TimeSpan timeOut);

		/// <summary>
		/// Waiting for the task to complete
		/// </summary>
		/// <returns>A reference to a current object</returns>
		const UnifiedThreadPoolJob& Join() const;

		/// <summary>
		/// Waiting for the task to complete with timeout
		/// </summary>
		/// <param name="timeOut">Waiting timeout</param>
		const UnifiedThreadPoolJob& Join(Time::TimeSpan timeOut) const;

	public:
		RefCountClass(UnifiedThreadPoolJob, This);

	public:
		friend class Threadables::ThreadableUnifiedThreadPoolJob;
	};
}