export module Cardinal.Threading:UnifiedThreadPool.Base;

export import :Base;
export import :IThreadable;
export import :Context;
export import :Exceptions;

export import :ScopedLock;
export import :SynchronizeEvent;
export import :Async.Base;

export import Cardinal.Containers.Delegate;

export namespace Cardinal::Threading
{
	class IThreadPoolObject;

	class UnifiedThreadPoolJob;
	class UnifiedThreadPoolScheduledTask;
	class UnifiedThreadPoolScheduledTaskBuilder;
	class UnifiedThreadPoolIoTask;

	class UnifiedThreadPoolBuilder;
	class UnifiedThreadPool;

	/// <summary>
	/// The thread pool task priority
	/// </summary>
	enum class EPoolThreadsPriority {
		/// <summary>
		/// Low priority
		/// </summary>
		Low,

		/// <summary>
		/// Medium (standard) priority
		/// </summary>
		Medium,

		/// <summary>
		/// High priority
		/// </summary>
		High
	};

	EnumCttiDefinition(EPoolThreadsPriority,
		EPoolThreadsPriority::Low,
		EPoolThreadsPriority::Medium,
		EPoolThreadsPriority::High
	);
}