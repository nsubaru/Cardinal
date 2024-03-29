export module Cardinal.Settings:SettingsData;

export import :Base;

export namespace Cardinal::Settings
{
	/// <summary>
	/// Settings data
	/// </summary>
	struct SettingsData {
		/// <summary>
		/// Memory manager settings
		/// </summary>
		struct MemoryManager {
			/// <summary>
			/// Heap settings
			/// </summary>
			struct HeapCfg {
				/// <summary>
				/// Starting heap size
				/// </summary>
				Memory::DataSizeT CommitSize;

				/// <summary>
				/// Maximal heap size
				/// </summary>
				Memory::DataSizeT ReservedSize;

				/// <summary>
				/// Additional heap flags
				/// </summary>
				struct PosibleAdditionalСhecks {
					union {
						struct {
							/// <summary>
							/// Checking the memory freeing
							/// </summary>
							UInt8 HEAP_FREE_CHECK : 1;

							/// <summary>
							/// Checking the tail of heap
							/// </summary>
							UInt8 HEAP_TAIL_CHECK : 1;

							/// <summary>
							/// Arguments checking during allocation/freeing
							/// </summary>
							UInt8 HEAP_ARGS_CHECK : 1;

							/// <summary>
							/// Checking the heap on every run
							/// </summary>
							UInt8 HEAP_VALIDATE_ALL : 1;
						};

						/// <summary>
						/// Heap check flags
						/// </summary>
						UInt8 HeapFlags;
					};

					StructCttiDef(PosibleAdditionalСhecks,
						DefStructFieldData(PosibleAdditionalСhecks, HeapFlags)
					);
				};

				/// <summary>
				/// Additional heap flags
				/// </summary>
				PosibleAdditionalСhecks AdditionalСhecks;

				/// <summary>
				/// Allocations logging
				/// </summary>
				Boolean IsLogingAllAllocationAndFree;

				StructCttiDef(HeapCfg,
					DefStructFieldData(HeapCfg, CommitSize),
					DefStructFieldData(HeapCfg, ReservedSize),
					DefStructFieldData(HeapCfg, AdditionalСhecks),
					DefStructFieldData(HeapCfg, IsLogingAllAllocationAndFree)
				);
			};

			/// <summary>
			/// Standard heap
			/// </summary>
			HeapCfg DefaultHeap;
			/// <summary>
			/// Sensitive heap
			/// </summary>
			HeapCfg SensitiveDataHeap;
			/// <summary>
			/// A heap for exceptions
			/// </summary>
			HeapCfg ExceptionHeap;

			/// <summary>
			/// Reservation size for strings as a number of characters
			/// </summary>
			SizeT StringReserveSize;

			/// <summary>
			/// Standard maximal number of heap elements
			/// </summary>
			SizeT DefaultPriorityQueueMaxSize;

			/// <summary>
			/// Reservation size for stacks as a number of characters
			/// </summary>
			SizeT DynamicStackReserveSize;

			/// <summary>
			/// Reservation size for queues as a number of characters
			/// </summary>
			SizeT DynamicQueueReserveSize;

			StructCttiDef(MemoryManager,
				DefStructFieldData(MemoryManager, DefaultHeap),
				DefStructFieldData(MemoryManager, SensitiveDataHeap),
				DefStructFieldData(MemoryManager, ExceptionHeap),
				DefStructFieldData(MemoryManager, StringReserveSize),
				DefStructFieldData(MemoryManager, DefaultPriorityQueueMaxSize),
				DefStructFieldData(MemoryManager, DynamicStackReserveSize),
				DefStructFieldData(MemoryManager, DynamicQueueReserveSize)
			);
		};

		/// <summary>
		/// Multithreading configuration
		/// </summary>
		struct ThreadingCfg {
			/// <summary>
			/// Threads configuration
			/// </summary>
			struct ThreadCfg {
				/// <summary>
				/// Starting size for threads stack
				/// </summary>
				Memory::DataSizeT StackMaxSize;

				/// <summary>
				/// Maximalsize for threads stack
				/// </summary>
				Memory::DataSizeT StackStartSize;

				/// <summary>
				/// Thread timeout during closing the kernel
				/// </summary>
				Time::TimeSpan StopKernelThreadsTimeout;

				StructCttiDef(ThreadCfg,
					//DefStructFieldData(ThreadCfg, StackMaxSize),
					//DefStructFieldData(ThreadCfg, StackStartSize),
					DefStructFieldData(ThreadCfg, StopKernelThreadsTimeout)
				);
			};
			/// <summary>
			/// Threads configuration
			/// </summary>
			ThreadCfg Thread;

			StructCttiDef(ThreadingCfg,
				DefStructFieldData(ThreadingCfg, Thread)
			);
		};

		/// <summary>
		/// Kernes services configuration
		/// </summary>
		struct KernelServicesConfigT {
			/// <summary>
			/// Task scheduler configuration
			/// </summary>
			struct TaskShedulerCfgT {
				/// <summary>
				/// Standard timer resolution for tasks in the task scheduler
				/// </summary>
				Time::TimeSpan DefaultTimerProximity;

				StructCttiDef(TaskShedulerCfgT,
					DefStructFieldData(TaskShedulerCfgT, DefaultTimerProximity)
				);
			};
			/// <summary>
			/// Task scheduler configuration
			/// </summary>
			TaskShedulerCfgT TaskSheduler;

			StructCttiDef(KernelServicesConfigT,
				DefStructFieldData(KernelServicesConfigT, TaskSheduler)
			);
		};

		/// <summary>
		/// Kernel logger configuration
		/// </summary>
		struct KernelLoggerT {
			/// <summary>
			/// Max msg size in bytes which can be writed into core log file
			/// </summary>
			inline static constexpr auto MaxKernelLogMsgSize = Cardinal::Settings::StaticSettingsData::MaxKernelLogMsgSize;

			/// <summary>
			/// Log levels for core subsystems
			/// </summary>
			Logger::LoggerLevel CurrentStaticLogger[static_cast<SizeT>(Logger::EStaticSubsystems::InvalidSubsystem)];

			StructCttiDef(KernelLoggerT,
				DefStructFieldData(KernelLoggerT, CurrentStaticLogger)
			);
		};

		/// <summary>
		/// Memory manager settings
		/// </summary>
		MemoryManager MainMemoryManager;

		/// <summary>
		/// Multithreading configuration
		/// </summary>
		ThreadingCfg Threading;

		/// <summary>
		/// Kernel services configuration
		/// </summary>
		KernelServicesConfigT KernelServicesConfig;

		/// <summary>
		/// Core logger configurations
		/// </summary>
		KernelLoggerT KernelLogger;

		/// <summary>
		/// The kernel version description structure
		/// </summary>
		struct KernelVersion {
			UInt16 Major;
			UInt16 Minor;
			UInt16 StableBuildNumber;
			UInt16 BuildNumber;

			/// <summary>
			/// Comparing objects by strong order item comparing
			/// </summary>
			/// <param name="second">Second operand</param>
			/// <returns>Comparing result</returns>
			constexpr auto operator<=>(const KernelVersion& second) const {
				auto res = this->Major <=> second.Major;
				if (res != 0)
					return res;

				res = this->Minor <=> second.Minor;
				if (res != 0)
					return res;

				res = this->StableBuildNumber <=> second.StableBuildNumber;
				if (res != 0)
					return res;

				return this->BuildNumber <=> second.BuildNumber;
			}

			constexpr bool operator==(const KernelVersion& second) const = default;
			constexpr bool operator!=(const KernelVersion& second) const = default;

			/// <summary>
			/// Convert data to string fomat "{Major}.{Minor}.{StableBuildNumber}.{BuildNumber}"
			/// </summary>
			/// <typeparam name="THeap">String heap</typeparam>
			/// <returns>String representation</returns>
			template<typename THeap = Memory::Heap>
			BaseString<THeap> toString() const;

			StructCttiDef(KernelVersion,
				DefStructFieldData(KernelVersion, Major),
				DefStructFieldData(KernelVersion, Minor),
				DefStructFieldData(KernelVersion, StableBuildNumber),
				DefStructFieldData(KernelVersion, BuildNumber)
			);
		};

		/// <summary>
		/// Settings version
		/// </summary>
		KernelVersion SettingsVersion;

		StructCttiDef(SettingsData,
			DefStructFieldData(SettingsData, MainMemoryManager),
			DefStructFieldData(SettingsData, Threading),
			DefStructFieldData(SettingsData, KernelServicesConfig),
			DefStructFieldData(SettingsData, KernelLogger),
			DefStructFieldData(SettingsData, SettingsVersion)
		);
		
	public:
		SettingsData() = default;
		SettingsData(const SettingsData & from) = default;

		SettingsData& operator=(const SettingsData & from) = default;
		SettingsData& operator=(SettingsData && from) = default;

		__declspec(noinline) volatile SettingsData& operator=(const volatile SettingsData & from) volatile {
#pragma region MemoryManager
#pragma region DefaultHeap
			Interlocked::Store(
				this->MainMemoryManager.DefaultHeap.CommitSize.Bytes,
				from.MainMemoryManager.DefaultHeap.CommitSize.Bytes);
			Interlocked::Store(
				this->MainMemoryManager.DefaultHeap.ReservedSize.Bytes,
				from.MainMemoryManager.DefaultHeap.ReservedSize.Bytes);
			Interlocked::Store(
				this->MainMemoryManager.DefaultHeap.IsLogingAllAllocationAndFree,
				from.MainMemoryManager.DefaultHeap.IsLogingAllAllocationAndFree);
			Interlocked::Store(
				this->MainMemoryManager.DefaultHeap.AdditionalСhecks.HeapFlags,
				from.MainMemoryManager.DefaultHeap.AdditionalСhecks.HeapFlags);
#pragma endregion

#pragma region SensitiveDataHeap
			Interlocked::Store(
				this->MainMemoryManager.SensitiveDataHeap.CommitSize.Bytes,
				from.MainMemoryManager.SensitiveDataHeap.CommitSize.Bytes);
			Interlocked::Store(
				this->MainMemoryManager.SensitiveDataHeap.ReservedSize.Bytes,
				from.MainMemoryManager.SensitiveDataHeap.ReservedSize.Bytes);
			Interlocked::Store(
				this->MainMemoryManager.SensitiveDataHeap.IsLogingAllAllocationAndFree,
				from.MainMemoryManager.SensitiveDataHeap.IsLogingAllAllocationAndFree);
			Interlocked::Store(
				this->MainMemoryManager.SensitiveDataHeap.AdditionalСhecks.HeapFlags,
				from.MainMemoryManager.SensitiveDataHeap.AdditionalСhecks.HeapFlags);
#pragma endregion

#pragma region ExceptionHeap
			Interlocked::Store(
				this->MainMemoryManager.ExceptionHeap.CommitSize.Bytes,
				from.MainMemoryManager.ExceptionHeap.CommitSize.Bytes);
			Interlocked::Store(
				this->MainMemoryManager.ExceptionHeap.ReservedSize.Bytes,
				from.MainMemoryManager.ExceptionHeap.ReservedSize.Bytes);
			Interlocked::Store(
				this->MainMemoryManager.ExceptionHeap.IsLogingAllAllocationAndFree,
				from.MainMemoryManager.ExceptionHeap.IsLogingAllAllocationAndFree);
			Interlocked::Store(
				this->MainMemoryManager.ExceptionHeap.AdditionalСhecks.HeapFlags,
				from.MainMemoryManager.ExceptionHeap.AdditionalСhecks.HeapFlags);
#pragma endregion

#pragma region Misc
			Interlocked::Store(this->MainMemoryManager.StringReserveSize, from.MainMemoryManager.StringReserveSize);
			Interlocked::Store(this->MainMemoryManager.DefaultPriorityQueueMaxSize, from.MainMemoryManager.DefaultPriorityQueueMaxSize);
			Interlocked::Store(this->MainMemoryManager.DynamicStackReserveSize, from.MainMemoryManager.DynamicStackReserveSize);
			Interlocked::Store(this->MainMemoryManager.DynamicQueueReserveSize, from.MainMemoryManager.DynamicQueueReserveSize);
#pragma endregion
#pragma endregion

#pragma region Threading
			Interlocked::Store(
				this->Threading.Thread.StackMaxSize.Bytes,
				from.Threading.Thread.StackMaxSize.Bytes);
			Interlocked::Store(
				this->Threading.Thread.StackStartSize.Bytes,
				from.Threading.Thread.StackStartSize.Bytes);
			Interlocked::Store(
				reinterpret_cast<volatile SizeT&>(this->Threading.Thread.StopKernelThreadsTimeout),
				reinterpret_cast<const volatile SizeT&>(from.Threading.Thread.StopKernelThreadsTimeout));
#pragma endregion

#pragma region SettingsVersion
			Interlocked::Store(
				this->SettingsVersion.Major,
				from.SettingsVersion.Major);
			Interlocked::Store(
				this->SettingsVersion.Minor,
				from.SettingsVersion.Minor);
			Interlocked::Store(
				this->SettingsVersion.StableBuildNumber,
				from.SettingsVersion.StableBuildNumber);
			Interlocked::Store(
				this->SettingsVersion.BuildNumber,
				from.SettingsVersion.BuildNumber);
#pragma endregion

#pragma region KernelServicesConfig
			Interlocked::Store(
				this->KernelServicesConfig.TaskSheduler.DefaultTimerProximity.Ticks,
				this->KernelServicesConfig.TaskSheduler.DefaultTimerProximity.Ticks);
#pragma endregion

#pragma region KernelLogger
			for (SizeT i = 0; i < static_cast<SizeT>(Logger::EStaticSubsystems::InvalidSubsystem); i++)
			{
				Interlocked::Store(
					reinterpret_cast<volatile Logger::LoggerLevel::ValueType&>(this->KernelLogger.CurrentStaticLogger[i]),
					reinterpret_cast<const volatile Logger::LoggerLevel::ValueType&>(from.KernelLogger.CurrentStaticLogger[i]));
			}
#pragma endregion

			return *this;
		}

		__declspec(noinline) volatile SettingsData& operator=(volatile SettingsData && from) volatile {
#pragma region MemoryManager
#pragma region DefaultHeap
			Interlocked::Store(
				this->MainMemoryManager.DefaultHeap.CommitSize.Bytes,
				from.MainMemoryManager.DefaultHeap.CommitSize.Bytes);
			Interlocked::Store(
				this->MainMemoryManager.DefaultHeap.ReservedSize.Bytes,
				from.MainMemoryManager.DefaultHeap.ReservedSize.Bytes);
			Interlocked::Store(
				this->MainMemoryManager.DefaultHeap.IsLogingAllAllocationAndFree,
				from.MainMemoryManager.DefaultHeap.IsLogingAllAllocationAndFree);
			Interlocked::Store(
				this->MainMemoryManager.DefaultHeap.AdditionalСhecks.HeapFlags,
				from.MainMemoryManager.DefaultHeap.AdditionalСhecks.HeapFlags);
#pragma endregion

#pragma region SensitiveDataHeap
			Interlocked::Store(
				this->MainMemoryManager.SensitiveDataHeap.CommitSize.Bytes,
				from.MainMemoryManager.SensitiveDataHeap.CommitSize.Bytes);
			Interlocked::Store(
				this->MainMemoryManager.SensitiveDataHeap.ReservedSize.Bytes,
				from.MainMemoryManager.SensitiveDataHeap.ReservedSize.Bytes);
			Interlocked::Store(
				this->MainMemoryManager.SensitiveDataHeap.IsLogingAllAllocationAndFree,
				from.MainMemoryManager.SensitiveDataHeap.IsLogingAllAllocationAndFree);
			Interlocked::Store(
				this->MainMemoryManager.SensitiveDataHeap.AdditionalСhecks.HeapFlags,
				from.MainMemoryManager.SensitiveDataHeap.AdditionalСhecks.HeapFlags);
#pragma endregion

#pragma region ExceptionHeap
			Interlocked::Store(
				this->MainMemoryManager.ExceptionHeap.CommitSize.Bytes,
				from.MainMemoryManager.ExceptionHeap.CommitSize.Bytes);
			Interlocked::Store(
				this->MainMemoryManager.ExceptionHeap.ReservedSize.Bytes,
				from.MainMemoryManager.ExceptionHeap.ReservedSize.Bytes);
			Interlocked::Store(
				this->MainMemoryManager.ExceptionHeap.IsLogingAllAllocationAndFree,
				from.MainMemoryManager.ExceptionHeap.IsLogingAllAllocationAndFree);
			Interlocked::Store(
				this->MainMemoryManager.ExceptionHeap.AdditionalСhecks.HeapFlags,
				from.MainMemoryManager.ExceptionHeap.AdditionalСhecks.HeapFlags);
#pragma endregion

#pragma region Misc
			Interlocked::Store(this->MainMemoryManager.StringReserveSize, from.MainMemoryManager.StringReserveSize);
			Interlocked::Store(this->MainMemoryManager.DefaultPriorityQueueMaxSize, from.MainMemoryManager.DefaultPriorityQueueMaxSize);
			Interlocked::Store(this->MainMemoryManager.DynamicStackReserveSize, from.MainMemoryManager.DynamicStackReserveSize);
			Interlocked::Store(this->MainMemoryManager.DynamicQueueReserveSize, from.MainMemoryManager.DynamicQueueReserveSize);
#pragma endregion
#pragma endregion

#pragma region Threading
			Interlocked::Store(
				this->Threading.Thread.StackMaxSize.Bytes,
				from.Threading.Thread.StackMaxSize.Bytes);
			Interlocked::Store(
				this->Threading.Thread.StackStartSize.Bytes,
				from.Threading.Thread.StackStartSize.Bytes);
			Interlocked::Store(
				reinterpret_cast<volatile SizeT&>(this->Threading.Thread.StopKernelThreadsTimeout),
				reinterpret_cast<volatile SizeT&>(from.Threading.Thread.StopKernelThreadsTimeout));
#pragma endregion

#pragma region SettingsVersion
			Interlocked::Store(
				this->SettingsVersion.Major,
				from.SettingsVersion.Major);
			Interlocked::Store(
				this->SettingsVersion.Minor,
				from.SettingsVersion.Minor);
			Interlocked::Store(
				this->SettingsVersion.StableBuildNumber,
				from.SettingsVersion.StableBuildNumber);
			Interlocked::Store(
				this->SettingsVersion.BuildNumber,
				from.SettingsVersion.BuildNumber);
#pragma endregion

#pragma region KernelServicesConfig
			Interlocked::Store(
				this->KernelServicesConfig.TaskSheduler.DefaultTimerProximity.Ticks,
				this->KernelServicesConfig.TaskSheduler.DefaultTimerProximity.Ticks);
#pragma endregion

#pragma region KernelLogger
			for (SizeT i = 0; i < static_cast<SizeT>(Logger::EStaticSubsystems::InvalidSubsystem); i++)
			{
				Interlocked::Store(
					reinterpret_cast<volatile Logger::LoggerLevel::ValueType&>(this->KernelLogger.CurrentStaticLogger[i]),
					reinterpret_cast<const volatile Logger::LoggerLevel::ValueType&>(from.KernelLogger.CurrentStaticLogger[i]));
			}
#pragma endregion

			return *this;
		}
	};

	using KernelVersionT = SettingsData::KernelVersion;
}

export namespace Cardinal
{
	using KernelVersionT = Settings::KernelVersionT;
}