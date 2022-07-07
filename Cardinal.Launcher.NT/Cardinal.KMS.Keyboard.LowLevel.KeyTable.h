#pragma once
#include "Cardinal.KMS.Keyboard.LowLevel.Base.h"
#include "Cardinal.KMS.Keyboard.LowLevel.VirtualKeys.h"

#pragma region KeyTable

static WORD KeyTable[] = {
// 0x00
	0x00,			VK_ESCAPE,		0x31,			0x32,
	0x33,			0x34,			0x35,			0x36,
	0x37,			0x38,			0x39,			0x30,
	VK_OEM_MINUS,	VK_OEM_PLUS,	VK_BACK,		VK_TAB,
// 0x10
	0x51,			0x57,			0x45,			0x52,
	0x54,			0x59,			0x55,			0x49,
	0x4f,			0x50,			VK_OEM_4,		VK_OEM_6,
	VK_RETURN,		VK_CONTROL,		0x41,			0x53,
// 0x20
	0x44,			0x46,			0x47,			0x48,
	0x4a,			0x4b,			0x4c,			VK_OEM_1,
	VK_OEM_7,		0xc0,			VK_LSHIFT,		VK_OEM_5,
	0x5a,			0x58,			0x43,			0x56,
// 0x30
	0x42,			0x4e,			0x4d,			VK_OEM_COMMA,
	VK_OEM_PERIOD,	VK_OEM_2,		VK_RSHIFT,		VK_MULTIPLY,
	VK_LMENU,		VK_SPACE,		VK_CAPITAL,		VK_F1,
	VK_F2,			VK_F3,			VK_F4,			VK_F5,
// 0x40
	VK_F6,			VK_F7,			VK_F8,			VK_F9,
	VK_F10,			VK_NUMLOCK,		VK_SCROLL,		VK_HOME,
	VK_UP,			VK_PRIOR,		VK_SUBTRACT,	VK_LEFT,
	0,				VK_RIGHT,		VK_ADD,			VK_END,
// 0x50
	VK_DOWN,		VK_NEXT,		VK_INSERT,		VK_DELETE,
	0,				0,				0,				VK_F11,
	VK_F12,			0,				0,				0,
	0,				0,				0,				0,
// 0x60
	0,				0,				0,				0,
	0,				0,				0,				0,
	0,				0,				0,				0,
	0,				0,				0,				0,
// 0x70
	0,				0,				0,				0,
	0,				0,				0,				0,
	0,				0,				0,				0,
	0,				0,				0,				0
};

#pragma endregion

#pragma region EnhantedKeyTable

static WORD KeyTableEnhanced[] = {
// 0x00
	0,			0,				0,			0,
	0,			0,				0,			0,
	0,			0,				0,			0,
	0,			0,				0,			0,
// 0x10
	0,			0,				0,			0,
	0,			0,				0,			0,
	0,			0,				0,			0,
	VK_RETURN,	VK_RCONTROL,	0,			0,
// 0x20
	0,			0,				0,			0,
	0,			0,				0,			0,
	0,			0,				0,			0,
	0,			0,				0,			0,
// 0x30
	0,			0,				0,			0,
	0,			VK_DIVIDE,		0,			VK_SNAPSHOT,
	VK_RMENU,	0,				0,			0,
	0,			0,				0,			0,
// 0x40
	0,			0,				0,			0,
	0,			0,				0,			VK_HOME,
	VK_UP,		VK_PRIOR,		0,			VK_LEFT,
	0,			VK_RIGHT,		0,			VK_END,
// 0x50
	VK_DOWN,	VK_NEXT,		VK_INSERT,	VK_DELETE,
	0,			0,				0,			0,
	0,			0,				0,			0x5B,
	0,			0x5D,			0,			0,
// 0x60/
	0,			0,				0,			0,
	0,			0,				0,			0,
	0,			0,				0,			0,
	0,			0,				0,			0,
// 0x70
	0,			0,				0,			0,
	0,			0,				0,			0,
	0,			0,				0,			0,
	0,			0,				0,			0
};

#pragma endregion

#pragma region NumlockKeyTable

static WORD KeyTableNumlock[] = {
// 0x00
	0,			0,			0,			0,
	0,			0,			0,			0,
	0,			0,			0,			0,
	0,			0,			0,			0,
//0x10
	0,			0,			0,			0,
	0,			0,			0,			0,
	0,			0,			0,			0,
	0,			0,			0,			0,
// 0x20
	0,			0,			0,			0,
	0,			0,			0,			0,
	0,			0,			0,			0,
	0,			0,			0,			0,
// 0x30
	0,			0,			0,			0,
	0,			0,			0,			0,
	0,			0,			0,			0,
	0,			0,			0,			0,
// 0x40
	0,			0,			0,			0,
	0,			0,			0,			VK_NUMPAD7,
	VK_NUMPAD8,	VK_NUMPAD9,	0,			VK_NUMPAD4,
	VK_NUMPAD5,	VK_NUMPAD6,	0,			VK_NUMPAD1,
// 0x50
	VK_NUMPAD2,	VK_NUMPAD3,	VK_NUMPAD0,	0,
	0,			0,			0,			0,
	0,			0,			0,			0,
	0,			0,			0,			0,
// 0x60
	0,			0,			0,			0,
	0,			0,			0,			0,
	0,			0,			0,			0,
	0,			0,			0,			0,
// 0x70
	0,			0,			0,			0,
	0,			0,			0,			0,
	0,			0,			0,			0,
	0,			0,			0,			0
};

#pragma endregion