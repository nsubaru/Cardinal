export module Cardinal.Exceptions.ABI:Base;

export import Cardinal.Core;
export import Cardinal.ConstString;
export import Cardinal.RTTI.Cxx.TypeInfo;
export import Cardinal.Threading.TLS;

#ifndef Release
constexpr Cardinal::UInt64 EH_EXCEPTION_NUMBER = ('msc' | 0xE0000000);	// The NT Exception # that we use
#else
constexpr Cardinal::UInt64 EH_EXCEPTION_NUMBER = ('cdr' | 0xE0000000);	// The NT Exception # that we use
#endif