#pragma once
#include "Cardinal.CTTI.Minimal.TypeTraits.Object.Base.h"

#define CttiConstructorDefaultInstanceRecord(Class) Cardinal::CTTI::TypeTraits::ConstructorInstanceRecord<Class>
#define CttiConstructorInstanceRecord(Class, ...) Cardinal::CTTI::TypeTraits::ConstructorInstanceRecord<Class, __VA_ARGS__>
#define CttiConstructorsRecord(Class, ...) Cardinal::CTTI::TypeTraits::ConstructorsRecord<Class, Cardinal::Containers::Tuple<__VA_ARGS__>>
#define NoCttiConstructors(Class) Cardinal::CTTI::TypeTraits::ConstructorsRecord<Class, Cardinal::Containers::Tuple<>>