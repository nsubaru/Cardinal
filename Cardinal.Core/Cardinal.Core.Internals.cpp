module;
#include "Cardinal.Core.Definitions.h"

#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

module Cardinal.Core;

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

using namespace Cardinal;

void Cardinal::Internals::UppercaseCharStrImpl(Cardinal::CharStr from, Cardinal::CharStr to) {
	UNICODE_STRING __from;
	UNICODE_STRING __to;
	RtlInitUnicodeString(&__from, from);
	__to.MaximumLength = __to.Length = __from.Length;
	__to.Buffer = to;
	RtlUpcaseUnicodeString(&__to, &__from, 0);
}

bool Cardinal::Internals::ConvertCharStrToUuid(Cardinal::CharStr str, Cardinal::UUID& uuid) {
	GUID guid;
	UNICODE_STRING raw_str;
	RtlInitUnicodeStringEx(&raw_str, str);
	if (!NT_SUCCESS(RtlGUIDFromString(&raw_str, &guid))) {
		return false;
	}
	uuid = UUID(guid);
	return true;
}

void Cardinal::Internals::ConvertUuidToCharStr(const Cardinal::UUID& id, Cardinal::CharStr buffer) {
	UNICODE_STRING raw_str;
	GUID rawId = id;
	RtlStringFromGUID(&rawId, &raw_str);
	CStr::Copy(raw_str.Buffer, buffer);
	RtlFreeUnicodeString(&raw_str);
}

Cardinal::Char Cardinal::Internals::UpcaseCharImp(Cardinal::Char symb) {
	return RtlUpcaseUnicodeChar(symb);
}

Cardinal::Char Cardinal::Internals::DowncaseCharImp(Cardinal::Char symb) {
	return RtlDowncaseUnicodeChar(symb);
}

void Cardinal::Internals::DowncaseCharStrImpl(Cardinal::CharStr from, Cardinal::CharStr to) {
	UNICODE_STRING __from;
	UNICODE_STRING __to;
	RtlInitUnicodeString(&__from, from);
	__to.MaximumLength = __to.Length = __from.Length;
	__to.Buffer = to;
	RtlDowncaseUnicodeString(&__to, &__from, FALSE);
}

Cardinal::Int32 Cardinal::Internals::CompareString(CharStr left, CharStr right, SizeT leftSize, SizeT rightSize) {
	return RtlCompareUnicodeStrings(left, leftSize, right, rightSize, FALSE);
}


Cardinal::Int32 Cardinal::Internals::CompareStringCaseInsesitve(CharStr left, CharStr right, SizeT leftSize, SizeT rightSize) {
	return RtlCompareUnicodeStrings(left, leftSize, right, rightSize, TRUE);
}

InternalApiCall Cardinal::Boolean Cardinal::Internals::IsNameInExpression(Cardinal::CharStr name, Cardinal::CharStr mask)
{
	UNICODE_STRING nameUs, maskUs;
	RtlInitUnicodeStringEx(&nameUs, name);
	RtlInitUnicodeStringEx(&maskUs, mask);
	return RtlIsNameInExpression(&maskUs, &nameUs, FALSE, NULL) == TRUE;
}

#pragma warning(pop)