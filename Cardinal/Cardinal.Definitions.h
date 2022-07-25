#pragma once
#include "Cardinal.Core.Definitions.h"

#undef SourcePosition
#define SourcePosition Cardinal::SourceLocation{_L(__FILE__), (Cardinal::SizeT) __LINE__}