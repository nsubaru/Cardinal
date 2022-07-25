module;

#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"
#include "Cardinal.KMS.Keyboard.LowLevel.h"

module Cardinal.KMS.Keyboard;

NTSTATUS ReadDataFromDriver(HANDLE hKbd, HANDLE hEvent, PKEYBOARD_INPUT_DATA inputData) {
	IO_STATUS_BLOCK Iosb = { 0 };
	LARGE_INTEGER ByteOffset = { 0 };
	NTSTATUS Status;

	Status = NtReadFile(hKbd,
		hEvent,
		NULL,
		NULL,
		&Iosb,
		inputData,
		sizeof(KEYBOARD_INPUT_DATA),
		&ByteOffset,
		NULL);

	if (Status == STATUS_PENDING)
	{
		Status = NtWaitForSingleObject(hEvent, NULL, NULL);
	}

	return Status;
}

void Cardinal::KMS::Keyboard::Keyboard::Dispose()
{
	NtClose(This->hEvent);
	NtClose(This->hPressAnyKeyEvent);
	NtClose(This->hKbd);
}

void Cardinal::KMS::Keyboard::Keyboard::UpdateControlKeys(VkCodes code, bool isPressed)
{
	switch (code) {
	case VkCodes::LSHIFT:
	case VkCodes::RSHIFT:
		if (isPressed) {
			Bits::BitSet(This->Modificators, (SizeT)EModificators::SHIFT);
		}
		else {
			Bits::BitReset(This->Modificators, (SizeT)EModificators::SHIFT);
		}
		break;

	case VkCodes::CONTROL:
		if (isPressed) {
			Bits::BitSet(This->Modificators, (SizeT)EModificators::CTRL);
		}
		else {
			Bits::BitReset(This->Modificators, (SizeT)EModificators::CTRL);
		}
		break;

	case VkCodes::RCONTROL:
		if (isPressed) {
			Bits::BitSet(This->Modificators, (SizeT)EModificators::RCTRL);
		}
		else {
			Bits::BitReset(This->Modificators, (SizeT)EModificators::RCTRL);
		}
		break;

	case VkCodes::LMENU:
		if (isPressed) {
			Bits::BitSet(This->Modificators, (SizeT)EModificators::LALT);
		}
		else {
			Bits::BitReset(This->Modificators, (SizeT)EModificators::LALT);
		}
		break;

	case VkCodes::RMENU:
		if (isPressed) {
			Bits::BitSet(This->Modificators, (SizeT)EModificators::RALT);
		}
		else {
			Bits::BitReset(This->Modificators, (SizeT)EModificators::RALT);
		}
		break;

	case VkCodes::CAPITAL:
		if (isPressed) {
			Bits::FlipBit(This->States, (SizeT)EStateCodes::CAPSLOCK);
		}
		break;

	case VkCodes::NUMLOCK:
		if (isPressed) {
			Bits::FlipBit(This->States, (SizeT)EStateCodes::NUMLOCK);
		}
		break;

	case VkCodes::SCROLL:
		if (isPressed) {
			Bits::FlipBit(This->States, (SizeT)EStateCodes::SCROLLOCK);
		}
		break;

	default:
		return;
	}
}

void Cardinal::KMS::Keyboard::Keyboard::RefrechKeyboardLeds()
{
	IO_STATUS_BLOCK               IoStatusBlock;
	NTSTATUS                      Status;
	KEYBOARD_INDICATOR_PARAMETERS kip;

	kip.LedFlags = 0;
	kip.UnitId = 0;

	if ((This->States & (int)EStateCodes::NUMLOCK))
		kip.LedFlags |= KEYBOARD_NUM_LOCK_ON;

	if ((This->States & (int)EStateCodes::CAPSLOCK))
		kip.LedFlags |= KEYBOARD_CAPS_LOCK_ON;

	if ((This->States & (int)EStateCodes::SCROLLOCK))
		kip.LedFlags |= KEYBOARD_SCROLL_LOCK_ON;

	// Update the state of the leds on primary keyboard

	Status = NtDeviceIoControlFile(
		This->hKbd,
		NULL,
		NULL,
		NULL,
		&IoStatusBlock,
		IOCTL_KEYBOARD_SET_INDICATORS,
		&kip,
		sizeof(kip),
		NULL,
		0);

	if (!NT_SUCCESS(Status))
	{
		//TODO: warning about this error
	}
}

Cardinal::KMS::Keyboard::VkCodes Cardinal::KMS::Keyboard::Keyboard::GetVkCode(bool isEnchancedState, void* pInputData)
{
	PKEYBOARD_INPUT_DATA InputData = (PKEYBOARD_INPUT_DATA)pInputData;

	if (!(isEnchancedState)) {
		if (Bits::BitCheck(This->States, (int)EStateCodes::NUMLOCK) &&
			KeyTableNumlock[InputData->MakeCode & 0x7f]) {
			return (VkCodes)KeyTableNumlock[InputData->MakeCode & 0x7f];
		}
		return (VkCodes)KeyTable[InputData->MakeCode & 0x7f];
	}
	return (VkCodes)KeyTableEnhanced[InputData->MakeCode & 0x7f];
}

void Cardinal::KMS::Keyboard::Keyboard::RefreshLayout(VkCodes code)
{
	if (
		Bits::BitCheck(This->Modificators, (SizeT)EModificators::SHIFT) &&
		Bits::BitCheck(This->Modificators, (SizeT)EModificators::LALT)
		) {
		if (code == VkCodes::LMENU || code == VkCodes::SHIFT || code == VkCodes::LSHIFT || code == VkCodes::RSHIFT) {
			This->layoutI++;
		}
	}
}

Cardinal::KMS::Keyboard::Keyboard::Keyboard() : This(new Data())
{
	UNICODE_STRING str;
	OBJECT_ATTRIBUTES attrb = { 0 };

	RtlInitUnicodeString(&str, (wchar_t*)L"\\Device\\KeyboardClass0");
	InitializeObjectAttributes(&attrb, &str, OBJ_CASE_INSENSITIVE, NULL, NULL);

	NtCreateFile(
		&This->hKbd,
		SYNCHRONIZE | GENERIC_READ | FILE_READ_ATTRIBUTES,
		&attrb,
		(PIO_STATUS_BLOCK)This->kbdIoBlk,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		0,
		FILE_OPEN,
		FILE_DIRECTORY_FILE,
		NULL,
		0);

	InitializeObjectAttributes(&attrb, NULL, 0, NULL, NULL);
	NtCreateEvent(&This->hEvent, GENERIC_ALL, &attrb, EVENT_TYPE::SynchronizationEvent, FALSE);
	NtCreateEvent(&This->hPressAnyKeyEvent, GENERIC_ALL, &attrb, EVENT_TYPE::NotificationEvent, FALSE);
	NtSetEvent(This->hPressAnyKeyEvent, NULL);
}

Cardinal::Char Cardinal::KMS::Keyboard::Keyboard::ConvertCodeCharacter(Code& code)
{
	bool isHigh = false;
	if (Bits::BitCheck(code.State, (int)EStateCodes::CAPSLOCK)) {
		isHigh = !isHigh;
	}

	if (Bits::BitCheck(code.Modificator, (int)EModificators::SHIFT)) {
		isHigh = !isHigh;
	}

	for (auto& keyValue : Layouts)
	{
		if (keyValue.code == code.VkCode) {
			if (This->layoutI % 2 == 0) {
				return (isHigh) ? keyValue.engHigh : keyValue.engLow;
			}
			else {
				return (isHigh) ? keyValue.ukrHigh : keyValue.ukrLow;
			}
		}
	}

	return INV;
}

Cardinal::KMS::Keyboard::Code Cardinal::KMS::Keyboard::Keyboard::ReadCode()
{
	KEYBOARD_INPUT_DATA inputData;
	LONG previousState = TRUE;
	EVENT_BASIC_INFORMATION state;

	// Use loop as we expect that durring read key from drive somebody can wait for "press any key"
	do {
		// Perform syncronized reading data from keyboard
		ReadDataFromDriver(This->hKbd, This->hEvent, &inputData);

		// Get current press any key request event state
		NtQueryEvent(This->hPressAnyKeyEvent, EventBasicInformation, &state, sizeof(EVENT_BASIC_INFORMATION), NULL);

		// If state is reseted than somebody start waiting for "press any key" event, than we check is key released
		// and if it is also true, than we set event, ignore release key and perform next read from driver without exit from loop
		if (state.EventState == FALSE && (!(inputData.Flags & KEY_BREAK)))
		{
			// we use previousState as previous event state. This state TRUE so we will be go to next read
			NtSetEvent(This->hPressAnyKeyEvent, &previousState);
			continue;
		}

		// if it is not "press any key" request than we use current event state as previousState
		previousState = state.EventState;

		// continue loop if there is active "press any key" request
	} while (previousState == FALSE);

	this->RefrechKeyboardLeds();

	auto vkCode = GetVkCode(inputData.Flags & KEY_E0, &inputData);
	this->UpdateControlKeys(vkCode, !(inputData.Flags & KEY_BREAK));
	this->RefreshLayout(vkCode);

	Code code = {
		.KbdCode = inputData.MakeCode,
		.KbdFlags = inputData.Flags,
		.VkCode = vkCode,
		.Modificator = This->Modificators,
		.State = This->States,
		.isPressed = (!(inputData.Flags & KEY_BREAK))
	};

	return code;
}

void Cardinal::KMS::Keyboard::Keyboard::WaitForAnyKeyDurringReadCode()
{
	// Reset "press any key" request event, this use to signale that we want catch next pressed key
	NtResetEvent(This->hPressAnyKeyEvent, NULL);
	// Wait for releasing any key as releasing key perform after pressing
	NtWaitForSingleObject(This->hPressAnyKeyEvent, TRUE, NULL);
}
