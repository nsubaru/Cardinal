export module Cardinal.Threading.TLS:Exceptions;

export import :Base;


export typedef void (__cdecl* unexpected_handler )();
export typedef void (__cdecl* unexpected_function)(); //-V677

// As magic numbers increase, we have to keep track of the versions that we are
// backwards compatible with.  The top 3 bits of the magic number DWORD are
// used for other purposes, so while the magic number started as a YYYYMMDD
// date, it can't represent the year 2000 or later.  Old CRTs also recognize
// unknown magic numbers with a >= test.  Therefore, we just increment the
// the magic number by one every time we need another.
//
// EH_MAGIC_NUMBER1     The original, still used in exception records for
//                      native or mixed C++ thrown objects.
// EH_MAGIC_NUMBER2     Used in the FuncInfo if exception specifications are
//                      supported and FuncInfo::pESTypeList is valid.
// EH_MAGIC_NUMBER3     Used in the FuncInfo if FuncInfo::EHFlags is valid, so
//                      we can check if the function was compiled -EHs or -EHa.

export constexpr Cardinal::UInt64 EH_MAGIC_NUMBER1 = 0x19930520;
export constexpr Cardinal::UInt64 EH_MAGIC_NUMBER2 = 0x19930521;
export constexpr Cardinal::UInt64 EH_MAGIC_NUMBER3 = 0x19930522;

// We use a different magic number in the thrown object's exception record to
// indicate it arose from a pure region.  Pure throws can't be caught by native
// or mixed code, and vice-versa.  Note that the pure magic number is only 7
// digits, not 8, so it will be less than any of the native EH magic numbers
// and won't be detected as an unknown newer magic number by the native EH
// handlers.

export constexpr auto EH_PURE_MAGIC_NUMBER1 = 0x01994000;

export constexpr auto FI_EHS_FLAG = 0x00000001;
export constexpr auto FI_DYNSTKALIGN_FLAG = 0x00000002;
export constexpr auto FI_EHNOEXCEPT_FLAG = 0x00000004;

static_assert(!((EH_MAGIC_NUMBER2 <= EH_MAGIC_NUMBER1) || (EH_MAGIC_NUMBER3 <= EH_MAGIC_NUMBER2)),
    "New magic number must be greater than the old one");

static_assert(!((EH_MAGIC_NUMBER1 & 0xE0000000) || (EH_MAGIC_NUMBER2 & 0xE0000000) || (EH_MAGIC_NUMBER3 & 0xE0000000)),
    "Magic number too big -- must fit into 29 bits");

static_assert(!(EH_PURE_MAGIC_NUMBER1 >= EH_MAGIC_NUMBER1),
    "Pure EH magic number must be less than native one");

export constexpr auto EH_MAGIC_HAS_ES = EH_MAGIC_NUMBER2;	// Magic number is greater or equal than that
// indicates presence of exception specification

export constexpr auto EH_EXCEPTION_PARAMETERS = 4;			// Number of parameters in exception record

//
// Current state of a function.
// -1 is the 'blank' state, i.e. there is nothing to unwind, no try blocks active.
//

export using __ehstate_t = Cardinal::Int32;		// The type of a state index

export constexpr __ehstate_t INVALID_CATCH_SPECIFIC_STATE = -2;
export constexpr __ehstate_t EH_EMPTY_STATE = -1;

export constexpr auto SCOPE_INDEX_SHIFT_VALUE = 2;

export constexpr auto CT_IsSimpleType = 0x00000001;		// type is a simple type
export constexpr auto CT_ByReferenceOnly = 0x00000002;		// type must be caught by reference
export constexpr auto CT_HasVirtualBase = 0x00000004;		// type is a class with virtual bases
export constexpr auto CT_IsWinRTHandle = 0x00000008;		// type is a winrt handle
export constexpr auto CT_IsStdBadAlloc = 0x00000010;		// type is a std::bad_alloc

export constexpr auto TI_IsConst = 0x00000001;		// thrown object has const qualifier
export constexpr auto TI_IsVolatile = 0x00000002;		// thrown object has volatile qualifier
export constexpr auto TI_IsUnaligned = 0x00000004;		// thrown object has unaligned qualifier
export constexpr auto TI_IsPure = 0x00000008;		// object thrown from a pure module
export constexpr auto TI_IsWinRT = 0x00000010;		// object thrown is a WinRT Exception

export constexpr auto HT_IsConst = 0x00000001;		// type referenced is 'const' qualified
export constexpr auto HT_IsVolatile = 0x00000002;		// type referenced is 'volatile' qualified
export constexpr auto HT_IsUnaligned = 0x00000004;		// type referenced is 'unaligned' qualified
export constexpr auto HT_IsReference = 0x00000008;		// catch type is by reference
export constexpr auto HT_IsResumable = 0x00000010;		// the catch may choose to resume (Reserved)
export constexpr auto HT_IsStdDotDot = 0x00000040;		// the catch is std C++ catch(...) which is supposed to catch only C++ exceptions
export constexpr auto HT_IsBadAllocCompat = 0x00000080;		// the WinRT type can catch a std::bad_alloc
export constexpr auto HT_IsComplusEh = 0x80000000;		// Is handling within complus EH

export struct TlsExceptionData
{
    // C++ Exception Handling (EH) state
    unsigned long      _NLG_dwCode = 0;      // Required by NLG routines
    unexpected_handler _unexpected = 0;      // unexpected() routine
    void*              _translator = 0;      // S.E. translator
    void*              _purecall = 0;        // called when pure virtual happens
    void*              _curexception = 0;    // current exception
    void*              _curcontext = 0;      // current exception context
    int                _ProcessingThrow = 0; // for uncaught_exception
    int                _cxxReThrow = 0;      // true if it's a rethrown C++ exception
    void*              _curexcspec = 0;      // for handling exceptions thrown from std::unexpected

    void*              _pExitContext = 0;
    void*              _pUnwindContext = 0;
    void*              _pFrameInfoChain = 0;
    Cardinal::SizeT    _ImageBase = 0;
    Cardinal::SizeT    _ThrowImageBase = 0;
    void*              _pForeignException = 0;
    int                _CatchStateInParent = INVALID_CATCH_SPECIFIC_STATE;   
                                                // Used to link together the catch funclet with the parent. During dispatch contains state associated
                                                // with catch in the parent. During unwind represents the current unwind state that is resumed to
                                                // during collided unwind and used to look for handlers of the throwing dtor.
    
    void*              _throwObjStorageForStoredException = 0; //Stack allocated stored storage 

    bool               _isInCatch = false;            //Use for storing exception
};

export TlsExceptionData* GetThreadExceptionData();