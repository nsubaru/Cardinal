export module Cardinal.KMS.Keyboard:Keyboard;

import :Base;
import :VkCodes;
import :Code;

export namespace Cardinal::KMS::Keyboard
{
	/// <summary>
	/// Keyboard reader class abstract low level kerboard key reading
	/// </summary>
	class Keyboard {
	private:
		struct Data {
			Core::Handle hKbd;
			Core::Handle hEvent;
			Core::Handle hPressAnyKeyEvent;
			Core::RawMemBlk kbdIoBlk[2];

			Core::SizeT layoutI;

			Core::SizeT Modificators;
			Core::SizeT States;

			Core::RefCountT RefCount;

			Data() :
				hKbd(nullptr),
				hEvent(nullptr),
				hPressAnyKeyEvent(nullptr),
				kbdIoBlk(),
				layoutI(0),
				Modificators(0),
				States(0),
				RefCount(1)
			{}
		};

	private:
		Data* This;

	private:
		void Dispose();

	private:
		/// <summary>
		/// Checks if special key (Num Lock, Caps Lock or Scroll Lock) or
		/// modifier key (Shift, Control, Alt) is pressed
		/// </summary>
		/// <param name="code">Key code</param>
		/// <param name="isPressed">Check if key is pressed</param>
		void UpdateControlKeys(VkCodes code, bool isPressed);

		/// <summary>
		/// On/Off diodes on keyboard depending on special key press
		/// </summary>
		void RefrechKeyboardLeds();

		/// <summary>
		/// Returns key value depending on Num Lock state
		/// </summary>
		/// <param name="isEnchancedState">If special key is on</param>
		/// <param name="pInputData">Input data</param>
		VkCodes GetVkCode(bool isEnchancedState, void* InputData);

		/// <summary>Language change checking</summary>
		/// <param name="code">Key code</param>
		void RefreshLayout(VkCodes code);

	public:
		/// <summary>
		/// Keyboard object constructor
		/// </summary>
		Keyboard();

		/// <summary>
		/// Checks if Shift or Caps Lock keys is pressed
		/// </summary>
		/// <param name="code">Key code</param>
		/// <returns>Corresponding value from table</returns>
		Core::Char ConvertCodeCharacter(Code& code);

		/// <summary>
		/// Reads information about pressed key
		/// </summary>
		/// <returns>Key code value</returns>
		Code ReadCode();

		/// <summary>
		/// Waiting for pressing any key during key code reading
		/// </summary>
		void WaitForAnyKeyDurringReadCode();

		/// <summary>
		/// Changes the language
		/// </summary>
		int GetCurrentLayout() { return This->layoutI % 2; }

	public:
		RefCountClass(Keyboard, This);
	};
}