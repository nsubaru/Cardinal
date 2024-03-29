#pragma once
#include "Cardinal.CTTI.Minimal.TypeTraits.Object.Base.h"

#define CttiClassRecordType(Class, Constructors, Interfaces, Properties, Methods) \
	Cardinal::CTTI::TypeTraits::ClassRecord<Class, Constructors, Interfaces, Properties, Methods>

#define CttiClassRecord(Class, Interfaces, Constructors, Methods, Properties) \
	constexpr static auto __identifier(class)() { return CttiClassRecordType(Class, Constructors, Interfaces, Properties, Methods)(); };