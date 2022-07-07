#pragma once
#include "Cardinal.Core.NTAPI.h"

typedef struct _SCANTOASCII {
	USHORT ScanCode;
	USHORT Enhanced;
	wchar_t Normal;
	wchar_t Shift;
	wchar_t NumLock;
	wchar_t bCAPS;
} SCANTOASCII, * PSCANTOASCII;

#pragma region ControlKeyState flags

#define RIGHT_ALT_PRESSED               0x0001
#define LEFT_ALT_PRESSED                0x0002
#define RIGHT_CTRL_PRESSED              0x0004
#define LEFT_CTRL_PRESSED               0x0008
#define SHIFT_PRESSED                   0x0010
#define NUMLOCK_ON                      0x0020
#define SCROLLLOCK_ON                   0x0040
#define CAPSLOCK_ON                     0x0080
#define ENHANCED_KEY                    0x0100

#pragma endregion

typedef struct _KEYBOARD_INPUT_DATA {
	USHORT UnitId;
	USHORT MakeCode;
	USHORT Flags;
	USHORT Reserved;
	ULONG ExtraInformation;
} KEYBOARD_INPUT_DATA, * PKEYBOARD_INPUT_DATA;

#define IOCTL_KEYBOARD_SET_INDICATORS \
  CTL_CODE(FILE_DEVICE_KEYBOARD, 0x0002, METHOD_BUFFERED, FILE_ANY_ACCESS)

#pragma region KEYBOARD_INPUT_DATA.Flags constants

#define KEY_MAKE                  0
#define KEY_BREAK                 1
#define KEY_E0                    2
#define KEY_E1                    4
#define KEY_TERMSRV_SET_LED       8
#define KEY_TERMSRV_SHADOW        0x10
#define KEY_TERMSRV_VKPACKET      0x20

#define KEYBOARD_LED_INJECTED     0x8000
#define KEYBOARD_SHADOW           0x4000
#define KEYBOARD_KANA_LOCK_ON     8
#define KEYBOARD_CAPS_LOCK_ON     4
#define KEYBOARD_NUM_LOCK_ON      2
#define KEYBOARD_SCROLL_LOCK_ON   1

#pragma endregion

typedef struct _KEY_EVENT_RECORD {
	BOOL bKeyDown;
	WORD wRepeatCount;
	WORD wVirtualKeyCode;
	WORD wVirtualScanCode;
	union {
		WCHAR UnicodeChar;
		CHAR  AsciiChar;
	} uChar;
	DWORD dwControlKeyState;
}KEY_EVENT_RECORD, * PKEY_EVENT_RECORD;

typedef struct _KEYBOARD_INDICATOR_PARAMETERS {
	USHORT UnitId;
	USHORT LedFlags;
} KEYBOARD_INDICATOR_PARAMETERS, * PKEYBOARD_INDICATOR_PARAMETERS;

