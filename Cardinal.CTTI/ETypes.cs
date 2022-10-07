namespace Cardinal.CTTI;

public enum ETypes : byte
{
	MetaData,
	Logic,
	Numeric,
	Enumeration,
	FloatNumeric,
	Character,
	Collection,
	String,
	FixedCollection,
	AssociableCollection,
	Box,
	Tuple,
	Pair,
	Pointer,
	Reference,
	FixedArray,
	Struct,
	Object,
	Dynamic,
	Abstract,
	Function,
	Union,
	Pod,
	Custom,
	Undefined = byte.MaxValue
}