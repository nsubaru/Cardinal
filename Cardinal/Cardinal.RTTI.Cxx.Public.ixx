module;
#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

export module Cardinal.RTTI:Cxx.Public;

// Implementation of casting rtti type to void*
void* __RTCastToVoidImpl(
	void* inptr)            // Pointer to polymorphic object
	noexcept(false);

/////////////////////////////////////////////////////////////////////////////
//
// __RTCastToVoid - Implements dynamic_cast<void*>
//
// Output: Pointer to complete object containing *inptr
//
// Side-effects: NONE.
//

extern "C" inline void* __cdecl __RTCastToVoid(
	void* inptr)            // Pointer to polymorphic object
	noexcept(false) {
	return __RTCastToVoidImpl(inptr);
}

// Implementation of getting cxx rtti type data
void* __RTtypeidImpl(
	void* inptr)            // Pointer to polymorphic object
	noexcept(false);

/////////////////////////////////////////////////////////////////////////////
//
// __RTtypeid - Implements typeid() operator
//
// Output: Pointer to type descriptor of complete object containing *inptr
//
// Side-effects: NONE.
//

extern "C" inline void* __cdecl __RTtypeid(
	void* inptr)            // Pointer to polymorphic object
	noexcept(false) {
	return __RTtypeidImpl(inptr);
}

// Implementation of casting class A to class B using dynamic_cast
void* __RTDynamicCastImpl(
	void* inptr,            // Pointer to polymorphic object
	LONG VfDelta,            // Offset of vfptr in object
	void* srcVoid,          // Static type of object pointed to by inptr
	void* targetVoid,       // Desired result of cast
	BOOL isReference)        // TRUE if input is reference, FALSE if input is ptr
	noexcept(false);

/////////////////////////////////////////////////////////////////////////////
//
// __RTDynamicCast - Runtime implementation of dynamic_cast<> operator
//
// Output: Pointer to the appropriate sub-object, if possible; nullptr otherwise
//
// Side-effects: Throws bad_cast() if cast fails & input of dynamic_cast<> is
// a reference
//
// Note: A normal runtime check can be generated for a down-cast (base to
// derived) and a cross-cast (a proper base of the complete object to some other
// proper base which is neither an accessible unambiguous base nor derived class
// of the first base).  But the compiler will also generate a runtime check for
// an up-cast (derived to base) that is disallowed because the target base class
// is not public or is ambiguous within the source derived class.  Such an
// invalid up-cast may be a valid cross-cast in the VI case, e.g.:
//
//      A
//     / \
//    B   C
//     \ /
//      D
//
//   class A { virtual ~A(); {} };
//   class B : virtual private A {};
//   class C : virtual public A {};
//   class D : public B, public C {};
//   ...
//   dynamic_cast<A*>((B*)new D);
//
// The up-cast B->A fails, since B inherits A privately.  But the cross-cast
// B->D->C->A succeeds with a runtime check.
//
#pragma optimize("",off)
extern "C" inline void* __cdecl __RTDynamicCast(
	void* inptr,            // Pointer to polymorphic object
	LONG VfDelta,            // Offset of vfptr in object
	void* srcVoid,          // Static type of object pointed to by inptr
	void* targetVoid,       // Desired result of cast
	BOOL isReference)        // TRUE if input is reference, FALSE if input is ptr
	noexcept(false) {
	return __RTDynamicCastImpl(inptr, VfDelta, srcVoid, targetVoid, isReference);
}