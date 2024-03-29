module;
#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

#include "Cardinal.Exceptions.ABI.SEH.h"

module Cardinal.Exceptions.ABI;

import Cardinal.Exceptions;
import Cardinal.Exceptions.Internals;
import Cardinal.Logger;
import Cardinal.Logger.StaticLogger;
import Cardinal.Logger.StaticLogger.Managed;
import Cardinal.Threading;

namespace Cardinal
{
    Exception* GetBaseForInteruptException(InteruptException* pEx);
}

// Defined values for the exception filter expression
#define EXCEPTION_EXECUTE_HANDLER      1
#define EXCEPTION_CONTINUE_SEARCH      0
#define EXCEPTION_CONTINUE_EXECUTION (-1)

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 4191)
#pragma warning(disable: 6255)

__ehstate_t GetMaxState(
        DispatcherContext  *pDC,
        FuncInfo4           *pFuncInfo //-V2558
    )
    {
        if (pFuncInfo->dispUnwindMap)
        {
            Cardinal::UInt8 * buffer = reinterpret_cast<Cardinal::UInt8 *>(__RVAtoRealOffset(pDC, pFuncInfo->dispUnwindMap));
            return ReadUnsigned(&buffer);
        }

        return 0;
    }

uintptr_t adjustIp(DispatcherContext* /*pDC*/, uintptr_t Ip)
{
    return Ip;
}

__ehstate_t StateFromControlPc(
    FuncInfo4          *pFuncInfo,
    DispatcherContext  *pDC
)
{
    uintptr_t Ip = pDC->ControlPc;

    return StateFromIp(pFuncInfo, pDC, Ip);
}
extern "C" {
    extern PVOID __cdecl _CallSettingFrameEncoded(
        void*,
        EHRegistrationNode,
        void*,
        ULONG
    );

    extern PVOID __cdecl _CallSettingFrame(
        void*,
        EHRegistrationNode*,
        ULONG
    );

    extern PVOID __cdecl _CallSettingFrame_LookupContinuationIndex(
        void*,
        EHRegistrationNode*,
        ULONG
    );

    extern PVOID __cdecl _CallSettingFrame_NotifyContinuationAddr(
        void*,
        EHRegistrationNode*
        );
}
//__declspec(guard(ignore)) 

inline void __stdcall _CallMemberFunction0(
    void* const pthis,
    void* const pmfn
    ) noexcept(false)
{
    auto const OneArgFn = reinterpret_cast<void (__vectorcall*)(void*)>(pmfn);
    OneArgFn(pthis);
}

//__declspec(guard(ignore)) 
inline void __stdcall _CallMemberFunction1(
    void* const pthis,
    void* const pmfn,
    void* const pthat
    ) noexcept(false)
{
    auto const TwoArgFn = reinterpret_cast<void (__vectorcall*)(void*, void*)>(pmfn);
    TwoArgFn(pthis, pthat);
}

//__declspec(guard(ignore)) 
inline void __stdcall _CallMemberFunction2(
    void* const pthis,
    void* const pmfn,
    void* const pthat,
    int   const val2
    ) noexcept(false)
{
    auto const ThreeArgFn = reinterpret_cast<void (__vectorcall*)(void*, void*, int)>(pmfn);
    ThreeArgFn(pthis, pthat, val2);
}

extern void StoreInnerException(Cardinal::Exception* outerException, Cardinal::Exception* innnerException);

////////////////////////////////////////////////////////////////////////////////
//
// __DestructExceptionObject - Call the destructor (if any) of the original
//   exception object.
//
// Returns: None.
//
// Side-effects:
//     Original exception object is destructed.
//
// Notes:
//     If destruction throws any exception, and we are destructing the exception
//       object as a result of a new exception, we give up.  If the destruction
extern "C" void __cdecl __DestructExceptionObject(
    EHExceptionRecord * pExcept,         // The original exception record
    BOOLEAN fThrowNotAllowed            // TRUE if destructor not allowed to
                                        //   throw
);

int TypeMatchWithGenericException(
    CatchableType* pCatchable,       // Type conversion under consideration
    ThrowInfo* pThrow        ,       // General information about the thrown
                                     //   type.
    void* throwImageBase             // Image base of throw object  
) {
    auto* rxxParrentExTypeInfo = &typeid(Cardinal::Exception);

    TypeDescriptor* catchableTypeDescriptor =
        reinterpret_cast<TypeDescriptor*>(
            (unsigned long long)throwImageBase +
            pCatchable->pType
            );

    // Not ellipsis; the basic types match if it's the same record *or* the
    // names are identical.
    if (!Cardinal::CStr::Compare(rxxParrentExTypeInfo->GetCxxName(), catchableTypeDescriptor->name))
    {
        return FALSE;
    }

    // Basic types match.  The actual conversion is valid if:
    //   caught by ref if ref required *and*
    //   the qualifiers are compatible *and*
    //   the alignments match *and*
    //   the volatility matches

    return TRUE; //-V2570
}

int TypeMatchWithInteruptException(
    CatchableType* pCatchable,       // Type conversion under consideration
    ThrowInfo* pThrow,              // General information about the thrown
                                     //   type.
    void* throwImageBase             // Image base of throw object  
) {
    auto* rxxParrentExTypeInfo = &typeid(Cardinal::InteruptException);

    TypeDescriptor* catchableTypeDescriptor =
        reinterpret_cast<TypeDescriptor*>(
            (unsigned long long)throwImageBase +
            pCatchable->pType
            );

    // Not ellipsis; the basic types match if it's the same record *or* the
    // names are identical.
    if (!Cardinal::CStr::Compare(rxxParrentExTypeInfo->GetCxxName(), catchableTypeDescriptor->name))
    {
        return FALSE;
    }

    return TRUE; //-V2570
}

Cardinal::Exception* Cardinal::GetBaseForInteruptException(InteruptException* pEx) {
    return (Exception*) pEx;
}

Cardinal::Exception* TryGetAsRcppException(
    EHExceptionRecord* pExcept         // Information for this (logical) exception
) {
    //TODO: _VCRT_VERIFY(pESTypeList);
    BOOLEAN bFoundMatchingTypeInES = FALSE;

    int* ppCatchable = nullptr;
    CatchableType* pCatchable = nullptr;
    int catchables = 0;
    
    // for all types that thrown object can be converted to...
    ppCatchable =
        (int*)(reinterpret_cast<CatchableTypeArray*>(
            (unsigned long long)pExcept->params.pThrowImageBase +
            pExcept->params.pThrowInfo->pCatchableTypeArray)->arrayOfCatchableTypes);

    auto catchableCount = reinterpret_cast<CatchableTypeArray*>(
        (unsigned long long)pExcept->params.pThrowImageBase +
        pExcept->params.pThrowInfo->pCatchableTypeArray)->nCatchableTypes;

    // Scan all types that thrown object can be converted to:
    for (int catchables = catchableCount; catchables > 0; catchables--, ppCatchable++) //-V2528
    {
        pCatchable = (CatchableType*)((unsigned long long)pExcept->params.pThrowImageBase + *ppCatchable);

        if (TypeMatchWithGenericException(pCatchable, pExcept->params.pThrowInfo, pExcept->params.pThrowImageBase))
        {
            return (Cardinal::Exception*)__AdjustPointer(pExcept->params.pExceptionObject, pCatchable->thisDisplacement);
        }

        if(TypeMatchWithInteruptException(pCatchable, pExcept->params.pThrowInfo, pExcept->params.pThrowImageBase))
        {
            return Cardinal::GetBaseForInteruptException((Cardinal::InteruptException*) __AdjustPointer(pExcept->params.pExceptionObject, pCatchable->thisDisplacement));
        }
    }

    return nullptr;
}

////////////////////////////////////////////////////////////////////////////////
//
// __FrameUnwindFilter - Allows possibility of continuing through SEH during
//   unwind.
//
extern "C" int __cdecl __FrameUnwindFilter(
    EXCEPTION_POINTERS *pExPtrs,
    EHExceptionRecord * pOuterExcept
) {
    EHExceptionRecord *pExcept = (EHExceptionRecord *)pExPtrs->ExceptionRecord;

    switch (pExcept->ExceptionCode) {
    case EH_EXCEPTION_NUMBER:
    {
        //Check is it unwind
        if (((PEXCEPTION_RECORD)pOuterExcept)->ExceptionCode == STATUS_UNWIND_CONSOLIDATE) {
            //Try get outer(parrent) exception
            auto* ptrToOuterException = TryGetAsRcppException((EHExceptionRecord*)((PEXCEPTION_RECORD)pOuterExcept)->ExceptionInformation[6]);
            //Try get inner(current) exception
            auto* ptrToInnerException = TryGetAsRcppException(pExcept);

            //Check is it our exceptions
            if (ptrToInnerException != nullptr && ptrToOuterException != nullptr) {
                //if true, get store inner exception in outer exception inners
                StoreInnerException(ptrToOuterException, ptrToInnerException);
                //destroy original inner exception
                __DestructExceptionObject(pExcept, FALSE);
                //signale that we can continue processing outer exception
                return EXCEPTION_EXECUTE_HANDLER;
            }
        }
        
        EmergencyShutdown(SourcePosition, L"Unknown stack winding exception");

        /*
         See VSW#544593 for more details. __ProcessingThrow is used to implement
         std::uncaught_exception(). The interaction between C++, SEH and managed
         exception wrt __ProcessingThrow is unspec'ed. From code inspection, it
         looks like that __ProcessingThrow works ok with all C++ exceptions.

         In this case, when we encounter a managed exception thrown from a destructor
         during unwind, we choose to decrement the count. This means that the previous
         C++ exception which incremented the count won't be considered any longer.
         In fact, the managed exception will be thrown, and the native C++ one will
         not have any possibility to be caught any longer.

         We should revisit std::uncaught_exception() and SEH/managed exception in the
         next version.
         */
        if (GetThreadExceptionData()->_ProcessingThrow > 0) //-V779
        {
            --GetThreadExceptionData()->_ProcessingThrow;
        }
        return EXCEPTION_CONTINUE_SEARCH;
    }
    default:
        return EXCEPTION_CONTINUE_SEARCH;
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// FrameUnwindToState - Unwind this frame until specified state is reached.
//
// Returns:
//     No return value.
//
// Side Effects:
//     All objects on frame which go out of scope as a result of the unwind are
//       destructed.
//     Registration node is updated to reflect new state.
//
// Usage:
//      This function is called both to do full-frame unwind during the unwind
//      phase (targetState = -1), and to do partial unwinding when the current
//      frame has an appropriate catch.
void FrameUnwindToState(
    EHRegistrationNode *pRN,            // Registration node for subject
                                        //   function
    DispatcherContext *pDC,             // Context within subject frame
    FuncInfo4  *pFuncInfo,              // Static information for subject
                                        //   function
    __ehstate_t targetState,            // State to unwind to
    EHExceptionRecord* pOuterExcept     // Current exception
    )
{
    ptrdiff_t unwindImageBase = GetThreadExceptionData()->_ImageBase;
    __ehstate_t curState = 0;

    // For collided unwinds
    if (pDC->ScopeIndex != 0)
    {
        curState = (__ehstate_t)pDC->ScopeIndex - SCOPE_INDEX_SHIFT_VALUE;
    }
    // For unwinds in parent after catch has been invoked
    else if (GetThreadExceptionData()->_CatchStateInParent != INVALID_CATCH_SPECIFIC_STATE)
    {
        curState = GetThreadExceptionData()->_CatchStateInParent;
        GetThreadExceptionData()->_CatchStateInParent = INVALID_CATCH_SPECIFIC_STATE;
    }
    else
    {
        curState = StateFromControlPc(pFuncInfo, pDC);
    }
    GetThreadExceptionData()->_ProcessingThrow++;

    // Find starting and ending indexes in the decompressed map
    auto unwindMap = UWMap4(pFuncInfo, pDC->ImageBase);

    auto start = unwindMap.begin();
    auto end = unwindMap.begin();

    //_VCRT_VERIFY(start >= end);
    unwindMap.getStartStop(curState, targetState, start, end);

    __try
    {
        while (start >= unwindMap.begin() && start > end)
        {
            UnwindMapEntry4 UWEntry = *start;
            auto prevIter = start;

            start.WalkBack();

            __ehstate_t prevState = curState;
            curState = UWMap4::getStateFromIterators(end, targetState, prevIter, prevState, start);

            // default values taken if UnwindMapEntry4::Type::NoUW
            unsigned dispAction = 0;

            if (UWEntry.type != UnwindMapEntry4::Type::NoUW)
            {
                dispAction = UWEntry.action;
            }

            if (dispAction != 0) {
                // Before calling unwind action, adjust state as if it were
                // already completed:
                pDC->ScopeIndex = (DWORD)(curState + SCOPE_INDEX_SHIFT_VALUE);


                //EHTRACE_FMT2("Unwind from state %d to state %d\n", prevState, curState);
                __try {
                    // Call the unwind action (if one exists):
                    if ((UWEntry.type == UnwindMapEntry4::Type::DtorWithObj) || (UWEntry.type == UnwindMapEntry4::Type::DtorWithPtrToObj))
                    {
                        VOID * objectAddr = nullptr;
                        if (UWEntry.type == UnwindMapEntry4::Type::DtorWithPtrToObj) {
                            objectAddr = (void*)(*(uintptr_t *)OffsetToAddress(UWEntry.object, *pRN));
                        }
                        else {
                            objectAddr = OffsetToAddress(UWEntry.object, *pRN);
                        }

                        _CallSettingFrameEncoded((void*)__RVAtoRealOffset(pDC, dispAction),
                            *pRN,
                                objectAddr,
                            0x103);
                    }
                    else
                    {   // TODO: this is currently hardcoded for non-x86, as x86 seems to encode
                        // its RVAs as non-image relative, will require fix for that
                        _CallSettingFrame((void*)__RVAtoRealOffset(pDC, dispAction), pRN,
                            0x103);
                    }

                    GetThreadExceptionData()->_ImageBase = unwindImageBase;
                }
                __except (__FrameUnwindFilter(exception_info(), pOuterExcept)) {
                    ; // Deliberately do nothing
                }
            }
        }
    }
    __finally {
        if (GetThreadExceptionData()->_ProcessingThrow > 0) {
            GetThreadExceptionData()->_ProcessingThrow--;
        }
    }

    //EHTRACE_FMT2("Moved from state %d to state %d", curState, targetState);
}

__ehstate_t StateFromIp(
    FuncInfo4           *pFuncInfo, //-V2558
    DispatcherContext   *pDC,
    uintptr_t           Ip
)
{
    unsigned int        index = 0;          //  loop control variable
    unsigned int        nIPMapEntry = 0;    //  # of IpMapEntry; must be > 0

    Ip = adjustIp(pDC, Ip);

    if (pFuncInfo->dispIPtoStateMap == 0)
    {
        return EH_EMPTY_STATE;
    }

    PBYTE buffer = (PBYTE)__RVAtoRealOffset(pDC, pFuncInfo->dispIPtoStateMap);

    nIPMapEntry = ReadUnsigned(&buffer);

    __ehstate_t prevState = EH_EMPTY_STATE;
    unsigned int funcRelIP = 0;
    for (index = 0; index < nIPMapEntry; index++) {
        // States are delta-encoded relative to start of the function
        funcRelIP += ReadUnsigned(&buffer);
        if (Ip < __FuncRelToRealOffset(pDC, funcRelIP)) {
            break;
        }
        // States are encoded +1 so as to not encode a negative
        prevState = ReadUnsigned(&buffer) - 1;
    }

    if (index == 0) {
        // We are at the first entry, could be an error
        return EH_EMPTY_STATE;
    }

    // We over-shot one iteration; return state from the previous slot
    return prevState;
}

//
// This routine returns TRUE if we are executing from within a catch.  Otherwise, FALSE is returned.
//
bool ExecutionInCatch(
    DispatcherContext*  /*pDC*/,
    FuncInfo4            *pFuncInfo //-V2558
    )
{
    return pFuncInfo->header.isCatch;
}

//
// Returns the establisher frame pointers. For catch handlers it is the parent's frame pointer.
//
EHRegistrationNode * GetEstablisherFrame
(
    EHRegistrationNode  *pRN, //-V2558
    DispatcherContext   *pDC,
    FuncInfo4            *pFuncInfo,
    EHRegistrationNode  *pEstablisher
    )
{

    *pEstablisher = *pRN;

    if (ExecutionInCatch(pDC, pFuncInfo))
    {
        *pEstablisher = *(EHRegistrationNode *)OffsetToAddress(pFuncInfo->dispFrame, *pRN);
    }

    return pEstablisher;
}

// The name of this function is rather misleading. This function won't really unwind
// to empty state. This function will unwind to lowest possible state for current block.
// Here is an example
//
// try {
//   // State = 1;
// } catch (...) {
//   // State when we enter catch is 2
//   // State inside catch is 3-5;
// }
// if __FrameUnwindToEmptyState is called for the main function, the target state will
// be -1 but if __FrameUnwindToEmptyState is called for catch block, the target state for
// __FrameUnwindToState will be 2 not -1. This way we are able to unwind the stack block
// by block not the whole function in single call.
void FrameUnwindToEmptyState(
    EHRegistrationNode *pRN,
    DispatcherContext  *pDC,
    FuncInfo4           *pFuncInfo,
    EHExceptionRecord* pOuterExcept     // Current exception
    )
{
    EHRegistrationNode EstablisherFramePointers = 0, *pEstablisher = nullptr;

    pEstablisher = GetEstablisherFrame(pRN, pDC, pFuncInfo, &EstablisherFramePointers);

    FrameUnwindToState(pEstablisher, pDC, pFuncInfo, EH_EMPTY_STATE, pOuterExcept);
}

////////////////////////////////////////////////////////////////////////////////
//
// __InternalCxxFrameHandler - the frame handler for all functions with C++ EH
// information.
//
// If exception is handled, this doesn't return; otherwise, it returns
// ExceptionContinueSearch.
//
// Note that this is called three ways:
//     From __CxxFrameHandler: primary usage, called to inspect whole function.
//         CatchDepth == 0, pMarkerRN == nullptr
//     From CatchGuardHandler: If an exception occurred within a catch, this is
//         called to check for try blocks within that catch only, and does not
//         handle unwinds.
//     From TranslatorGuardHandler: Called to handle the translation of a
//         non-C++ EH exception.  Context considered is that of parent.
EXCEPTION_DISPOSITION InternalCxxFrameHandler(
    EHExceptionRecord* pExcept,         // Information for this exception
    EHRegistrationNode* pRN,            // Dynamic information for this frame
    CONTEXT* pContext,                  // Context info
    DispatcherContext* pDC,             // Context within subject frame
    FuncInfo4* pFuncInfo,               // Static information for this frame
    int CatchDepth,                     // How deeply nested are we?
    EHRegistrationNode* pMarkerRN,      // Marker node for when checking inside
                                        //  catch block
    BOOLEAN recursive                   // Are we handling a translation?
) {
    if (GetThreadExceptionData()->_cxxReThrow == false &&
        pExcept->ExceptionCode != EH_EXCEPTION_NUMBER &&
         /* On the 64 bit/ARM platforms, ExceptionCode maybe set to STATUS_UNWIND_CONSOLIDATE
           when called from _UnwindNestedFrames during Logical Unwind. _UnwindNestedFrames
           will also set EH_MAGIC_NUMBER1 in the 8 element */
        (!(pExcept->ExceptionCode == STATUS_UNWIND_CONSOLIDATE && pExcept->NumberParameters == 15 && ((PULONG_PTR) &pExcept->params.magicNumber)[8] == EH_MAGIC_NUMBER1)) && //-V557
        pExcept->ExceptionCode != STATUS_LONGJUMP &&
        pFuncInfo->header.EHs)
    {
         /*
         * This function was compiled /EHs so we don't need to do anything in
         * this handler.
         */
        return ExceptionContinueSearch;
    }

    if (IS_UNWINDING(pExcept->ExceptionFlags))
    {
        // We're at the unwinding stage of things.  Don't care about the
        // exception itself.  (Check this first because it's easier)

        if (GetMaxState(pDC, pFuncInfo) != 0 && CatchDepth == 0)
        {
            // Only unwind if there's something to unwind
            // AND we're being called through the primary RN.

            if (IS_TARGET_UNWIND(pExcept->ExceptionFlags) && pExcept->ExceptionCode == STATUS_LONGJUMP)
            {
                __ehstate_t target_state = StateFromIp(
                    pFuncInfo,
                    pDC,
                    pDC->TargetIp
                );

                //TODO: _VCRT_VERIFY(target_state >= EH_EMPTY_STATE && target_state < T::GetMaxState(pDC, pFuncInfo));
                FrameUnwindToState(pRN, pDC, pFuncInfo, target_state, pExcept);
                //TODO: Log it: EHTRACE_HANDLER_EXIT(ExceptionContinueSearch);
                return ExceptionContinueSearch;
            }

            if (IS_TARGET_UNWIND(pExcept->ExceptionFlags) && pExcept->ExceptionCode == STATUS_UNWIND_CONSOLIDATE)
            {
                PEXCEPTION_RECORD pSehExcept = (PEXCEPTION_RECORD)pExcept;
                __ehstate_t target_state = (__ehstate_t)pSehExcept->ExceptionInformation[3];

                //TODO: _VCRT_VERIFY(target_state >= EH_EMPTY_STATE && target_state < T::GetMaxState(pDC, pFuncInfo));
                FrameUnwindToState((EHRegistrationNode *)pSehExcept->ExceptionInformation[1],
                                   pDC,
                                   pFuncInfo,
                                   target_state,
                                   pExcept);
                //TODO: Log it: EHTRACE_HANDLER_EXIT(ExceptionContinueSearch);
                return ExceptionContinueSearch;
            }

            FrameUnwindToEmptyState(pRN, pDC, pFuncInfo, pExcept);
        }
        return ExceptionContinueSearch;
    }

    auto tryBlockMap = TryBlockMap4(pFuncInfo, pDC->ImageBase);
    
    // Set threadable state
    if (Cardinal::Threading::GetTLS()->State != nullptr || Cardinal::Threading::GetTLS()->SecondState != nullptr) {
        *Cardinal::Threading::GetTLS()->SecondState = Cardinal::Threading::EThreadabledStates::ExceptionProcessing;
        Cardinal::Swap(*Cardinal::Threading::GetTLS()->SecondState, *Cardinal::Threading::GetTLS()->State);
    }

    if (tryBlockMap.getNumTryBlocks() != 0
        //
        // If the function has no try block, we still want to call the
        // frame handler if there is an exception specification
        //
        || (pFuncInfo->header.NoExcept != 0))
    {
        // NT is looking for handlers.  We've got handlers.
        // Let's check this puppy out.  Do we recognize it?

        if (pExcept->ExceptionCode == EH_EXCEPTION_NUMBER &&
            pExcept->NumberParameters >= 3 &&
            pExcept->params.magicNumber > EH_MAGIC_NUMBER3)
        {
            const auto pfn = pExcept->params.pThrowInfo->pForwardCompat != 0 ?
                reinterpret_cast<int(__cdecl*)(...)>(
                    reinterpret_cast<Cardinal::SizeT>(pExcept->params.pThrowImageBase) +
                    pExcept->params.pThrowInfo->pForwardCompat
                    )
                :
                nullptr;

            if (pfn != nullptr)
            {
                // Forward compatibility:  The thrown object appears to have been
                // created by a newer version of our compiler.  Let that version's
                // frame handler do the work (if one was specified).

                EXCEPTION_DISPOSITION result = (EXCEPTION_DISPOSITION)pfn(pExcept, pRN, pContext, pDC, pFuncInfo,
                    CatchDepth, pMarkerRN, recursive);
                //TODO: Log it: EHTRACE_HANDLER_EXIT(result);
                return result;
            }
        }

        // Anything else: we'll handle it here.
        FindHandler(pExcept, pRN, (_CONTEXT*) pContext, pDC, pFuncInfo, recursive, CatchDepth, pMarkerRN);
    }

    // We had nothing to do with it or it was rethrown.  Keep searching.
    //TODO: Log it: EHTRACE_HANDLER_EXIT(ExceptionContinueSearch);
    return ExceptionContinueSearch;
}

int TypeMatchHelper(
    HandlerType4 *pCatch,            // Type of the 'catch' clause 
    CatchableType *pCatchable,       // Type conversion under consideration
    ThrowInfo *pThrow                // General information about the thrown
                                     //   type.
) {
    TypeDescriptor* typeDescriptor = pCatch->dispType != 0 ?
        reinterpret_cast<TypeDescriptor*>(
            GetThreadExceptionData()->_ImageBase +
            pCatch->dispType
            )
        :
        nullptr;

    // First, check for match with ellipsis:
    if (typeDescriptor == nullptr || typeDescriptor->name[0] == '\0') {
        return TRUE;
    }

    // Check is bad alloc exception
    if (pCatch->adjectives & HT_IsBadAllocCompat && pCatchable->properties & CT_IsStdBadAlloc)
    {
        return TRUE;
    }

    TypeDescriptor* catchableTypeDescriptor =
        reinterpret_cast<TypeDescriptor*>(
            GetThreadExceptionData()->_ThrowImageBase +
            pCatchable->pType
            );

    // Not ellipsis; the basic types match if it's the same record *or* the
    // names are identical.
    if (!(typeDescriptor == catchableTypeDescriptor ||
        Cardinal::CStr::Compare(typeDescriptor->name, catchableTypeDescriptor->name)))
    {
        return FALSE;
    }

    // Basic types match.  The actual conversion is valid if:
    //   caught by ref if ref required *and*
    //   the qualifiers are compatible *and*
    //   the alignments match *and*
    //   the volatility matches
    return 
        (!(pCatchable->properties & CT_ByReferenceOnly) || pCatch->adjectives & HT_IsReference) &&
        (!(pThrow->attributes & TI_IsConst)             || pCatch->adjectives & HT_IsConst) &&
        (!(pThrow->attributes & TI_IsUnaligned)         || pCatch->adjectives & HT_IsUnaligned) &&
        (!(pThrow->attributes & TI_IsVolatile)          || pCatch->adjectives & HT_IsVolatile); 
}

////////////////////////////////////////////////////////////////////////////////
//
// __TypeMatch - Check if the catch type matches the given throw conversion.
//
// Returns:
//     TRUE if the catch can catch using this throw conversion, FALSE otherwise.
int TypeMatch(
    HandlerType4   *pCatch,
    CatchableType *pCatchable,
    ThrowInfo     *pThrow
)
{
    return TypeMatchHelper(pCatch, pCatchable, pThrow);
};

//////////////////////////////////////////////////////////////////////////////////
// IsInExceptionSpec - Checks if the exception matches the exception specification
//                     list. Returns TRUE if it does, otherwise FALSE
//
BOOLEAN IsInExceptionSpec(
    EHExceptionRecord *pExcept,         // Information for this (logical) exception
    ESTypeList *pESTypeList             // Static information for subject frame //-V2558
)
{
    //TODO: _VCRT_VERIFY(pESTypeList);
    BOOLEAN bFoundMatchingTypeInES = FALSE;

    __int32 *ppCatchable = nullptr;
    CatchableType *pCatchable = nullptr;
    int catchables = 0;

    // for every type in the exception spec...
    for (int i = 0; i < pESTypeList->nCount; i++)
    {
        // for all types that thrown object can be converted to...
        ppCatchable =
            reinterpret_cast<int*>(
                reinterpret_cast<Cardinal::SizeT>(pExcept->params.pThrowImageBase) +
                *reinterpret_cast<CatchableTypeArray*>(
                    static_cast<unsigned long long>(reinterpret_cast<Cardinal::SizeT>(pExcept->params.pThrowImageBase)) +
                    pExcept->params.pThrowInfo->pCatchableTypeArray
                    )->arrayOfCatchableTypes
                );
        ;

        auto n = reinterpret_cast<CatchableTypeArray*>(
            reinterpret_cast<Cardinal::SizeT>(pExcept->params.pThrowImageBase) +
            pExcept->params.pThrowInfo->pCatchableTypeArray
            )->nCatchableTypes;

        HandlerType4* handlerTypes = reinterpret_cast<HandlerType4*>
            (reinterpret_cast<Cardinal::SizeT>(pExcept->params.pThrowImageBase) +
                pESTypeList->dispTypeArray
                );

        for (catchables = n; catchables > 0; catchables--, ppCatchable++) { //-V2528
            pCatchable = (CatchableType *)(reinterpret_cast<Cardinal::SizeT>(pExcept->params.pThrowImageBase) + *ppCatchable);

            if (TypeMatch(&handlerTypes[i], pCatchable, pExcept->params.pThrowInfo))
            {
                bFoundMatchingTypeInES = TRUE;
                break;
            }
        }
    }

    return bFoundMatchingTypeInES;
}

//////////////////////////////////////////////////////////////////////////////////
// Is_bad_exception_allowed - checks if std::bad_exception belongs to the list
//
BOOLEAN Is_bad_exception_allowed(ESTypeList *pESTypeList) //-V2558
{
    HandlerType* typesList =
        reinterpret_cast<HandlerType*>(
            GetThreadExceptionData()->_ImageBase +
            pESTypeList->dispTypeArray);

    for (int i = 0; i < pESTypeList->nCount; i++)
    {
        type_info* typeinfo = reinterpret_cast<type_info*>(
            GetThreadExceptionData()->_ImageBase +
            typesList[i].dispType);

        if (typeinfo != nullptr && *typeinfo == GetBadExceptionTypeId())
        {
            return TRUE;
        }
    }

    return FALSE;
}

DWORD _FilterSetCurrentException(EXCEPTION_POINTERS* pointers, BOOLEAN fThrowNotAllowed)
{
    if (fThrowNotAllowed) {
        const auto eRecord = reinterpret_cast<EHExceptionRecord*>(pointers->ExceptionRecord);
        if (
            eRecord->ExceptionCode == EH_EXCEPTION_NUMBER &&
            eRecord->NumberParameters == EH_EXCEPTION_PARAMETERS &&
            (
                eRecord->params.magicNumber == EH_MAGIC_NUMBER1 ||
                eRecord->params.magicNumber == EH_MAGIC_NUMBER2 ||
                eRecord->params.magicNumber == EH_MAGIC_NUMBER3 
                )
            )
        {
            // Can't have new exceptions when we're unwinding due to another
            // exception.

            //Check is it unwind

            //Try get outer(parrent) exception
            auto* ptrToOuterException = TryGetAsRcppException((EHExceptionRecord*) ((PEXCEPTION_RECORD) GetThreadExceptionData())->ExceptionInformation[6]);
            //Try get inner(current) exception
            auto* ptrToInnerException = TryGetAsRcppException(eRecord);

            //Check is it our exceptions
            if (ptrToInnerException != nullptr && ptrToOuterException != nullptr)
            {
                //if true, get store inner exception in outer exception inners
                StoreInnerException(ptrToOuterException, ptrToInnerException);
                //destroy original inner exception
                __DestructExceptionObject(eRecord, FALSE);
                //signale that we can continue processing outer exception
                return EXCEPTION_EXECUTE_HANDLER;
            }

            EmergencyShutdown(SourcePosition, L"Nested exceptions are not implemented");
        }
    }

    return EXCEPTION_CONTINUE_SEARCH;
}

////////////////////////////////////////////////////////////////////////////////
//
// __DestructExceptionObject - Call the destructor (if any) of the original
//   exception object.
//
// Returns: None.
//
// Side-effects:
//     Original exception object is destructed.
//
// Notes:
//     If destruction throws any exception, and we are destructing the exception
//       object as a result of a new exception, we give up.  If the destruction
extern "C" void __cdecl __DestructExceptionObject(
    EHExceptionRecord *pExcept,         // The original exception record
    BOOLEAN fThrowNotAllowed            // TRUE if destructor not allowed to
                                        //   throw
) {
    // Ignore if not a C++ exception (since this is now called from
    // _except_handler3)
    if (pExcept == nullptr || !(pExcept->ExceptionCode == EH_EXCEPTION_NUMBER))
    {
        return;
    }

    //TODO: EHTRACE_FMT1("Destroying object @ 0x%p", PER_PEXCEPTOBJ(pExcept));

    /*UNDONE:Is this _SYSCRT specific */
#pragma prefast(suppress:__WARNING_REDUNDANT_POINTER_TEST, "Current definition of PER_CODE ensures that pExcept cannot be nullptr")
    if (pExcept->params.pThrowInfo != nullptr)  {
        if (pExcept->params.pThrowInfo->pmfnUnwind != 0) {

            __try {
                

                _CallMemberFunction0(
                    pExcept->params.pExceptionObject,
                    reinterpret_cast<void(*)(void*)>(
                        reinterpret_cast<Cardinal::SizeT>(
                            pExcept->params.pThrowImageBase) + pExcept->params.pThrowInfo->pmfnUnwind)
                );

            } __except(_FilterSetCurrentException(GetExceptionInformation(), fThrowNotAllowed)) {}
        }
    }
}

TryBlockMap4::IteratorPair GetRangeOfTrysToCheck(
    TryBlockMap4    &tryBlockMap,
    __ehstate_t    curState,
    DispatcherContext * /*pDC*/,
    FuncInfo4          * /*pFuncInfo*/,
    int            /*CatchDepth*/
)
{
    auto iterStart = tryBlockMap.begin();
    auto iterEnd = tryBlockMap.begin(); //-V656
    tryBlockMap.setBuffer(iterStart);

    for (auto iter = tryBlockMap.begin(); iter != tryBlockMap.end(); ++iter)
    {
        auto tryBlock = *iter;
        if (curState >= tryBlock.tryLow && curState <= tryBlock.tryHigh) {
            if (iterStart != tryBlockMap.begin()) {
                iterStart = iter;
            }
            iterEnd = iter;
        }
    }
    // change to be (start, end]
    iterEnd.incrementToSentinel();
    // Reset so when we start reading it starts at the correct location
    tryBlockMap.setBuffer(iterStart);
    return TryBlockMap4::IteratorPair{ iterStart, iterEnd };
}

// Call the SEH to EH translator.
int SehTransFilter(
    EXCEPTION_POINTERS    *ExPtrs,
    EHExceptionRecord     *pExcept,
    EHRegistrationNode    *pRN,
    CONTEXT               *pContext,
    DispatcherContext     *pDC,
    FuncInfo4             *pFuncInfo,
    BOOL                  *pResult
) {
    GetThreadExceptionData()->_pForeignException = pExcept;
    GetThreadExceptionData()->_ThrowImageBase = (uintptr_t)((EHExceptionRecord *)ExPtrs->ExceptionRecord)->params.pThrowImageBase;

    InternalCxxFrameHandler((EHExceptionRecord *)ExPtrs->ExceptionRecord,
                                pRN,
                                pContext,
                                pDC,
                                pFuncInfo,
                                0,
                                nullptr,
                                TRUE );
    GetThreadExceptionData()->_pForeignException = nullptr;
    *pResult = TRUE;
    return EXCEPTION_EXECUTE_HANDLER;
}

BOOL _CallSETranslator(
    EHExceptionRecord    *pExcept,    // The exception to be translated
    EHRegistrationNode   *pRN,        // Dynamic info of function with catch //-V2558
    CONTEXT              *pContext,   // Context info
    DispatcherContext    *pDC,        // More dynamic info of function with catch (ignored)
    FuncInfo4            *pFuncInfo,  // Static info of function with catch
    ULONG                CatchDepth,  // How deeply nested in catch blocks are we?
    EHRegistrationNode   *pMarkerRN   // Marker for parent context
    )
{
    UNREFERENCED_PARAMETER(pMarkerRN);

    BOOL result = FALSE;
    pRN;
    pDC;
    pFuncInfo;
    CatchDepth;

    // Call the translator.

    _EXCEPTION_POINTERS excptr = { (PEXCEPTION_RECORD)pExcept, pContext };

    __try {
        SehTranslatorFunction(pExcept->ExceptionCode, &excptr);
        result = FALSE;
    } __except(SehTransFilter(exception_info(),
                                pExcept,
                                pRN,
                                pContext,
                                pDC,
                                pFuncInfo,
                                &result
                            )) {}

    // If we got back, then we were unable to translate it.

    return result;
}

////////////////////////////////////////////////////////////////////////////////
//
// FindHandlerForForeignException - We've got an exception which wasn't ours.
//     Try to translate it into C++ EH, and also check for match with ellipsis.
//
// Description:
//     If an SE-to-EH translator has been installed, call it.  The translator
//     must throw the appropriate typed exception or return.  If the translator
//     throws, we invoke FindHandler again as the exception filter.
//
// Returns:
//     Returns if exception was not fully handled.
//     No return value.
//
// Assumptions:
//     Only called if there are handlers in this function.

void FindHandlerForForeignException(
    EHExceptionRecord *pExcept,         // Information for this (logical)
                                        //   exception
    EHRegistrationNode *pRN,            // Dynamic information for subject frame
    CONTEXT *pContext,                  // Context info
    DispatcherContext *pDC,             // Context within subject frame
    FuncInfo4 *pFuncInfo,               // Static information for subject frame
    __ehstate_t curState,               // Current state
    int catchDepth,                     // Level of nested catch that is being
                                        //   checked
    EHRegistrationNode *pMarkerRN       // Extra marker RN for nested catch
                                        //   handling
    )
{
    // We don't want to touch BreakPoint generated Exception.
    if (pExcept->ExceptionCode == STATUS_BREAKPOINT) {
        return;
    }

    // Call the translator.  If the translator knows what to
    // make of it, it will throw an appropriate C++ exception.
    // We intercept it and use it (recursively) for this
    // frame.  Don't recurse more than once.
    if (_CallSETranslator(pExcept, pRN, pContext, pDC, pFuncInfo,
        catchDepth, pMarkerRN)) {
        return;
    }

    auto tryBlockMap = TryBlockMap4(pFuncInfo, pDC->ImageBase);

    //TODO: _VCRT_VERIFY(tryBlockMap.getNumTryBlocks() > 0);

    if (tryBlockMap.getNumTryBlocks() > 0)
    {
        // Didn't have a translator, or the translator returned normally (i.e.
        // didn't translate it).  Still need to check for match with ellipsis:
        auto startStop = GetRangeOfTrysToCheck(tryBlockMap, curState, pDC, pFuncInfo, catchDepth);

        // Scan the try blocks in the function:
        for (auto iter = startStop.first; iter < startStop.last; ++iter)
        {
            auto tryBlock = *iter;

            // If the try-block was in scope...
            if (tryBlock.tryLow > curState || curState > tryBlock.tryHigh) {
                continue;
            }

            // *and* the last catch in that try is an ellipsis (no other can be)
            auto handlerMap = HandlerMap4(&tryBlock, pDC->ImageBase, pDC->FunctionEntry->BeginAddress);
            auto handler = handlerMap.getLastEntry();

            TypeDescriptor* handlerType =
                reinterpret_cast<TypeDescriptor*>(
                    GetThreadExceptionData()->_ImageBase +
                    handler->dispType
                    );

            if (!(handlerType == nullptr ||  handlerType->name[0] == '\0')
                && !(handler->adjectives & HT_IsStdDotDot))
            {
                continue;
            }

            // Found an ellipsis.  Handle exception.
            CatchIt(pExcept,
                    pRN,
                    (_CONTEXT*) pContext,
                    pDC,
                    pFuncInfo,
                    handler,
                    nullptr,
                    &tryBlock,
                    catchDepth,
                    pMarkerRN,
                    TRUE,
                    FALSE);
        }

        // If it returns, handler re-threw.  Keep searching.

    } // Search for try

    // If we got here, that means we didn't have anything to do with the
    // exception.  Continue search.
}

/*
* Here We find what is the actual State of current function. The way we
* do this is first get State from ControlPc.
*
* Remember we have __GetUnwindTryBlock to remember the last State for which
* Exception was handled and __GetCurrentState for retriving the current
* state of the function. Please Note that __GetCurrentState is used
* primarily for unwinding purpose.
*
* Also remember that all the catch blocks act as funclets. This means that
* ControlPc for all the catch blocks are different from ControlPc of parent
* catch block or function.
*
* take a look at this example
* try {
*   // STATE1 = 1
*   try {
*     // STATE2
*     // THROW
*   } catch (...) { // CatchB1
*     // STATE3
*     // RETHROW OR NEW THROW
*   }
* } catch (...) { // CatchB2
* }
*
* If we have an exception comming from STATE3, the FindHandler will be
* called for CatchB1, at this point we do the test which State is our
* real state, curState from ControlPc or state from __GetUnwindTryBlock.
* Since curState from ControlPc is greater, we know that real State is
* curState from ControlPc and thus we update the UnwindTryBlockState.
*
* On further examination, we found out that there is no handler within
* this catch block, we return without handling the exception. For more
* info on how we determine if we have handler, have a look at
* GetRangeOfTrysToCheck.
*
* Now FindHandler will again be called for parent function. Here again
* we test which is real State, state from ControlPc or State from
* __GetUnwindTryBlock. This time state from __GetUnwindTryBlock is correct.
*
* Also look at code in __CxxCallCatchBlock, you will se that as soon as we get
* out of last catch block, we reset __GetUnwindTryBlock state to -1.
*/
inline __ehstate_t GetHandlerSearchState(
    EHRegistrationNode* /*pRN*/,
    DispatcherContext   *pDC,
    FuncInfo4           *pFuncInfo
)
{
    __ehstate_t curState = StateFromControlPc(pFuncInfo, pDC);

    if (pDC->ScopeIndex != 0)
    {
        // ScopeIndex state from collided unwinds take precedence-there should also
        // be no _CatchStateInParent to use at all
        //_VCRT_VERIFY(CatchStateInParent == INVALID_CATCH_SPECIFIC_STATE);
        curState = (__ehstate_t)pDC->ScopeIndex - SCOPE_INDEX_SHIFT_VALUE;
    }
    else if (GetThreadExceptionData()->_CatchStateInParent != INVALID_CATCH_SPECIFIC_STATE)
    {
        curState = GetThreadExceptionData()->_CatchStateInParent;
        GetThreadExceptionData()->_CatchStateInParent = INVALID_CATCH_SPECIFIC_STATE;
    }

    return curState;
}

void FindHandler(
    EHExceptionRecord* pExcept,         // Information for this (logical)
                                        //   exception
    EHRegistrationNode* pRN,            // Dynamic information for subject frame
    CONTEXT* pContext,                  // Context info
    DispatcherContext* pDC,             // Context within subject frame
    FuncInfo4* pFuncInfo,               // Static information for subject frame
    BOOLEAN recursive,                  // TRUE if we're handling the
                                        //   translation
    int CatchDepth,                     // Level of nested catch that is being
                                        //   checked
    EHRegistrationNode* pMarkerRN       // Extra marker RN for nested catch
                                        //   handling
) {
    BOOLEAN IsRethrow = FALSE;
    BOOLEAN gotMatch = FALSE;

    // Get the current state (machine-dependent)
    __ehstate_t curState = EH_EMPTY_STATE;
    curState = GetHandlerSearchState(pRN, pDC, pFuncInfo);

    //TODO: _VCRT_VERIFY(curState >= EH_EMPTY_STATE && curState < T::GetMaxState(pDC, pFuncInfo));

    // Check if it's a re-throw.  Use the exception we stashed away if it is.
    if ((
        pExcept->ExceptionCode == EH_EXCEPTION_NUMBER &&
        pExcept->NumberParameters == EH_EXCEPTION_PARAMETERS &&
        (
            pExcept->params.magicNumber == EH_MAGIC_NUMBER1 ||
            pExcept->params.magicNumber == EH_MAGIC_NUMBER2 ||
            pExcept->params.magicNumber == EH_MAGIC_NUMBER3
            )
        ) && pExcept->params.pThrowInfo == nullptr)
    {
        if (GetThreadExceptionData()->_curexception == nullptr)
        {
            // Oops!  User re-threw a non-existent exception!  Let it propagate.
            return;
        }

        pExcept = reinterpret_cast<EHExceptionRecord*>(GetThreadExceptionData()->_curexception);
        pContext = reinterpret_cast<CONTEXT*>(GetThreadExceptionData()->_curcontext);
        IsRethrow = TRUE;
        GetThreadExceptionData()->_ThrowImageBase = reinterpret_cast<ptrdiff_t>(pExcept->params.pThrowImageBase);
        //TODO: _VCRT_VERIFY(pExcept && (!PER_IS_MSVC_EH(pExcept) || PER_PTHROW(pExcept)));

        //
        // We know it is a rethrow -- did we come here as a result of an
        // exception re-thrown from CallUnexpected() ?
        //
        if (GetThreadExceptionData()->_curexcspec)
        {
            // remember it in a local variable
            ESTypeList* pCurrentFuncInfo = reinterpret_cast<ESTypeList*>(GetThreadExceptionData()->_curexcspec); 
            // and reset it immediately -- so we don't forget to do it later
            GetThreadExceptionData()->_curexcspec = nullptr;

            // Does the exception thrown by CallUnexpected belong to the exception specification?

            if (IsInExceptionSpec(pExcept, pCurrentFuncInfo))
            {
                // Yes it does -- so "continue the search for another handler at the call of the function
                // whose exception-specification was violated"
                ;
            }
            else
            {
                // Nope, it does not. Is std::bad_exception allowed by the spec?

                if (Is_bad_exception_allowed(pCurrentFuncInfo))
                {
                    // yup -- so according to the standard, we need to replace the thrown
                    // exception by an implementation-defined object of the type std::bad_exception
                    // and continue the search for another handler at the call of the function
                    // whose exception-specification was violated.

                    // Just throw bad_exception -- we will then come into FindHandler for the third time --
                    // but make sure we will not get here again

                    __DestructExceptionObject(pExcept, TRUE);   // destroy the original object

                    ThrowBadException();
                }
                else
                {
                    EmergencyShutdown(SourcePosition, L"BadException is not allowed on this stack frame");
                }
            }
        }
    }

    auto tryBlockMap = TryBlockMap4(pFuncInfo, pDC->ImageBase);

    if (
        pExcept->ExceptionCode == EH_EXCEPTION_NUMBER &&
        pExcept->NumberParameters == EH_EXCEPTION_PARAMETERS &&
        (
            pExcept->params.magicNumber == EH_MAGIC_NUMBER1 ||
            pExcept->params.magicNumber == EH_MAGIC_NUMBER2 ||
            pExcept->params.magicNumber == EH_MAGIC_NUMBER3
            )
        )
    {
        // Looks like it's ours.  Let's see if we have a match:
        //
        // First, determine range of try blocks to consider:
        // Only try blocks which are at the current catch depth are of interest.

        if (tryBlockMap.getNumTryBlocks() > 0)
        {
            auto startStop = GetRangeOfTrysToCheck(tryBlockMap, curState, pDC, pFuncInfo, CatchDepth);

            // Scan the try blocks in the function:
            for (auto iter = startStop.first; iter < startStop.last; ++iter)
            {
                auto tryBlock = *iter;

                __int32 const *ppCatchable = nullptr;

                CatchableType *pCatchable = nullptr;

                if (tryBlock.tryLow > curState || curState > tryBlock.tryHigh) {
                    continue;
                }
                // Try block was in scope for current state.  Scan catches for this
                // try:
                auto handlerMap = HandlerMap4(&tryBlock, pDC->ImageBase, pDC->FunctionEntry->BeginAddress);

                for (auto handler : handlerMap)
                {
                    ppCatchable =
                        reinterpret_cast<CatchableTypeArray*>(
                                GetThreadExceptionData()->_ThrowImageBase + 
                                pExcept->params.pThrowInfo->pCatchableTypeArray)->arrayOfCatchableTypes;

                    auto catchableCount = reinterpret_cast<CatchableTypeArray*>(
                            GetThreadExceptionData()->_ThrowImageBase +
                        pExcept->params.pThrowInfo->pCatchableTypeArray)->nCatchableTypes;
                    // Scan all types that thrown object can be converted to:
                    for (int catchables = catchableCount; catchables > 0; catchables--, ppCatchable++) //-V2528
                    {
                        pCatchable = (CatchableType *)(GetThreadExceptionData()->_ThrowImageBase + *ppCatchable);

                        if (!TypeMatch(&handler, pCatchable, pExcept->params.pThrowInfo)) //-V2570
                        {
                            continue;   
                        }

                        // OK.  We finally found a match.  Activate the catch.  If
                        // control gets back here, the catch did a re-throw, so
                        // keep searching.

                        gotMatch = TRUE;

                        CatchIt(pExcept,
                                pRN,
                                (_CONTEXT*) pContext,
                                pDC,
                                pFuncInfo,
                                &handler,
                                pCatchable,
                                &tryBlock,
                                CatchDepth,
                                pMarkerRN,
                                IsRethrow,
                                recursive
                                );

                        goto NextTryBlock;
                    } // Scan possible conversions
                } // Scan catch clauses
                NextTryBlock: ;
            } // Scan try blocks
        } // if FUNC_NTRYBLOCKS( pFuncInfo ) > 0

        if (!gotMatch && pFuncInfo->header.NoExcept &&
            !ExecutionInCatch(pDC, pFuncInfo)
            )
        {
            GetThreadExceptionData()->_curexception = pExcept;
            GetThreadExceptionData()->_curcontext = pContext;

            auto pEx = TryGetAsRcppException(pExcept);
            if (pEx != nullptr)
            {
                Cardinal::Logger::StaticManagedLogger<Cardinal::Logger::EStaticSubsystems::ExceptionABI>::toLog(
                    Cardinal::Logger::ELoggerLevel::Critical, 
                    L"Critical error. Exception not processed during unwinding noexcept method. Exception details: "S, 
                    *pEx);
            }

            EmergencyShutdown(SourcePosition, L"Processing exception strictly forbiden in methods/functions with noexcept modificator");
        }
    } // It was a C++ EH exception
    else
    {
        // Not ours.  But maybe someone told us how to make it ours.
        if (tryBlockMap.getNumTryBlocks() > 0)
        {
            if (recursive) {
                // We're recursive, and the exception wasn't a C++ EH!
                // Translator threw something unintelligible.

                // Two choices here: we could let the new exception take over, or we could abort. We abort.
                EmergencyShutdown(SourcePosition, L"Unknown exception object");
            }

            FindHandlerForForeignException(pExcept, pRN, pContext, pDC, pFuncInfo, curState, CatchDepth, pMarkerRN);
        }
    } // It wasn't our exception
}

////////////////////////////////////////////////////////////////////////////////
//
// __AdjustPointer - Adjust the pointer to the exception object to a pointer to a
//   base instance.
//
// Output:
//     The address point of the base.
//
// Side-effects:
//     NONE.
void *__AdjustPointer(
    void *pThis,                        // Address point of exception object
    const PMD& pmd                      // Generalized pointer-to-member
                                        //   descriptor
) {
    char *pRet = (char *)pThis + pmd.mdisp;

    if (pmd.pdisp >= 0) {
        pRet += *(__int32 *)((char *)*(ptrdiff_t *)((char *)pThis + pmd.pdisp)
           + (unsigned __int64)pmd.vdisp
           );
        pRet += pmd.pdisp;
    }

    return pRet;
}

////////////////////////////////////////////////////////////////////////////////
//
// BuildCatchObjectHelper - Copy or construct the catch object from the object thrown.
//
// Returns:
//     0 if nothing to be done for constructing object from caller side
//     1 if single parameter constructor is to be called.
//     2 if two parameter constructor is to be called.
//
// Side-effects:
//     A buffer in the subject function's frame is initialized.
//
// Open issues:
//     What happens if the constructor throws?  (or faults?)
int BuildCatchObjectHelperInternal(
    EHExceptionRecord *pExcept,      // Original exception thrown
    void *pRN,                       // This is a pointer to the object
                                     // that we want to build while doing
                                     // COM+ eh. If we are in our own eh,
                                     // then this is a Registration node of
                                     // catching function
    HandlerType4 *pCatch,            // The catch clause that got it //-V2558
    CatchableType *pConv             // The conversion to use
    )
{
    int retval = 0;

    TypeDescriptor* typeDescriptor = pCatch->dispType != 0 
        ? reinterpret_cast<TypeDescriptor*>(GetThreadExceptionData()->_ImageBase + pCatch->dispType)
        : nullptr;

    // If the catch is by ellipsis, then there is no object to construct.
    // If the catch is by type(No Catch Object), then leave too!
    if ((typeDescriptor == nullptr || typeDescriptor->name[0] == '\0')||
        (!pCatch->dispCatchObj && !(pCatch->adjectives & HT_IsComplusEh))) 
    {
        return 0;
    }

    void **pCatchBuffer = nullptr;
    if (pCatch->adjectives & HT_IsComplusEh)
    {
        pCatchBuffer = (void **)pRN;
    }
    else
    {
        pCatchBuffer = (void **)OffsetToAddress(
                                pCatch->dispCatchObj,
                                *((EHRegistrationNode *)pRN)
                                );
    }
    __try {
        if (pCatch->adjectives & HT_IsReference) {

            // The catch is of form 'reference to T'.  At the throw point we
            // treat both 'T' and 'reference to T' the same, i.e.
            // pExceptionObject is a (machine) pointer to T.  Adjust as
            // required.

            //TODO: _VCRT_VERIFY(PER_PEXCEPTOBJ(pExcept) && pCatchBuffer);
            *pCatchBuffer = pExcept->params.pExceptionObject;
            *pCatchBuffer = __AdjustPointer(*pCatchBuffer, pConv->thisDisplacement);
        }
        else if (pConv->properties & CT_IsSimpleType) {

            // Object thrown is of simple type (this including pointers) copy
            // specified number of bytes.  Adjust the pointer as required.  If
            // the thing is not a pointer, then this should be safe since all
            // the entries in the THISDISP are 0.

            //TODO: _VCRT_VERIFY(PER_PEXCEPTOBJ(pExcept) && pCatchBuffer);
            memmove(pCatchBuffer, pExcept->params.pExceptionObject, pConv->sizeOrOffset);

            if (pConv->sizeOrOffset == sizeof(void*) && *pCatchBuffer) {
                *pCatchBuffer = __AdjustPointer(*pCatchBuffer, pConv->thisDisplacement);
            }
        }
        else {
            // Object thrown is UDT.
            if (pConv->copyFunction == 0) {
                // The UDT had a simple ctor.  Adjust in the thrown object,
                // then copy n bytes.

                //TODO: _VCRT_VERIFY(PER_PEXCEPTOBJ(pExcept) && pCatchBuffer);
                memmove(pCatchBuffer, __AdjustPointer(pExcept->params.pExceptionObject, pConv->thisDisplacement), pConv->sizeOrOffset);
            }
            else {
                // It's a UDT: make a copy using copy ctor
                //TODO: _VCRT_VERIFY(PER_PEXCEPTOBJ(pExcept) && pCatchBuffer && CT_COPYFUNC(*pConv));
                if (pConv->properties & CT_HasVirtualBase) {
                    retval = 2;
                } else {
                    retval = 1;
                }
            }
        }
    } __except(EXCEPTION_EXECUTE_HANDLER) {
        // Something went wrong when building the catch object.
        EmergencyShutdown(SourcePosition, L"The exception object is corrupted");
    }

    return retval;
}

int BuildCatchObjectHelper(
    EHExceptionRecord *pExcept,
    void              *pRN,
    HandlerType4      *pCatch,
    CatchableType     *pConv
    )
{
    return BuildCatchObjectHelperInternal(pExcept, pRN, pCatch, pConv);
}

////////////////////////////////////////////////////////////////////////////////
//
// BuildCatchObjectInternal - Copy or construct the catch object from the object thrown.
//
// Returns:
//     nothing.
//
// Side-effects:
//     A buffer in the subject function's frame is initialized.
//
// Open issues:
//     What happens if the constructor throws?  (or faults?)
void BuildCatchObjectInternal(
    EHExceptionRecord *pExcept,         // Original exception thrown
    void *pRN,                          // This is a pointer to the object
                                        // that we want to build while doing
                                        // COM+ eh. If we are in our own eh,
                                        // then this is a Registration node of
                                        // catching function
    HandlerType4 *pCatch,               // The catch clause that got it //-V2558
    CatchableType *pConv                // The conversion to use
    )
{
    void **pCatchBuffer = nullptr;
    if (pCatch->adjectives & HT_IsComplusEh)
    {
        pCatchBuffer = (void **)pRN;
    }
    else
    {
        pCatchBuffer = (void **)OffsetToAddress(
                                pCatch->dispCatchObj,
                                *((EHRegistrationNode *)pRN)
                                );
    }

    __try {
        switch(BuildCatchObjectHelperInternal(pExcept, pRN, pCatch, pConv))
        {
            case 1:
                _CallMemberFunction1(
                    (char*) pCatchBuffer,
                    pConv->copyFunction != 0
                        ? reinterpret_cast<void*>(GetThreadExceptionData()->_ThrowImageBase + pConv->copyFunction)
                        : nullptr,
                    __AdjustPointer(pExcept->params.pExceptionObject, pConv->thisDisplacement)
                );
                break;
            case 2:
                _CallMemberFunction2(
                    (char*) pCatchBuffer,
                    pConv->copyFunction != 0
                        ? reinterpret_cast<void*>(GetThreadExceptionData()->_ThrowImageBase + pConv->copyFunction)
                        : nullptr,
                    __AdjustPointer(pExcept->params.pExceptionObject, pConv->thisDisplacement),
                    1
                );
                break;
            case 0:
                break;
            default:
                break;
        }
    } __except(EXCEPTION_EXECUTE_HANDLER) {
        // Something went wrong when building the catch object.
        EmergencyShutdown(SourcePosition, L"Copying constructor caused an exception");
    }
}

void BuildCatchObject(
    EHExceptionRecord *pExcept,
    void *pRN,
    HandlerType4 *pCatch,
    CatchableType *pConv
    )
{
    BuildCatchObjectInternal(pExcept, pRN, pCatch, pConv);
}

/////////////////////////////////////////////////////////////////////////////
//
// _CreateFrameInfo - Save the frame information for this scope just before
//  calling the catch block.  Put it at the head of the linked list.  For
//  x86, all we need to save is the pointer to the exception object, so we
//  can determine when that object is no longer used by any nested catch
//  handler and can thus be destroyed on exiting from a catch.
//
// Returns:
//      Pointer to the frame info (the first input argument).
//
FRAMEINFO * __cdecl _CreateFrameInfo(
    FRAMEINFO * pFrameInfo,
    PVOID       pExceptionObject
) {
    pFrameInfo->pExceptionObject               = pExceptionObject;
    pFrameInfo->pNext                          = reinterpret_cast<FRAMEINFO*>(GetThreadExceptionData()->_pFrameInfoChain);
    GetThreadExceptionData()->_pFrameInfoChain = pFrameInfo;
    return pFrameInfo;
}

////////////////////////////////////////////////////////////////////////////////
//
// ExFilterRethrow - Exception filter for re-throw exceptions.
//
// Returns:
//     EXCEPTION_EXECUTE_HANDLER - exception was a re-throw
//     EXCEPTION_CONTINUE_SEARCH - anything else
//
// Side-effects: sets rethrow = TRUE if exception objects of the two Exception matches

//__declspec(guard(ignore)) 
int ExFilterRethrow(
    EXCEPTION_POINTERS *pExPtrs,
    EHExceptionRecord *pOldExcept, //-V2558
    int *rethrow
) {
    // Get the exception record thrown (don't care about other info)
    EHExceptionRecord *pExcept = (EHExceptionRecord *)pExPtrs->ExceptionRecord;
    *rethrow = 0;
    if ((
        pExcept->ExceptionCode == EH_EXCEPTION_NUMBER &&
        pExcept->NumberParameters == EH_EXCEPTION_PARAMETERS &&
        (
            pExcept->params.magicNumber == EH_MAGIC_NUMBER1 ||
            pExcept->params.magicNumber == EH_MAGIC_NUMBER2 ||
            pExcept->params.magicNumber == EH_MAGIC_NUMBER3
            )
        ) && pExcept->params.pExceptionObject == pOldExcept->params.pExceptionObject)
        *rethrow = 1;
    // Check if it's ours and it's has no exception information.
    if ((
        pExcept->ExceptionCode == EH_EXCEPTION_NUMBER &&
        pExcept->NumberParameters == EH_EXCEPTION_PARAMETERS &&
        (
            pExcept->params.magicNumber == EH_MAGIC_NUMBER1 ||
            pExcept->params.magicNumber == EH_MAGIC_NUMBER2 ||
            pExcept->params.magicNumber == EH_MAGIC_NUMBER3
            )
        ) && pExcept->params.pExceptionObject == nullptr) {
    // In a rethrow, ExceptionCode isn't set to EH_EXCEPTION_NUMBER even for a
    // C++ Exception. Hence we use cxxReThrow in __InternalCxxFrameHandler
    // to check for a C++ Exception
        GetThreadExceptionData()->_cxxReThrow = true; //-V601
        *rethrow = 1;
        return EXCEPTION_EXECUTE_HANDLER;
    }
    return EXCEPTION_CONTINUE_SEARCH;
}

// Wraps around ExFilterRethrow to set _CatchStateInParent if this isn't a rethrow.
//__declspec(guard(ignore)) 
int ExFilterRethrowFH4(
    EXCEPTION_POINTERS *pExPtrs,
    EHExceptionRecord *pOldExcept,
    __ehstate_t HandlerSearchState,
    int *rethrow
)
{
    int exceptionCode =  ExFilterRethrow(pExPtrs, pOldExcept, rethrow);
    if (exceptionCode == EXCEPTION_CONTINUE_SEARCH)
    {
        // Set correct parent state to be used
        GetThreadExceptionData()->_CatchStateInParent = HandlerSearchState;
    }

    return exceptionCode;
}

union RawExceptionInfo {
    EXCEPTION_RECORD NtExceptionInfo;
    EHExceptionRecord CxxExceptionInfo;
};

ForceInline __declspec(noreturn)
void __RethrowException(EHExceptionRecord* pThisException)
{
    RawExceptionInfo* orig = (RawExceptionInfo*) pThisException; //-V1027

    EXCEPTION_RECORD record = orig->NtExceptionInfo;

    RtlRaiseException(&record);
}

/////////////////////////////////////////////////////////////////////////////
//
// _FindAndUnlinkFrame - Pop the frame information for this scope that was
//  pushed by _CreateFrameInfo.  This should be the first frame in the list,
//  but the code will look for a nested frame and pop all frames, just in
//  case.
//
extern "C" void __cdecl _FindAndUnlinkFrame(
    FRAMEINFO * pFrameInfo //-V2558
) {
    //TODO: _VCRT_VERIFY(pFrameInfo == pFrameInfoChain);
    
    for (FRAMEINFO *pCurFrameInfo = reinterpret_cast<FRAMEINFO*>(GetThreadExceptionData()->_pFrameInfoChain);
         pCurFrameInfo;
         pCurFrameInfo = pCurFrameInfo->pNext)
    {
        if (pFrameInfo == pCurFrameInfo) {
            GetThreadExceptionData()->_pFrameInfoChain = pCurFrameInfo->pNext;
            return;
        }
    }

    // Should never be reached.
    EmergencyShutdown(SourcePosition, L"The stack object chain is damaged");
}

/////////////////////////////////////////////////////////////////////////////
//
// _IsExceptionObjectToBeDestroyed - Determine if an exception object is still
//  in use by a more deeply nested catch frame, or if it unused and should be
//  destroyed on exiting from the current catch block.
//
// Returns:
//      TRUE if exception object not found and should be destroyed.
//
extern "C" BOOL __cdecl _IsExceptionObjectToBeDestroyed(
    PVOID pExceptionObject //-V2558
) {
    FRAMEINFO * pFrameInfo = nullptr;

    for (pFrameInfo = reinterpret_cast<FRAMEINFO*>(GetThreadExceptionData()->_pFrameInfoChain); 
        pFrameInfo; pFrameInfo = pFrameInfo->pNext ) 
    {
        if (pFrameInfo->pExceptionObject == pExceptionObject) {
            return FALSE;
        }
    }
    return TRUE;
}

void * CxxCallCatchBlock(
    EXCEPTION_RECORD *pExcept
    )
{
    int rethrow = 0, TranslatedCatch = 0;
    void *handlerAddress = nullptr;
    void *continuationAddress = nullptr;
    FRAMEINFO FrameInfo{};
    CONTEXT *pContext = nullptr, *pSaveContext = nullptr;
    EHRegistrationNode *pEstablisherFrame = nullptr;
    EHExceptionRecord *pThisException = nullptr, *pSaveException = nullptr, *pForeignException = nullptr;
    void *continuationAddresses[MAX_CONT_ADDRESSES]{};
    memset(continuationAddresses, 0, sizeof(continuationAddresses));
    FuncInfo4 FuncInfo;

    pSaveContext = reinterpret_cast<CONTEXT*>(GetThreadExceptionData()->_curcontext);
    pSaveException = reinterpret_cast<EHExceptionRecord*>(GetThreadExceptionData()->_curexception);

    // Copy Necessary Information which is passed from UnwindNestedFrames.
    pThisException = (EHExceptionRecord *)pExcept->ExceptionInformation[6];
    pContext = (CONTEXT *)pExcept->ExceptionInformation[4];
    handlerAddress = (void *)pExcept->ExceptionInformation[2];
    pEstablisherFrame = (EHRegistrationNode *)pExcept->ExceptionInformation[1];

    continuationAddresses[0] = (PBYTE)pExcept->ExceptionInformation[5];
    continuationAddresses[1] = (PBYTE)pExcept->ExceptionInformation[9];

    // State used if an exception inside the called catch needs to be handled in the parent
    __ehstate_t HandlerSearchState = (__ehstate_t) pExcept->ExceptionInformation[11];
    // State used during unwinding to bypass already unwound states in the parent
    __ehstate_t UnwindTryState = (__ehstate_t)pExcept->ExceptionInformation[3];

    //TODO: __except_validate_context_record(pContext);

    GetThreadExceptionData()->_curexception = pThisException;
    GetThreadExceptionData()->_curcontext   = pContext;

    FRAMEINFO *pFrameInfo = _CreateFrameInfo(&FrameInfo,
        (void *)pThisException->params.pExceptionObject);

    if (pExcept->ExceptionInformation[7]) {
        // Stuff for SehTranslation book keeping
        // This Exception Object is Translation of Foreign Exception And should
        // be destroyed in any case. If there is rethrow, throw ForeignException.
        // IMPORTANT: No one else can rethrow this Exception Object.
        TranslatedCatch = 1;
        pForeignException = reinterpret_cast<EHExceptionRecord*>(GetThreadExceptionData()->_pForeignException);
        // End Translation Stuff
    }
    //Store current state of isInCath flag(we can procced catch in another catch)
    bool isAlredyInCatch = GetThreadExceptionData()->_isInCatch;

    __try {
        __try {
            //Signale that we proceed catch block
            GetThreadExceptionData()->_isInCatch = true;
            continuationAddress = _CallSettingFrame_LookupContinuationIndex(
                handlerAddress,
                pEstablisherFrame,
                0x100);

            Cardinal::UInt64 continuationIndex = reinterpret_cast<Cardinal::UInt64>(continuationAddress);
            if (continuationIndex < MAX_CONT_ADDRESSES)
            {
                //TODO: _VCRT_VERIFY(continuationAddresses[continuationIndex] != 0);
                continuationAddress = continuationAddresses[continuationIndex];
            }
            _CallSettingFrame_NotifyContinuationAddr(continuationAddress, pEstablisherFrame);
            //Restore state
            GetThreadExceptionData()->_isInCatch = isAlredyInCatch;
        }
        // Filter will be invoked in pass 1 looking for an exception, this lets us set up the correct _CatchStateInParent that should
        // be used when looking for a handler in the parent of a catch funclet inside ExFilterRethrowFH4.
        // Note: in case of rethrow we unwind through the catch handler back to this filter in order to rethrow.
        // In that case, the _CatchStateInParent should not be set until we get into the except.
        __except (ExFilterRethrowFH4(exception_info(), pThisException, HandlerSearchState, &rethrow))
        {
            if (Cardinal::Threading::GetTLS()->State != nullptr || Cardinal::Threading::GetTLS()->SecondState != nullptr) {
                Cardinal::Swap(*Cardinal::Threading::GetTLS()->SecondState, *Cardinal::Threading::GetTLS()->State);
            }

            //Restore state
            GetThreadExceptionData()->_isInCatch = isAlredyInCatch;
            GetThreadExceptionData()->_cxxReThrow = false; //-V601
            GetThreadExceptionData()->_CatchStateInParent = HandlerSearchState;

            if (TranslatedCatch) {
                // Note in case of rethrow, no one else can convert rethrow to
                // pThisException. This means only this except will deal with conversion
                // of rethrow to pThisException. Instead of pThisException, we
                // will throw original Foreign Exception. Also we will need to
                // destroy Exception Object before Raising Foreign Exception.
                __DestructExceptionObject(pThisException, TRUE);
                __RethrowException(pForeignException);
            }
            else
            {
                __RethrowException(pThisException);
            }
        }
    }
    __finally { 
        _FindAndUnlinkFrame(pFrameInfo);
        if (!rethrow &&
            (
        pThisException->ExceptionCode == EH_EXCEPTION_NUMBER &&
        pThisException->NumberParameters == EH_EXCEPTION_PARAMETERS &&
        (
            pThisException->params.magicNumber == EH_MAGIC_NUMBER1 ||
            pThisException->params.magicNumber == EH_MAGIC_NUMBER2 ||
            pThisException->params.magicNumber == EH_MAGIC_NUMBER3
            )
        ) &&
            _IsExceptionObjectToBeDestroyed(pThisException->params.pExceptionObject))
        {
            __DestructExceptionObject(pThisException, TRUE);
        }

        GetThreadExceptionData()->_curexception = pSaveException;
        GetThreadExceptionData()->_curcontext = pSaveContext;

        // Finally is invoked in pass 2, letting us set the correct state to unwind from when back in the parent frame
        GetThreadExceptionData()->_CatchStateInParent = UnwindTryState;
    }

    // exception caught successfully, reset this value
    GetThreadExceptionData()->_CatchStateInParent = INVALID_CATCH_SPECIFIC_STATE;
    GetThreadExceptionData()->_curexception = nullptr;

    if (Cardinal::Threading::GetTLS()->State != nullptr || Cardinal::Threading::GetTLS()->SecondState != nullptr) {
        Cardinal::Swap(*Cardinal::Threading::GetTLS()->SecondState, *Cardinal::Threading::GetTLS()->State);
    }

    return continuationAddress;
}

void UnwindNestedFrames(
    EHRegistrationNode  *pFrame,            // Unwind up to (but not including) this frame
    EHExceptionRecord   *pExcept,           // The exception that initiated this unwind
    CONTEXT             *pContext,          // Context info for current exception
    EHRegistrationNode  *pEstablisher,
    void                *Handler,
    FuncInfo4*          /*pFuncInfo*/,
    __ehstate_t         TargetUnwindState,
    __ehstate_t         CatchState,        // State outside of current try but inside of any enclosing trys
    HandlerType4        *pCatch, //-V2558
    DispatcherContext   *pDC,
    BOOLEAN             recursive
    )
{
    const EXCEPTION_RECORD ExceptionTemplate = // A generic exception record
    {
        (DWORD)STATUS_UNWIND_CONSOLIDATE,  // STATUS_UNWIND_CONSOLIDATE
        EXCEPTION_NONCONTINUABLE,          // Exception flags (we don't do resume)
        nullptr,                           // Additional record (none)
        nullptr,                           // Address of exception (OS fills in)
        15,                                // Number of parameters
        { EH_MAGIC_NUMBER1,                // Our version control magic number
        0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0
        }                                  // pThrowInfo
    };

    CONTEXT Context = { 0 };

    PCONTEXT UnwindContext = &Context;

    EXCEPTION_RECORD ExceptionRecord = ExceptionTemplate;
    ExceptionRecord.ExceptionInformation[0] = (ULONG_PTR)CxxCallCatchBlock;
    // Address of call back function
    ExceptionRecord.ExceptionInformation[1] = (ULONG_PTR)pEstablisher;
    // Used by callback function
    ExceptionRecord.ExceptionInformation[2] = (ULONG_PTR)Handler;
    // Used by callback function to call catch block
    ExceptionRecord.ExceptionInformation[3] = (ULONG_PTR)TargetUnwindState;
    // Used by CxxFrameHandler to unwind to target_state
    ExceptionRecord.ExceptionInformation[4] = (ULONG_PTR)pContext;
    // used to set pCurrentExContext in callback function
    ExceptionRecord.ExceptionInformation[5] = pCatch->continuationAddress[0] + pDC->ImageBase;
    // Used in callback function for continuation address lookup
    ExceptionRecord.ExceptionInformation[6] = (ULONG_PTR)pExcept;
    // Used for passing current Exception
    ExceptionRecord.ExceptionInformation[7] = (ULONG_PTR)recursive;
    // Used for translated Exceptions
    ExceptionRecord.ExceptionInformation[8] = EH_MAGIC_NUMBER1;
    // Used in __InternalCxxFrameHandler to detected if it's being
    // called from _UnwindNestedFrames.

    // TODO: make these contiguous
    ExceptionRecord.ExceptionInformation[9] = pCatch->continuationAddress[1] + pDC->ImageBase;
    // Used in callback function for continuation address lookup

    // Used to associate Catch Handler state to parent state
    ExceptionRecord.ExceptionInformation[11] = CatchState;

    RtlUnwindEx((void *)*pFrame,
        (void *)pDC->ControlPc,    // Address where control left function
        &ExceptionRecord,
        nullptr,
        UnwindContext,
        (PUNWIND_HISTORY_TABLE)pDC->HistoryTable);
}

////////////////////////////////////////////////////////////////////////////////
//
// CatchIt - A handler has been found for the thrown type.  Do the work to
//   transfer control.
//
// Description:
//     Builds the catch object
//     Unwinds the stack to the point of the try
//     Calls the address of the handler (funclet) with the frame set up for that
//       function but without resetting the stack.
//     Handler funclet returns address to continue execution, or nullptr if the
//       handler re-threw ("throw;" lexically in handler)
//     If the handler throws an EH exception whose exception info is nullptr, then
//       it's a re-throw from a dynamicly enclosed scope.
//
// It is an open question whether the catch object is built before or after the local unwind.
//
// Returns:
//     No return value.  Returns iff handler re-throws.
void CatchIt(
    EHExceptionRecord *pExcept,           // The exception thrown
    EHRegistrationNode *pRN,              // Dynamic info of function with catch
    CONTEXT *pContext,                    // Context info
    DispatcherContext *pDC,               // Context within subject frame
    FuncInfo4 *pFuncInfo,                 // Static info of function with catch
    HandlerType4 *pCatch,                 // The catch clause selected
    CatchableType *pConv,                 // The rules for making the conversion
    TryBlockMapEntry4 *pEntry,            // Description of the try block //-V2558
    int CatchDepth,                       // How many catches are we nested in?
    EHRegistrationNode *pMarkerRN,        // Special node if nested in catch
    BOOLEAN IsRethrow,                    // Is this a rethrow ?
    BOOLEAN recursive                     // Is this from a translated exception ?
) {

    // These parameters are not used in some compilations
    UNREFERENCED_PARAMETER(CatchDepth);
    UNREFERENCED_PARAMETER(pMarkerRN);
    UNREFERENCED_PARAMETER(IsRethrow);
    UNREFERENCED_PARAMETER(recursive);

    //EHTRACE_FMT1("Catching object @ 0x%p", PER_PEXCEPTOBJ(pExcept));

    EHRegistrationNode *pEstablisher = pRN;

    EHRegistrationNode EstablisherFramePointers = 0;
    pEstablisher = GetEstablisherFrame(pRN, pDC, pFuncInfo, &EstablisherFramePointers); //-V519

    // Copy the thrown object into a buffer in the handler's stack frame,
    // unless the catch was by elipsis (no conversion) OR the catch was by
    // type without an actual 'catch object'.

    if (pConv) {
        BuildCatchObject(pExcept, pEstablisher, pCatch, pConv);
    }

    // Unwind stack objects to the entry of the try that caught this exception.

    // This call will never return. This call will end up calling CxxCallCatchBlock
    // through RtlUnwind (STATUS_CONSULIDATE_FRAMES) mechanism.
    UnwindNestedFrames(
        pRN,
        pExcept,
        pContext,
        pEstablisher,
        reinterpret_cast<void*>((Cardinal::SizeT)GetThreadExceptionData()->_ImageBase + pCatch->dispOfHandler),
        pFuncInfo,
        pEntry->tryLow,
        pEntry->catchHigh,
        pCatch,
        pDC,
        recursive
        );
}

inline TlsExceptionData * GetThreadExceptionData()
{
    return &Cardinal::Threading::GetTLS()->ExceptionInfo;
}

//Storing exception region

////////////////////////////////////////////////////////////////////////////////
//
// void _CallCopyCtor();
//
// Helper function for calling the copy-ctor on the C++ exception object.  It reads
// the passed info throw info and determine the type of exception.  If it's a simple type
// it calls memcpy.  It's a UDT with copy-ctor, it uses the copy-ctor.
//
void _CallCopyCtor(_Out_writes_bytes_(size) void* dst, _In_reads_bytes_(size) void* src, size_t size, _In_ const CatchableType* const pType, void* ThrowImageBase)
{
    if (pType->properties & CT_IsSimpleType && // this is a simple type
        pType->copyFunction == NULL    // this is a user defined type without copy ctor
        )
    {
        // just copy with memcpy
        Cardinal::Memory::Copy(src, dst, size);
    }
    else
    {
        try
        {
            // it's a user defined type with a copy ctor
            if (pType->properties & CT_HasVirtualBase)
            {
                // this exception got a virtual base
                _CallMemberFunction2((char*)dst,
                    reinterpret_cast<void*>(reinterpret_cast<Cardinal::SizeT>(ThrowImageBase) + pType->copyFunction),
                    __AdjustPointer(src,
                        pType->thisDisplacement), 1);
            }
            else
            { //-V565
                _CallMemberFunction1((char*)dst,
                    reinterpret_cast<void*>(reinterpret_cast<Cardinal::SizeT>(ThrowImageBase) + pType->copyFunction),
                    __AdjustPointer(src,
                        pType->thisDisplacement));
            }
        }
        catch (...)
        {
            // the copy-Ctor() threw.  We need to terminate().
            
            EmergencyShutdown(SourcePosition, L"copy-Ctor throw exception");
        }
    }
}

////////////////////////////////////////////////////////////////////////////////
//
// void _CopyException();
//
// Builds an __ExceptionPtr with the given c++ exception object and its corresponding
// throwinfo.
//
// This function returns the static copy of exception_ptr which points to a
// std::bad_alloc exception in out-of-memory situation.
//
void _CopyException(RawExceptionInfo* orig, RawExceptionInfo* copy, void*(*allocator)(Cardinal::SizeT))
{
    //copying the _EXCEPTION_RECORD (for SEH exceptions)
    copy->NtExceptionInfo.ExceptionCode = orig->NtExceptionInfo.ExceptionCode;
    copy->NtExceptionInfo.ExceptionFlags = orig->NtExceptionInfo.ExceptionFlags;
    copy->NtExceptionInfo.ExceptionRecord = NULL; // We don't chain SEH exceptions
    copy->NtExceptionInfo.ExceptionAddress = NULL; // Useless field to copy->  It will be overwritten by RaiseException()
    copy->NtExceptionInfo.NumberParameters = orig->NtExceptionInfo.NumberParameters;

    // copying any addition parameters
    for (ULONG i = 0; i < copy->NtExceptionInfo.NumberParameters && i < EXCEPTION_MAXIMUM_PARAMETERS; ++i)
    {
        copy->NtExceptionInfo.ExceptionInformation[i] = orig->NtExceptionInfo.ExceptionInformation[i];
    }
    // NULLing out any un-used parameters
    for (ULONG i = copy->NtExceptionInfo.NumberParameters; i < EXCEPTION_MAXIMUM_PARAMETERS; ++i)
    {
        copy->NtExceptionInfo.ExceptionInformation[i] = 0;
    }

    if (orig->CxxExceptionInfo.ExceptionCode == EH_EXCEPTION_NUMBER &&
        orig->CxxExceptionInfo.NumberParameters == EH_EXCEPTION_PARAMETERS && (
            orig->CxxExceptionInfo.params.magicNumber == EH_MAGIC_NUMBER1 ||
            orig->CxxExceptionInfo.params.magicNumber == EH_MAGIC_NUMBER2 ||
            orig->CxxExceptionInfo.params.magicNumber == EH_MAGIC_NUMBER3))
    {
        // this is a C++ exception
        // we need to copy the C++ exception object

        // nulling out the Exception object pointer, because we haven't copied it yet
        copy->CxxExceptionInfo.params.pExceptionObject = NULL;

        ThrowInfo* pThrow = orig->CxxExceptionInfo.params.pThrowInfo;
        if (orig->CxxExceptionInfo.params.pExceptionObject == NULL ||
            pThrow == NULL ||
            pThrow->pCatchableTypeArray == NULL
            )
        {
            // No ThrowInfo exists.  If this were a C++ exception, something must have corrupted it.
            //terminate();
            EmergencyShutdown(SourcePosition, L"Throw info empty");
        }

        // we finally got the type info we want
        auto catchableTypeArray = reinterpret_cast<CatchableTypeArray*>(
            reinterpret_cast<Cardinal::SizeT>(orig->CxxExceptionInfo.params.pThrowImageBase) +
            orig->CxxExceptionInfo.params.pThrowInfo->pCatchableTypeArray
            );

        auto ppCatchable =
            reinterpret_cast<int*>(
                reinterpret_cast<Cardinal::SizeT>(orig->CxxExceptionInfo.params.pThrowImageBase) +
                catchableTypeArray->arrayOfCatchableTypes[0]
                );

        void* pExceptionBuffer = NULL;
        auto typeInfo = (CatchableType *)(ppCatchable);
        
        //HACK: hardcoded max buffer size for exception
        pExceptionBuffer = allocator((typeInfo->sizeOrOffset == 0) ? (4Kb).Bytes - 1 : typeInfo->sizeOrOffset);

        _CallCopyCtor(pExceptionBuffer, orig->CxxExceptionInfo.params.pExceptionObject, typeInfo->sizeOrOffset, typeInfo, orig->CxxExceptionInfo.params.pThrowImageBase);

        copy->CxxExceptionInfo.params.pExceptionObject = pExceptionBuffer;
    }
}

void StoreException(void *& ptr, void *(* alocator)(Cardinal::SizeT))
{
    if (GetThreadExceptionData()->_isInCatch) {
        RawExceptionInfo* current = static_cast<RawExceptionInfo*>(GetThreadExceptionData()->_curexception);
        RawExceptionInfo* record = static_cast<RawExceptionInfo*>(alocator(sizeof(RawExceptionInfo)));
        RtlSecureZeroMemory(record, sizeof(RawExceptionInfo));
        _CopyException(current, record, alocator);
        ptr = record;
        return;
    }

    Cardinal::Internals::Exceptions::ThrowNullPointException(SourcePosition);
}

void ThrowExceptionFromExceptionStorage(EXCEPTION_RECORD record) {
    RtlRaiseException(&record);
}

_Ret_notnull_ _Post_writable_byte_size_(_Size)
extern "C" void* __cdecl _alloca(_In_ size_t _Size);

__declspec(noreturn) void ThrowStoredException(void* exception)
{
    if (exception == nullptr)
        Cardinal::Internals::Exceptions::ThrowNullPointException(SourcePosition);

    EXCEPTION_RECORD record = { 0 };
    auto copy = (RawExceptionInfo*)&record;
    auto orig = (RawExceptionInfo*)exception;

    //copying the _EXCEPTION_RECORD (for SEH exceptions)
    copy->NtExceptionInfo.ExceptionCode = orig->NtExceptionInfo.ExceptionCode;
    copy->NtExceptionInfo.ExceptionFlags = orig->NtExceptionInfo.ExceptionFlags;
    copy->NtExceptionInfo.ExceptionRecord = NULL; // We don't chain SEH exceptions
    copy->NtExceptionInfo.ExceptionAddress = NULL; // Useless field to copy->  It will be overwritten by RaiseException()
    copy->NtExceptionInfo.NumberParameters = orig->NtExceptionInfo.NumberParameters;

    // copying any addition parameters
    for (ULONG i = 0; i < copy->NtExceptionInfo.NumberParameters && i < EXCEPTION_MAXIMUM_PARAMETERS; ++i)
    {
        copy->NtExceptionInfo.ExceptionInformation[i] = orig->NtExceptionInfo.ExceptionInformation[i];
    }
    // NULLing out any un-used parameters
    for (ULONG i = copy->NtExceptionInfo.NumberParameters; i < EXCEPTION_MAXIMUM_PARAMETERS; ++i)
    {
        copy->NtExceptionInfo.ExceptionInformation[i] = 0;
    }

    if (orig->CxxExceptionInfo.ExceptionCode == EH_EXCEPTION_NUMBER &&
        orig->CxxExceptionInfo.NumberParameters == EH_EXCEPTION_PARAMETERS && (
            orig->CxxExceptionInfo.params.magicNumber == EH_MAGIC_NUMBER1 ||
            orig->CxxExceptionInfo.params.magicNumber == EH_MAGIC_NUMBER2 ||
            orig->CxxExceptionInfo.params.magicNumber == EH_MAGIC_NUMBER3))
    {
        // this is a C++ exception
        // we need to copy the C++ exception object

        // nulling out the Exception object pointer, because we haven't copied it yet
        copy->CxxExceptionInfo.params.pExceptionObject = NULL;

        ThrowInfo* pThrow = orig->CxxExceptionInfo.params.pThrowInfo;
        if (orig->CxxExceptionInfo.params.pExceptionObject == NULL ||
            pThrow == NULL ||
            pThrow->pCatchableTypeArray == NULL
            )
        {
            // No ThrowInfo exists.  If this were a C++ exception, something must have corrupted it.
            //terminate();
            EmergencyShutdown(SourcePosition, L"The exception object is corrupted or it is not a C++ exception");
        }

        // we finally got the type info we want
        //auto typeInfo = GetByOffset<CatchableType>((SizeT)orig->CxxExceptionInfo.params.pThrowImageBase, pThrow->pCatchableTypeArray);
        // we finally got the type info we want
        // we finally got the type info we want
        auto catchableTypeArray = reinterpret_cast<CatchableTypeArray*>(
            reinterpret_cast<Cardinal::SizeT>(orig->CxxExceptionInfo.params.pThrowImageBase) +
            orig->CxxExceptionInfo.params.pThrowInfo->pCatchableTypeArray
            );

        auto ppCatchable =
            reinterpret_cast<int*>(
                reinterpret_cast<Cardinal::SizeT>(orig->CxxExceptionInfo.params.pThrowImageBase) +
                catchableTypeArray->arrayOfCatchableTypes[0]
                );

        void* pExceptionBuffer = NULL;
        auto typeInfo = (CatchableType *)(ppCatchable);

        __assume(typeInfo->sizeOrOffset < (4Kb).Bytes);

        GetThreadExceptionData()->_throwObjStorageForStoredException = _alloca((typeInfo->sizeOrOffset == 0) ? (4Kb).Bytes - 1 : typeInfo->sizeOrOffset);

        pExceptionBuffer = GetThreadExceptionData()->_throwObjStorageForStoredException;

        _CallCopyCtor(
            pExceptionBuffer, 
            orig->CxxExceptionInfo.params.pExceptionObject, 
            typeInfo->sizeOrOffset, 
            typeInfo, 
            orig->CxxExceptionInfo.params.pThrowImageBase);

        copy->CxxExceptionInfo.params.pExceptionObject = pExceptionBuffer;

        ThrowExceptionFromExceptionStorage(record);
    }
    else
    {
        ThrowExceptionFromExceptionStorage(record);
    }
}

void DestroyStoredException(void*& ptr, void(*dealocator)(void*))
{
    if (ptr != nullptr) {
        RawExceptionInfo* data = (RawExceptionInfo*)ptr;
        __DestructExceptionObject(&data->CxxExceptionInfo, TRUE);
        dealocator(data->CxxExceptionInfo.params.pExceptionObject);
        data->CxxExceptionInfo.params.pExceptionObject = nullptr;
        dealocator(ptr);
        ptr = nullptr;
    }
}

#pragma runtime_checks("", off)
#pragma check_stack(off)
__declspec(noreturn, noinline, safebuffers) void InteruptThread() {
    if (GetThreadExceptionData()->_curexception != nullptr)
    {
         __DestructExceptionObject(reinterpret_cast<EHExceptionRecord*>(GetThreadExceptionData()->_curexception), TRUE);   // destroy the original object
         GetThreadExceptionData()->_curexception = nullptr;
    }
	Throw(Cardinal::InteruptException(SourcePosition));
}
#pragma check_stack(on)
#pragma runtime_checks("", restore)

int ArrayUnwindFilter(EXCEPTION_POINTERS* pExPtrs) {
    EHExceptionRecord* pExcept = reinterpret_cast<EHExceptionRecord*>(pExPtrs->ExceptionRecord);

    switch (pExcept->ExceptionCode) //-V2520
    {
        case EH_EXCEPTION_NUMBER:
        EmergencyShutdown(SourcePosition, L"Dtors can't throw exceptions");
#ifdef ALLOW_UNWIND_ABORT
        case EH_ABORT_FRAME_UNWIND_PART:
        return EXCEPTION_EXECUTE_HANDLER;
#endif
        default:
        return EXCEPTION_CONTINUE_SEARCH;
    }
}

void __stdcall __ArrayUnwind(
    void* ptr,                // Pointer to array to destruct
    size_t      size,               // Size of each element (including padding)
    int         count,              // Number of elements in the array
    void(__stdcall* pDtor)(void*)    // The destructor to call
) {
    // 'unwind' rest of array

    __try
    {
        while (--count >= 0)
        {
            ptr = static_cast<char*>(ptr) - size;
            (*pDtor)(ptr);
        }
    }
    __except (ArrayUnwindFilter(exception_info()))
    {
        ; // Deliberately do nothing
    }
}

void __stdcall __ehvec_ctor(
    void* ptr,                // Pointer to array to destruct
    size_t      size,               // Size of each element (including padding)
    size_t      count,              // Number of elements in the array
    void(__stdcall* pCtor)(void*),   // Constructor to call
    void(__stdcall* pDtor)(void*)    // Destructor to call should exception be thrown
) {
    size_t i = 0;      // Count of elements constructed
    int success = 0;

    __try
    {
        // Construct the elements of the array
        for (; i < count; i++)
        {
            (*pCtor)(ptr);
            ptr = static_cast<char*>(ptr) + size;
        }
        success = 1;
    }
    __finally
    {
        if (!success)
            __ArrayUnwind(ptr, size, static_cast<int>(i), pDtor);
    }
}

void __stdcall __ehvec_dtor(
    void* ptr,                // Pointer to array to destruct
    size_t      size,               // Size of each element (including padding)
    size_t      count,              // Number of elements in the array
    void(__stdcall* pDtor)(void*)    // The destructor to call
) {
    _Analysis_assume_(count > 0);

    int success = 0;

    // Advance pointer past end of array
    ptr = static_cast<char*>(ptr) + size * count;

    __try
    {
        // Destruct elements
        while (count-- > 0)
        {
            ptr = static_cast<char*>(ptr) - size;
            (*pDtor)(ptr);
        }
        success = 1;
    }
    __finally
    {
        if (!success)
            __ArrayUnwind(ptr, size, static_cast<int>(count), pDtor);
    }
}

#pragma warning(pop)