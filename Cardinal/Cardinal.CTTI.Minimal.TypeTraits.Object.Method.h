#pragma once
#include "Cardinal.CTTI.Minimal.TypeTraits.Object.Base.h"

#define CttiMethodRecord(cls, method)														\
	Cardinal::CTTI::TypeTraits::MethodRecord<cls,											\
		decltype(Cardinal::TypeTraits::CreateTypeStr(L"" #method)),							\
		[] () constexpr { return Cardinal::TypeTraits::CreateTypeStr(L"" #method); },		\
		decltype(&cls::method), &cls::method>

#define CttiExtendedMethodRecord(cls, method, signature)									\
	Cardinal::CTTI::TypeTraits::MethodRecord<cls,											\
		decltype(Cardinal::TypeTraits::CreateTypeStr(L"" #method)),							\
		[] () constexpr { return Cardinal::TypeTraits::CreateTypeStr(L"" #method); },		\
		decltype((signature)&cls::method), (signature) &cls::method>

#define CttiMethodGroup(...) Cardinal::Containers::Tuple<## __VA_ARGS__>