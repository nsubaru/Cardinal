module;
#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

export module Cardinal.Exceptions.ABI:HighLevelStructs;

export import :Base;
export import :Types;
export import :Inlines;


// Copyright (c) Microsoft Corporation. All rights reserved.
//
// Defines the on-disk data format and higher-level logical classes to interact with __CxxFrameHandler4 metadata.
// Valid EH state values are -1, 0 ... MAX_INT. States are always encoded as compressed integers which assumes positive values. Thus, all
// state values are encoded as EHstate + 1 to avoid encoding a negative (a state of -1 is encoded as a compressed 0, 0 as a compressed 1 etc.)
//
// The on-disk format is as follows; fields are listed in order as they would appear on the disk. When reading, fields that are not used
// means the next field that exists will be stored next:
//
// FuncInfo4: Top-level metadata structure for a function
//   header:           1 byte, bitfield fully defined in FuncInfoHeader structure
//   bbtFlags:         compressed int, only exists when header.BBT == 1
//   dispUnwindMap:    4 bytes, RVA to UnwindMap which only exists when header.UnwindMap == 1
//   dispTryBlockMap:  4 bytes, RVA to TryBlockMap which only exists when header.TryBlockMap == 1
//   dispIPtoStateMap: 4 bytes, RVA to IPtoStateMap OR SepIPtoStateMap that is always assumed to exist.
//                     Is RVA to IPtoStateMap if header.isSeparated == 0, otherwise RVA to SepIPtoStateMap.
//   dispFrame:        compressed int, only exists when header.isCatch == 1
//
// UWMap4: Describes what unwind actions to take
//   NumEntries: compressed int, number of entries in the map
//   UnwindMapEntry4[NumEntries]:
//     nextOffset << 2 | UnwindEntryType: compressed int, nextOffset represents the byte offset from the start of this entry to the previous entry.
//                                        Note that the entry for state -1 doesn't explicitly exist in the table as there's never any unwind actions associated with it. Instead
//                                        entries that go to state -1 encode an offset that points to before the start of the UnwindMapEntry4 buffer. See 'enum Type' in UnwindMapEntry4
//                                        for a description of what the different UnwindEntryType represent.
//     action:                            4 bytes, RVA to the destructor action.
//     object:                            compressed int, offset to object directly or to stack location containing its pointer depending on UnwindEntryType.
//
// TryBlockMap4: Describes what Try structures exist in the program
//   NumEntries: compressed int, number of entries in the map
//   TryBlockMapEntry4[numEntries]:
//     tryLow:               compressed int, starting state of the try
//     tryHigh:              compressed int, final state of the try--the range of try states is [tryLow, tryHigh]
//     catchHigh:            compressed int, final state of all catches--the range of catch states is (tryHigh, catchHigh]
//     RVA to Handler Array: 4 bytes, RVA to metadata about the various catch handlers for this try
//
// HandlerMap4: Describes Catch(es) associated with a specific Try
//   NumEntries: compressed int, number of entries in the map
//   HandlerType4[numEntries]:
//     header:              1 byte, bitfield fully defined in HandlerTypeHeader structure
//     adjectives:          compressed int, only present if header.adjectives == 1
//     dispType:            4 bytes, RVA to type descriptor only present if header.dispType == 1
//     dispCatchObj:        compressed int, displacement from establisher from to catch object only present if header.dispCatchObj == 1
//     dispOfHandler:       4 bytes, RVA to 'catch' code
//     continuationAddress: between 0 (no continuation) and up to 2 compressed ints (up to 2 continuation addresses),
//                          used by the runtime to figure out the next normal flow instruction to execute after the catch
//
// IPtoStateMap4: Mapping between IP to EH state
//   NumEntries: compressed int, number of entries in map
//   IPtoStateMapEntry4[NumEntries]:
//     Ip:    compressed int, function-relative AND delta-encoded from the previous entry.
//            For example, entries of 0,5,15 map to 0,5,20 bytes from the start of the function
//     State: compressed int, stored as the EHstate+1 to avoid encoding a negative
//
// SepIPtoStateMap4: Used when code is separated out (e.g. POGO) and contains RVAs to IP2StateMap(s) for each function contribution
//  NumEntries: compressed int, number of entries in map
//  SepIPtoStateMapEntry4[NumEntries]:
//    addrStartRVA: 4 bytes, RVA to start address of function contribution, used to differentiate between segments
//    dispOfIPMap:  4 bytes, RVA to IP2StateMap structure corresponding to the function contribution with this addrStartRVA
//
// Header Usage:
// Reading in data:
//   DecompFuncInfo:                 entry-point that provides a FuncInfo4 used to build other structures.
//   UWMap4/TryBlockMap4:            build up high-level representation of UnwindMap/TryBlockMap from FuncInfo4.
//   IPtoStateMap4/SepIPtoStateMap4: build up high-level representation of IPtoStateMap/SepIPtoStateMap from FuncInfo4.
//                                   Note that SepIPtoStateMap4 has support for non-separated IptoStateMap and can be used
//                                   for both cases.
//   HandlerMap4:                    build up high-level representation of HandlerMap from a TryBlockMapEntry4 in a TryBlockMap4.


// High level representations for easy processing and traversal of low level structures

// Higher-level abstraction of compressed __CxxFrameHandler4 data structures

struct UnwindMapEntry4 {
    // To save space, we encode common dtor patterns that used to require a compiler-generated stub to execute into an offset + RVA to the destructor method itself.
    enum Type {
        NoUW = 0b00,              // No unwind action associated with this state
        DtorWithObj = 0b01,       // Dtor with an object offset
        DtorWithPtrToObj = 0b10,  // Dtor with an offset that contains a pointer to the object to be destroyed
        RVA = 0b11                // Dtor that has a direct function that is called that knows where the object is and can perform more exotic destruction
    };
    Cardinal::UInt32        nextOffset;   // State this action takes us to (in offset form, unlike FH3!)
    Type                     type;         // Type of entry
    Cardinal::Int32         action;       // Image-relative offset of action, exists for all NoUW entry types
    Cardinal::UInt32        object;       // Frame offset of object pointer to be destroyed, exists for DtorWithObj and DtorWithPtrToObj types
};


class UWMap4 {
public:
    UWMap4(const FuncInfo4* pFuncInfo, uintptr_t imageBase) : _UWEntry(), _bufferStart(0) {
        if (pFuncInfo->dispUnwindMap != 0)
        {
            Cardinal::UInt8* buffer = imageRelToByteBuffer(imageBase, pFuncInfo->dispUnwindMap);
            _numEntries = ReadUnsigned(&buffer);
            _bufferStart = buffer;
        }
        else
        {
            _numEntries = 0;
        }
    }

    class iterator //-V690
    {
    public:
        iterator(UWMap4& UWMap, Cardinal::UInt8* currEntry)
            : _UWMap(UWMap), _currEntry(currEntry) {
        }

        void WalkBack() {
            _UWMap.WalkBack(&_currEntry);
        }

        iterator& operator++() {
            _UWMap.WalkForward(&_currEntry);
            return *this;
        }

        UnwindMapEntry4 operator*() {
            Cardinal::UInt8* origOffset = _currEntry;
            _UWMap.ReadEntry(&_currEntry);
            _currEntry = origOffset;
            return _UWMap._UWEntry;
        }

        iterator& operator=(const iterator& other) {
            this->_currEntry = other._currEntry;
            this->_UWMap = other._UWMap;
            return *this;
        }

        bool operator!=(const iterator& other) const {
            return (_currEntry != other._currEntry);
        }

        bool operator>(const iterator& other) const {
            return (_currEntry > other._currEntry);
        }

        bool operator>=(const iterator& other) const {
            return (_currEntry >= other._currEntry);
        }

        ptrdiff_t operator-(const iterator& other) const {
            return (_currEntry - other._currEntry);
        }

    private:
        UWMap4& _UWMap;
        Cardinal::UInt8* _currEntry;
    };

    iterator begin() {
        return iterator(*this, _bufferStart);
    }

    iterator end() {
        Cardinal::UInt8* currOffset = _bufferStart;
        for (__ehstate_t i = 0; i < _numEntries; i++)
        {
            ReadEntry(&currOffset);
        }

        return iterator(*this, currOffset);
    }

    void getStartStop(__ehstate_t start, __ehstate_t stop, iterator& startIter, iterator& stopIter) {
        Cardinal::UInt8* startOffset = _bufferStart - 1;
        Cardinal::UInt8* stopOffset = _bufferStart - 1;
        Cardinal::UInt8* currOffset = _bufferStart;
        bool foundStart = false;
        bool foundStop = false;
        for (__ehstate_t i = 0; i < _numEntries; i++)
        {
            if (i == start)
            {
                startOffset = currOffset;
                foundStart = true;
            }

            if (i == stop)
            {
                stopOffset = currOffset;
                foundStop = true;
            }

            if (foundStart && foundStop)
            {
                break;
            }

            ReadEntry(&currOffset);
        }

        startIter = iterator(*this, startOffset);
        stopIter = iterator(*this, stopOffset);
    }

    // Using known offsets/state values, find the state of an unknown iterator known to be between
    // lowState and highState
    static __ehstate_t getStateFromIterators(const iterator& lowStateIter, const __ehstate_t lowState,
        const iterator& highStateIter, const __ehstate_t highState, const iterator& toStateIter) {
        __ehstate_t state = -1;

        // Out of range
        // TODO: error condition on exceeding max
        // TODO: check to make sure if below min, below by 1 byte
        if (toStateIter > highStateIter || lowStateIter > toStateIter)
        {
            return state;
        }

        // Find from lowState
        if ((toStateIter - lowStateIter) < (highStateIter - toStateIter))
        {
            auto lowStateIterCopy = lowStateIter;
            state = lowState;
            while (toStateIter > lowStateIterCopy)
            {
                ++lowStateIterCopy;
                ++state;
            }
        }
        // Find from highState
        else
        {
            auto toStateIterCopy = toStateIter;
            state = highState;
            // Can't effectively walk backwards, so walk forwards
            // from the state we want.
            while (highStateIter > toStateIterCopy)
            {
                ++toStateIterCopy;
                --state;
            }
        }

        return state;
    }

    Cardinal::UInt32 getNumEntries() {
        return _numEntries;
    }

private:
    int _numEntries;
    Cardinal::UInt8* _bufferStart;
    UnwindMapEntry4 _UWEntry;

    void ReadEntry(Cardinal::UInt8** currOffset) {
        // First field has 2 lower bits indicating type, rest indicating next offset
        _UWEntry.nextOffset = ReadUnsigned(currOffset);
        _UWEntry.type = static_cast<UnwindMapEntry4::Type>(_UWEntry.nextOffset & 0b11);
        _UWEntry.nextOffset >>= 2;

        if (_UWEntry.type == UnwindMapEntry4::Type::DtorWithObj || _UWEntry.type == UnwindMapEntry4::Type::DtorWithPtrToObj)
        {
            _UWEntry.action = ReadInt(currOffset);
            _UWEntry.object = ReadUnsigned(currOffset);
        }
        else if (_UWEntry.type == UnwindMapEntry4::Type::RVA)
        {
            _UWEntry.action = ReadInt(currOffset);
        }
    }

    void WalkBack(Cardinal::UInt8** currOffset) {
        Cardinal::UInt8* origOffset = *currOffset;

        ReadEntry(currOffset);

        *currOffset = origOffset - _UWEntry.nextOffset;
    }

    void WalkForward(Cardinal::UInt8** currOffset) {
        ReadEntry(currOffset);
    }

};
#pragma warning(push)
//Not initialize all variables
#pragma warning(disable: 26495)
//Integer overlow
#pragma warning(disable: 26451)

struct TryBlockMapEntry4 {
    __ehstate_t       tryLow;             // Lowest state index of try
    __ehstate_t       tryHigh;            // Highest state index of try
    __ehstate_t       catchHigh;          // Highest state index of any associated catch
    Cardinal::UInt32 dispHandlerArray;   // Image relative offset of list of handlers for this try
};

class TryBlockMap4 {
public:
    TryBlockMap4(const FuncInfo4* pFuncInfo, uintptr_t imageBase) {
        if (pFuncInfo->dispTryBlockMap != 0)
        {
            _buffer = imageRelToByteBuffer(imageBase, pFuncInfo->dispTryBlockMap);
            _numTryBlocks = ReadUnsigned(&_buffer);
            // Set reset position after count field
            _bufferStart = _buffer;
            DecompTryBlock();
        }
        else
        {
            _numTryBlocks = 0;
        }
    }

    class iterator //-V690
    {
    public:
        iterator(TryBlockMap4& tryBlockMap, Cardinal::UInt32 currBlock)
            : _tryBlockMap(tryBlockMap), _currBlock(currBlock) {
        }

        iterator& operator++() {
            _tryBlockMap.DecompTryBlock();
            _currBlock++;
            return *this;
        }

        TryBlockMapEntry4 operator*() {
            return _tryBlockMap._tryBlock;
        }

        bool operator!=(const iterator& other) const {
            return (_currBlock != other._currBlock);
        }

        bool operator<(const iterator& other) const {
            return (_currBlock < other._currBlock);
        }

        iterator& operator=(const iterator& other) {
            this->_currBlock = other._currBlock;
            return *this;
        }

        // End iterators can exceed total length but we
        // don't actually want to try and read it.
        void incrementToSentinel() {
            _currBlock++;
        }

    private:
        TryBlockMap4& _tryBlockMap;
        Cardinal::UInt32 _currBlock;
    };

    iterator begin() {
        return iterator(*this, 0);
    }

    iterator end() {
        return iterator(*this, _numTryBlocks);
    }

    Cardinal::UInt32 getNumTryBlocks() {
        return _numTryBlocks;
    }

    // Iterators are detached from the main data structure for easy
    // creation of the end iterator. To get meaningful reads from the structure
    // it needs to be set to the start iterator before traversal.
    void setBuffer(const iterator& iter) {
        _buffer = _bufferStart;
        DecompTryBlock();
        for (iterator start = begin(); start != iter; ++start)
        {
            DecompTryBlock();
        }
    }

    struct IteratorPair {
        iterator first;
        iterator last;
    };

private:
    Cardinal::UInt32 _numTryBlocks = 0;
    Cardinal::UInt8* _buffer = nullptr;
    Cardinal::UInt8* _bufferStart = nullptr;
    TryBlockMapEntry4 _tryBlock;
    // Assumes Number of Try Blocks field has been read out already
    void DecompTryBlock() {
        _tryBlock.tryLow = ReadUnsigned(&_buffer);
        _tryBlock.tryHigh = ReadUnsigned(&_buffer);
        _tryBlock.catchHigh = ReadUnsigned(&_buffer);
        _tryBlock.dispHandlerArray = ReadInt(&_buffer);
    }

};

inline constexpr Cardinal::UInt32 MAX_CONT_ADDRESSES = 2;

struct HandlerTypeHeader {
    // See contAddr for description of these values
    enum contType {
        NONE = 0b00,
        ONE = 0b01,
        TWO = 0b10,
        RESERVED = 0b11
    };
    union {
#pragma warning(push)
#pragma warning(disable: 4201) // nonstandard extension used: nameless struct/union
        struct {
            Cardinal::UInt8 adjectives : 1; // Existence of Handler Type adjectives (bitfield)
            Cardinal::UInt8 dispType : 1; // Existence of Image relative offset of the corresponding type descriptor
            Cardinal::UInt8 dispCatchObj : 1; // Existence of Displacement of catch object from base
            Cardinal::UInt8 contIsRVA : 1; // Continuation addresses are RVAs rather than function relative, used for separated code
            Cardinal::UInt8 contAddr : 2; // 1.   00: no continuation address in metadata, use what the catch funclet returns
            // 2.   01: one function-relative continuation address
            // 3.   10: two function-relative continuation addresses
            // 4.   11: reserved
            Cardinal::UInt8 unused : 2;
        };
#pragma warning(pop)
        Cardinal::UInt8 value;
    };
};

static_assert(sizeof(HandlerTypeHeader) == sizeof(Cardinal::UInt8), "Size of HandlerTypeHeader not 1 Byte");

struct HandlerType4 {
    HandlerTypeHeader header;
    Cardinal::UInt32          adjectives;                              // Handler Type adjectives (bitfield)
    Cardinal::Int32           dispType;                                // Image relative offset of the corresponding type descriptor
    Cardinal::UInt32          dispCatchObj;                            // Displacement of catch object from base
    Cardinal::Int32           dispOfHandler;                           // Image relative offset of 'catch' code
    uintptr_t                  continuationAddress[MAX_CONT_ADDRESSES]; // Continuation address(es) of catch funclet

    void reset() {
        header.value = 0;
        adjectives = 0;
        dispType = 0;
        dispCatchObj = 0;
        dispOfHandler = 0;
        memset(continuationAddress, 0, sizeof(continuationAddress));
    }

    HandlerType4() {
        reset();
    }
};

class HandlerMap4 {
public:
    HandlerMap4(const TryBlockMapEntry4* tryMap, uintptr_t imageBase, Cardinal::Int32 functionStart) : _imageBase(imageBase), _functionStart(functionStart) {
        if (tryMap->dispHandlerArray != 0)
        {
            _buffer = imageRelToByteBuffer(_imageBase, tryMap->dispHandlerArray);
            _numHandlers = ReadUnsigned(&_buffer);
            _bufferStart = _buffer;
            DecompHandler();
        }
        else
        {
            _numHandlers = 0;
        }
    }

    class iterator {
    public:
        iterator(HandlerMap4& handlerMap, Cardinal::UInt32 currBlock)
            : _handlerMap(handlerMap), _currBlock(currBlock) {
        }

        iterator& operator++() {
            _handlerMap.DecompHandler();
            _currBlock++;
            return *this;
        }

        iterator operator++(int) {
            iterator tmp(*this);
            operator++();
            return tmp;
        }

        HandlerType4 operator*() {
            return _handlerMap._handler;
        }

        bool operator==(const iterator& other) const {
            return (_currBlock == other._currBlock);
        }

        bool operator!=(const iterator& other) const {
            return !(*this == other);
        }

    private:
        HandlerMap4& _handlerMap;
        Cardinal::UInt32 _currBlock;
    };

    iterator begin() {
        return iterator(*this, 0);
    }

    iterator end() {
        return iterator(*this, _numHandlers);
    }

    void setBuffer(Cardinal::UInt32 index) {
        _buffer = _bufferStart;
        DecompHandler();
        for (Cardinal::UInt32 i = 0; i < index; i++)
        {
            DecompHandler();
        }
    }

    void resetBuffer() {
        setBuffer(0);
    }

    Cardinal::UInt32 getNumHandlers() {
        return _numHandlers;
    }

    // This is destructive to the current state
    HandlerType4* getLastEntry() {
        resetBuffer();
        setBuffer(_numHandlers - 1);
        return &_handler;
    }

private:
    Cardinal::UInt32 _numHandlers;
    Cardinal::UInt32 _functionStart;
    Cardinal::UInt8* _buffer;
    Cardinal::UInt8* _bufferStart;
    HandlerType4 _handler;
    uintptr_t _imageBase;

    // Assumes Number of Try Blocks field has been read out already
    void DecompHandler() {
        _handler.reset();
        _handler.header.value = _buffer[0];
        ++_buffer;

        if (_handler.header.adjectives)
        {
            _handler.adjectives = ReadUnsigned(&_buffer);
        }

        if (_handler.header.dispType)
        {
            _handler.dispType = ReadInt(&_buffer);
        }

        if (_handler.header.dispCatchObj)
        {
            _handler.dispCatchObj = ReadUnsigned(&_buffer);
        }

        _handler.dispOfHandler = ReadInt(&_buffer);

        if (_handler.header.contIsRVA)
        {
            if (_handler.header.contAddr == HandlerTypeHeader::contType::ONE)
            {
                _handler.continuationAddress[0] = ReadInt(&_buffer);
            }
            else if (_handler.header.contAddr == HandlerTypeHeader::contType::TWO)
            {
                _handler.continuationAddress[0] = ReadInt(&_buffer);
                _handler.continuationAddress[1] = ReadInt(&_buffer);
            }
            else
            {
                // no encoded cont addresses or unknown
            }
        }
        else
        {
            if (_handler.header.contAddr == HandlerTypeHeader::contType::ONE)
            {
                _handler.continuationAddress[0] = _functionStart + ReadUnsigned(&_buffer);
            }
            else if (_handler.header.contAddr == HandlerTypeHeader::contType::TWO)
            {
                _handler.continuationAddress[0] = _functionStart + ReadUnsigned(&_buffer);
                _handler.continuationAddress[1] = _functionStart + ReadUnsigned(&_buffer);
            }
            else
            {
                // no encoded cont addresses or unknown
            }
        }
    }
};
#pragma warning(pop)