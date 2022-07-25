export module Cardinal.KMS.Keyboard:Code;

import :Base;
import :VkCodes;

export namespace Cardinal::KMS::Keyboard
{
	/// <summary>
	/// Invalid character code
	/// </summary>
	constexpr Char INV = Math::Max<Char>();

	/// <summary>
	/// Possible key modificators
	/// </summary>
	enum class EModificators : int {
		LALT = 1,
		RALT = 2,
		CTRL = 3,
		RCTRL = 4,
		SHIFT = 5
	};

	/// <summary>
	/// Possible control key states
	/// </summary>
	enum class EStateCodes : int {
		NUMLOCK = 1,
		CAPSLOCK = 2,
		SCROLLOCK = 3
	};

	/// <summary>
	/// Keyboard key information
	/// </summary>
	struct Code {
		SizeT KbdCode;
		SizeT KbdFlags;
		VkCodes VkCode;
		SizeT Modificator;
		SizeT State;
		bool isPressed;
	};

	/// <summary>
	/// Keyboard layout
	/// </summary>
	struct KeyboardLayout {
		VkCodes code;
		Char engLow;
		Char engHigh;
		Char ukrLow;
		Char ukrHigh;
	};
}