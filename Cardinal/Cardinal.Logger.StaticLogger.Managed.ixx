export module Cardinal.Logger.StaticLogger.Managed;

export import Cardinal.Logger;
export import Cardinal.Logger.StaticLogger;
export import Cardinal.String;
export import Cardinal.Exceptions;

namespace Cardinal::Logger
{
	InternalApiCall Boolean ToCoreLog(const Char* value, SizeT lenght, const Exception& ex);
}

export namespace Cardinal::Logger
{
	// <summary>
	/// Extended static logger
	/// </summary>
	template<EStaticSubsystems TSubsystem>
	class StaticManagedLogger : public StaticLogger<TSubsystem> {
	public:
		/// <summary>
		/// Writing the message in logs
		/// </summary>
		/// <param name="logLevel">Message type</param>
		/// <param name="msg">The message</param>
		template<typename THeap>
		static void toLog(ELoggerLevel logLevel, const BaseString<THeap>& msg) {
			if (CurrentSettings().KernelLoggerConfiguration.GetLoggerLevelForStaticSubsystem(TSubsystem) & logLevel)
			{
				switch (logLevel)
				{
				case ELoggerLevel::Debug:
					toLogWithLevel<ELoggerLevel::Debug>(msg);
					break;
				case ELoggerLevel::Verbose:
					toLogWithLevel<ELoggerLevel::Verbose>(msg);
					break;
				case ELoggerLevel::Warning:
					toLogWithLevel<ELoggerLevel::Warning>(msg);
					break;
				case ELoggerLevel::Error:
					toLogWithLevel<ELoggerLevel::Error>(msg);
					break;
				case ELoggerLevel::Critical:
					toLogWithLevel<ELoggerLevel::Critical>(msg);
					break;
				default:
					break;
				}
			}
		}

		/// <summary>
		/// Writing the message in logs
		/// </summary>
		/// <param name="logLevel">Message type</param>
		/// <param name="msg">The message</param>
		template<typename THeap>
		static void toLog(ELoggerLevel logLevel, BaseString<THeap>&& msg) {
			if (CurrentSettings().KernelLoggerConfiguration.GetLoggerLevelForStaticSubsystem(TSubsystem) & logLevel)
			{
				switch (logLevel)
				{
				case ELoggerLevel::Debug:
					toLogWithLevel<ELoggerLevel::Debug>(msg);
					break;
				case ELoggerLevel::Verbose:
					toLogWithLevel<ELoggerLevel::Verbose>(msg);
					break;
				case ELoggerLevel::Warning:
					toLogWithLevel<ELoggerLevel::Warning>(msg);
					break;
				case ELoggerLevel::Error:
					toLogWithLevel<ELoggerLevel::Error>(msg);
					break;
				case ELoggerLevel::Critical:
					toLogWithLevel<ELoggerLevel::Critical>(msg);
					break;
				default:
					break;
				}
			}
		}

		/// <summary>
		/// Writing the message in logs
		/// </summary>
		/// <param name="logLevel">Message type</param>
		/// <param name="msgGen">The message</param>
		template<typename TLambda> requires Concepts::IsInvockable<TLambda>
		static void toLog(ELoggerLevel logLevel, TLambda&& msgGen) {
			if (CurrentSettings().KernelLoggerConfiguration.GetLoggerLevelForStaticSubsystem(TSubsystem) & logLevel)
			{
				switch (logLevel)
				{
				case ELoggerLevel::Debug:
					toLogWithLevel<ELoggerLevel::Debug>(msgGen());
					break;
				case ELoggerLevel::Verbose:
					toLogWithLevel<ELoggerLevel::Verbose>(msgGen());
					break;
				case ELoggerLevel::Warning:
					toLogWithLevel<ELoggerLevel::Warning>(msgGen());
					break;
				case ELoggerLevel::Error:
					toLogWithLevel<ELoggerLevel::Error>(msgGen());
					break;
				case ELoggerLevel::Critical:
					toLogWithLevel<ELoggerLevel::Critical>(msgGen());
					break;
				default:
					break;
				}
			}
		}
	
	public:
		/// <summary>
		/// Writing the message in logs
		/// </summary>
		/// <param name="logLevel">Message type</param>
		/// <param name="ex">The transmitted exception object</param>
		/// <param name="msg">The message</param>
		template<typename THeap>
		static void toLog(ELoggerLevel logLevel, const BaseString<THeap>& msg, const Exception& ex) {
			if (CurrentSettings().KernelLoggerConfiguration.GetLoggerLevelForStaticSubsystem(TSubsystem) & logLevel)
			{
				switch (logLevel)
				{
				case ELoggerLevel::Debug:
					toLogWithLevel<ELoggerLevel::Debug>(msg, ex);
					break;
				case ELoggerLevel::Verbose:
					toLogWithLevel<ELoggerLevel::Verbose>(msg, ex);
					break;
				case ELoggerLevel::Warning:
					toLogWithLevel<ELoggerLevel::Warning>(msg, ex);
					break;
				case ELoggerLevel::Error:
					toLogWithLevel<ELoggerLevel::Error>(msg, ex);
					break;
				case ELoggerLevel::Critical:
					toLogWithLevel<ELoggerLevel::Critical>(msg, ex);
					break;
				default:
					break;
				}
			}
		}

		/// <summary>
		/// Writing the message in logs
		/// </summary>
		/// <param name="logLevel">Message type</param>
		/// <param name="ex">The transmitted exception object</param>
		/// <param name="msg">The message</param>
		template<typename THeap>
		static void toLog(ELoggerLevel logLevel, BaseString<THeap>&& msg, const Exception& ex) {
			if (CurrentSettings().KernelLoggerConfiguration.GetLoggerLevelForStaticSubsystem(TSubsystem) & logLevel)
			{
				switch (logLevel)
				{
				case ELoggerLevel::Debug:
					toLogWithLevel<ELoggerLevel::Debug>(msg, ex);
					break;
				case ELoggerLevel::Verbose:
					toLogWithLevel<ELoggerLevel::Verbose>(msg, ex);
					break;
				case ELoggerLevel::Warning:
					toLogWithLevel<ELoggerLevel::Warning>(msg, ex);
					break;
				case ELoggerLevel::Error:
					toLogWithLevel<ELoggerLevel::Error>(msg, ex);
					break;
				case ELoggerLevel::Critical:
					toLogWithLevel<ELoggerLevel::Critical>(msg, ex);
					break;
				default:
					break;
				}
			}
		}

		/// <summary>
		/// Writing the message in logs
		/// </summary>
		/// <param name="logLevel">Message type</param>
		/// <param name="msgGen">The message</param>
		/// <param name="ex">The transmitted exception object</param>
		template<typename TLambda> requires Concepts::IsInvockable<TLambda>
		static void toLog(ELoggerLevel logLevel, TLambda&& msgGen, const Exception& ex) {
			if (CurrentSettings().KernelLoggerConfiguration.GetLoggerLevelForStaticSubsystem(TSubsystem) & logLevel)
			{
				switch (logLevel)
				{
				case ELoggerLevel::Debug:
					toLogWithLevel<ELoggerLevel::Debug>(msgGen(), ex);
					break;
				case ELoggerLevel::Verbose:
					toLogWithLevel<ELoggerLevel::Verbose>(msgGen(), ex);
					break;
				case ELoggerLevel::Warning:
					toLogWithLevel<ELoggerLevel::Warning>(msgGen(), ex);
					break;
				case ELoggerLevel::Error:
					toLogWithLevel<ELoggerLevel::Error>(msgGen(), ex);
					break;
				case ELoggerLevel::Critical:
					toLogWithLevel<ELoggerLevel::Critical>(msgGen(), ex);
					break;
				default:
					break;
				}
			}
		}
	
	protected:
		template<ELoggerLevel TLogLevel>
		constexpr static auto GenerateManagedLogTextHeader() {
			return
				TypeTraits::CreateTypeStr(L"[") +
				Logger::GetLogLevelName<TLogLevel>() +
				TypeTraits::CreateTypeStr(L" -> ") +
				Logger::GetStaticSubsystemName<TSubsystem>() +
				TypeTraits::CreateTypeStr(L"] \0");
		}

	protected:
		template<ELoggerLevel TLogLevel, typename THeap>
		static void toLogWithLevel(const BaseString<THeap>& msg) {
			constexpr decltype(auto) header = GenerateManagedLogTextHeader<TLogLevel>();
			constexpr auto headerSize = header.Count - 1;

			UChar buffer[StaticLogger<TSubsystem>::MaxLogMessage]{};
			CStr::Copy((UChar*)header.Raw(), buffer);
					
			if (msg.FullLenght > StaticLogger<TSubsystem>::MaxLogMessage - headerSize) {
				auto str = msg.Clone().Resize(((StaticLogger<TSubsystem>::MaxLogMessage - 1) - headerSize) - 3);
				str += "...";

				CStr::Copy(const_cast<UChar*>(static_cast<ConstCharStr>(str)), buffer + headerSize);
			}
			else {
				CStr::Copy(const_cast<UChar*>(static_cast<ConstCharStr>(msg)), buffer + headerSize);
			}

			ToCoreLog(buffer, headerSize + msg.Lenght);
		}

		template<ELoggerLevel TLogLevel, typename THeap>
		static void toLogWithLevel(BaseString<THeap>&& msg) {
			constexpr decltype(auto) header = GenerateManagedLogTextHeader<TLogLevel>();
			constexpr auto headerSize = header.Count - 1;

			UChar buffer[StaticLogger<TSubsystem>::MaxLogMessage]{};
			CStr::Copy(const_cast<UChar*>(header.Raw()), buffer);

			if (msg.FullLenght > StaticLogger<TSubsystem>::MaxLogMessage - headerSize)
			{
				auto str = msg.Clone().Resize(((StaticLogger<TSubsystem>::MaxLogMessage - 1) - headerSize) - 3);
				str += "...";

				CStr::Copy(const_cast<UChar*>(static_cast<ConstCharStr>(str)), buffer + headerSize);
			}
			else
			{
				CStr::Copy(const_cast<UChar*>(static_cast<ConstCharStr>(msg)), buffer + headerSize);
			}

			ToCoreLog(buffer, headerSize + msg.Lenght);
		}

	protected:
		template<ELoggerLevel TLogLevel, typename THeap>
		static void toLogWithLevel(const BaseString<THeap>& msg, const Exception& ex) {
			constexpr decltype(auto) header = GenerateManagedLogTextHeader<TLogLevel>();
			constexpr auto headerSize = header.Count - 1;

			UChar buffer[StaticLogger<TSubsystem>::MaxLogMessage]{};
			CStr::Copy(const_cast<UChar*>(header.Raw()), buffer);

			if (msg.FullLenght > StaticLogger<TSubsystem>::MaxLogMessage - headerSize)
			{
				auto str = msg.Clone().Resize(((StaticLogger<TSubsystem>::MaxLogMessage - 1) - headerSize) - 3);
				str += "...";

				CStr::Copy(const_cast<UChar*>(static_cast<ConstCharStr>(str)), buffer + headerSize);
			}
			else
			{
				CStr::Copy(const_cast<UChar*>(static_cast<ConstCharStr>(msg)), buffer + headerSize);
			}

			ToCoreLog(buffer, headerSize + msg.Lenght, ex);
		}

		template<ELoggerLevel TLogLevel, typename THeap>
		static void toLogWithLevel(BaseString<THeap>&& msg, const Exception& ex) {
			constexpr decltype(auto) header = GenerateManagedLogTextHeader<TLogLevel>();
			constexpr auto headerSize = header.Count - 1;

			UChar buffer[StaticLogger<TSubsystem>::MaxLogMessage]{};
			CStr::Copy(const_cast<UChar*>(header.Raw()), buffer);

			if (msg.FullLenght > StaticLogger<TSubsystem>::MaxLogMessage - headerSize)
			{
				auto str = msg.Resize(((StaticLogger<TSubsystem>::MaxLogMessage - 1) - headerSize) - 3);
				str += "...";

				CStr::Copy(const_cast<UChar*>(static_cast<ConstCharStr>(str)), buffer + headerSize);
			}
			else
			{
				CStr::Copy(const_cast<UChar*>(static_cast<ConstCharStr>(msg)), buffer + headerSize);
			}

			ToCoreLog(buffer, headerSize + msg.Lenght, ex);
		}
	};
}