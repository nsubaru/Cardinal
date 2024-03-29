export module Cardinal.RTTI:Base.Exceptions;

export import :Base.Types;

export namespace Cardinal::RTTI::Exceptions
{
	class GenericRttiException;
	class DynamicCallException;
	class DynamicCallInvalidArgumentsCountException;
	class DynamicCallInvalidArgumentTypeException;
	class DynamicCallInvalidTargetException;
	class DynamicInterfaceCastInvalidTargetException;
	class InvalidRttiTypeException;
}
