#pragma once
#include "Cardinal.CTTI.Minimal.TypeTraits.Object.Base.h"

#define CttiPropertyRecord(Name, Getters, Setters) Cardinal::CTTI::TypeTraits::PropertyRecord<				\
	decltype(Cardinal::TypeTraits::CreateTypeStr(L"" #Name)),												\
	[] () constexpr { return Cardinal::TypeTraits::CreateTypeStr(L"" #Name); },								\
	Getters,																								\
	Setters>

#define CttiProperties(...) Cardinal::Containers::Tuple<## __VA_ARGS__>