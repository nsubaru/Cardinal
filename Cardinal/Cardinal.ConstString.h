#pragma once
#include "Cardinal.Definitions.Base.h"

#undef SourcePosition
#define SourcePosition Cardinal::SourceLocation{_L(__FILE__), (Cardinal::SizeT) __LINE__}