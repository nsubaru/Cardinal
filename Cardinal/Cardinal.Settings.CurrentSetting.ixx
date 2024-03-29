export module Cardinal.Settings:CurrentSetting;

export import :Base;
export import :SettingsData;

export namespace Cardinal
{
	/// <summary>
	/// Singleton static class of current settings
	/// </summary>
	class CurrentSettings final {
	private:
		static const volatile Settings::SettingsData& GetCurrentSettingDataRaw();
		static volatile Settings::SettingsData& SetCurrentSettingDataRaw();
		static void InformAboutSettingsChange();
		static Threading::SRWLock& GetSettingsLock();

	private:
		template<typename Lambda>
		ForceInline static decltype(auto) GetCurrentSettingData(Lambda&& lambda) {
			return GetSettingsLock().SharedLock([&, lambda = MoveRValue(lambda)] () {
				decltype(auto) dataPtr = &GetCurrentSettingDataRaw();

				return lambda(*Interlocked::Load(dataPtr));
				});
		}

		template<typename Lambda> requires Concepts::IsInvockable<Lambda, volatile Settings::SettingsData&>
		ForceInline static void SetCurrentSettingData(Lambda&& lambda) {
			GetSettingsLock().ExclusiveLock([&, lambda = MoveRValue(lambda)] () -> void {
				lambda(SetCurrentSettingDataRaw());
				});
			InformAboutSettingsChange();
		}

	public:
		/// <summary> 
		/// Environment settings
		/// </summary>
		struct VirtualSettingData {
			/// <summary>
			/// Kernel executable file path
			/// </summary>
			Char KernelExecutableLocation[Math::Max<Int16>()];
			/// <summary>
			/// Number of cores in kernel host system
			/// </summary>
			UInt64 CpuCoresCount;
		};

	private:
		static const volatile VirtualSettingData& GetVirtualSettingDataRaw();

	public:
		class MemoryManagerT final {
		public:
			class DefaultHeapT final {
			public:
				constexpr ForceInline DefaultHeapT() = default;

			public:
				ForceInline Memory::DataSizeT GetStartSize() const {
					return GetCurrentSettingData([](auto& data) InlineLambda
						{
							return data.MainMemoryManager.DefaultHeap.CommitSize;
						});
				}

				ForceInline void SetStartSize(Memory::DataSizeT NewSize) {
					SetCurrentSettingData([&](auto& data) InlineLambda
						{
							Interlocked::Store(
								reinterpret_cast<volatile SizeT&>(data.MainMemoryManager.DefaultHeap.CommitSize),
								reinterpret_cast<volatile SizeT&>(NewSize));
						});
				}

				ForceInline Memory::DataSizeT GetMaxSize() const {
					return GetCurrentSettingData([&](auto& data) InlineLambda
						{
							return data.MainMemoryManager.DefaultHeap.ReservedSize;
						});
				}

				ForceInline void SetMaxSize(Memory::DataSizeT NewSize) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							Interlocked::Store(
								reinterpret_cast<volatile SizeT&>(CurrentSettingsData.MainMemoryManager.DefaultHeap.ReservedSize),
								reinterpret_cast<volatile SizeT&>(NewSize));
						});
				}

				ForceInline Boolean GetHeapFreeCheck() const {
					return GetCurrentSettingData([](auto& data) InlineLambda
						{
							return data.MainMemoryManager.DefaultHeap.AdditionalСhecks.HEAP_FREE_CHECK == 0x1;
						});
				}

				ForceInline void SetHeapFreeCheck(Boolean NewState) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							CurrentSettingsData.MainMemoryManager.DefaultHeap.AdditionalСhecks.HEAP_FREE_CHECK = (NewState) ? 0x1 : 0x0;
						});
				}

				ForceInline Boolean GetHeapTailCheck() const {
					return GetCurrentSettingData([](auto data) InlineLambda
						{
							return data.MainMemoryManager.DefaultHeap.AdditionalСhecks.HEAP_TAIL_CHECK == 0x1;
						});
				}

				ForceInline void SetHeapTailCheck(Boolean NewState) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							CurrentSettingsData.MainMemoryManager.DefaultHeap.AdditionalСhecks.HEAP_TAIL_CHECK = (NewState) ? 0x1 : 0x0;
						});
				}

				ForceInline Boolean GetHeapArgsCheck() const {
					return GetCurrentSettingData([](auto& data) InlineLambda
						{
							return data.MainMemoryManager.DefaultHeap.AdditionalСhecks.HEAP_ARGS_CHECK == 0x1;
						});
				}

				ForceInline void SetHeapArgsCheck(Boolean NewState) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							CurrentSettingsData.MainMemoryManager.DefaultHeap.AdditionalСhecks.HEAP_ARGS_CHECK = (NewState) ? 0x1 : 0x0;
						});
				}

				ForceInline Boolean GetHeapValidationChecks() const {
					return GetCurrentSettingData([] (auto& data) {
						return data.MainMemoryManager.DefaultHeap.AdditionalСhecks.HEAP_VALIDATE_ALL == 0x1;
						});
				}

				ForceInline void SetHeapValidationChecks(Boolean NewState) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							CurrentSettingsData.MainMemoryManager.DefaultHeap.AdditionalСhecks.HEAP_VALIDATE_ALL = (NewState) ? 0x1 : 0x0;
						});
				}

				ForceInline Boolean GetHeapAllocsLogging() const {
					return GetCurrentSettingData([] (auto& data) {
						return data.MainMemoryManager.DefaultHeap.IsLogingAllAllocationAndFree;
						});
				}

				ForceInline void SetHeapAllocsLogging(Boolean NewState) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							CurrentSettingsData.MainMemoryManager.DefaultHeap.IsLogingAllAllocationAndFree = NewState;
						});
				}

			public:
				/// <summary>
				/// Starting heap size
				/// </summary>
				Memory::DataSizeT Property(GetStartSize, SetStartSize) CommitSize;

				/// <summary>
				/// Maximal heap size
				/// <para>0 - means without restrictions</para>
				/// </summary>
				Memory::DataSizeT Property(GetMaxSize, SetMaxSize) ReservedSize;

				/// <summary>
				/// Checking the heap for integrity when freeing up memory area
				/// </summary>
				Boolean Property(GetHeapFreeCheck, SetHeapFreeCheck) HeapFreeCheck;

				/// <summary>
				/// Checking the tail of heap
				/// </summary>
				Boolean Property(GetHeapTailCheck, SetHeapTailCheck) HeapTailCheck;

				/// <summary>
				/// Arguments checking during the allocation/freeing
				/// </summary>
				Boolean Property(GetHeapArgsCheck, SetHeapArgsCheck) HeapArgsCheck;

				/// <summary>
				/// Heap checking during the memory allocation/freeing
				/// </summary>
				Boolean Property(GetHeapValidationChecks, SetHeapValidationChecks) HeapValidationChecks;

				/// <summary>
				/// All memory allocations and freeings logging
				/// </summary>
				Boolean Property(GetHeapAllocsLogging, SetHeapAllocsLogging) HeapAllocsLogging;
			};

			class SensitiveDataHeapT final {
			public:
				constexpr ForceInline SensitiveDataHeapT() = default;

			public:
				ForceInline Memory::DataSizeT GetStartSize() const {
					return GetCurrentSettingData([](auto& data) InlineLambda
						{
							return data.MainMemoryManager.SensitiveDataHeap.CommitSize;
						});
				}

				ForceInline void SetStartSize(Memory::DataSizeT NewSize) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							Interlocked::Store(
								reinterpret_cast<volatile SizeT&>(CurrentSettingsData.MainMemoryManager.DefaultHeap.CommitSize),
								reinterpret_cast<volatile SizeT&>(NewSize));
						});
				}

				ForceInline Memory::DataSizeT GetMaxSize() const {
					return GetCurrentSettingData([](auto& data) InlineLambda
						{
							return data.MainMemoryManager.SensitiveDataHeap.ReservedSize;
						});
				}

				ForceInline void SetMaxSize(Memory::DataSizeT NewSize) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							Interlocked::Store(
								reinterpret_cast<volatile SizeT&>(CurrentSettingsData.MainMemoryManager.DefaultHeap.ReservedSize),
								reinterpret_cast<volatile SizeT&>(NewSize));
						});
				}

				ForceInline Boolean GetHeapFreeCheck() const {
					return GetCurrentSettingData([](auto& data) InlineLambda
						{
							return data.MainMemoryManager.SensitiveDataHeap.AdditionalСhecks.HEAP_FREE_CHECK == 0x1;
						});
				}

				ForceInline void SetHeapFreeCheck(Boolean NewState) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							CurrentSettingsData.MainMemoryManager.SensitiveDataHeap.AdditionalСhecks.HEAP_FREE_CHECK = (NewState) ? 0x1 : 0x0;
						});
				}

				ForceInline Boolean GetHeapTailCheck() const {
					return GetCurrentSettingData([](auto& data) InlineLambda
						{
							return data.MainMemoryManager.SensitiveDataHeap.AdditionalСhecks.HEAP_TAIL_CHECK == 0x1;
						});
				}

				ForceInline void SetHeapTailCheck(Boolean NewState) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							CurrentSettingsData.MainMemoryManager.SensitiveDataHeap.AdditionalСhecks.HEAP_TAIL_CHECK = (NewState) ? 0x1 : 0x0;
						});
				}

				ForceInline Boolean GetHeapArgsCheck() const {
					return GetCurrentSettingData([](auto& data) InlineLambda
						{
							return data.MainMemoryManager.SensitiveDataHeap.AdditionalСhecks.HEAP_ARGS_CHECK == 0x1;
						});
				}

				ForceInline void SetHeapArgsCheck(Boolean NewState) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							CurrentSettingsData.MainMemoryManager.SensitiveDataHeap.AdditionalСhecks.HEAP_ARGS_CHECK = (NewState) ? 0x1 : 0x0;
						});
				}

				ForceInline Boolean GetHeapValidationChecks() const {
					return GetCurrentSettingData([](auto& data) InlineLambda
						{
							return data.MainMemoryManager.SensitiveDataHeap.AdditionalСhecks.HEAP_VALIDATE_ALL == 0x1;
						});
				}

				ForceInline void SetHeapValidationChecks(Boolean NewState) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							CurrentSettingsData.MainMemoryManager.SensitiveDataHeap.AdditionalСhecks.HEAP_VALIDATE_ALL = (NewState) ? 0x1 : 0x0;
						});
				}

				ForceInline Boolean GetHeapAllocsLogging() const {
					return GetCurrentSettingData([](auto& data) InlineLambda
						{
							return data.MainMemoryManager.SensitiveDataHeap.IsLogingAllAllocationAndFree;
						});
				}

				ForceInline void SetHeapAllocsLogging(Boolean NewState) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							CurrentSettingsData.MainMemoryManager.SensitiveDataHeap.IsLogingAllAllocationAndFree = NewState;
						});
				}

			public:
				/// <summary>
				/// Starting heap size
				/// </summary>
				Memory::DataSizeT Property(GetStartSize, SetStartSize) CommitSize;

				/// <summary>
				/// Maximal heap size
				/// <para>0 - means without restrictions</para>
				/// </summary>
				Memory::DataSizeT Property(GetMaxSize, SetMaxSize) ReservedSize;

				/// <summary>
				/// Checking the heap for integrity when freeing up memory area
				/// </summary>
				Boolean Property(GetHeapFreeCheck, SetHeapFreeCheck) HeapFreeCheck;

				/// <summary>
				/// Checking the tail of heap
				/// </summary>
				Boolean Property(GetHeapTailCheck, SetHeapTailCheck) HeapTailCheck;

				/// <summary>
				/// Arguments checking during the allocation/freeing
				/// </summary>
				Boolean Property(GetHeapArgsCheck, SetHeapArgsCheck) HeapArgsCheck;

				/// <summary>
				/// Heap checking during the memory allocation/freeing
				/// </summary>
				Boolean Property(GetHeapValidationChecks, SetHeapValidationChecks) HeapValidationChecks;

				/// <summary>
				/// All memory allocations and freeings logging
				/// </summary>
				Boolean Property(GetHeapAllocsLogging, SetHeapAllocsLogging) HeapAllocsLogging;
			};

			class ExceptionHeapT final {
			public:
				constexpr ForceInline ExceptionHeapT() = default;

			public:
				ForceInline Memory::DataSizeT GetStartSize() const {
					return GetCurrentSettingData([](auto& data) InlineLambda
						{
							return data.MainMemoryManager.ExceptionHeap.CommitSize;
						});
				}

				ForceInline void SetStartSize(Memory::DataSizeT NewSize) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							Interlocked::Store(
								reinterpret_cast<volatile SizeT&>(CurrentSettingsData.MainMemoryManager.SensitiveDataHeap.CommitSize),
								reinterpret_cast<volatile SizeT&>(NewSize));
						});
				}

				ForceInline Memory::DataSizeT GetMaxSize() const {
					return GetCurrentSettingData([](auto& data) InlineLambda
						{
							return data.MainMemoryManager.ExceptionHeap.ReservedSize;
						});
				}

				ForceInline void SetMaxSize(Memory::DataSizeT NewSize) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							Interlocked::Store(
								reinterpret_cast<volatile SizeT&>(CurrentSettingsData.MainMemoryManager.DefaultHeap.ReservedSize),
								reinterpret_cast<volatile SizeT&>(NewSize));
						});
				}

				ForceInline Boolean GetHeapFreeCheck() const {
					return GetCurrentSettingData([](auto& data) InlineLambda
						{
							return data.MainMemoryManager.ExceptionHeap.AdditionalСhecks.HEAP_FREE_CHECK == 0x1;
						});
				}

				ForceInline void SetHeapFreeCheck(Boolean NewState) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							CurrentSettingsData.MainMemoryManager.ExceptionHeap.AdditionalСhecks.HEAP_FREE_CHECK = (NewState) ? 0x1 : 0x0;
						});
				}

				ForceInline Boolean GetHeapTailCheck() const {
					return GetCurrentSettingData([](auto& data) InlineLambda
						{
							return data.MainMemoryManager.ExceptionHeap.AdditionalСhecks.HEAP_TAIL_CHECK == 0x1;
						});
				}

				ForceInline void SetHeapTailCheck(Boolean NewState) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							CurrentSettingsData.MainMemoryManager.ExceptionHeap.AdditionalСhecks.HEAP_TAIL_CHECK = (NewState) ? 0x1 : 0x0;
						});
				}

				ForceInline Boolean GetHeapArgsCheck() const {
					return GetCurrentSettingData([](auto& data) InlineLambda
						{
							return data.MainMemoryManager.ExceptionHeap.AdditionalСhecks.HEAP_ARGS_CHECK == 0x1;
						});
				}

				ForceInline void SetHeapArgsCheck(Boolean NewState) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							CurrentSettingsData.MainMemoryManager.ExceptionHeap.AdditionalСhecks.HEAP_ARGS_CHECK = (NewState) ? 0x1 : 0x0;
						});
				}

				ForceInline Boolean GetHeapValidationChecks() const {
					return GetCurrentSettingData([](auto& data) InlineLambda
						{
							return data.MainMemoryManager.ExceptionHeap.AdditionalСhecks.HEAP_VALIDATE_ALL == 0x1;
						});
				}

				ForceInline void SetHeapValidationChecks(Boolean NewState) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							CurrentSettingsData.MainMemoryManager.ExceptionHeap.AdditionalСhecks.HEAP_VALIDATE_ALL = (NewState) ? 0x1 : 0x0;
						});
				}

			public:
				/// <summary>
				/// Starting heap size
				/// </summary>
				Memory::DataSizeT Property(GetStartSize, SetStartSize) CommitSize;

				/// <summary>
				/// Maximal heap size, 0 - means without restrictions
				/// </summary>
				Memory::DataSizeT Property(GetMaxSize, SetMaxSize) ReservedSize;

				/// <summary>
				/// Checking the heap for integrity when freeing up memory area
				/// </summary>
				Boolean Property(GetHeapFreeCheck, SetHeapFreeCheck) HeapFreeCheck;

				/// <summary>
				/// Checking the tail of heap
				/// </summary>
				Boolean Property(GetHeapTailCheck, SetHeapTailCheck) HeapTailCheck;

				/// <summary>
				/// Arguments checking during the allocation/freeing
				/// </summary>
				Boolean Property(GetHeapArgsCheck, SetHeapArgsCheck) HeapArgsCheck;

				/// <summary>
				/// Heap checking during the memory allocation/freeing
				/// </summary>
				Boolean Property(GetHeapValidationChecks, SetHeapValidationChecks) HeapValidationChecks;
			};

		public:
			constexpr ForceInline MemoryManagerT() = default;

			ForceInline DefaultHeapT GetDefaultHeap() const { return DefaultHeapT(); }
			ForceInline void SetDefaultHeap(DefaultHeapT) {}

			ForceInline SensitiveDataHeapT GetSensitiveDataHeap() const { return SensitiveDataHeapT(); }
			ForceInline void SetSensitiveDataHeap(SensitiveDataHeapT) {}

			ForceInline ExceptionHeapT GetExceptionHeap() const { return ExceptionHeapT(); }
			ForceInline void SetExceptionHeap(ExceptionHeapT) {}

			ForceInline SizeT GetStringReserveSize() const {
				return GetCurrentSettingData([](auto& data) InlineLambda
					{
						return data.MainMemoryManager.StringReserveSize;
					});
			}

			ForceInline void SetStringReserveSize(SizeT NewSize) {
				SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
					{
						CurrentSettingsData.MainMemoryManager.StringReserveSize = NewSize;
					});
			}

			ForceInline SizeT GetDefaultPriorityQueueMaxSize() const {
				return GetCurrentSettingData([](auto& data) InlineLambda
					{
						return data.MainMemoryManager.DefaultPriorityQueueMaxSize;
					});
			}

			ForceInline void SetDefaultPriorityQueueMaxSize(SizeT NewSize) {
				SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
					{
						CurrentSettingsData.MainMemoryManager.DefaultPriorityQueueMaxSize = NewSize;
					});
			}

			ForceInline SizeT GetStackReserveSize() const {
				return GetCurrentSettingData([](auto& data) InlineLambda
					{
						return data.MainMemoryManager.DynamicStackReserveSize;
					});
			}

			ForceInline void SetStackReserveSize(SizeT NewSize) {
				SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
					{
						CurrentSettingsData.MainMemoryManager.DynamicStackReserveSize = NewSize;
					});
			}

			ForceInline SizeT GetQueueReserveSize() const {
				return GetCurrentSettingData([](auto& data) InlineLambda
					{
						return data.MainMemoryManager.DynamicQueueReserveSize;
					});
			}

			ForceInline void SetQueueReserveSize(SizeT NewSize) {
				SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
					{
						CurrentSettingsData.MainMemoryManager.DynamicQueueReserveSize = NewSize;
					});
			}

		public:
			/// <summary>
			/// Standard memory heap configuration
			/// </summary>
			DefaultHeapT Property(GetDefaultHeap, SetDefaultHeap) DefaultHeap;

			/// <summary>
			/// Sensitive memory heap configuration
			/// </summary>
			SensitiveDataHeapT Property(GetSensitiveDataHeap, SetSensitiveDataHeap) SensitiveDataHeap;

			/// <summary>
			/// Heap settings for exceptions
			/// </summary>
			ExceptionHeapT Property(GetExceptionHeap, SetExceptionHeap) ExceptionHeap;

			/// <summary>
			/// Reservation size for string variable (in number of characters)
			/// </summary>
			SizeT Property(GetStringReserveSize, SetStringReserveSize) StringReserveSize;

			/// <summary>
			/// Standard maximal number of heap elements
			/// </summary>
			SizeT Property(GetDefaultPriorityQueueMaxSize, SetDefaultPriorityQueueMaxSize) DefaultPriorityQueueMaxSize;

			/// <summary>
			/// Reservation size for stacks as a number of characters
			/// </summary>
			SizeT Property(GetStackReserveSize, SetStackReserveSize) StackReserveSize;

			/// <summary>
			/// Reservation size for queues as a number of characters
			/// </summary>
			SizeT Property(GetQueueReserveSize, SetQueueReserveSize) QueueReserveSize;
		};

		class ThreadingCfgT final {
		public:
			class ThreadCfg final {
			public:
				constexpr ForceInline ThreadCfg() = default;

				ForceInline Memory::DataSizeT GetStackStartSize() const {
					return GetCurrentSettingData([](auto& data) InlineLambda
						{
							return data.Threading.Thread.StackStartSize;
						});
				}

				ForceInline void SetStackStartSize(Memory::DataSizeT NewSize) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							Interlocked::Store(
								reinterpret_cast<volatile SizeT&>(CurrentSettingsData.Threading.Thread.StackStartSize),
								reinterpret_cast<volatile SizeT&>(NewSize));
						});
				}

				ForceInline Memory::DataSizeT GetStackMaxSize() const {
					return GetCurrentSettingData([](auto& data) InlineLambda
						{
							return data.Threading.Thread.StackMaxSize;
						});
				}

				ForceInline void SetStackMaxSize(Memory::DataSizeT NewSize) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							Interlocked::Store(
								reinterpret_cast<volatile SizeT&>(CurrentSettingsData.Threading.Thread.StackMaxSize),
								reinterpret_cast<volatile SizeT&>(NewSize));
						});
				}

				ForceInline Time::TimeSpan GetStopKernelThreadsTimeout() const {
					return GetCurrentSettingData([](auto& data) InlineLambda
						{
							return data.Threading.Thread.StopKernelThreadsTimeout;
						});
				}

				ForceInline void SetStopKernelThreadsTimeout(Time::TimeSpan NewStopKernelThreadsTimeout) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							CurrentSettingsData.Threading.Thread.StopKernelThreadsTimeout.Ticks = NewStopKernelThreadsTimeout.Ticks;
						});
				}

			public:
				/// <summary>
				/// Starting stack size for threads
				/// </summary>
				Memory::DataSizeT Property(GetStackStartSize, SetStackStartSize) StackStartSize;

				/// <summary>
				/// Maximal stack size for threads
				/// </summary>
				Memory::DataSizeT Property(GetStackMaxSize, SetStackMaxSize) StackMaxSize;

				/// <summary>
				/// Timeout waiting for threads when the kernel completes
				/// </summary>
				Time::TimeSpan Property(GetStopKernelThreadsTimeout, SetStopKernelThreadsTimeout) StopKernelThreadsTimeout;
			};

		public:
			constexpr ForceInline ThreadingCfgT() = default;

			ForceInline ThreadCfg GetThread() const { return ThreadCfg(); }
			ForceInline void SetThread(ThreadCfg) {}

		public:
			/// <summary>
			/// Threads configuration
			/// </summary>
			ThreadCfg Property(GetThread, SetThread) Thread;
		};

		class KernelServicesConfigT final {
		public:
			class TaskShedulerCfgT final {
			public:
				constexpr ForceInline TaskShedulerCfgT() = default;

				ForceInline Time::TimeSpan GetDefaultTimerProximity() const {
					return GetCurrentSettingData([](auto& data) InlineLambda
						{
							return data.KernelServicesConfig.TaskSheduler.DefaultTimerProximity;
						});
				}

				ForceInline void SetDefaultTimerProximity(Time::TimeSpan NewDefaultTimerProximity) {
					SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
						{
							CurrentSettingsData.KernelServicesConfig.TaskSheduler.DefaultTimerProximity.Ticks = NewDefaultTimerProximity.Ticks;
						});
				}

			public:
				/// <summary>
				/// Standard timer accuracy for the tasks
				/// </summary>
				Time::TimeSpan Property(GetDefaultTimerProximity, SetDefaultTimerProximity) DefaultTimerProximity;
			};

		public:
			ForceInline TaskShedulerCfgT GetTaskSheduler() const { return TaskShedulerCfgT(); }

			ForceInline void SetTaskSheduler(TaskShedulerCfgT) {}

		public:
			/// <summary>
			/// Task scheduler service configuration
			/// </summary>
			TaskShedulerCfgT Property(GetTaskSheduler, SetTaskSheduler) TaskSheduler;
		};

		class VirtualSettingDataT final {
		public:
			typedef const Char(&KernelExcecutableLocationT)[Math::Max<Int16>()];

		public:
			constexpr ForceInline VirtualSettingDataT() = default;

			ForceInline KernelExcecutableLocationT GetKernelExcecutableLocation() const {
				return (KernelExcecutableLocationT) GetVirtualSettingDataRaw().KernelExecutableLocation;
			}

			ForceInline UInt64 GetCpuCoresCount() const {
				return GetVirtualSettingDataRaw().CpuCoresCount;
			}

		public:
			/// <summary>
			/// Full "NT" address of kernel executable file
			/// </summary>
			KernelExcecutableLocationT Property(GetKernelExcecutableLocation) KernelExcecutableLocation;

			/// <summary>
			/// Number of cores in processor in the operating system
			/// </summary>
			UInt64 Property(GetCpuCoresCount) CpuCoresCount;
		};

		class KernelLoggerConfigurationT final {
		public:
			ForceInline constexpr KernelLoggerConfigurationT() = default;

			ForceInline Logger::LoggerLevel GetLoggerLevelForStaticSubsystem(Logger::EStaticSubsystems staticSubsystem) {
				return GetCurrentSettingData([&](auto& data) InlineLambda
					{
						if (static_cast<Logger::EStaticSubsystems>(staticSubsystem) < Logger::EStaticSubsystems::InvalidSubsystem)
							return data.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(staticSubsystem)];
						else
							return Logger::LoggerLevel();
					});
			}

			ForceInline void SetLoggerLevelForStaticSubsystem(Logger::EStaticSubsystems staticSubsystem, Logger::LoggerLevel newLoggerLevel) {
				SetCurrentSettingData([&](auto& CurrentSettingsData) InlineLambda
					{
						if (staticSubsystem < Logger::EStaticSubsystems::InvalidSubsystem)
							Interlocked::Store(
								reinterpret_cast<volatile TypeTraits::EnumUnderlyingTypeV<Logger::ELoggerLevel>&>(
									CurrentSettingsData.KernelLogger.CurrentStaticLogger[static_cast<SizeT>(staticSubsystem)]),
								reinterpret_cast<volatile TypeTraits::EnumUnderlyingTypeV<Logger::ELoggerLevel>&>(newLoggerLevel));
					});
			}

		public:
			//Logger::LoggerLevel Property(GetLoggerLevelForStaticSubsystem) StaticLoggerLevel[];
		};

	public:
		ForceInline constexpr CurrentSettings() = default;

		ForceInline VirtualSettingDataT GetVirtualSettingData() const { return VirtualSettingDataT(); }
		ForceInline void SetVirtualSettingDataT(VirtualSettingDataT) {}

		ForceInline MemoryManagerT GetMemoryManager() const { return MemoryManagerT(); }
		ForceInline void SetMemoryManager(MemoryManagerT) {}

		ForceInline KernelServicesConfigT GetKernelServicesConfig() const { return KernelServicesConfigT(); }
		ForceInline void SetKernelServicesConfig(KernelServicesConfigT) {}

		ForceInline ThreadingCfgT GetThreadinfConfig() const { return ThreadingCfgT(); }
		ForceInline void SetThreadinfConfig(ThreadingCfgT) {}

		ForceInline KernelLoggerConfigurationT GetKernelLoggerConfiguration() const { return KernelLoggerConfigurationT(); }
		ForceInline void SetKernelLoggerConfiguration(KernelLoggerConfigurationT) {}

		ForceInline KernelVersionT GetSettingsKernelVersion() const {
			return GetCurrentSettingData([](auto data) InlineLambda
				{
					return data.SettingsVersion;
				});
		}

		static KernelVersionT GetCurrentKernelVersion();

		Settings::SettingsData GetSettingsData() {
			return GetCurrentSettingData([](auto data) InlineLambda
				{
					return data;
				});
		}

		void SetSettingsData(Settings::SettingsData& newData) {
			SetCurrentSettingData([&](auto& data) InlineLambda{
				data = newData;
				});
		}

	public:
		/// <summary>
		/// Memory manager settings
		/// </summary>
		MemoryManagerT Property(GetMemoryManager, SetMemoryManager) MemoryManager;

		/// <summary>
		/// Kernel services configuration
		/// </summary>
		KernelServicesConfigT Property(GetKernelServicesConfig, SetKernelServicesConfig) KernelServices;

		/// <summary>
		/// Runtime configuration
		/// </summary>
		VirtualSettingDataT Property(GetVirtualSettingData, SetVirtualSettingDataT) VirtualSetting;

		/// <summary>
		/// Multithreading configuration
		/// </summary>
		ThreadingCfgT Property(GetThreadinfConfig, SetThreadinfConfig) Threading;

		/// <summary>
		/// Logger configuration
		/// </summary>
		KernelLoggerConfigurationT Property(GetKernelLoggerConfiguration, SetKernelLoggerConfiguration) KernelLoggerConfiguration;

		/// <summary>
		/// Kernel version
		/// </summary>
		KernelVersionT Property(GetCurrentKernelVersion) CurrentKernelVersion;

		/// <summary>
		/// Kernel settings version
		/// </summary>
		KernelVersionT Property(GetSettingsKernelVersion) SettingsKernelVersion;

		/// <summary>
		/// Raw settings data
		/// </summary>
		Settings::SettingsData Property(GetSettingsData, SetSettingsData) SettingsData;

	public:
		/// <summary>
		/// Syncronize runtime settings data with persistent storage
		/// </summary>
		void SaveCurrentSettings();
	};
}