export module Cardinal.Threading:UnifiedThreadPool.PoolBuilder;

export import :UnifiedThreadPool.Base;
export import :UnifiedThreadPool.Job;
export import :UnifiedThreadPool.ScheduledTask;
export import :UnifiedThreadPool.IO;
export import :UnifiedThreadPool.Pool;

export namespace Cardinal::Threading
{
	/// <summary>
	/// Thread pool constructor
	/// </summary>
	class UnifiedThreadPoolBuilder final {
	private:
		struct Data {
			String threadPoolName;
			UInt32 minThreadCount;
			UInt32 maxThreadCount;
			EPoolThreadsPriority jobsPriority;
			EPoolThreadsPriority sheduledTasksPriority;
			EPoolThreadsPriority irpRequestPriority;
			EPoolThreadsPriority eventsRequestPriority;
			EPoolThreadsPriority sleepsRequestPriority;
			EPoolThreadsPriority alpcRequestPriority;

			Data(
				String&& threadPoolName,
				UInt32 minThreadCount,
				UInt32 maxThreadCount,
				EPoolThreadsPriority jobsPriority,
				EPoolThreadsPriority sheduledTasksPriority,
				EPoolThreadsPriority irpRequestPriority,
				EPoolThreadsPriority eventsRequestPriority,
				EPoolThreadsPriority sleepsRequestPriority,
				EPoolThreadsPriority alpcRequestPriority
			) :
				threadPoolName(MoveRValue(threadPoolName)),
				minThreadCount(minThreadCount),
				maxThreadCount(maxThreadCount),
				jobsPriority(jobsPriority),
				sheduledTasksPriority(sheduledTasksPriority),
				irpRequestPriority(irpRequestPriority),
				eventsRequestPriority(eventsRequestPriority),
				sleepsRequestPriority(sleepsRequestPriority),
				alpcRequestPriority(alpcRequestPriority) {
			}
		};

	private:
		Data* This;

	public:
		/// <summary>
		/// An empty constructor of thread pool, 
		/// initializes the settings of unnamed thread pool:
		/// with the minimal number of threads which equals to half of host processor cores;
		/// with the maximal number of threads which equals to number of host processor cores;
		/// standard priority of all tasks types
		/// </summary>
		ForceInline UnifiedThreadPoolBuilder() :This(new Data(
			String(),
			static_cast<UInt32>(CurrentSettings().VirtualSetting.CpuCoresCount) / 2,
			static_cast<UInt32>(CurrentSettings().VirtualSetting.CpuCoresCount),
			EPoolThreadsPriority::Medium,
			EPoolThreadsPriority::Medium,
			EPoolThreadsPriority::Medium,
			EPoolThreadsPriority::Medium,
			EPoolThreadsPriority::Medium,
			EPoolThreadsPriority::Medium)) {
		}

		/// <summary>
		/// An empty constructor of thread pool, 
		/// initializes the settings of named thread pool:
		/// with the minimal number of threads which equals to half of host processor cores;
		/// with the maximal number of threads which equals to number of host processor cores;
		/// standard priority of all tasks types
		/// </summary>
		/// <param name="threadPoolName">Task pool name</param>
		ForceInline UnifiedThreadPoolBuilder(String&& threadPoolName) :This(new Data(
			MoveRValue(threadPoolName),
			static_cast<UInt32>(CurrentSettings().VirtualSetting.CpuCoresCount) / 2,
			static_cast<UInt32>(CurrentSettings().VirtualSetting.CpuCoresCount),
			EPoolThreadsPriority::Medium,
			EPoolThreadsPriority::Medium,
			EPoolThreadsPriority::Medium,
			EPoolThreadsPriority::Medium,
			EPoolThreadsPriority::Medium,
			EPoolThreadsPriority::Medium)) {
		}

		/// <summary>
		/// An empty constructor of thread pool, 
		/// initializes the settings of named thread pool:
		/// with the minimal number of threads which equals to half of host processor cores;
		/// with the maximal number of threads which equals to number of host processor cores;
		/// standard priority of all tasks types
		/// </summary>
		/// <param name="threadPoolName">Task pool name</param>
		ForceInline UnifiedThreadPoolBuilder(const String& threadPoolName) : UnifiedThreadPoolBuilder(String(threadPoolName)) {}

	public:
		/// <summary>
		/// Sets the priority to all tasks types
		/// </summary>
		/// <param name="newThreadPoolPriority">Priority to be set</param>
		/// <returns>"RValue" reference to a current object</returns>
		ForceInline UnifiedThreadPoolBuilder&& SetThreadPoolPriority(EPoolThreadsPriority newThreadPoolPriority)&& {
			This->irpRequestPriority = newThreadPoolPriority;
			This->jobsPriority = newThreadPoolPriority;
			This->sheduledTasksPriority = newThreadPoolPriority;
			return MoveRValue(*this);
		}

		/// <summary>
		/// Sets the informing processing priority of I/O operations
		/// </summary>
		/// <param name="newIoThreadsPriority">Priority to be set</param>
		/// <returns>"RValue" reference to a current object</returns>
		ForceInline UnifiedThreadPoolBuilder&& SetIOThreadsPriority(EPoolThreadsPriority newIoThreadsPriority)&& {
			This->irpRequestPriority = newIoThreadsPriority;
			return MoveRValue(*this);
		}

		/// <summary>
		/// Sets the priority to timer tasks
		/// </summary>
		/// <param name="newShedulerThreadsPriority">Priority to be set</param>
		/// <returns>"RValue" reference to a current object</returns>
		ForceInline UnifiedThreadPoolBuilder&& SetShedulerThreadsPriority(EPoolThreadsPriority newShedulerThreadsPriority)&& {
			This->sheduledTasksPriority = newShedulerThreadsPriority;
			return MoveRValue(*this);
		}

		/// <summary>
		/// The tasks priority of thread pool
		/// </summary>
		/// <param name="newJobThreadsPriority">Priority to be set</param>
		/// <returns>"RValue" reference to a current object</returns>
		ForceInline UnifiedThreadPoolBuilder&& SetJobThreadsPriority(EPoolThreadsPriority newJobThreadsPriority)&& {
			This->jobsPriority = newJobThreadsPriority;
			return MoveRValue(*this);
		}

		/// <summary>
		/// The tasks priority of event processing thread pool
		/// </summary>
		/// <param name="eventsRequestPriority">Priority to be set</param>
		/// <returns>"RValue" reference to a current object</returns>
		ForceInline UnifiedThreadPoolBuilder&& SetEvetntsThreadsPriority(EPoolThreadsPriority eventsRequestPriority)&& {
			This->eventsRequestPriority = eventsRequestPriority;
			return MoveRValue(*this);
		}

		/// <summary>
		/// The tasks priority of execution delays thread pool
		/// </summary>
		/// <param name="sleepsRequestPriority">Priority to be set</param>
		/// <returns>"RValue" reference to a current object</returns>
		ForceInline UnifiedThreadPoolBuilder&& SetSleepsThreadsPriority(EPoolThreadsPriority sleepsRequestPriority)&& {
			This->sleepsRequestPriority = sleepsRequestPriority;
			return MoveRValue(*this);
		}

		/// <summary>
		/// The tasks priority of execution alpc thread pool
		/// </summary>
		/// <param name="alpcRequestPriority">Priority to be set</param>
		/// <returns>"RValue" reference to a current object</returns>
		ForceInline UnifiedThreadPoolBuilder&& SetAlpcThreadsPriority(EPoolThreadsPriority alpcRequestPriority)&& {
			This->alpcRequestPriority = alpcRequestPriority;
			return MoveRValue(*this);
		}

	public:
		/// <summary>
		/// Sets the minimal number of threads in pool, can be 0
		/// </summary>
		/// <param name="newMinThreadCount">Minimal number of threads in pool</param>
		/// <returns>"RValue" reference to a current object</returns>
		ForceInline UnifiedThreadPoolBuilder&& SetMinThreadCount(UInt32 newMinThreadCount)&& {
			This->minThreadCount = newMinThreadCount;
			return MoveRValue(*this);
		}

		/// <summary>
		/// Sets the maximal number of threads in pool
		/// </summary>
		/// <param name="newMaxThreadCount">Maximal number of threads in thread pool</param>
		/// <returns>"RValue" reference to a current object</returns>
		ForceInline UnifiedThreadPoolBuilder&& SetMaxThreadCount(UInt32 newMaxThreadCount)&& {
			This->maxThreadCount = newMaxThreadCount;
			return MoveRValue(*this);
		}

	public:
		/// <summary>
		/// Sets the thread pool name
		/// </summary>
		/// <param name="newName">New pool name</param>
		/// <returns>"RValue" reference to a current object</returns>
		ForceInline UnifiedThreadPoolBuilder&& SetName(String&& newName)&& {
			This->threadPoolName = MoveRValue(newName);
			return MoveRValue(*this);
		}

		/// <summary>
		/// Sets the thread pool name
		/// </summary>
		/// <param name="newName">New pool name</param>
		/// <returns>"RValue" reference to a current object</returns>
		ForceInline UnifiedThreadPoolBuilder&& SetName(const String& newName)&& {
			This->threadPoolName = newName;
			return MoveRValue(*this);
		}

	public:
		/// <summary>
		/// Creates the thread pool with specified configuration
		/// </summary>
		/// <returns>Thread pool with specified configuration</returns>
		UnifiedThreadPool CreateThreadPool()&&;

	private:
		UnifiedThreadPool::PoolConfigureation CreateJobConfiguration(ImplSpecificT poolHandle);
		UnifiedThreadPool::PoolConfigureation CreateScheduledTaskConfiguration(ImplSpecificT poolHandle);
		UnifiedThreadPool::PoolConfigureation CreateIrpConfiguration(ImplSpecificT poolHandle);
		UnifiedThreadPool::PoolConfigureation CreateEventsConfiguration(ImplSpecificT poolHandle);
		UnifiedThreadPool::PoolConfigureation CreateSleepsConfiguration(ImplSpecificT poolHandle);
		UnifiedThreadPool::PoolConfigureation CreateAlpcConfiguration(ImplSpecificT poolHandle);

	public:
		/// <summary>
		/// Destructor
		/// </summary>
		ForceInline ~UnifiedThreadPoolBuilder() {
			if (This != nullptr)
			{
				delete This;
				This = nullptr;
			}
		}

	public:
		UnifiedThreadPoolBuilder(const UnifiedThreadPoolBuilder&) = delete;
		UnifiedThreadPoolBuilder& operator=(const UnifiedThreadPoolBuilder&) = delete;

	public:
		/// <summary>
		/// Moving constructor
		/// </summary>
		/// <param name="orig">Original</param>
		ForceInline UnifiedThreadPoolBuilder(UnifiedThreadPoolBuilder&& orig) : This(orig.This) { orig.This = nullptr; }

		/// <summary>
		/// Moving operator
		/// </summary>
		/// <param name="orig">Original</param>
		/// <returns>A reference to a current object</returns>
		ForceInline UnifiedThreadPoolBuilder& operator=(UnifiedThreadPoolBuilder&& orig) {
			this->~UnifiedThreadPoolBuilder();
			this->UnifiedThreadPoolBuilder::UnifiedThreadPoolBuilder(MoveRValue(orig));
			return *this;
		}
	};
}