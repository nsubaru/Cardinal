#pragma once
#ifndef CARDINAL_RT_ALLOW_INTERNAL_NT_API
#error "Internal NT API not allowed"
#endif

#pragma comment(lib, "ntdll.lib")

#include "base.h"
#include "sal.h"
#include "interlocked.h"
#include "ntstatus.h"
#include "ntapi.h"