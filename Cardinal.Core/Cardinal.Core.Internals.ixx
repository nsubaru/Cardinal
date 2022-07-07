module;
#include "Cardinal.Core.Definitions.h"
export module Cardinal.Core:Internals;

import :Types;
import :UUID;

#pragma warning(push)
#pragma warning(disable: 4996)

namespace Cardinal::Core::Internals
{
	InternalApiCall void UppercaseCharStrImpl(CharStr from, CharStr to);
	InternalApiCall bool ConvertCharStrToUuid(CharStr str, UUID& uuid);
	InternalApiCall void ConvertUuidToCharStr(const UUID& id, CharStr buffer);
	InternalApiCall Char UpcaseCharImp(Char symb);
	InternalApiCall Char DowncaseCharImp(Char symb);
	InternalApiCall void DowncaseCharStrImpl(CharStr from, CharStr to);
	InternalApiCall Int32 CompareString(CharStr left, CharStr right, SizeT leftSize, SizeT rightSize);
	InternalApiCall Int32 CompareStringCaseInsesitve(CharStr left, CharStr right, SizeT leftSize, SizeT rightSize);
	InternalApiCall Boolean IsNameInExpression(CharStr name, CharStr mask);
}

#pragma warning(pop)