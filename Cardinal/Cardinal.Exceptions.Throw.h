#pragma once
#include "Cardinal.Exceptions.Base.h"

#pragma warning(push)
#pragma warning(disable: 4996)
#pragma warning(disable: 4251)

#pragma region Throw
#ifndef CXXTHROWAPI
/// <summary>
/// The exception generation with type checking
/// </summary>
/// <param name = 'ExceptionObj'>The exception object or function that returns this kind of object</param>
#define Throw(ExceptionObj)                                                                                  \
	do{                                                                                                          \
		static_assert(__IsThrowable<decltype(ExceptionObj)>::State ||                                              \
			Cardinal::TypeTraits::IsSameT<Cardinal::InteruptException, decltype(ExceptionObj)>::State,   \
		"Only Cardinal::Exception and derived classes can be thrown as exception");                                   \
		__assume(__IsThrowable<decltype(ExceptionObj)>());                                                         \
		__pragma(warning(push));                                                                                 \
		__pragma(warning(disable: 4996));                                                                        \
		___cxxthrow(ExceptionObj);                                                                               \
		__pragma(warning(pop));                                                                                  \
	 }while(false)

#define ReThrow                                                                                              \
	do{                                                                                                          \
	__pragma(warning(push));                                                                                     \
	__pragma(warning(disable: 4996));                                                                            \
	__cxxrethrow();                                                                                              \
	__pragma(warning(pop));                                                                                      \
	} while(false)

	/// <summary>
	/// Prohibit the use of <c>throw</c> without checking the exception object type
	/// </summary>
#define throw static_cast(false, "Use Throw insted") 
#endif // !CXXTHROWAPI
#pragma endregion

#pragma warning(pop)