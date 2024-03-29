export module Cardinal.Settings:Default;

export import :Base;
export import :SettingsData;
export import :CurrentSetting;

export namespace Cardinal::Settings
{
	/// <summary>
	/// File name with kernel settings
	/// </summary>
	constexpr Char SettingsFileName[16] = L"CoreSettings";

	/// <summary>
	/// Starting stack size of the main thread
	/// </summary>
	constexpr Memory::DataSizeT MainThreadStackStartSize = 512Kb;

	/// <summary>
	/// Maximal stack size of the main thread
	/// </summary>
	constexpr Memory::DataSizeT MainThreadStackMaxSize = 1Mb;

	/// <summary>
	/// Standard Rem runtime settings
	/// </summary>
	namespace Default
	{
		__declspec(noinline) inline SettingsData InitDefaultSettings() {
			SettingsData DefaultSettings;
			// Memory manager
			{
				// Standard memory allocator section
				DefaultSettings.MainMemoryManager.DefaultHeap.CommitSize = 256Mb;
				DefaultSettings.MainMemoryManager.DefaultHeap.ReservedSize = 256Mb;
#ifdef PreReleaseMode
				DefaultSettings.MainMemoryManager.DefaultHeap.AdditionalСhecks = { 0x0, 0x0, 0x0, 0x0 };
				DefaultSettings.MainMemoryManager.DefaultHeap.IsLogingAllAllocationAndFree = false;
#elif DebugMode
				DefaultSettings.MainMemoryManager.DefaultHeap.AdditionalСhecks = { 0x1, 0x1, 0x1, 0x0 };
				DefaultSettings.MainMemoryManager.DefaultHeap.IsLogingAllAllocationAndFree = false;
#else
				DefaultSettings.MainMemoryManager.DefaultHeap.AdditionalСhecks = { 0 };
				DefaultSettings.MainMemoryManager.DefaultHeap.IsLogingAllAllocationAndFree = false;
#endif
				// Protected memory allocator section
				DefaultSettings.MainMemoryManager.SensitiveDataHeap.CommitSize = 64Mb;
				DefaultSettings.MainMemoryManager.SensitiveDataHeap.ReservedSize = 64Mb;
#ifdef PreReleaseMode
				DefaultSettings.MainMemoryManager.SensitiveDataHeap.AdditionalСhecks = { 0x0, 0x0, 0x0, 0x0 };
				DefaultSettings.MainMemoryManager.SensitiveDataHeap.IsLogingAllAllocationAndFree = false;
#elif DebugMode
				DefaultSettings.MainMemoryManager.SensitiveDataHeap.AdditionalСhecks = { 0x0, 0x0, 0x0, 0x0 };
				DefaultSettings.MainMemoryManager.SensitiveDataHeap.IsLogingAllAllocationAndFree = false;
#else
				DefaultSettings.MainMemoryManager.SensitiveDataHeap.AdditionalСhecks = { 0 };
				DefaultSettings.MainMemoryManager.SensitiveDataHeap.IsLogingAllAllocationAndFree = false;
#endif
				// Memory allocator section or exception handling
				DefaultSettings.MainMemoryManager.ExceptionHeap.CommitSize = 128Mb;
				DefaultSettings.MainMemoryManager.ExceptionHeap.ReservedSize = 128Mb;
#ifdef PreReleaseMode
				DefaultSettings.MainMemoryManager.ExceptionHeap.AdditionalСhecks = { 0x0, 0x0, 0x0, 0x0 };
				DefaultSettings.MainMemoryManager.ExceptionHeap.IsLogingAllAllocationAndFree = false;
#elif DebugMode
				DefaultSettings.MainMemoryManager.ExceptionHeap.AdditionalСhecks = { 0x0, 0x0, 0x0, 0x0 };
				DefaultSettings.MainMemoryManager.ExceptionHeap.IsLogingAllAllocationAndFree = false;
#else
				DefaultSettings.MainMemoryManager.ExceptionHeap.AdditionalСhecks = { 0 };
				DefaultSettings.MainMemoryManager.ExceptionHeap.IsLogingAllAllocationAndFree = false;
#endif

				// Memory reservation section for string buffer
				DefaultSettings.MainMemoryManager.StringReserveSize = 50;

				// Section of the standard maximal queue size
				DefaultSettings.MainMemoryManager.DefaultPriorityQueueMaxSize = 64;

				// Memory reservation section for dynamic stacks buffer
				DefaultSettings.MainMemoryManager.DynamicStackReserveSize = 100;

				// Memory reservation section for dynamic queues buffer
				DefaultSettings.MainMemoryManager.DynamicQueueReserveSize = 100;
			}

			// Multithreading
			{
				// Threads
				{
					DefaultSettings.Threading.Thread.StackStartSize = 5Mb;
					DefaultSettings.Threading.Thread.StackMaxSize = 10Mb;
					DefaultSettings.Threading.Thread.StopKernelThreadsTimeout = Time::TimeSpan().AddMinutes(3);
				}
			}

			// Kernel settings
			{
				// Task scheduler
				{
					// Standard timer resolution for task scheduler
					DefaultSettings.KernelServicesConfig.TaskSheduler.DefaultTimerProximity = Time::TimeSpan(1 * Time::SecondInNtTime);
				}
			}
			{
				// Kernel logger
#ifdef PreReleaseMode
				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::BootLoaderAPI)]
					= Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::DependencyInjectionAPI)]
					= Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::EventsAPI)]
					= Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::ExceptionABI)]
					= Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::InvalidSubsystem)]
					= Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::IoAPI)]
					= Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::MemoryAPI)]
					= Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::ObjectNamespaceAPI)]
					= Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::SerializationAPI)]
					= Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::AutomaticTestingAPI)]
					= Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::ManualTestingAPI)]
					= Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::ThreadingAPI)]
					= Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::CryptAPI)]
					= Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::RttiApi)]
					= Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::SettingsAPI)]
					= Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;
#elif DebugMode
				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::BootLoaderAPI)]
					= Logger::ELoggerLevel::Debug | Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::DependencyInjectionAPI)]
					= Logger::ELoggerLevel::Debug | Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::EventsAPI)]
					= Logger::ELoggerLevel::Debug | Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::ExceptionABI)]
					= Logger::ELoggerLevel::Debug | Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::InvalidSubsystem)]
					= Logger::ELoggerLevel::Debug | Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::IoAPI)]
					= Logger::ELoggerLevel::Debug | Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::MemoryAPI)]
					= Logger::ELoggerLevel::Debug | Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::ObjectNamespaceAPI)]
					= Logger::ELoggerLevel::Debug | Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::SerializationAPI)]
					= Logger::ELoggerLevel::Debug | Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::AutomaticTestingAPI)]
					= Logger::ELoggerLevel::Debug | Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::ManualTestingAPI)]
					= Logger::ELoggerLevel::Debug | Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::ThreadingAPI)]
					= Logger::ELoggerLevel::Debug | Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::CryptAPI)]
					= Logger::ELoggerLevel::Debug | Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::RttiAPI)]
					= Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::SettingsAPI)]
					= Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Warning | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;
#else //Release
				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::BootLoaderAPI)]
					= Logger::ELoggerLevel::Verbose | Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::DependencyInjectionAPI)]
					= Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::EventsAPI)]
					= Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::ExceptionABI)]
					= Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::InvalidSubsystem)]
					= Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::IoAPI)]
					= Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::MemoryAPI)]
					= Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::ObjectNamespaceAPI)]
					= Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::SerializationAPI)]
					= Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::ManualTestingAPI)]
					= Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::AutomaticTestingAPI)]
					= Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::ThreadingAPI)]
					= Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::CryptAPI)]
					= Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::RttiAPI)]
					= Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;

				DefaultSettings.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::SettingsAPI)]
					= Logger::ELoggerLevel::Error | Logger::ELoggerLevel::Critical;
#endif
			}

			// Current kernel version
			DefaultSettings.SettingsVersion = CurrentSettings::GetCurrentKernelVersion();

			return DefaultSettings;
		}
	}
}