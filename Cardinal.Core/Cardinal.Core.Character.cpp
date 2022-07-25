module;
#define CARDINAL_RT_ALLOW_INTERNAL_NT_API
#include "Cardinal.Core.NTAPI.h"

module Cardinal.Core;

Cardinal::StrongOrdering Cardinal::Characters::CaseInsesitiveCompare(Cardinal::AChar left, Cardinal::AChar right) {
	AChar s1b[2] = { 0 }, s2b[2] = { 0 };
	s1b[0] = left; s2b[0] = right;

	STRING s1 = { 0 }, s2 = { 0 };
	s1.Length = 1;        s2.Length = 1;
	s1.MaximumLength = 2; s2.MaximumLength = 2;
	s1.Buffer = s1b;      s2.Buffer = s2b;

	auto rawResult = RtlCompareString(&s1, &s2, TRUE);

	if (rawResult == 0)
		return StrongOrdering::equal;
	else if (rawResult > 0)
		return StrongOrdering::greater;
	else
		return StrongOrdering::less;
}

Cardinal::StrongOrdering Cardinal::Characters::CaseInsesitiveCompare(Cardinal::UChar left, Cardinal::UChar right) {
	UChar s1b[2] = { 0 }, s2b[2] = { 0 };
	s1b[0] = left; s2b[0] = right;

	UNICODE_STRING s1 = { 0 }, s2 = { 0 };

	s1.Length = 2;        s2.Length = 2;
	s1.MaximumLength = 4; s2.MaximumLength = 4;
	s1.Buffer = s1b;      s2.Buffer = s2b;

	auto rawResult = RtlCompareUnicodeString(&s1, &s2, TRUE);

	if (rawResult == 0)
		return StrongOrdering::equal;
	else if (rawResult > 0)
		return StrongOrdering::greater;
	else
		return StrongOrdering::less;
}