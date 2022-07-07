export module Cardinal.Debug;

import Cardinal.Core;

namespace Cardinal::Internals
{
	void PrintLineWithHexImpl(const wchar_t* msg, Core::Int64 code, int mode);
}

namespace Cardinal
{
	template<Core::SizeT N>
	void DebugPrint(Core::StaticTextRef<N> msg, Core::Int64 ntStatusCode) {
		Internals::PrintLineWithHexImpl(msg, ntStatusCode, 16);
	}
}

#ifdef _DEBUG
#define DiagPrint(ntStatusCode) Rem::DebugPrint(L"" __FILE__ " [ " __FUNCDNAME__  " : " __TOSTRING(__LINE__) L"] -> ", ntStatusCode)
#else
#define DiagPrint(ntStatusCode)
#endif // _DEBUG