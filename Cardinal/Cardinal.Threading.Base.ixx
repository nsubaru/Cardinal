export module Cardinal.Threading:Base;

export import Cardinal.Time;
export import Cardinal.CTTI;
export import Cardinal.RTTI;

export namespace Cardinal::Threading
{
	/// <summary>
	/// Threadable entity types
	/// </summary>
	enum class EThreadableTypes : UInt8 {
		Thread, JobTask, ScheduledTask, IoTask, AlpcTask, EventTask, AsyncSleep, Unknown = 255
	};

	EnumCttiDefinition(EThreadableTypes,
		EThreadableTypes::Thread,
		EThreadableTypes::JobTask,
		EThreadableTypes::ScheduledTask,
		EThreadableTypes::IoTask,
		EThreadableTypes::AlpcTask,
		EThreadableTypes::EventTask,
		EThreadableTypes::AsyncSleep,
		EThreadableTypes::Unknown
	);

	/// <summary>
	/// Threadable entity states
	/// </summary>
	enum class EThreadabledStates : UInt8 {
		NotStarted, Starting, Executing, Paused, ExceptionProcessing, Interupted, Ended, Unknown = 255
	};

	EnumCttiDefinition(EThreadabledStates,
		EThreadabledStates::NotStarted,
		EThreadabledStates::Starting,
		EThreadabledStates::Executing,
		EThreadabledStates::Paused,
		EThreadabledStates::ExceptionProcessing,
		EThreadabledStates::Interupted,
		EThreadabledStates::Ended,
		EThreadabledStates::Unknown
	);

	/// <summary>
	/// Threadable entity code type
	/// </summary>
	enum class EThreadableCodeType : UInt8 {
		Native, Managed, Unknown = 255
	};

	EnumCttiDefinition(EThreadableCodeType,
		EThreadableCodeType::Native,
		EThreadableCodeType::Managed,
		EThreadableCodeType::Unknown
	);

	/// <summary>
	/// An object interface on which can be waited asynchronously
	/// </summary>
	VirtualClass IAwaitable {
	protected:
		virtual Handle GetAwaitableHandle() const = 0;
	
	public:
		/// <summary>
		/// Destructor
		/// </summary>
		virtual ~IAwaitable() {};
	
	private:
		friend Handle GetHandleFromIAwaitable(const IAwaitable& awaitable);
	};

	/// <summary>
	/// Returns the identifier of a threadable entity within which the threadable code is running
	/// </summary>
	ID GetTID();

	/// <summary>
	/// Suspends the thread execution on a specified period of time
	/// </summary>
	/// <param name="time">Time</param>
	/// <param name="IsRelative">If "true" then the time takes as a relative value relative to suspending time</param>
	void Sleep(Time::DateTime time, Boolean IsRelative);

	/// <summary>
	/// Suspends the thread execution on a specified number of milliseconds
	/// </summary>
	/// <param name="ms">Number of milliseconds</param>
	ForceInline void Sleep(UInt64 ms) { Sleep(Time::DateTime((Time::SecondInNtTime / 1000) * ms), true); }

	/// <summary>
	/// Suspends the thread execution on a specified time interval
	/// </summary>
	/// <param name="time">Specified time interval</param>
	ForceInline void Sleep(Time::TimeSpan time) { Sleep(Time::DateTime(time.Ticks), true); }

	/// <summary>
	/// Task states
	/// </summary>
	enum class EJobStates :UInt8 {
		/// <summary>
		/// Not yet launched
		/// </summary>
		NotStarted, 

		/// <summary>
		/// In the execution process
		/// </summary>
		Executing,

		/// <summary>
		/// Finished
		/// </summary>
		Finished, 

		/// <summary>
		/// In the exceprion processing process
		/// </summary>
		ExceptionCatched,

		/// <summary>
		/// Unknown state
		/// </summary>
		Unknown = 255
	};

	EnumCttiDefinition(EJobStates,
		EJobStates::NotStarted,
		EJobStates::Executing,
		EJobStates::Finished,
		EJobStates::ExceptionCatched,
		EJobStates::Unknown
	);

	
}

export namespace Cardinal::Threading::Threadables
{
	class ThreadableThread;
	class ThreadableUnifiedThreadPoolJob;
	class ThreadableUnifiedThreadPoolIoTask;
	class ThreadableUnifiedThreadPoolAlpcTask;
	class ThreadableUnifiedThreadPoolScheduledTask;
	class ThreadableUnifiedThreadPoolAsyncSleep;
	class ThreadableUnifiedThreadPoolScheduledEvent;
}