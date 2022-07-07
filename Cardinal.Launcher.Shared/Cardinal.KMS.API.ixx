export module Cardinal.KMS.API;

import Cardinal.Core;

export namespace Cardinal::KMS::API
{
#pragma pack(push, 1)

	/// <summary>
	/// Message type
	/// </summary>
	enum class EMessageType : Core::UInt8
	{
		Undefined,
		Command,
		Datagram
	};

	/// <summary>
	/// Command type
	/// </summary>
	enum class ECommandType : Core::UInt8
	{
		Undefined,
		PasswordRequest,
		PasswordAnswer,
		ArgumentRequest,
		ArgumentAnswer,
		CommandRequest
	};

	/// <summary>
	/// Datagram type
	/// </summary>
	enum class EDatagramType : Core::UInt8
	{
		Undefined,
		Info,
		Log,
		Warning,
		Error
	};

	/// <summary>
	/// Log level
	/// </summary>
	enum class ELogLevel : Core::UInt8
	{
		Undefined,
		Debug,
		Verbose,
		Warning,
		Error,
		Critical
	};

	struct MessageHeader {
		EMessageType MessageType;
		ECommandType CommandType;
		EDatagramType DatagramType;
		ELogLevel LogLevel;
	};

	/// <summary>
	/// Full message size with header from text size
	/// </summary>
	template<Cardinal::Core::SizeT Size>
	constexpr auto FullMessageSize = Size + sizeof(MessageHeader);

	/// <summary>
	/// Text size from full message size
	/// </summary>
	template<Cardinal::Core::SizeT Size>
	constexpr auto TextMessageSize = Size - sizeof(MessageHeader);

	/// <summary>
	/// Fixed sized KMS message
	/// </summary>
	template<Core::SizeT FullMessageSize>
	struct FixedSizeMessage : MessageHeader {
		Core::Char Data[TextMessageSize<FullMessageSize> / sizeof(Core::Char)];
	};

	/// <summary>
	/// Dynamic sized KMS message
	/// </summary>
	struct DynamicSizeMessage : MessageHeader {
		Core::Char Data[1];
	};
#pragma pack(pop)
}