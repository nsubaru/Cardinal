export module Cardinal.Threading:SystemThreadPool;

export import :UnifiedThreadPool;

export namespace Cardinal::Threading::SystemThreadPool
{
#pragma region Jobs
	/// <summary>
	/// Task launch in a system task pool
	/// </summary>
	/// <param name="action">Task callback</param>
	/// <param name="name">Task name</param>
	/// <param name="priority">Task priority</param>
	/// <returns>Task object</returns>
	UnifiedThreadPoolJob PostPriorityJob(Delegate&& action, String&& name, Threading::EPoolThreadsPriority priority = Threading::EPoolThreadsPriority::Medium);

	/// <summary>
	/// Task launch in a system task pool
	/// </summary>
	/// <param name="action">Task callback</param>
	/// <param name="name">Task name</param>
	/// <param name="priority">Task priority</param>
	/// <returns>Task object</returns>
	UnifiedThreadPoolJob PostPriorityJob(Delegate&& action, const String& name, Threading::EPoolThreadsPriority priority = Threading::EPoolThreadsPriority::Medium);

	/// <summary>
	/// Unnamed task launch in a system task pool
	/// </summary>
	/// <param name="action">Task callback</param>
	/// <param name="priority">Task priority</param>
	/// <returns>Task object</returns>
	UnifiedThreadPoolJob PostPriorityJob(Delegate&& action, Threading::EPoolThreadsPriority priority = Threading::EPoolThreadsPriority::Medium);
#pragma endregion

#pragma region Scheduled task
	/// <summary>
	/// Timer starting in a system task pool
	/// </summary>
	/// <param name="callback">Timer callback</param>
	/// <param name="name">Timer name</param>
	/// <param name="priority">Timer priority</param>
	/// <returns>Timer constructor</returns>
	UnifiedThreadPoolScheduledTaskBuilder CreatePriorityScheduledTask(SmartPtr<IUnifiedThreadPoolShedulerCallback>&& callback, String&& name, Threading::EPoolThreadsPriority priority = Threading::EPoolThreadsPriority::Medium);

	/// <summary>
	/// Timer starting in a system task pool
	/// </summary>
	/// <param name="callback">Timer callback</param>
	/// <param name="name">Timer name</param>
	/// <param name="priority">Timer priority</param>
	/// <returns>Timer constructor</returns>
	UnifiedThreadPoolScheduledTaskBuilder CreatePriorityScheduledTask(SmartPtr<IUnifiedThreadPoolShedulerCallback>&& callback, const String& name, Threading::EPoolThreadsPriority priority = Threading::EPoolThreadsPriority::Medium);

	/// <summary>
	/// Starts the unnamed timer in a system task pool
	/// </summary>
	/// <param name="callback">Timer callback</param>
	/// <param name="priority">Timer priority</param>
	/// <returns>Timer constructor</returns>
	UnifiedThreadPoolScheduledTaskBuilder CreatePriorityScheduledTask(SmartPtr<IUnifiedThreadPoolShedulerCallback>&& callback, Threading::EPoolThreadsPriority priority = Threading::EPoolThreadsPriority::Medium);
#pragma endregion

#pragma region Statistic
	/// <summary>
	/// Returns the total number of objects in a system task pool
	/// </summary>
	/// <returns>Total number of objects in a system task pool</returns>
	SizeT AllItemCount();

	/// <summary>
	/// Returns the number of objects in a system task pool with a specified priority
	/// </summary>
	/// <param name="priority">Priority of objects</param>
	/// <returns>Number of objects in a system task pool with a specified priority</returns>
	SizeT AllItemSpecificPriorityCount(Threading::EPoolThreadsPriority priority = Threading::EPoolThreadsPriority::Medium);

	/// <summary>
	/// Returns the number of tasks in a system pool
	/// </summary>
	/// <returns>Number of tasks in a system pool</returns>
	SizeT JobCount();

	/// <summary>
	/// Returns the number of tasks with specified priority in a system pool
	/// </summary>
	/// <param name="priority">Task proirity</param>
	/// <returns>Number of tasks with specified priority in a system pool</returns>
	SizeT JobSpecificPriorityCount(Threading::EPoolThreadsPriority priority = Threading::EPoolThreadsPriority::Medium);

	/// <summary>
	/// Returns the number of timers in a system pool
	/// </summary>
	/// <returns>Number of timers in a system pool</returns>
	SizeT ScheduledTaskCount();

	/// <summary>
	/// Returns the number of timers in a system pool with specified priority
	/// </summary>
	/// <param name="priority">Priority of timers</param>
	/// <returns>Number of timers in a system pool with specified priority</returns>
	SizeT ScheduledTaskSpecificPriorityCount(Threading::EPoolThreadsPriority priority = Threading::EPoolThreadsPriority::Medium);

	/// <summary>
	/// Returns the number of file objects in a system pool
	/// </summary>
	/// <returns>Number of file objects in a system pool</returns>
	SizeT IoTaskCount();

	/// <summary>
	/// Returns the number of file objects in a system pool with specified priority
	/// </summary>
	/// <param name="priority">Priority of file objects</param>
	/// <returns>Number of file objects in a system pool with specified priority</returns>
	SizeT IoTaskSpecificPriorityCount(Threading::EPoolThreadsPriority priority = Threading::EPoolThreadsPriority::Medium);
#pragma endregion

#pragma region System Thread Pools objest getters
	/// <summary>
	/// Returns the system pool object with the specified thread priority
	/// </summary>
	/// <param name="priority">Thread pool priority which need to receive</param>
	/// <returns>Thread pool object</returns>
	UnifiedThreadPool GetThreadPoolByPoolThreadsPriority(Threading::EPoolThreadsPriority priority = Threading::EPoolThreadsPriority::Medium);
#pragma endregion 
}