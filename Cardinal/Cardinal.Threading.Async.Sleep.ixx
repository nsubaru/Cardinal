//export module Cardinal.Threading:Async.Sleep;
//
//export import :Async.Base;
//export import :UnifiedThreadPool;
//
//export namespace Cardinal::Threading
//{
//	class AwaitTimePayload;
//
//	/// <summary>
//	/// Asynchronous await object of certain period of time
//	/// </summary>
//	using SleepTask = BaseTask<void, AwaitTimePayload>;
//
//	/// <summary>
//	/// Asynchronous await object of certain period of time
//	/// </summary>
//	class AwaitTimePayload {
//	private:
//		ContextsBlock context;
//		Time::TimeSpan delay;
//		Time::TimeSpan timerProximity;
//
//	private:
//		AwaitTimePayload(Time::TimeSpan delay, Time::TimeSpan timerProximity);
//
//	private:
//		friend ContextsBlock& GetContext(AwaitTimePayload& payload);
//
//	public:
//		/// <summary>
//		/// Returns the time interval to wait
//		/// </summary>
//		NoDiscard Time::TimeSpan GetDelay() const { return delay; }
//
//		/// <summary>
//		/// Time interval to wait
//		/// </summary>
//		Time::TimeSpan Property(GetDelay) Delay;
//
//		/// <summary>
//		/// Returns the waiting timer precision
//		/// </summary>
//		NoDiscard Time::TimeSpan GetTimerProximity() const { return timerProximity; }
//
//		/// <summary>
//		/// Waiting timer precision
//		/// </summary>
//		Time::TimeSpan Property(GetTimerProximity) TimerProximity;
//
//	public:
//		friend SleepTask AwaitSleep(Time::TimeSpan delay, Time::TimeSpan timerProximity, const UnifiedThreadPool& pool);
//		friend SleepTask AwaitSleep(Time::TimeSpan delay, Time::TimeSpan timerProximity, UnifiedThreadPool&& pool);
//	};
//
//	/// <summary>
//	/// Creation the task of asynchronous timer awaiting
//	/// </summary>
//	/// <param name="delay">Time interval to wait</param>
//	/// <param name="timerProximity">Waiting timer precision</param>
//	/// <param name="pool">Task pool which performs the await</param>
//	/// <returns>Task object</returns>
//	SleepTask AwaitSleep(Time::TimeSpan delay, Time::TimeSpan timerProximity, const UnifiedThreadPool& pool);
//
//	/// <summary>
//	/// Creation the task of asynchronous timer awaiting
//	/// </summary>
//	/// <param name="delay">Time interval to wait</param>
//	/// <param name="timerProximity">Waiting timer precision</param>
//	/// <param name="pool">Task pool which performs the await</param>
//	/// <returns>Task object</returns>
//	SleepTask AwaitSleep(Time::TimeSpan delay, Time::TimeSpan timerProximity, UnifiedThreadPool&& pool);
//
//	/// <summary>
//	/// Creation the task of asynchronous timer awaiting with standard timer precision
//	/// </summary>
//	/// <param name="delay">Time interval to wait</param>
//	/// <param name="pool">Task pool which performs the await</param>
//	/// <returns>Task object</returns>
//	ForceInline SleepTask AwaitSleep(Time::TimeSpan delay, const UnifiedThreadPool& pool) {
//		return AwaitSleep(delay, CurrentSettings().KernelServices.TaskSheduler.DefaultTimerProximity, pool);
//	}
//
//	/// <summary>
//	/// Creation the task of asynchronous timer awaiting with standard timer precision
//	/// </summary>
//	/// <param name="delay">Time interval to wait</param>
//	/// <param name="pool">Task pool which performs the await</param>
//	/// <returns>Task object</returns>
//	ForceInline SleepTask AwaitSleep(Time::TimeSpan delay, UnifiedThreadPool&& pool) {
//		return AwaitSleep(delay, CurrentSettings().KernelServices.TaskSheduler.DefaultTimerProximity, MoveRValue(pool));
//	}
//}
//
///// <summary>
///// Running the asynchronous awaiting with standard thread pool and timer precision
///// </summary>
///// <param name="delay">Time interval to wait</param>
///// <returns>Void</returns>
//inline auto operator co_await(Cardinal::Time::TimeSpan delay) {
//	using namespace Cardinal;
//	using namespace Cardinal::Threading;
//
//	return TaskAwaiter<void, AwaitTimePayload>{
//		AwaitSleep(
//			delay,
//			CurrentSettings().KernelServices.TaskSheduler.DefaultTimerProximity,
//			Contexts::GetContext<ThreadPoolThreadContext>().ThreadPool)
//	};
//}