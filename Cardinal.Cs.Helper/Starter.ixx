export module Cardinal.CsHelper;

import Cardinal.Core;

namespace Cardinal::CsHelper
{
	/// <summary>
	/// Start Rem kernel executable in suspended state 
	/// </summary>
	/// <param name="path">Executable path</param>
	/// <returns>Process ID</returns>
	extern "C" __declspec(dllexport) Cardinal::UInt64 StartRemKernel(wchar_t* path);
}