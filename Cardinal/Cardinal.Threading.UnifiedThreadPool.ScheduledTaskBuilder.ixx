export module Cardinal.Threading:UnifiedThreadPool.ScheduledTaskBuilder;

export import :UnifiedThreadPool.Base;
export import :UnifiedThreadPool.Job;
export import :UnifiedThreadPool.ScheduledTask;
export import :UnifiedThreadPool.IO;
export import :UnifiedThreadPool.Pool;

export namespace Cardinal::Threading
{
	/// <summary>
	/// The constructor class of timer thread pool
	/// </summary>
	class UnifiedThreadPoolScheduledTaskBuilder final {
	private:
		friend class UnifiedThreadPool;

	private:
		struct Data {
			String taskName;
			UInt16 intervalsCount;
			Time::TimeSpan proximity;
			Time::TimeSpan intervalLenght;
			UInt64 startTime;
			Boolean isRelaive;
			SmartPtr<IUnifiedThreadPoolShedulerCallback> callback;
			UnifiedThreadPool ownerPool;

			Data(UnifiedThreadPool& ownerPool, SmartPtr<IUnifiedThreadPoolShedulerCallback>&& callback) :
				taskName(),
				intervalsCount(0),
				proximity(CurrentSettings().KernelServices.TaskSheduler.DefaultTimerProximity),
				intervalLenght(0),
				startTime(0),
				isRelaive(true),
				callback(MoveRValue(callback)),
				ownerPool(ownerPool) {
			}

			Data(UnifiedThreadPool& ownerPool, SmartPtr<IUnifiedThreadPoolShedulerCallback>&& callback, String&& taskName) :
				taskName(MoveRValue(taskName)),
				intervalsCount(0),
				proximity(CurrentSettings().KernelServices.TaskSheduler.DefaultTimerProximity),
				intervalLenght(0),
				startTime(0),
				isRelaive(true),
				callback(MoveRValue(callback)),
				ownerPool(ownerPool) {
			}

			Data(UnifiedThreadPool& ownerPool, SmartPtr<IUnifiedThreadPoolShedulerCallback>&& callback, const String& taskName) :
				taskName(taskName),
				intervalsCount(0),
				proximity(CurrentSettings().KernelServices.TaskSheduler.DefaultTimerProximity),
				intervalLenght(0),
				startTime(0),
				isRelaive(true),
				callback(MoveRValue(callback)),
				ownerPool(ownerPool) {
			}
		};

	private:
		Data* This;

	private:
		/// <summary>
		/// Creates the configuration of unnamed timer
		/// with the proximity of 100ms, one-time startup immediately after creation
		/// and specified callback
		/// </summary>
		/// <param name="ownerPool">Thread pool</param>
		/// <param name="callback">Callback</param>
		ForceInline UnifiedThreadPoolScheduledTaskBuilder(UnifiedThreadPool& ownerPool, SmartPtr<IUnifiedThreadPoolShedulerCallback>&& callback)
			: This(new Data(ownerPool, MoveRValue(callback))) {}

		/// <summary>
		/// Creates the configuration of named timer
		/// with the proximity of 100ms, one-time startup immediately after creation
		/// and specified callback
		/// </summary>
		/// <param name="ownerPool">Thread pool</param>
		/// <param name="callback">Callback</param>
		/// <param name="taskName">Task name</param>
		ForceInline UnifiedThreadPoolScheduledTaskBuilder(UnifiedThreadPool& ownerPool, SmartPtr<IUnifiedThreadPoolShedulerCallback>&& callback, String&& taskName)
			: This(new Data(ownerPool, MoveRValue(callback), MoveRValue(taskName))) {}

		/// <summary>
		/// Creates the configuration of named timer
		/// with the proximity of 100ms, one-time startup immediately after creation
		/// and specified callback
		/// </summary>
		/// <param name="ownerPool">Thread pool</param>
		/// <param name="callback">Callback</param>
		/// <param name="taskName">Task name</param>
		ForceInline UnifiedThreadPoolScheduledTaskBuilder(UnifiedThreadPool& ownerPool, SmartPtr<IUnifiedThreadPoolShedulerCallback>&& callback, const String& taskName)
			: This(new Data(ownerPool, MoveRValue(callback), taskName)) {}

	public:
		/// <summary>
		/// Sets new callback
		/// </summary>
		/// <param name="callback">New callback</param>
		/// <returns>"RValue" reference to a current object</returns>
		ForceInline UnifiedThreadPoolScheduledTaskBuilder&& SetCallback(SmartPtr<IUnifiedThreadPoolShedulerCallback>&& callback)&& {
			This->callback = MoveRValue(callback);
			return MoveRValue(*this);
		}

	public:
		/// <summary>
		/// Sets the timer name
		/// </summary>
		/// <param name="name">New timer name</param>
		/// <returns>"RValue" reference to a current object</returns>
		ForceInline UnifiedThreadPoolScheduledTaskBuilder&& SetName(String&& name)&& {
			This->taskName = MoveRValue(name);
			return MoveRValue(*this);
		}

		/// <summary>
		/// Sets the timer name
		/// </summary>
		/// <param name="name">New timer name</param>
		/// <returns>"RValue" reference to a current object</returns>
		ForceInline UnifiedThreadPoolScheduledTaskBuilder&& SetName(const String& name)&& {
			This->taskName = name;
			return MoveRValue(*this);
		}

	public:
		/// <summary>
		/// Sets the number of timer launches
		/// </summary>
		/// <param name="count">Number of timer launches</param>
		/// <returns>"RValue" reference to a current object</returns>
		ForceInline UnifiedThreadPoolScheduledTaskBuilder&& SetExecutionCount(UInt16 count)&& {
			This->intervalsCount = count;
			return MoveRValue(*this);
		}

		/// <summary>
		/// Sets the interval length between timer launches
		/// </summary>
		/// <param name="intervalLenght">The length between intervals</param>
		/// <returns>"RValue" reference to a current object</returns>
		ForceInline UnifiedThreadPoolScheduledTaskBuilder&& SetIntervalLenght(Time::TimeSpan intervalLenght)&& {
			if (This->intervalLenght > MaxIntervalLenght)
				Throw(InvalidArgumentException(SourcePosition, L"IntervalLenght", L"Too big interval lenght"));
			This->intervalLenght = intervalLenght;
			return MoveRValue(*this);
		}

		/// <summary>
		/// Sets the absolute time of timer launch
		/// </summary>
		/// <param name="absoluteStartTime">Absolute time of timer launch</param>
		/// <returns>"RValue" reference to a current object</returns>
		ForceInline UnifiedThreadPoolScheduledTaskBuilder&& SetStartTime(Time::DateTime absoluteStartTime)&& {
			This->startTime = absoluteStartTime.TimeFromStartEpoch;
			This->isRelaive = false;
			return MoveRValue(*this);
		}

		/// <summary>
		/// Sets the relative time of timer launch
		/// </summary>
		/// <param name="relativeStartTime">Relative time of timer launch</param>
		/// <returns>"RValue" reference to a current object</returns>
		ForceInline UnifiedThreadPoolScheduledTaskBuilder&& SetStartTime(Time::TimeSpan relativeStartTime)&& {
			This->startTime = relativeStartTime.Ticks;
			This->isRelaive = true;
			return MoveRValue(*this);
		}

		/// <summary>
		/// Sets the timer proximity
		/// </summary>
		/// <param name="proximity">Timer precision</param>
		/// <returns>"RValue" reference to a current object</returns>
		ForceInline UnifiedThreadPoolScheduledTaskBuilder&& SetProximity(Time::TimeSpan proximity)&& {
			This->proximity = proximity;
			return MoveRValue(*this);
		}

	public:
		/// <summary>
		/// Creates the timer with specified configurations
		/// </summary>
		/// <returns>Timer task object</returns>
		UnifiedThreadPoolScheduledTask Shedule()&&;

	public:
		/// <summary>
		/// Maximal allowable interval between timer launches
		/// </summary>
		static constexpr auto MaxIntervalLenght = UnifiedThreadPoolScheduledTask::MaxIntervalLenght;

	public:
		/// <summary>
		/// Destructor
		/// </summary>
		~UnifiedThreadPoolScheduledTaskBuilder() {
			delete This;
		}
	};

	class ThreadableUnifiedThreadPoolJob;

	ForceInline UnifiedThreadPoolScheduledTaskBuilder UnifiedThreadPool::SheduleJob(SmartPtr<IUnifiedThreadPoolShedulerCallback>&& callback) {
		return UnifiedThreadPoolScheduledTaskBuilder(*this, MoveRValue(callback));
	}

	ForceInline UnifiedThreadPoolScheduledTaskBuilder UnifiedThreadPool::SheduleJob(String&& taskName, SmartPtr<IUnifiedThreadPoolShedulerCallback>&& callback) {
		return UnifiedThreadPoolScheduledTaskBuilder(*this, MoveRValue(callback), MoveRValue(taskName));
	}

	ForceInline UnifiedThreadPoolScheduledTaskBuilder UnifiedThreadPool::SheduleJob(const String& taskName, SmartPtr<IUnifiedThreadPoolShedulerCallback>&& callback) {
		return UnifiedThreadPoolScheduledTaskBuilder(*this, MoveRValue(callback), String(taskName));
	}
}