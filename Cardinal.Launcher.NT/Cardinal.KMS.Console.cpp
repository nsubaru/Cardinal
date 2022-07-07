module;

#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

module Cardinal.KMS;

using namespace Cardinal;
using namespace Cardinal::Core;

extern void PrintLineWithHex(const wchar_t* msg, LONG code, int mode);

void Cardinal::KMS::Console::ReadLine(Console::LineBufferT& buffer) {
	if (GetLastMessageType() == API::EMessageType::Command)
	{
		switch (GetLastCommandType())
		{
		case API::ECommandType::PasswordRequest:
			ReadDataLine(buffer, true);
			break;
		case API::ECommandType::ArgumentRequest:
			ReadDataLine(buffer, false);
			break;
		default:
			break;
		}
	}
	ReadDataLine(buffer, false);
}

void Cardinal::KMS::Console::ReadDataLine(Console::LineBufferT &buffer, bool hideInput)
{
	// Allow only one "read line" per time
	This->kbdReadLock.ExclusiveLock([&] {
		This->currentBuffer = &buffer;
		UNICODE_STRING str;
		RtlZeroMemory(buffer, Console::MessageSize);

		This->currentBufferSize = 0;
		Cardinal::Core::SizeT cmdListOffset = 0;
		bool isContinue = true;
		do {
			auto code = This->keyboard.ReadCode();
			// Processing is doing in lock for thread safety
			isContinue = This->kbdReadPrintLock.ExclusiveLock([&] {
				if (code.isPressed)
				{
					if (code.VkCode == Keyboard::VkCodes::INVALID) {
						return true;
					}

					if (code.VkCode == Keyboard::VkCodes::RETURN || code.VkCode == Keyboard::VkCodes::SELECT) {	//enter
						RtlInitUnicodeString(&str, (wchar_t*)NextLine);
						NtDisplayString(&str);
						if (This->currentBufferSize > 0 && !hideInput)
						{
							Console::StoreComand(buffer, This->currentBufferSize);
						}
						return false;
					}

					switch (code.VkCode)
					{
					case Keyboard::VkCodes::BACK: {
						if (This->currentBufferSize == 0) return true;
						RtlInitUnicodeString(&str, (wchar_t*)L"\b");
						NtDisplayString(&str);
						This->currentBufferSize--;
						buffer[This->currentBufferSize] = '\0';
						return true;
					}
					case Keyboard::VkCodes::UP: {
						This->backspaces[This->currentBufferSize] = L'\0';
						RtlInitUnicodeString(&str, This->backspaces);
						NtDisplayString(&str);
						This->backspaces[This->currentBufferSize] = L'\b';

						GetPreviousCommand(buffer, cmdListOffset, This->currentBufferSize);
						RtlInitUnicodeString(&str, buffer);
						NtDisplayString(&str);
						return true;
					}
					case Keyboard::VkCodes::DOWN: {
						This->backspaces[This->currentBufferSize] = L'\0';
						RtlInitUnicodeString(&str, This->backspaces);
						NtDisplayString(&str);
						This->backspaces[This->currentBufferSize] = L'\b';

						GetNextCommand(buffer, cmdListOffset, This->currentBufferSize);
						RtlInitUnicodeString(&str, buffer);
						NtDisplayString(&str);
						return true;
					}
					default:
						break;
					}

					auto ch = This->keyboard.ConvertCodeCharacter(code);
					if (ch == Keyboard::INV)
						return true;

					cmdListOffset = 0;

					if (!hideInput)
					{
						wchar_t text[]{ ch,L'\0' };
						RtlInitUnicodeString(&str, text);
						NtDisplayString(&str);
						buffer[This->currentBufferSize] = ch;
						This->currentBufferSize++;
						if (This->currentBufferSize == Console::BufferSize - 2) {
							Console::StoreComand(buffer, This->currentBufferSize);
							return false;
						}
					}
					else
					{
						wchar_t text[]{ '*',L'\0' };
						RtlInitUnicodeString(&str, text);
						NtDisplayString(&str);
						buffer[This->currentBufferSize] = ch;
						This->currentBufferSize++;
						if (This->currentBufferSize == Console::BufferSize - 2) {
							return false;
						}
					}
				}

				return true;
			});
		} while (isContinue);
		buffer[This->currentBufferSize] = L'\0';
		This->currentBuffer = nullptr;
		This->currentBufferSize = 0;
	});
}

void Cardinal::KMS::Console::RequestPressAnyKeyIfNeed()
{
	constexpr Cardinal::KMS::Console::LineBufferT pressAnyKeyReq = L"Press any key";
	constexpr Cardinal::KMS::Console::LineBufferT pressAnyKeyDelete = L"\b\b\b\b\b\b\b\b\b\b\b\b\b";

	UNICODE_STRING str;

	This->linesCount++;
	if (This->linesCount % 10 == 0) {
		RtlInitUnicodeString(&str, pressAnyKeyReq);
		NtDisplayString(&str);

		this->WaitForAnyKeyDurringReadCode();

		RtlInitUnicodeString(&str, pressAnyKeyDelete);
		NtDisplayString(&str);
	}
}

void Cardinal::KMS::Console::WriteLine(const Console::LineBufferT &buffer)
{
	This->kbdReadPrintLock.ExclusiveLock([&] {
		UNICODE_STRING str;

		if (This->currentBuffer != nullptr) {
			This->backspaces[This->currentBufferSize] = L'\0';
			RtlInitUnicodeString(&str, This->backspaces);
			NtDisplayString(&str);
			This->backspaces[This->currentBufferSize] = L'\b';
		}
		
		RtlInitUnicodeString(&str, buffer);
		NtDisplayString(&str);
		
		if (This->currentBuffer != nullptr) {
			RtlInitUnicodeString(&str, (wchar_t*)*This->currentBuffer);
			NtDisplayString(&str);
		}
	});
}

void WriteLineInternal(const Cardinal::KMS::Console::LineBufferT& buffer, SizeT startPos) {
	UNICODE_STRING str;

	RtlInitUnicodeString(&str, buffer + startPos);
	NtDisplayString(&str);

	RtlInitUnicodeString(&str, (wchar_t*)Cardinal::KMS::Console::NextLine);
	NtDisplayString(&str);
}

void Cardinal::KMS::Console::WriteMultiLineMessage(const LineBufferT& message)
{
	LineBufferT buffer;
	RtlCopyMemory(buffer, message, sizeof(LineBufferT));

	This->kbdReadPrintLock.ExclusiveLock([&] {
		UNICODE_STRING str;

		if (This->currentBuffer != nullptr) 
		{
			This->backspaces[This->currentBufferSize] = L'\0';
			RtlInitUnicodeString(&str, This->backspaces);
			NtDisplayString(&str);
			This->backspaces[This->currentBufferSize] = L'\b';
		}

		SizeT i = 0;
		SizeT startPos = 0;

		while (buffer[i] != '\0' && i < MessageSize / sizeof(Core::Char)) 
		{
			if (buffer[i + 1] !=  L'\0' && buffer[i] == '\r' && buffer[i + 1] == '\n') 
			{
				buffer[i] = L'\0';

				WriteLineInternal(buffer, startPos);

				i += 2; startPos = i;

				RequestPressAnyKeyIfNeed();

				continue;
			}

			i++;
		}

		WriteLineInternal(buffer, startPos);

		RequestPressAnyKeyIfNeed();

		if (This->currentBuffer != nullptr)
		{
			RtlInitUnicodeString(&str, (wchar_t*)*This->currentBuffer);
			NtDisplayString(&str);
		}
	});
}

void Cardinal::KMS::Console::StoreComand(Console::LineBufferT &buffer, Cardinal::Core::SizeT len)
{
	Copy(buffer, This->CmdHistoryEntities[This->cmdEntityIndex].cmd);
	This->CmdHistoryEntities[This->cmdEntityIndex].len = len;

	if (This->cmdEntitiesCount < CmdHistoryLen) {
		This->cmdEntitiesCount++;
	}

	if (++This->cmdEntityIndex == CmdHistoryLen) {
		This->cmdEntityIndex = 0;
	}
}

void Cardinal::KMS::Console::GetPreviousCommand(LineBufferT& buffer, Cardinal::Core::SizeT& offset, volatile Cardinal::Core::SizeT& cmdLen)
{
	if (offset == CmdHistoryLen || (offset == This->cmdEntitiesCount)) {
		return;
	}

	if (++offset == 1) {
		Copy(buffer, This->currentEntity.cmd);
		This->currentEntity.len = cmdLen;
	}

	auto cmdEntityLocalIndex = ((This->cmdEntityIndex + CmdHistoryLen) - offset) % CmdHistoryLen;
	Copy(This->CmdHistoryEntities[cmdEntityLocalIndex].cmd, buffer);
	cmdLen = This->CmdHistoryEntities[cmdEntityLocalIndex].len;
}

void Cardinal::KMS::Console::GetNextCommand(LineBufferT& buffer, Cardinal::Core::SizeT& offset, volatile Cardinal::Core::SizeT& cmdLen)
{
	if (offset == 0)
		return;

	if (--offset == 0) {
		Copy(This->currentEntity.cmd, buffer);
		cmdLen = This->currentEntity.len;
	}
	else {
		auto cmdEntityLocalIndex = ((This->cmdEntityIndex + CmdHistoryLen) - offset) % CmdHistoryLen;
		Copy(This->CmdHistoryEntities[cmdEntityLocalIndex].cmd, buffer);
		cmdLen = This->CmdHistoryEntities[cmdEntityLocalIndex].len;
	}
}

void Cardinal::KMS::Console::DisplayToBootScreen(const Cardinal::Core::Char* buffer)
{
	UNICODE_STRING str;
	RtlInitUnicodeString(&str, (Cardinal::Core::Char*)buffer);
	NtDrawText(&str);
}
