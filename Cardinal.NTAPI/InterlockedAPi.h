#pragma once
#include "interlocked.h"
__forceinline void* implInterlockedExchangePointer(void* volatile* Target, void* Val) {
	return _InterlockedExchangePointer(Target, Val);
}

__forceinline void implMemoryBarrier() {
	__faststorefence();
}