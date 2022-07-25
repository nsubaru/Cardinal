export module Cardinal.Exceptions.Internals;

export import Cardinal.Core;
export import Cardinal.ConstString;

export namespace Cardinal::Internals::Exceptions
{
	InternalApiCall __declspec(noreturn) void ThrowNullPointException(
		SourceLocation&& sourceLocation,
		SizeT addr,
		UInt8 type);

	InternalApiCall __declspec(noreturn) void ThrowStringToLong(
		SourceLocation&& sourceLocation,
		const SizeT requiredSize);

	InternalApiCall __declspec(noreturn) void ThrowNullPointException(
		SourceLocation&& sourceLocation);

	InternalApiCall __declspec(noreturn) void ThrowIntegerOverflowException(
		SourceLocation&& sourceLocation);

	InternalApiCall __declspec(noreturn) void ThrowInvalidFormatException(
		SourceLocation&& sourceLocation);

	InternalApiCall __declspec(noreturn) void ThrowIndexOutOfRange(
		SourceLocation&& sourceLocation,
		const SizeT required,
		const SizeT maxAllowed);

	InternalApiCall __declspec(noreturn) void ThrowEndNodeNotDeferencable(
		SourceLocation&& sourceLocation);

	InternalApiCall __declspec(noreturn) void ThrowNotImplemented(
		SourceLocation&& sourceLocation);

	InternalApiCall __declspec(noreturn) void ThrowKeyAlredyExist(
		SourceLocation&& sourceLocation);

	InternalApiCall __declspec(noreturn) void ThrowNotFound(
		SourceLocation&& sourceLocation);

	InternalApiCall __declspec(noreturn) void ThrowInternalError(
		SourceLocation&& sourceLocation);

	InternalApiCall __declspec(noreturn) void ThrowDivideByZero(
		SourceLocation&& sourceLocation);

	InternalApiCall __declspec(noreturn) void ThrowInvalidArgumentException(
		SourceLocation&& sourceLocation,
		const ConstString& argName,
		const ConstString& description);

	InternalApiCall __declspec(noreturn) void __ReThrow();
}