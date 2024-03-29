module;
#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

export module Cardinal.Exceptions.ABI:Types;

export import :Base;

//
// PMFN - Pointer to Member Function
//
typedef	int	PMFN;					// Image relative offset of Member Function

//
// ThrowInfo - information describing the thrown object, statically built
// at the throw site.
//
// pExceptionObject (the dynamic part of the throw; see below) is always a
// reference, whether or not it is logically one.  If 'isSimpleType' is true,
// it is a reference to the simple type, which is 'size' bytes long.  If
// 'isReference' and 'isSimpleType' are both false, then it's a UDT or
// a pointer to any type (i.e. pExceptionObject points to a pointer).  If it's
// a pointer, copyFunction is NULL, otherwise it is a pointer to a copy
// constructor or copy constructor closure.
//
// The pForwardCompat function pointer is intended to be filled in by future
// versions, so that if say a DLL built with a newer version (say C10) throws,
// and a C9 frame attempts a catch, the frame handler attempting the catch (C9)
// can let the version that knows all the latest stuff do the work.
//
typedef const struct _s_ThrowInfo {
	unsigned int	attributes;							// Throw Info attributes (Bit field)
	PMFN			pmfnUnwind;							// Destructor to call when exception has been handled or aborted
	int				pForwardCompat;						// Image relative offset of Forward compatibility frame handler
	int				pCatchableTypeArray;				// Image relative offset of CatchableTypeArray
} ThrowInfo;

//#endif // !__has_include(Rem.Core.RTTI.ExceptionsData.h)

#ifdef __INTELLISENSE__
typedef _s_ThrowInfo _ThrowInfo;
#endif

//
// On P7 we don't have a registration node, just a pointer to the stack frame base
//
typedef Cardinal::SizeT EHRegistrationNode;

/////////////////////////////////////////////////////////////////////////////
//
// The NT Exception record that we use to pass information from the throw to
// the possible catches.
//
// The constants in the comments are the values we expect.
// This is based on the definition of EXCEPTION_RECORD in winnt.h.
//

#pragma pack(push, EHExceptionRecord, 8)
typedef struct EHExceptionRecord {
	unsigned long	ExceptionCode;			// The code of this exception. (= EH_EXCEPTION_NUMBER)
	unsigned long	ExceptionFlags;			// Flags determined by NT
	struct _EXCEPTION_RECORD* ExceptionRecord;	// An extra exception record (not used)
	void* ExceptionAddress;		// Address at which exception occurred
	unsigned long 		NumberParameters;		// Number of extended parameters. (= EH_EXCEPTION_PARAMETERS)
	struct EHParameters {
		unsigned long		magicNumber;		// = EH_MAGIC_NUMBER1
		void* pExceptionObject;	// Pointer to the actual object thrown
		ThrowInfo* pThrowInfo;		// Description of thrown object
		void* pThrowImageBase;	// Image base of thrown object
	} params;
} EHExceptionRecord;

#pragma pack(pop, EHExceptionRecord)

struct DispatcherContext {
	long long ControlPc;
	long long ImageBase;
	PRUNTIME_FUNCTION FunctionEntry;
	ULONG_PTR EstablisherFrame;
	ULONG64 TargetIp;
	PCONTEXT ContextRecord;
	PVOID LanguageHandler;
	PVOID HandlerData;
	PUNWIND_HISTORY_TABLE HistoryTable;
	ULONG ScopeIndex;
};					// changed the case of the name to conform to EH conventions

struct FuncInfoHeader
{
    union
    {
#pragma warning(push)
#pragma warning(disable: 4201) // nonstandard extension used: nameless struct/union
        struct
        {
            Cardinal::UInt8 isCatch        : 1;  // 1 if this represents a catch funclet, 0 otherwise
            Cardinal::UInt8 isSeparated    : 1;  // 1 if this function has separated code segments, 0 otherwise
            Cardinal::UInt8 BBT            : 1;  // Flags set by Basic Block Transformations
            Cardinal::UInt8 UnwindMap      : 1;  // Existence of Unwind Map RVA
            Cardinal::UInt8 TryBlockMap    : 1;  // Existence of Try Block Map RVA
            Cardinal::UInt8 EHs            : 1;  // EHs flag set
            Cardinal::UInt8 NoExcept       : 1;  // NoExcept flag set
            Cardinal::UInt8 reserved       : 1;
        };
#pragma warning(pop)
        Cardinal::UInt8 value;
    };

    FuncInfoHeader()
    {
        value = 0;
    }
};

static_assert(sizeof(FuncInfoHeader) == sizeof(Cardinal::Byte), "Size of FuncInfoHeader not 1 Byte");

struct FuncInfo4
{
    FuncInfoHeader         header;
    Cardinal::UInt32            bbtFlags;            // flags that may be set by BBT processing

    Cardinal::Int32             dispUnwindMap;       // Image relative offset of the unwind map
    Cardinal::Int32             dispTryBlockMap;     // Image relative offset of the handler map
    Cardinal::Int32             dispIPtoStateMap;    // Image relative offset of the IP to state map
    Cardinal::UInt32            dispFrame;           // displacement of address of function frame wrt establisher frame, only used for catch funclets

    FuncInfo4()
    {
        header.value = 0;
        bbtFlags = 0;
        dispUnwindMap = 0;
        dispTryBlockMap = 0;
        dispIPtoStateMap = 0;
        dispFrame = 0;
    }

};

typedef const struct _s_ESTypeList
{
	int 			nCount;			// how many types are there
	int				dispTypeArray;	// offset of list of types in exception specification
}ESTypeList;

//
// PMD - Pointer to Member Data: generalized pointer-to-member descriptor
//
typedef struct PMD
{
	int	mdisp;	// Offset of intended data within base
	int	pdisp;	// Displacement to virtual base pointer
	int	vdisp;	// Index within vbTable to offset of base
	} PMD;


//
// PMFN - Pointer to Member Function
//
typedef	int	PMFN;					// Image relative offset of Member Function

//
// CatchableType - description of a type that can be caught.
//
// Note:  although isSimpleType can be part of ThrowInfo, it is more
//		  convenient for the run-time to have it here.
//
typedef const struct _s_CatchableType {
	unsigned int		properties;			// Catchable Type properties (Bit field)

	int					pType;				// Image relative offset of TypeDescriptor

	PMD 				thisDisplacement;	// Pointer to instance of catch type within thrown object.
	int					sizeOrOffset;		// Size of simple-type object or offset into
											//  buffer of 'this' pointer for catch object
	PMFN				copyFunction;		// Copy constructor or CC-closure
} CatchableType;

//
// CatchableTypeArray - array of pointers to catchable types, with length
//
#pragma warning(push)
#pragma warning(disable:4200)	// nonstandard extension used: array of runtime bound
typedef const struct _s_CatchableTypeArray {
	int	nCatchableTypes;
	int				arrayOfCatchableTypes[];	// Image relative offset of Catchable Types
	} CatchableTypeArray;
#pragma warning(pop)

//
// HandlerType - description of a single 'catch'
//
typedef const struct _s_HandlerType {
	unsigned int	adjectives;			// Handler Type adjectives (bitfield)

	int				dispType;			// Image relative offset of the corresponding type descriptor
	int				dispCatchObj;		// Displacement of catch object from base
	int				dispOfHandler;		// Image relative offset of 'catch' code

	int				dispFrame;			// displacement of address of function frame wrt establisher frame
} HandlerType;

//
// TypeDescriptor - per-type record which uniquely identifies the type.
//
// Each type has a decorated name which uniquely identifies it, and a hash
// value which is computed by the compiler.  The hash function used is not
// important; the only thing which is essential is that it be the same for
// all time.
//
// The special type '...' (ellipsis) is represented by a null name.
//
#pragma warning(push)
#pragma warning(disable:4200)	// nonstandard extension used: array of runtime bound

#if defined(_M_X64) || defined(_M_ARM64) || defined(BUILDING_C1XX_FORCEINCLUDE)
#pragma pack(push, TypeDescriptor, 8)
#endif

typedef struct TypeDescriptor
{
	const void * pVFTable;	// Field overloaded by RTTI
	void *	spare;			// reserved, possible for RTTI
	char			name[];			// The decorated name of the type; 0 terminated.
	} TypeDescriptor;

#if defined(_M_X64) || defined(_M_ARM64) || defined(BUILDING_C1XX_FORCEINCLUDE)
#pragma pack(pop, TypeDescriptor)
#endif
#pragma warning(pop)

// Link together all existing catch objects to determine when they should
// be destroyed
typedef struct FrameInfo
{
    PVOID              pExceptionObject;
    struct FrameInfo*  pNext;
} FRAMEINFO;