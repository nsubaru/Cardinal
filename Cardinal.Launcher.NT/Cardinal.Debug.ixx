export module Cardinal.Debug;

import Cardinal.Core;

namespace Cardinal::Internals
{
	void PrintLineWithHexImpl(const wchar_t* msg, Int64 code, int mode);
}

namespace Cardinal
{
	template<SizeT N>
	void DebugPrint(StaticTextRef<N> msg, Int64 ntStatusCode) {
		Internals::PrintLineWithHexImpl(msg, ntStatusCode, 16);
	}
}

#ifdef _DEBUG
#define DiagPrint(ntStatusCode) Rem::DebugPrint(L"" __FILE__ " [ " __FUNCDNAME__  " : " __TOSTRING(__LINE__) L"] -> ", ntStatusCode)
#else
#define DiagPrint(ntStatusCode)
#endif // _DEBUG