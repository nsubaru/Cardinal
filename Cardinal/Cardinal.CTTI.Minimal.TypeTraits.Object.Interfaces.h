#pragma once
#include "Cardinal.CTTI.Minimal.TypeTraits.Object.Base.h"

#define CttiInterfaces(Class, ...) decltype(Cardinal::CTTI::TypeTraits::BuildInterfaceRecords<Class, ## __VA_ARGS__>())

#define NoCttiInterfaces() Cardinal::Containers::Tuple<>