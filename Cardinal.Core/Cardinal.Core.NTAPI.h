#pragma once
#ifndef CARDINAL_RT_ALLOW_INTERNAL_NT_API
#error "Internal NT API not allowed"
#endif

#pragma comment(lib, "ntdll.lib")

#include "ntapi.h"

#define _RTTI_RELATIVE_TYPEINFO 1