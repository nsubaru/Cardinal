module;
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)
module Cardinal.ConstString;
import Cardinal.Exceptions.Internals;
import Cardinal.Core;

using namespace Cardinal;

Char Cardinal::ConstString::operator[](SizeT index) const {
	if (index >= size)
		Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, index, size);

	return str[index];
}

AChar Cardinal::ConstAString::operator[](SizeT index) const {
	if (index >= size)
		Internals::Exceptions::ThrowIndexOutOfRange(SourcePosition, index, size);

	return str[index];
}