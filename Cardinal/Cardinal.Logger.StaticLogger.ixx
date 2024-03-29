export module Cardinal.Logger.StaticLogger;

export import Cardinal.Logger;
export import Cardinal.Settings;

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

export namespace Cardinal::Logger
{
	/// <summary>
	/// Static logger
	/// </summary>
	template<EStaticSubsystems TSubsystem>
	class StaticLogger {
	public:
		static constexpr SizeT MaxLogMessage = Settings::StaticSettingsData::MaxKernelLogMsgSize.Bytes;

	public:
		/// <summary>
		/// Writing the message in logs
		/// </summary>
		/// <param name="logLevel">Message type</param>
		/// <param name="Msg">The message</param>
		template<SizeT Lenght>
		ForceInline static void toLog(ELoggerLevel logLevel, StaticTextRef<Lenght> Msg) {
			if (CurrentSettings().KernelLoggerConfiguration.GetLoggerLevelForStaticSubsystem(TSubsystem) & logLevel)
			{
				switch (logLevel)
				{
					case ELoggerLevel::Debug:
					toLogWithLevel<ELoggerLevel::Debug>(Msg);
					break;
					case ELoggerLevel::Verbose:
					toLogWithLevel<ELoggerLevel::Verbose>(Msg);
					break;
					case ELoggerLevel::Warning:
					toLogWithLevel<ELoggerLevel::Warning>(Msg);
					break;
					case ELoggerLevel::Error:
					toLogWithLevel<ELoggerLevel::Error>(Msg);
					break;
					case ELoggerLevel::Critical:
					toLogWithLevel<ELoggerLevel::Critical>(Msg);
					break;
					default:
					break;
				}
			}
		}

	protected:
		template<ELoggerLevel TLogLevel>
		ForceInline constexpr static auto GenerateLogTextHeader() {
			return
				TypeTraits::CreateTypeStr(L"[") +
				Logger::GetLogLevelName<TLogLevel>() +
				TypeTraits::CreateTypeStr(L" -> ") +
				Logger::GetStaticSubsystemName<TSubsystem>() +
				TypeTraits::CreateTypeStr(L"] \0");
		}

	protected:
		template<ELoggerLevel TLogLevel, SizeT Lenght>
		static void toLogWithLevel(StaticTextRef<Lenght> Msg) {
			constexpr decltype(auto) header = GenerateLogTextHeader<TLogLevel>();

			UChar buffer[MaxLogMessage];

			CStr::Copy(static_cast<const UChar*>(header.Raw()), buffer);

			constexpr SizeT headerSize = ((header.Count - 1) * sizeof(UChar));

			if constexpr (Lenght * sizeof(UChar) >= 4095 - headerSize)
			{
				Memory::Copy(
					const_cast<UChar*>(Msg),
					buffer + headerSize,
					(MaxLogMessage - 1) - ((header.Count * sizeof(UChar)) + 3));

				buffer[MaxLogMessage - 4] = L'.';
				buffer[MaxLogMessage - 3] = L'.';
				buffer[MaxLogMessage - 2] = L'.';
				buffer[MaxLogMessage - 1] = L'\0';
			}
			else
			{
				CStr::Copy(const_cast<UChar*>(Msg), buffer + headerSize / 2);
			}

			ToCoreLog(buffer, (headerSize / 2) + Lenght + 1);
		}
	};
}

#pragma warning(pop)