export module Cardinal.Logger;

export import Cardinal.Core;
export import Cardinal.CTTI.Minimal;

export namespace Cardinal::Logger
{
	InternalApiCall Boolean ToCoreLog(const Char* value, SizeT lenght);

	/// <summary>
	/// Record the static message into core logs
	/// </summary>
	template<SizeT Lenght>
	ForceInline Boolean ToCoreLog(StaticTextRef<Lenght> msg) {
#pragma warning(push)
#pragma warning(disable: 4996)
		return ToCoreLog(msg, Lenght);
#pragma warning(pop)
	}
}

export	namespace Cardinal::Logger
{
	/// <summary>
	/// Possible logging levels
	/// </summary>
	enum class ELoggerLevel : UInt16 {
		Debug		= 0b00001,
		Verbose		= 0b00010,
		Warning		= 0b00100,
		Error		= 0b01000,
		Critical	= 0b10000,
	};

	EnumCttiDefinition(ELoggerLevel,
		ELoggerLevel::Debug,
		ELoggerLevel::Verbose,
		ELoggerLevel::Warning,
		ELoggerLevel::Error,
		ELoggerLevel::Critical);

	/// <summary>
	/// Logging levels
	/// </summary>
	using LoggerLevel = EnumFlags<ELoggerLevel>;

	/// <summary>
	/// Static subsystems list
	/// </summary>
	enum class EStaticSubsystems : UInt8 {
		SettingsAPI,
		ExceptionABI,
		ThreadingAPI,
		SWRLockAPI,
		SerializationAPI,
		EventsAPI,
		ObjectNamespaceAPI,
		AutomaticTestingAPI, 
		ManualTestingAPI,
		DependencyInjectionAPI,
		BootLoaderAPI,
		MemoryAPI,
		IoAPI,
		CryptAPI,
		RttiAPI,
		InvalidSubsystem
	};

	EnumCttiDefinition(EStaticSubsystems,
		EStaticSubsystems::SettingsAPI,
		EStaticSubsystems::ExceptionABI,
		EStaticSubsystems::ThreadingAPI,
		EStaticSubsystems::SerializationAPI,
		EStaticSubsystems::EventsAPI,
		EStaticSubsystems::SWRLockAPI,
		EStaticSubsystems::ObjectNamespaceAPI,
		EStaticSubsystems::AutomaticTestingAPI,
		EStaticSubsystems::ManualTestingAPI,
		EStaticSubsystems::DependencyInjectionAPI,
		EStaticSubsystems::BootLoaderAPI,
		EStaticSubsystems::MemoryAPI,
		EStaticSubsystems::IoAPI,
		EStaticSubsystems::CryptAPI,
		EStaticSubsystems::RttiAPI,
		EStaticSubsystems::InvalidSubsystem
	);

	/// <summary>
	/// Returns the text representation of logging levels
	/// </summary>
	/// <returns>Compile-time string</returns>
	template<ELoggerLevel TLogLevel>
	constexpr ForceInline decltype(auto) GetLogLevelName() {
		if constexpr (TLogLevel == ELoggerLevel::Critical)
			return TypeTraits::CreateTypeStr(L"Critical");
		else if constexpr (TLogLevel == ELoggerLevel::Verbose)
			return TypeTraits::CreateTypeStr(L"Verbose");
		else if constexpr (TLogLevel == ELoggerLevel::Debug)
			return TypeTraits::CreateTypeStr(L"Debug");
		else if constexpr (TLogLevel == ELoggerLevel::Error)
			return TypeTraits::CreateTypeStr(L"Error");
		else if constexpr (TLogLevel == ELoggerLevel::Warning)
			return TypeTraits::CreateTypeStr(L"Warning");
		else
			static_assert(TLogLevel > ELoggerLevel::Critical);
	}

	/// <summary>
	/// Returns the text representation of the static subsystems
	/// </summary>
	/// <returns>Compile-time string</returns>
	template<EStaticSubsystems TStaticSubsystems>
	constexpr ForceInline decltype(auto) GetStaticSubsystemName() {
		if constexpr (TStaticSubsystems == EStaticSubsystems::SettingsAPI)
			return TypeTraits::CreateTypeStr(L"Settings API");
		else if constexpr (TStaticSubsystems == EStaticSubsystems::BootLoaderAPI)
			return TypeTraits::CreateTypeStr(L"BootLoader API");
		else if constexpr (TStaticSubsystems == EStaticSubsystems::DependencyInjectionAPI)
			return TypeTraits::CreateTypeStr(L"Dependency Injection API");
		else if constexpr (TStaticSubsystems == EStaticSubsystems::EventsAPI)
			return TypeTraits::CreateTypeStr(L"Events API");
		else if constexpr (TStaticSubsystems == EStaticSubsystems::ExceptionABI)
			return TypeTraits::CreateTypeStr(L"Exception ABI");
		else if constexpr (TStaticSubsystems == EStaticSubsystems::IoAPI)
			return TypeTraits::CreateTypeStr(L"I/O API");
		else if constexpr (TStaticSubsystems == EStaticSubsystems::CryptAPI)
			return TypeTraits::CreateTypeStr(L"Cryptography API");
		else if constexpr (TStaticSubsystems == EStaticSubsystems::MemoryAPI)
			return TypeTraits::CreateTypeStr(L"Memory API");
		else if constexpr (TStaticSubsystems == EStaticSubsystems::ObjectNamespaceAPI)
			return TypeTraits::CreateTypeStr(L"Object Namespace API");
		else if constexpr (TStaticSubsystems == EStaticSubsystems::SerializationAPI)
			return TypeTraits::CreateTypeStr(L"Serialization API");
		else if constexpr (TStaticSubsystems == EStaticSubsystems::AutomaticTestingAPI)
			return TypeTraits::CreateTypeStr(L"Automatic Testing API");
		else if constexpr (TStaticSubsystems == EStaticSubsystems::ManualTestingAPI)
			return TypeTraits::CreateTypeStr(L"Manual Testing API");
		else if constexpr (TStaticSubsystems == EStaticSubsystems::ThreadingAPI)
			return TypeTraits::CreateTypeStr(L"Threading API");
		else if constexpr (TStaticSubsystems == EStaticSubsystems::SWRLockAPI)
			return TypeTraits::CreateTypeStr(L"SWRLock API");
		else if constexpr (TStaticSubsystems == EStaticSubsystems::RttiAPI)
			return TypeTraits::CreateTypeStr(L"Rtti API");
		else
			static_assert(TStaticSubsystems >= EStaticSubsystems::InvalidSubsystem);
	}
}