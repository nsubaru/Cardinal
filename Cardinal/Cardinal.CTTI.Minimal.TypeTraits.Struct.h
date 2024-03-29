#pragma once
#include "Cardinal.CTTI.Minimal.TypeTraits.Base.h"

#define DefStructFieldData(Struct, Name)                                           \
    Cardinal::CTTI::TypeTraits::FieldTypeData<                                     \
        Struct,                                                                    \
        decltype(Struct::Name),                                                    \
        (Cardinal::SizeT) StructOffsetOfMember(Struct, Name),                      \
        decltype(Cardinal::TypeTraits::CreateTypeStr(L"" #Name))                   \
    >(Cardinal::TypeTraits::CreateTypeStr(L"" #Name))


#define DefStructMetaDataObj(Struct, ...)                                          \
    Cardinal::CTTI::TypeTraits::MakeCustomStruct<Struct>(__VA_ARGS__)


#define DefStructMetaData(Struct, ...)                                             \
    static constexpr auto FieldMetaData = DefStructMetaDataObj(Struct, __VA_ARGS__) 


#define StructCttiDef(StructName, ...)												 \
	consteval static decltype(auto) FieldMetaDataGetter()							 \
	{																				 \
		return DefStructMetaDataObj(StructName,__VA_ARGS__);                         \
	}
