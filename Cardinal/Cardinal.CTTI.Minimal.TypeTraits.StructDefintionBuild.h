#pragma once
#include "Cardinal.CTTI.Minimal.TypeTraits.Base.h"
#include "Cardinal.CTTI.Minimal.TypeTraits.Struct.h"

#define StructCttiDef(StructName, ...)												 \
	consteval static decltype(auto) FieldMetaDataGetter()							 \
	{																				 \
		return DefStructMetaDataObj(StructName,__VA_ARGS__);                         \
	}