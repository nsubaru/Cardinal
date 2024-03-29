export module Cardinal.Threading:UnifiedThreadPool.ScheduledTask;

export import :UnifiedThreadPool.Base;

export namespace Cardinal::Threading
{
	/// <summary>
	/// The callback of task timer in pool
	/// </summary>
	class IUnifiedThreadPoolShedulerCallback : public IDisposable {
	public:
		/// <summary>
		/// Called when the specified time comes
		/// </summary>
		virtual void Shedule() = 0;

	public:
		/// <summary>
		/// Destructor
		/// </summary>
		ForceInline virtual ~IUnifiedThreadPoolShedulerCallback() override /*= 0*/ {}; //Ref -> IThreadable
	};

	/// <summary>
	/// Timer in thread pool
	/// </summary>
	class UnifiedThreadPoolScheduledTask final : public IDisposable {
	private:
		friend class UnifiedThreadPoolScheduledTaskBuilder;
		friend class ThreadableUnifiedThreadPoolScheduledTask;

	private:
		struct Data {
			String taskName;
			SmartPtr<IUnifiedThreadPoolShedulerCallback> callback;

			ImplSpecificT ownerPool;
			Containers::ConcurencyList<UnifiedThreadPoolScheduledTask>::Iterator poolQueueRef;

			ContextsBlock contexts;

			Time::DateTime startTime;
			Time::TimeSpan intervalLenght;
			Time::DateTime lastStartTime;

			RefCountT intervalsCount;
			RefCountT executedCount;
			RefCountT remainingCount;
			Int32 timerProximity;

			RefCountT activeInstanceRefCount;

			Handle hTask;

			RefCountT RefCount;

			Data(
				String&& taskName,
				SmartPtr<IUnifiedThreadPoolShedulerCallback>&& callback,
				ContextsBlock&& contexts,
				ImplSpecificT                                  ownerPool,
				Time::DateTime                                 startTime,
				Time::TimeSpan                                 intervalLenght,
				UInt64                                         intervalsCount,
				Int32                                          timerProximity
			)
				: taskName(MoveRValue(taskName)),
				callback(MoveRValue(callback)),
				ownerPool(ownerPool),
				poolQueueRef(),
				contexts(MoveRValue(contexts)),
				startTime(startTime),
				intervalLenght(intervalLenght),
				lastStartTime(0),
				intervalsCount(intervalsCount),
				executedCount(0),
				remainingCount(intervalsCount),
				timerProximity(timerProximity),
				activeInstanceRefCount(0),
				hTask(nullptr),
				RefCount(1) {
			}
		};

	private:
		Data* This;

	protected:
		virtual void Dispose() override;

	private:
		ForceInline UnifiedThreadPoolScheduledTask(Data* This) : This(This) {}

	private:
		friend bool IsUnifiedThreadPoolTerminate(UnifiedThreadPoolScheduledTask* task);
		friend SRWLock& GetUnifiedPoolLock(UnifiedThreadPoolScheduledTask* task);
		friend SmartPtr<IUnifiedThreadPoolShedulerCallback>& GetJobEntryPoint(UnifiedThreadPoolScheduledTask* task);
		friend void ZeroingTaskHandle(UnifiedThreadPoolScheduledTask* thread);

	private:
		friend Threadables::ThreadableRef RegisterThreadable(UnifiedThreadPoolScheduledTask* thread, ImplSpecificT callbackInstance);
		friend void UnRegisterThreadable(UnifiedThreadPoolScheduledTask* thread, Threadables::ThreadableRef&& ref);
		friend void UnRegisterTaskFromPoolQueue(UnifiedThreadPoolScheduledTask* thread);

	public:
		/// <summary>
		/// Returns the timer name
		/// </summary>
		/// <returns>Timer text name</returns>
		ForceInline const String& GetName() const { return This->taskName; }

		/// <summary>
		/// Sets the new timer name
		/// </summary>
		/// <param name="newTaskName">New timer name</param>
		void SetName(String&& newTaskName);

		/// <summary>
		/// Sets the new timer name
		/// </summary>
		/// <param name="newTaskName">New timer name</param>
		ForceInline void SetName(const String& newTaskName) { SetName(String(newTaskName)); }

	public:
		/// <summary>
		/// Timer name
		/// </summary>
		String Property(GetName, SetName) Name;

	public:
		/// <summary>
		/// Returns the number of passed cycles
		/// </summary>
		/// <returns>Number of passed cycles</returns>
		ForceInline SizeT GetExecutedCount() const { return This->executedCount; }

		/// <summary>
		/// Returns the last timer call time
		/// </summary>
		/// <returns>Last timer call time</returns>
		ForceInline Time::DateTime GetLastStartTime() const { return This->lastStartTime; }

		/// <summary>
		/// Returns whether the timer task is active
		/// </summary>
		/// <returns>If the timer task is active</returns>
		ForceInline Boolean GetIsActive() const { return This->activeInstanceRefCount != 0; }

		/// <summary>
		/// Returns the number of active instances of timer task
		/// </summary>
		/// <returns>Current number of active instances of timer task</returns>
		ForceInline UInt64 GetActiveInstancesCount() const { return This->activeInstanceRefCount; }

		/// <summary>
		/// Returns the task pool which processes the timer
		/// </summary>
		/// <returns>Task pool which processes the timer</returns>
		UnifiedThreadPool GetOwnerPool() const;

		/// <summary>
		/// Returns the timer precision
		/// </summary>
		/// <returns>Timer precision</returns>
		ForceInline Time::TimeSpan GetTimerProximity() const { return Time::TimeSpan(This->timerProximity); }

		/// <summary>
		/// Indicates the time of first launch
		/// </summary>
		/// <returns>Time of first launch</returns>
		ForceInline Time::DateTime GetFirstStartTime() const { return This->startTime; }

		/// <summary>
		/// Returns the callback instance
		/// </summary>
		/// <returns>Callback instance</returns>
		ForceInline const SmartPtr<IUnifiedThreadPoolShedulerCallback>& GetCallbackObject() const { return This->callback; }

	public:
		/// <summary>
		/// Number of passed cycles
		/// </summary>
		SizeT Property(GetExecutedCount) ExecutedCount;

		/// <summary>
		/// Last timer call time
		/// </summary>
		Time::DateTime Property(GetLastStartTime) LastStartTime;

		/// <summary>
		/// If timer task is active now
		/// </summary>
		Boolean Property(GetIsActive) IsActive;

		/// <summary>
		/// Returns the task pool which processes the timer
		/// </summary>
		UnifiedThreadPool Property(OwnerPool) OwnerPool;

		/// <summary>
		/// Timer precision
		/// </summary>
		Time::TimeSpan Property(GetTimerProximity) TimerProximity;

		/// <summary>
		/// Time of first launch
		/// </summary>
		Time::DateTime Property(GetFirstStartTime)FirstStartTime;

		/// <summary>
		/// Callback instance
		/// </summary>
		SmartPtr<IUnifiedThreadPoolShedulerCallback> Property(GetCallbackObject) CallbackObject;

	public:
		/// <summary>
		/// Stops the timer
		/// </summary>
		void StopSheduling()&&;

	public:
		/// <summary>
		/// Maximal interval between timer task starts
		/// </summary>
		static constexpr auto MaxIntervalLenght = Time::TimeSpan().AddMiliSeconds(Math::Max<Int32>());

	public:
		RefCountClass(UnifiedThreadPoolScheduledTask, This);

	public:
		friend class Threadables::ThreadableUnifiedThreadPoolScheduledTask;
	};
}