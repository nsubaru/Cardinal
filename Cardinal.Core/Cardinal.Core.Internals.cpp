module;
#include "Cardinal.Core.Definitions.h"

#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

module Cardinal.Core;

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

using namespace Cardinal::Core;

void Cardinal::Core::Internals::UppercaseCharStrImpl(Cardinal::Core::CharStr from, Cardinal::Core::CharStr to) {
	UNICODE_STRING __from;
	UNICODE_STRING __to;
	RtlInitUnicodeString(&__from, from);
	__to.MaximumLength = __to.Length = __from.Length;
	__to.Buffer = to;
	RtlUpcaseUnicodeString(&__to, &__from, 0);
}

bool Cardinal::Core::Internals::ConvertCharStrToUuid(Cardinal::Core::CharStr str, Cardinal::Core::UUID& uuid) {
	GUID guid;
	UNICODE_STRING raw_str;
	RtlInitUnicodeStringEx(&raw_str, str);
	if (!NT_SUCCESS(RtlGUIDFromString(&raw_str, &guid))) {
		return false;
	}
	uuid = UUID(guid);
	return true;
}

void Cardinal::Core::Internals::ConvertUuidToCharStr(const Cardinal::Core::UUID& id, Cardinal::Core::CharStr buffer) {
	UNICODE_STRING raw_str;
	GUID rawId = id;
	RtlStringFromGUID(&rawId, &raw_str);
	CStr::Copy(raw_str.Buffer, buffer);
	RtlFreeUnicodeString(&raw_str);
}

Cardinal::Core::Char Cardinal::Core::Internals::UpcaseCharImp(Cardinal::Core::Char symb) {
	return RtlUpcaseUnicodeChar(symb);
}

Cardinal::Core::Char Cardinal::Core::Internals::DowncaseCharImp(Cardinal::Core::Char symb) {
	return RtlDowncaseUnicodeChar(symb);
}

void Cardinal::Core::Internals::DowncaseCharStrImpl(Cardinal::Core::CharStr from, Cardinal::Core::CharStr to) {
	UNICODE_STRING __from;
	UNICODE_STRING __to;
	RtlInitUnicodeString(&__from, from);
	__to.MaximumLength = __to.Length = __from.Length;
	__to.Buffer = to;
	RtlDowncaseUnicodeString(&__to, &__from, FALSE);
}

Cardinal::Core::Int32 Cardinal::Core::Internals::CompareString(CharStr left, CharStr right, SizeT leftSize, SizeT rightSize) {
	return RtlCompareUnicodeStrings(left, leftSize, right, rightSize, FALSE);
}


Cardinal::Core::Int32 Cardinal::Core::Internals::CompareStringCaseInsesitve(CharStr left, CharStr right, SizeT leftSize, SizeT rightSize) {
	return RtlCompareUnicodeStrings(left, leftSize, right, rightSize, TRUE);
}

InternalApiCall Cardinal::Core::Boolean Cardinal::Core::Internals::IsNameInExpression(Cardinal::Core::CharStr name, Cardinal::Core::CharStr mask)
{
	UNICODE_STRING nameUs, maskUs;
	RtlInitUnicodeStringEx(&nameUs, name);
	RtlInitUnicodeStringEx(&maskUs, mask);
	return RtlIsNameInExpression(&maskUs, &nameUs, FALSE, NULL) == TRUE;
}

#pragma warning(pop)