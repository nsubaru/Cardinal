/******************************************************************
*                                                                 *
*  ntintsafe.h -- This module defines helper functions to prevent *
*                 integer overflow bugs for drivers. A similar    *
*                 file, intsafe.h, is available for applications. *
*                                                                 *
*  Copyright (c) Microsoft Corp.  All rights reserved.            *
*                                                                 *
******************************************************************/
#include "ntstatus.h"

#ifndef _NTINTSAFE_H_INCLUDED_
#define _NTINTSAFE_H_INCLUDED_

#if (_MSC_VER > 1000)
#pragma once
#endif

#include "specstrings.h"    // for __in, etc.

#if defined(_AMD64_)
#ifdef __cplusplus
extern "C" {
#endif

#define UnsignedMultiply128 _umul128

	ULONG64
		UnsignedMultiply128(
			__in ULONG64  Multiplier,
			__in ULONG64  Multiplicand,
			__out ULONG64* HighProduct
		);
#pragma intrinsic(_umul128)

#ifdef __cplusplus
}
#endif
#endif // _AMD64_

#if !defined(_W64)
#if !defined(__midl) && (defined(_X86_) || defined(_M_IX86)) && (_MSC_VER >= 1300)
#define _W64 __w64
#else
#define _W64
#endif
#endif

//
// typedefs
//
typedef char                CHAR; //-V677
typedef signed char         INT8; //-V677
typedef unsigned char       UCHAR;
typedef unsigned char       UINT8;
typedef unsigned char       BYTE;
typedef short               SHORT;
typedef signed short        INT16;
typedef unsigned short      USHORT;
typedef unsigned short      UINT16;
typedef unsigned short      WORD;
typedef int                 INT;
typedef signed int          INT32;
typedef unsigned int        UINT;
typedef unsigned int        UINT32;
typedef long                LONG;
typedef unsigned long       ULONG;
typedef unsigned long       DWORD;
typedef __int64             LONGLONG;
typedef __int64             LONG64;
typedef signed __int64      RtlINT64;
typedef unsigned __int64    ULONGLONG;
typedef unsigned __int64    DWORDLONG;
typedef unsigned __int64    ULONG64;
typedef unsigned __int64    DWORD64;
typedef unsigned __int64    UINT64;

#if (__midl > 501)
typedef [public] __int3264 INT_PTR;
typedef [public] unsigned __int3264 UINT_PTR;
typedef [public] __int3264 LONG_PTR;
typedef [public] unsigned __int3264 ULONG_PTR;
#else
#ifdef _WIN64
typedef __int64             INT_PTR;
typedef unsigned __int64    UINT_PTR;
typedef __int64             LONG_PTR;
typedef unsigned __int64    ULONG_PTR;
#else
typedef _W64 int            INT_PTR;
typedef _W64 unsigned int   UINT_PTR;
typedef _W64 long           LONG_PTR;
typedef _W64 unsigned long  ULONG_PTR;
#endif // WIN64
#endif // (__midl > 501)

#ifdef _WIN64
typedef __int64             ptrdiff_t;
typedef unsigned __int64    size_t;
#else
typedef _W64 int            ptrdiff_t;
typedef _W64 unsigned int   size_t;
#endif

typedef ULONG_PTR   DWORD_PTR;
typedef LONG_PTR    SSIZE_T;
typedef ULONG_PTR   SIZE_T;

typedef __success(return >= 0) long NTSTATUS;

#define NT_SUCCESS(Status)  (((NTSTATUS)(Status)) >= 0)

#define STATUS_SUCCESS  ((NTSTATUS)0x00000000L)
#ifndef SORTPP_PASS
// compiletime asserts (failure results in error C2118: negative subscript)
#define C_ASSERT(e) typedef char __C_ASSERT__[(e)?1:-1]
#else
#define C_ASSERT(e)
#endif

#define LOWORD(_dw)     ((WORD)(((DWORD_PTR)(_dw)) & 0xffff))
//#define HIWORD(_dw)     ((WORD)(((DWORD_PTR)(_dw)) >> 16))
#define LODWORD(_qw)    ((DWORD)(_qw))
#define HIDWORD(_qw)    ((DWORD)((_qw) >> 32))

#define HIWORD(l)           ((WORD)((((DWORD_PTR)(l)) >> 16) & 0xffff))

//
// UInt32x32To64 macro
//
#if defined(MIDL_PASS) || defined(RC_INVOKED) || defined(_M_CEE_PURE) \
    || defined(_68K_) || defined(_MPPC_) \
    || defined(_M_IA64) || defined(_M_AMD64)
#define UInt32x32To64(a, b) (((unsigned __int64)((unsigned int)(a))) * ((unsigned __int64)((unsigned int)(b))))
#elif defined(_M_IX86)
#define UInt32x32To64(a, b) ((unsigned __int64)(((unsigned __int64)((unsigned int)(a))) * ((unsigned int)(b))))
#else
#error Must define a target architecture.
#endif

//
// Min/Max type values
//
#define INT8_MIN        (-127i8 - 1)
#define SHORT_MIN       (-32768)
#define INT16_MIN       (-32767i16 - 1)
#define INT_MIN         (-2147483647 - 1)
#define INT32_MIN       (-2147483647i32 - 1)
#define LONG_MIN        (-2147483647L - 1)
#define LONGLONG_MIN    (-9223372036854775807i64 - 1)
#define LONG64_MIN      (-9223372036854775807i64 - 1)
#define INT64_MIN       (-9223372036854775807i64 - 1)
#define INT128_MIN      (-170141183460469231731687303715884105727i128 - 1)

#ifdef _WIN64
#define INT_PTR_MIN     (-9223372036854775807i64 - 1)
#define LONG_PTR_MIN    (-9223372036854775807i64 - 1)
#define PTRDIFF_T_MIN   (-9223372036854775807i64 - 1)
#define SSIZE_T_MIN     (-9223372036854775807i64 - 1)
#else
#define INT_PTR_MIN     (-2147483647 - 1)
#define LONG_PTR_MIN    (-2147483647L - 1)
#define PTRDIFF_T_MIN   (-2147483647 - 1)
#define SSIZE_T_MIN     (-2147483647L - 1)
#endif

#define INT8_MAX        127i8
#define UINT8_MAX       0xffui8
#define BYTE_MAX        0xff
#define SHORT_MAX       32767
#define INT16_MAX       32767i16
#define USHORT_MAX      0xffff
#define UINT16_MAX      0xffffui16
#define WORD_MAX        0xffff
#define INT_MAX         2147483647
#define INT32_MAX       2147483647i32
#define UINT_MAX        0xffffffff
#define UINT32_MAX      0xffffffffui32
#define LONG_MAX        2147483647L
#define ULONG_MAX       0xffffffffUL
#define DWORD_MAX       0xffffffffUL
#define LONGLONG_MAX    9223372036854775807i64
#define LONG64_MAX      9223372036854775807i64
#define INT64_MAX       9223372036854775807i64
#define ULONGLONG_MAX   0xffffffffffffffffui64
#define DWORDLONG_MAX   0xffffffffffffffffui64
#define ULONG64_MAX     0xffffffffffffffffui64
#define DWORD64_MAX     0xffffffffffffffffui64
#define UINT64_MAX      0xffffffffffffffffui64
#define INT128_MAX      170141183460469231731687303715884105727i128
#define UINT128_MAX     0xffffffffffffffffffffffffffffffffui128

#ifdef _WIN64
#define INT_PTR_MAX     9223372036854775807i64
#define UINT_PTR_MAX    0xffffffffffffffffui64
#define LONG_PTR_MAX    9223372036854775807i64
#define ULONG_PTR_MAX   0xffffffffffffffffui64
#define DWORD_PTR_MAX   0xffffffffffffffffui64
#define PTRDIFF_T_MAX   9223372036854775807i64
#define SIZE_T_MAX      0xffffffffffffffffui64
#define SSIZE_T_MAX     9223372036854775807i64
#define _SIZE_T_MAX     0xffffffffffffffffui64
#else
#define INT_PTR_MAX     2147483647
#define UINT_PTR_MAX    0xffffffff
#define LONG_PTR_MAX    2147483647L
#define ULONG_PTR_MAX   0xffffffffUL
#define DWORD_PTR_MAX   0xffffffffUL
#define PTRDIFF_T_MAX   2147483647
#define SIZE_T_MAX      0xffffffff
#define SSIZE_T_MAX     2147483647L
#define _SIZE_T_MAX     0xffffffffUL
#endif

//
// It is common for -1 to be used as an error value
//
#define INT8_ERROR      (-1i8)
#define UINT8_ERROR     0xffui8
#define BYTE_ERROR      0xff
#define SHORT_ERROR     (-1)
#define INT16_ERROR     (-1i16)
#define USHORT_ERROR    0xffff
#define UINT16_ERROR    0xffffui16
#define WORD_ERROR      0xffff
#define INT_ERROR       (-1)
#define INT32_ERROR     (-1i32)
#define UINT_ERROR      0xffffffff
#define UINT32_ERROR    0xffffffffui32
#define LONG_ERROR      (-1L)
#define ULONG_ERROR     0xffffffffUL
#define DWORD_ERROR     0xffffffffUL
#define LONGLONG_ERROR  (-1i64)
#define LONG64_ERROR    (-1i64)
#define INT64_ERROR     (-1i64)
#define ULONGLONG_ERROR 0xffffffffffffffffui64
#define DWORDLONG_ERROR 0xffffffffffffffffui64
#define ULONG64_ERROR   0xffffffffffffffffui64
#define UINT64_ERROR    0xffffffffffffffffui64

#ifdef _WIN64
#define INT_PTR_ERROR   (-1i64)
#define UINT_PTR_ERROR  0xffffffffffffffffui64
#define LONG_PTR_ERROR  (-1i64)
#define ULONG_PTR_ERROR 0xffffffffffffffffui64
#define DWORD_PTR_ERROR 0xffffffffffffffffui64
#define PTRDIFF_T_ERROR (-1i64)
#define SIZE_T_ERROR    0xffffffffffffffffui64
#define SSIZE_T_ERROR   (-1i64)
#define _SIZE_T_ERROR   0xffffffffffffffffui64
#else
#define INT_PTR_ERROR   (-1)
#define UINT_PTR_ERROR  0xffffffff
#define LONG_PTR_ERROR  (-1L)
#define ULONG_PTR_ERROR 0xffffffffUL
#define DWORD_PTR_ERROR 0xffffffffUL
#define PTRDIFF_T_ERROR (-1)
#define SIZE_T_ERROR    0xffffffff
#define SSIZE_T_ERROR   (-1L)
#define _SIZE_T_ERROR   0xffffffffUL
#endif

//
// We make some assumptions about the sizes of various types. Let's be
// explicit about those assumptions and check them.
//
C_ASSERT(sizeof(USHORT) == 2);
C_ASSERT(sizeof(INT) == 4);
C_ASSERT(sizeof(UINT) == 4);
C_ASSERT(sizeof(LONG) == 4);
C_ASSERT(sizeof(ULONG) == 4);
C_ASSERT(sizeof(UINT_PTR) == sizeof(ULONG_PTR));

//=============================================================================
// Conversion functions
//
// There are three reasons for having conversion functions:
//
// 1. We are converting from a signed type to an unsigned type of the same
//    size, or vice-versa.
//
//    Since we only have unsigned math functions, this allows people to convert
//    to unsigned, do the math, and then convert back to signed.
//
// 2. We are converting to a smaller type, and we could therefore possibly
//    overflow.
//
// 3. We are converting to a bigger type, and we are signed and the type we are
//    converting to is unsigned.
//
//=============================================================================

//
// INT8 -> UCHAR conversion
//
__inline
NTSTATUS
RtlInt8ToUChar(
	__in INT8 i8Operand,
	__out UCHAR * pch)
{
	NTSTATUS status;

	if (i8Operand >= 0)
	{
		*pch = (UCHAR)i8Operand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT8 -> UINT8 conversion
//
__inline
NTSTATUS
RtlInt8ToUInt8(
	__in INT8 i8Operand,
	__out UINT8 * pu8Result)
{
	NTSTATUS status;

	if (i8Operand >= 0)
	{
		*pu8Result = (UINT8)i8Operand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pu8Result = UINT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT8 -> BYTE conversion
//
#define RtlInt8ToByte  RtlInt8ToUInt8

//
// INT8 -> USHORT conversion
//
__inline
NTSTATUS
RtlInt8ToUShort(
	__in INT8 i8Operand,
	__out USHORT * pusResult)
{
	NTSTATUS status;

	if (i8Operand >= 0)
	{
		*pusResult = (USHORT)i8Operand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pusResult = USHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT8 -> UINT16 conversion
//
#define RtlInt8ToUInt16    RtlInt8ToUShort

//
// INT8 -> WORD conversion
//
#define RtlInt8ToWord  RtlInt8ToUShort

//
// INT8 -> UINT conversion
//
__inline
NTSTATUS
RtlInt8ToUInt(
	__in INT8 i8Operand,
	__out UINT * puResult)
{
	NTSTATUS status;

	if (i8Operand >= 0)
	{
		*puResult = (UINT)i8Operand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*puResult = UINT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT8 -> UINT32 conversion
//
#define RtlInt8ToUInt32    RtlInt8ToUInt

//
// INT8 -> UINT_PTR conversion
//
__inline
NTSTATUS
RtlInt8ToUIntPtr(
	__in INT8 i8Operand,
	__out UINT_PTR * puResult)
{
	NTSTATUS status;

	if (i8Operand >= 0)
	{
		*puResult = (UINT_PTR)i8Operand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*puResult = UINT_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT8 -> ULONG conversion
//
__inline
NTSTATUS
RtlInt8ToULong(
	__in INT8 i8Operand,
	__out ULONG * pulResult)
{
	NTSTATUS status;

	if (i8Operand >= 0)
	{
		*pulResult = (ULONG)i8Operand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pulResult = ULONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT8 -> ULONG_PTR conversion
//
__inline
NTSTATUS
RtlInt8ToULongPtr(
	__in INT8 i8Operand,
	__out ULONG_PTR * pulResult)
{
	NTSTATUS status;

	if (i8Operand >= 0)
	{
		*pulResult = (ULONG_PTR)i8Operand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pulResult = ULONG_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT8 -> DWORD conversion
//
#define RtlInt8ToDWord RtlInt8ToULong

//
// INT8 -> DWORD_PTR conversion
//
#define RtlInt8ToDWordPtr  RtlInt8ToULongPtr

//
// INT8 -> ULONGLONG conversion
//
__inline
NTSTATUS
RtlInt8ToULongLong(
	__in INT8 i8Operand,
	__out ULONGLONG * pullResult)
{
	NTSTATUS status;

	if (i8Operand >= 0)
	{
		*pullResult = (ULONGLONG)i8Operand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pullResult = ULONGLONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT8 -> DWORDLONG conversion
//
#define RtlInt8ToDWordLong RtlInt8ToULongLong

//
// INT8 -> ULONG64 conversion
//
#define RtlInt8ToULong64   RtlInt8ToULongLong

//
// INT8 -> DWORD64 conversion
//
#define RtlInt8ToDWord64   RtlInt8ToULongLong

//
// INT8 -> UINT64 conversion
//
#define RtlInt8ToUInt64    RtlInt8ToULongLong

//
// INT8 -> size_t conversion
//
#define RtlInt8ToSizeT RtlInt8ToUIntPtr

//
// INT8 -> SIZE_T conversion
//
#define RtlInt8ToSIZET RtlInt8ToULongPtr

//
// UINT8 -> CHAR conversion
//
#ifdef _CHAR_UNSIGNED
__forceinline
NTSTATUS
RtlUInt8ToChar(
	__in UINT8 u8Operand,
	__out CHAR * pch)
{
	*pch = (CHAR)u8Operand;
	return STATUS_SUCCESS;
}
#else
__inline
NTSTATUS
RtlUInt8ToChar(
	__in UINT8 u8Operand,
	__out CHAR * pch)
{
	NTSTATUS status;

	if (u8Operand <= 127)
	{
		*pch = (CHAR)u8Operand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif

//
// UINT8 -> INT8 conversion
//
__inline
NTSTATUS
RtlUInt8ToInt8(
	__in UINT8 u8Operand,
	__out INT8 * pi8Result)
{
	NTSTATUS status;

	if (u8Operand <= INT8_MAX)
	{
		*pi8Result = (INT8)u8Operand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pi8Result = INT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// BYTE -> CHAR conversion
//
#ifdef _CHAR_UNSIGNED
__forceinline
NTSTATUS
RtlByteToChar(
	__in BYTE bOperand,
	__out CHAR * pch)
{
	*pch = (CHAR)bOperand;
	return STATUS_SUCCESS;
}
#else
__inline
NTSTATUS
RtlByteToChar(
	__in BYTE bOperand,
	__out CHAR * pch)
{
	NTSTATUS status;

	if (bOperand <= 127)
	{
		*pch = (CHAR)bOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif

//
// BYTE -> INT8 conversion
//
__inline
NTSTATUS
RtlByteToInt8(
	__in BYTE bOperand,
	__out INT8 * pi8Result)
{
	NTSTATUS status;

	if (bOperand <= INT8_MAX)
	{
		*pi8Result = (INT8)bOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pi8Result = INT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// SHORT -> INT8 conversion
//
__inline
NTSTATUS
RtlShortToInt8(
	__in SHORT sOperand,
	__out INT8 * pi8Result)
{
	NTSTATUS status;

	if ((sOperand >= INT8_MIN) && (sOperand <= INT8_MAX))
	{
		*pi8Result = (INT8)sOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pi8Result = INT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// SHORT -> UCHAR conversion
//
__inline
NTSTATUS
RtlShortToUChar(
	__in SHORT sOperand,
	__out UCHAR * pch)
{
	NTSTATUS status;

	if ((sOperand >= 0) && (sOperand <= 255))
	{
		*pch = (UCHAR)sOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// SHORT -> CHAR conversion
//
#ifdef _CHAR_UNSIGNED
__forceinline
NTSTATUS
RtlShortToChar(
	__in SHORT sOperand,
	__out CHAR * pch)
{
	return RtlShortToUChar(sOperand, (UCHAR*)pch);
}
#else
__inline
NTSTATUS
RtlShortToChar(
	__in SHORT sOperand,
	__out CHAR * pch)
{
	NTSTATUS status;

	if ((sOperand >= -128) && (sOperand <= 127))
	{
		*pch = (CHAR)sOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif // _CHAR_UNSIGNED

//
// SHORT -> UINT8 conversion
//
__inline
NTSTATUS
RtlShortToUInt8(
	__in SHORT sOperand,
	__out UINT8 * pui8Result)
{
	NTSTATUS status;

	if ((sOperand >= 0) && (sOperand <= UINT8_MAX))
	{
		*pui8Result = (UINT8)sOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pui8Result = UINT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// SHORT -> BYTE conversion
//
#define RtlShortToByte  RtlShortToUInt8

//
// SHORT -> USHORT conversion
//
__inline
NTSTATUS
RtlShortToUShort(
	__in SHORT sOperand,
	__out USHORT * pusResult)
{
	NTSTATUS status;

	if (sOperand >= 0)
	{
		*pusResult = (USHORT)sOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pusResult = USHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// SHORT -> UINT16 conversion
//
#define RtlShortToUInt16   RtlShortToUShort

//
// SHORT -> WORD conversion
//
#define RtlShortToWord RtlShortToUShort

//
// SHORT -> UINT conversion
//
__inline
NTSTATUS
RtlShortToUInt(
	__in SHORT sOperand,
	__out UINT * puResult)
{
	NTSTATUS status;

	if (sOperand >= 0)
	{
		*puResult = (UINT)sOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*puResult = UINT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// SHORT -> UINT32 conversion
//
#define RtlShortToUInt32   RtlShortToUInt

//
// SHORT -> UINT_PTR conversion
//
__inline
NTSTATUS
RtlShortToUIntPtr(
	__in SHORT sOperand,
	__out UINT_PTR * puResult)
{
	NTSTATUS status;

	if (sOperand >= 0)
	{
		*puResult = (UINT_PTR)sOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*puResult = UINT_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// SHORT -> ULONG conversion
//
__inline
NTSTATUS
RtlShortToULong(
	__in SHORT sOperand,
	__out ULONG * pulResult)
{
	NTSTATUS status;

	if (sOperand >= 0)
	{
		*pulResult = (ULONG)sOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pulResult = ULONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// SHORT -> ULONG_PTR conversion
//
__inline
NTSTATUS
RtlShortToULongPtr(
	__in SHORT sOperand,
	__out ULONG_PTR * pulResult)
{
	NTSTATUS status;

	if (sOperand >= 0)
	{
		*pulResult = (ULONG_PTR)sOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pulResult = ULONG_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// SHORT -> DWORD conversion
//
#define RtlShortToDWord    RtlShortToULong

//
// SHORT -> DWORD_PTR conversion
//
__inline
NTSTATUS
RtlShortToDWordPtr(
	__in SHORT sOperand,
	__out DWORD_PTR * pdwResult)
{
	NTSTATUS status;

	if (sOperand >= 0)
	{
		*pdwResult = (DWORD_PTR)sOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pdwResult = DWORD_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// SHORT -> ULONGLONG conversion
//
__inline
NTSTATUS
RtlShortToULongLong(
	__in SHORT sOperand,
	__out ULONGLONG * pullResult)
{
	NTSTATUS status;

	if (sOperand >= 0)
	{
		*pullResult = (ULONGLONG)sOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pullResult = ULONGLONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// SHORT -> DWORDLONG conversion
//
#define RtlShortToDWordLong    RtlShortToULongLong

//
// SHORT -> ULONG64 conversion
//
#define RtlShortToULong64  RtlShortToULongLong

//
// SHORT -> DWORD64 conversion
//
#define RtlShortToDWord64  RtlShortToULongLong

//
// SHORT -> UINT64 conversion
//
#define RtlShortToUInt64   RtlShortToULongLong

//
// SHORT -> size_t conversion
//
#define RtlShortToSizeT    RtlShortToUIntPtr

//
// SHORT -> SIZE_T conversion
//
#define RtlShortToSIZET    RtlShortToULongPtr

//
// INT16 -> CHAR conversion
//
#define RtlInt16ToChar RtlShortToChar

//
// INT16 -> INT8 conversion
//
#define RtlInt16ToInt8 RtlShortToInt8

//
// INT16 -> UCHAR conversion
//
#define RtlInt16ToUChar    RtlShortToUChar

//
// INT16 -> UINT8 conversion
//
#define RtlInt16ToUInt8    RtlShortToUInt8

//
// INT16 -> BYTE conversion
//
#define RtlInt16ToByte RtlShortToUInt8

//
// INT16 -> USHORT conversion
//
#define RtlInt16ToUShort   RtlShortToUShort

//
// INT16 -> UINT16 conversion
//
#define RtlInt16ToUInt16   RtlShortToUShort

//
// INT16 -> WORD conversion
//
#define RtlInt16ToWord RtlShortToUShort

//
// INT16 -> UINT conversion
//
#define RtlInt16ToUInt RtlShortToUInt

//
// INT16 -> UINT32 conversion
//
#define RtlInt16ToUInt32   RtlShortToUInt

//
// INT16 -> UINT_PTR conversion
//
#define RtlInt16ToUIntPtr  RtlShortToUIntPtr

//
// INT16 -> ULONG conversion
//
#define RtlInt16ToULong    RtlShortToULong

//
// INT16 -> ULONG_PTR conversion
//
#define RtlInt16ToULongPtr RtlShortToULongPtr

//
// INT16 -> DWORD conversion
//
#define RtlInt16ToDWord    RtlShortToULong

//
// INT16 -> DWORD_PTR conversion
//
#define RtlInt16ToDWordPtr RtlShortToULongPtr

//
// INT16 -> ULONGLONG conversion
//
#define RtlInt16ToULongLong    RtlShortToULongLong

//
// INT16 -> DWORDLONG conversion
//
#define RtlInt16ToDWordLong    RtlShortToULongLong

//
// INT16 -> ULONG64 conversion
//
#define RtlInt16ToULong64  RtlShortToULongLong

//
// INT16 -> DWORD64 conversion
//
#define RtlInt16ToDWord64  RtlShortToULongLong

//
// INT16 -> UINT64 conversion
//
#define RtlInt16ToUInt64   RtlShortToULongLong

//
// INT16 -> size_t conversion
//
#define RtlInt16ToSizeT    RtlShortToUIntPtr

//
// INT16 -> SIZE_T conversion
//
#define RtlInt16ToSIZET    RtlShortToULongPtr

//
// USHORT -> INT8 conversion
//
__inline
NTSTATUS
RtlUShortToInt8(
	__in USHORT usOperand,
	__out INT8 * pi8Result)
{
	NTSTATUS status;

	if (usOperand <= INT8_MAX)
	{
		*pi8Result = (INT8)usOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pi8Result = INT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// USHORT -> UCHAR conversion
//
__inline
NTSTATUS
RtlUShortToUChar(
	__in USHORT usOperand,
	__out UCHAR * pch)
{
	NTSTATUS status;

	if (usOperand <= 255)
	{
		*pch = (UCHAR)usOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// USHORT -> CHAR conversion
//
#ifdef _CHAR_UNSIGNED
__forceinline
NTSTATUS
RtlUShortToChar(
	__in USHORT usOperand,
	__out CHAR * pch)
{
	return RtlUShortToUChar(usOperand, (UCHAR*)pch);
}
#else
__inline
NTSTATUS
RtlUShortToChar(
	__in USHORT usOperand,
	__out CHAR * pch)
{
	NTSTATUS status;

	if (usOperand <= 127)
	{
		*pch = (CHAR)usOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif // _CHAR_UNSIGNED

//
// USHORT -> UINT8 conversion
//
__inline
NTSTATUS
RtlUShortToUInt8(
	__in USHORT usOperand,
	__out UINT8 * pui8Result)
{
	NTSTATUS status;

	if (usOperand <= UINT8_MAX)
	{
		*pui8Result = (UINT8)usOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pui8Result = UINT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// USHORT -> BYTE conversion
//
#define RtlUShortToByte    RtlUShortToUInt8

//
// USHORT -> SHORT conversion
//
__inline
NTSTATUS
RtlUShortToShort(
	__in USHORT usOperand,
	__out SHORT * psResult)
{
	NTSTATUS status;

	if (usOperand <= SHORT_MAX)
	{
		*psResult = (SHORT)usOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*psResult = SHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// USHORT -> INT16 conversion
//
#define RtlUShortToInt16   RtlUShortToShort

//
// UINT16 -> CHAR conversion
//
#define RtlUInt16ToChar    RtlUShortToChar

//
// UINT16 -> INT8 conversion
//
#define RtlUInt16ToInt8    RtlUShortToInt8

//
// UINT16 -> UCHAR conversion
//
#define RtlUInt16ToUChar   RtlUShortToUChar

//
// UINT16 -> UINT8 conversion
//
#define RtlUInt16ToUInt8   RtlUShortToUInt8

//
// UINT16 -> BYTE conversion
//
#define RtlUInt16ToByte    RtlUShortToUInt8

//
// UINT16 -> SHORT conversion
//
#define RtlUInt16ToShort   RtlUShortToShort

//
// UINT16 -> INT16 conversion
//
#define RtlUInt16ToInt16   RtlUShortToShort

//
// WORD -> INT8 conversion
//
#define RtlWordToInt8  RtlUShortToInt8

//
// WORD -> CHAR conversion
//
#define RtlWordToChar  RtlUShortToChar

//
// WORD -> UCHAR conversion
//
#define RtlWordToUChar RtlUShortToUChar

//
// WORD -> UINT8 conversion
//
#define RtlWordToUInt8 RtlUShortToUInt8

//
// WORD -> BYTE conversion
//
#define RtlWordToByte  RtlUShortToUInt8

//
// WORD -> SHORT conversion
//
#define RtlWordToShort RtlUShortToShort

//
// WORD -> INT16 conversion
//
#define RtlWordToInt16 RtlUShortToShort

//
// INT -> INT8 conversion
//
__inline
NTSTATUS
RtlIntToInt8(
	__in INT iOperand,
	__out INT8 * pi8Result)
{
	NTSTATUS status;

	if ((iOperand >= INT8_MIN) && (iOperand <= INT8_MAX))
	{
		*pi8Result = (INT8)iOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pi8Result = INT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT -> UCHAR conversion
//
__inline
NTSTATUS
RtlIntToUChar(
	__in INT iOperand,
	__out UCHAR * pch)
{
	NTSTATUS status;

	if ((iOperand >= 0) && (iOperand <= 255))
	{
		*pch = (UCHAR)iOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT -> CHAR conversion
//
#ifdef _CHAR_UNSIGNED
__forceinline
NTSTATUS
RtlIntToChar(
	__in INT iOperand,
	__out CHAR * pch)
{
	return RtlIntToUChar(iOperand, (UCHAR*)pch);
}
#else
__inline
NTSTATUS
RtlIntToChar(
	__in INT iOperand,
	__out CHAR * pch)
{
	NTSTATUS status;

	if ((iOperand >= -128) && (iOperand <= 127))
	{
		*pch = (CHAR)iOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif // _CHAR_UNSIGNED

//
// INT -> BYTE conversion
//
#define RtlIntToByte   RtlIntToUInt8

//
// INT -> UINT8 conversion
//
__inline
NTSTATUS
RtlIntToUInt8(
	__in INT iOperand,
	__out UINT8 * pui8Result)
{
	NTSTATUS status;

	if ((iOperand >= 0) && (iOperand <= UINT8_MAX))
	{
		*pui8Result = (UINT8)iOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pui8Result = UINT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT -> SHORT conversion
//
__inline
NTSTATUS
RtlIntToShort(
	__in INT iOperand,
	__out SHORT * psResult)
{
	NTSTATUS status;

	if ((iOperand >= SHORT_MIN) && (iOperand <= SHORT_MAX))
	{
		*psResult = (SHORT)iOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*psResult = SHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT -> INT16 conversion
//
#define RtlIntToInt16  RtlIntToShort

//
// INT -> USHORT conversion
//
__inline
NTSTATUS
RtlIntToUShort(
	__in INT iOperand,
	__out USHORT * pusResult)
{
	NTSTATUS status;

	if ((iOperand >= 0) && (iOperand <= USHORT_MAX))
	{
		*pusResult = (USHORT)iOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pusResult = USHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT -> UINT16 conversion
//
#define RtlIntToUInt16  RtlIntToUShort

//
// INT -> WORD conversion
//
#define RtlIntToWord   RtlIntToUShort

//
// INT -> UINT conversion
//
__inline
NTSTATUS
RtlIntToUInt(
	__in INT iOperand,
	__out UINT * puResult)
{
	NTSTATUS status;

	if (iOperand >= 0)
	{
		*puResult = (UINT)iOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*puResult = UINT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT -> UINT_PTR conversion
//
#ifdef _WIN64
#define RtlIntToUIntPtr    RtlIntToULongLong
#else
#define RtlIntToUIntPtr    RtlIntToUInt
#endif

//
// INT -> ULONG conversion
//
__inline
NTSTATUS
RtlIntToULong(
	__in INT iOperand,
	__out ULONG * pulResult)
{
	NTSTATUS status;

	if (iOperand >= 0)
	{
		*pulResult = (ULONG)iOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pulResult = ULONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT -> ULONG_PTR conversion
//
#ifdef _WIN64
#define RtlIntToULongPtr   RtlIntToULongLong
#else
#define RtlIntToULongPtr   RtlIntToULong
#endif

//
// INT -> DWORD conversion
//
#define RtlIntToDWord  RtlIntToULong

//
// INT -> DWORD_PTR conversion
//
#define RtlIntToDWordPtr   RtlIntToULongPtr

//
// INT -> ULONGLONG conversion
//
__inline
NTSTATUS
RtlIntToULongLong(
	__in INT iOperand,
	__out ULONGLONG * pullResult)
{
	NTSTATUS status;

	if (iOperand >= 0)
	{
		*pullResult = (ULONGLONG)iOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pullResult = ULONGLONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT -> DWORDLONG conversion
//
#define RtlIntToDWordLong  RtlIntToULongLong

//
// INT -> ULONG64 conversion
//
#define RtlIntToULong64    RtlIntToULongLong

//
// INT -> DWORD64 conversion
//
#define RtlIntToDWord64    RtlIntToULongLong

//
// INT -> UINT64 conversion
//
#define RtlIntToUInt64 RtlIntToULongLong

//
// INT -> size_t conversion
//
#define RtlIntToSizeT  RtlIntToUIntPtr

//
// INT -> SIZE_T conversion
//
#define RtlIntToSIZET  RtlIntToULongPtr

//
// INT32 -> CHAR conversion
//
#define RtlInt32ToChar RtlIntToChar

//
// INT32 -> INT328 conversion
//
#define RtlInt32ToInt8 RtlIntToInt8

//
// INT32 -> UCHAR conversion
//
#define RtlInt32ToUChar    RtlIntToUChar

//
// INT32 -> BYTE conversion
//
#define RtlInt32ToByte RtlIntToUInt8

//
// INT32 -> UINT8 conversion
//
#define RtlInt32ToUInt8    RtlIntToUInt8

//
// INT32 -> SHORT conversion
//
#define RtlInt32ToShort    RtlIntToShort

//
// INT32 -> INT16 conversion
//
#define RtlInt32ToInt16    RtlIntToShort

//
// INT32 -> USHORT conversion
//
#define RtlInt32ToUShort   RtlIntToUShort

//
// INT32 -> UINT16 conversion
//
#define RtlInt32ToUInt16   RtlIntToUShort

//
// INT32 -> WORD conversion
//
#define RtlInt32ToWord RtlIntToUShort

//
// INT32 -> UINT conversion
//
#define RtlInt32ToUInt RtlIntToUInt

//
// INT32 -> UINT32 conversion
//
#define RtlInt32ToUInt32   RtlIntToUInt

//
// INT32 -> UINT_PTR conversion
//
#define RtlInt32ToUIntPtr  RtlIntToUIntPtr

//
// INT32 -> ULONG conversion
//
#define RtlInt32ToULong    RtlIntToULong

//
// INT32 -> ULONG_PTR conversion
//
#define RtlInt32ToULongPtr RtlIntToULongPtr

//
// INT32 -> DWORD conversion
//
#define RtlInt32ToDWord    RtlIntToULong

//
// INT32 -> DWORD_PTR conversion
//
#define RtlInt32ToDWordPtr RtlIntToULongPtr

//
// INT32 -> ULONGLONG conversion
//
#define RtlInt32ToULongLong    RtlIntToULongLong

//
// INT32 -> DWORDLONG conversion
//
#define RtlInt32ToDWordLong    RtlIntToULongLong

//
// INT32 -> ULONG64 conversion
//
#define RtlInt32ToULong64  RtlIntToULongLong

//
// INT32 -> DWORD64 conversion
//
#define RtlInt32ToDWord64  RtlIntToULongLong

//
// INT32 -> UINT64 conversion
//
#define RtlInt32ToUInt64   RtlIntToULongLong

//
// INT32 -> size_t conversion
//
#define RtlInt32ToSizeT    RtlIntToUIntPtr

//
// INT32 -> SIZE_T conversion
//
#define RtlInt32ToSIZET    RtlIntToULongPtr

//
// INT_PTR -> INT8 conversion
//
__inline
NTSTATUS
RtlIntPtrToInt8(
	__in INT_PTR iOperand,
	__out INT8 * pi8Result)
{
	NTSTATUS status;

	if ((iOperand >= INT8_MIN) && (iOperand <= INT8_MAX))
	{
		*pi8Result = (INT8)iOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pi8Result = INT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT_PTR -> UCHAR conversion
//
__inline
NTSTATUS
RtlIntPtrToUChar(
	__in INT_PTR iOperand,
	__out UCHAR * pch)
{
	NTSTATUS status;

	if ((iOperand >= 0) && (iOperand <= 255))
	{
		*pch = (UCHAR)iOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT_PTR -> CHAR conversion
//
#ifdef _CHAR_UNSIGNED
__forceinline
NTSTATUS
RtlIntPtrToChar(
	__in INT_PTR iOperand,
	__out CHAR * pch)
{
	return RtlIntPtrToUChar(iOperand, (UCHAR*)pch);
}
#else
__inline
NTSTATUS
RtlIntPtrToChar(
	__in INT_PTR iOperand,
	__out CHAR * pch)
{
	NTSTATUS status;

	if ((iOperand >= -128) && (iOperand <= 127))
	{
		*pch = (CHAR)iOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif // _CHAR_UNSIGNED

//
// INT_PTR -> UINT8 conversion
//
__inline
NTSTATUS
RtlIntPtrToUInt8(
	__in INT_PTR iOperand,
	__out UINT8 * pui8Result)
{
	NTSTATUS status;

	if ((iOperand >= 0) && (iOperand <= UINT8_MAX))
	{
		*pui8Result = (UINT8)iOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pui8Result = UINT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT_PTR -> BYTE conversion
//
#define RtlIntPtrToByte    RtlIntPtrToUInt8

//
// INT_PTR -> SHORT conversion
//
__inline
NTSTATUS
RtlIntPtrToShort(
	__in INT_PTR iOperand,
	__out SHORT * psResult)
{
	NTSTATUS status;

	if ((iOperand >= SHORT_MIN) && (iOperand <= SHORT_MAX))
	{
		*psResult = (SHORT)iOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*psResult = SHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT_PTR -> INT16 conversion
//
#define RtlIntPtrToInt16   RtlIntPtrToShort

//
// INT_PTR -> USHORT conversion
//
__inline
NTSTATUS
RtlIntPtrToUShort(
	__in INT_PTR iOperand,
	__out USHORT * pusResult)
{
	NTSTATUS status;

	if ((iOperand >= 0) && (iOperand <= USHORT_MAX))
	{
		*pusResult = (USHORT)iOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pusResult = USHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// INT_PTR -> UINT16 conversion
//
#define RtlIntPtrToUInt16  RtlIntPtrToUShort

//
// INT_PTR -> WORD conversion
//
#define RtlIntPtrToWord    RtlIntPtrToUShort

//
// INT_PTR -> INT conversion
//
#ifdef _WIN64
#define RtlIntPtrToInt RtlLongLongToInt
#else
__inline
NTSTATUS
RtlIntPtrToInt(
	__in INT_PTR iOperand,
	__out INT * piResult)
{
	*piResult = (INT)iOperand;
	return STATUS_SUCCESS;
}
#endif

//
// INT_PTR -> INT32 conversion
//
#define RtlIntPtrToInt32   RtlIntPtrToInt

//
// INT_PTR -> UINT conversion
//
#ifdef _WIN64
#define RtlIntPtrToUInt    RtlLongLongToUInt
#else
__inline
NTSTATUS
RtlIntPtrToUInt(
	__in INT_PTR iOperand,
	__out UINT * puResult)
{
	NTSTATUS status;

	if (iOperand >= 0)
	{
		*puResult = (UINT)iOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*puResult = UINT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif

//
// INT_PTR -> UINT32 conversion
//
#define RtlIntPtrToUInt32  RtlIntPtrToUInt

//
// INT_PTR -> UINT_PTR conversion
//
#ifdef _WIN64
#define RtlIntPtrToUIntPtr RtlLongLongToULongLong
#else
__inline
NTSTATUS
RtlIntPtrToUIntPtr(
	__in INT_PTR iOperand,
	__out UINT_PTR * puResult)
{
	NTSTATUS status;

	if (iOperand >= 0)
	{
		*puResult = (UINT_PTR)iOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*puResult = UINT_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif

//
// INT_PTR -> LONG conversion
//
#ifdef _WIN64
#define RtlIntPtrToLong    RtlLongLongToLong
#else
__inline
NTSTATUS
RtlIntPtrToLong(
	__in INT_PTR iOperand,
	__out LONG * plResult)
{
	*plResult = (LONG)iOperand;
	return STATUS_SUCCESS;
}
#endif

//
// INT_PTR -> LONG_PTR conversion
//
__inline
NTSTATUS
RtlIntPtrToLongPtr(
	__in INT_PTR iOperand,
	__out LONG_PTR * plResult)
{
	*plResult = (LONG_PTR)iOperand;
	return STATUS_SUCCESS;
}

//
// INT_PTR -> ULONG conversion
//
#ifdef _WIN64
#define RtlIntPtrToULong   RtlLongLongToULong
#else
__inline
NTSTATUS
RtlIntPtrToULong(
	__in INT_PTR iOperand,
	__out ULONG * pulResult)
{
	NTSTATUS status;

	if (iOperand >= 0)
	{
		*pulResult = (ULONG)iOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pulResult = ULONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif

//
// INT_PTR -> ULONG_PTR conversion
//
#ifdef _WIN64
#define RtlIntPtrToULongPtr    RtlLongLongToULongLong
#else
__inline
NTSTATUS
RtlIntPtrToULongPtr(
	__in INT_PTR iOperand,
	__out ULONG_PTR * pulResult)
{
	NTSTATUS status;

	if (iOperand >= 0)
	{
		*pulResult = (ULONG_PTR)iOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pulResult = ULONG_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif

//
// INT_PTR -> DWORD conversion
//
#define RtlIntPtrToDWord   RtlIntPtrToULong

//
// INT_PTR -> DWORD_PTR conversion
//
#define RtlIntPtrToDWordPtr    RtlIntPtrToULongPtr

//
// INT_PTR -> ULONGLONG conversion
//
#ifdef _WIN64
#define RtlIntPtrToULongLong   RtlLongLongToULongLong
#else
__inline
NTSTATUS
RtlIntPtrToULongLong(
	__in INT_PTR iOperand,
	__out ULONGLONG * pullResult)
{
	NTSTATUS status;

	if (iOperand >= 0)
	{
		*pullResult = (ULONGLONG)iOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pullResult = ULONGLONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif

//
// INT_PTR -> DWORDLONG conversion
//
#define RtlIntPtrToDWordLong   RtlIntPtrToULongLong

//
// INT_PTR -> ULONG64 conversion
//
#define RtlIntPtrToULong64 RtlIntPtrToULongLong

//
// INT_PTR -> DWORD64 conversion
//
#define RtlIntPtrToDWord64 RtlIntPtrToULongLong

//
// INT_PTR -> UINT64 conversion
//
#define RtlIntPtrToUInt64  RtlIntPtrToULongLong

//
// INT_PTR -> size_t conversion
//
#define RtlIntPtrToSizeT   RtlIntPtrToUIntPtr

//
// INT_PTR -> SIZE_T conversion
//
#define RtlIntPtrToSIZET   RtlIntPtrToULongPtr

//
// UINT -> INT8 conversion
//
__inline
NTSTATUS
RtlUIntToInt8(
	__in UINT uOperand,
	__out INT8 * pi8Result)
{
	NTSTATUS status;

	if (uOperand <= INT8_MAX)
	{
		*pi8Result = (INT8)uOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pi8Result = INT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT -> UCHAR conversion
//
__inline
NTSTATUS
RtlUIntToUChar(
	__in UINT uOperand,
	__out UCHAR * pch)
{
	NTSTATUS status;

	if (uOperand <= 255)
	{
		*pch = (UCHAR)uOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT -> CHAR conversion
//
#ifdef _CHAR_UNSIGNED
__forceinline
NTSTATUS
RtlUIntToChar(
	__in UINT uOperand,
	__out CHAR * pch)
{
	return RtlUIntToUChar(uOperand, (UCHAR*)pch);
}
#else
__inline
NTSTATUS
RtlUIntToChar(
	__in UINT uOperand,
	__out CHAR * pch)
{
	NTSTATUS status;

	if (uOperand <= 127)
	{
		*pch = (CHAR)uOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif // _CHAR_UNSIGNED

//
// UINT -> UINT8 conversion
//
__inline
NTSTATUS
RtlUIntToUInt8(
	__in UINT uOperand,
	__out UINT8 * pui8Result)
{
	NTSTATUS status;

	if (uOperand <= UINT8_MAX)
	{
		*pui8Result = (UINT8)uOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pui8Result = UINT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT -> BYTE conversion
//
#define RtlUIntToByte   RtlUIntToUInt8

//
// UINT -> SHORT conversion
//
__inline
NTSTATUS
RtlUIntToShort(
	__in UINT uOperand,
	__out SHORT * psResult)
{
	NTSTATUS status;

	if (uOperand <= SHORT_MAX)
	{
		*psResult = (SHORT)uOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*psResult = SHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT -> INT16 conversion
//
#define RtlUIntToInt16 RtlUIntToShort

//
// UINT -> USHORT conversion
//
__inline
NTSTATUS
RtlUIntToUShort(
	__in UINT uOperand,
	__out USHORT * pusResult)
{
	NTSTATUS status;

	if (uOperand <= USHORT_MAX)
	{
		*pusResult = (USHORT)uOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pusResult = USHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT -> UINT16 conversion
//
#define RtlUIntToUInt16    RtlUIntToUShort

//
// UINT -> WORD conversion
//
#define RtlUIntToWord  RtlUIntToUShort

//
// UINT -> INT conversion
//
__inline
NTSTATUS
RtlUIntToInt(
	__in UINT uOperand,
	__out INT * piResult)
{
	NTSTATUS status;

	if (uOperand <= INT_MAX)
	{
		*piResult = (INT)uOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*piResult = INT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT -> INT32 conversion
//
#define RtlUIntToInt32 RtlUIntToInt

//
// UINT -> INT_PTR conversion
//
#ifdef _WIN64
__inline
NTSTATUS
RtlUIntToIntPtr(
	__in UINT uOperand,
	__out INT_PTR * piResult)
{
	*piResult = uOperand;
	return STATUS_SUCCESS;
}
#else
#define RtlUIntToIntPtr    RtlUIntToInt
#endif

//
// UINT -> LONG conversion
//
__inline
NTSTATUS
RtlUIntToLong(
	__in UINT uOperand,
	__out LONG * plResult)
{
	NTSTATUS status;

	if (uOperand <= LONG_MAX) //-V547
	{
		*plResult = (LONG)uOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*plResult = LONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT -> LONG_PTR conversion
//
#ifdef _WIN64
__inline
NTSTATUS
RtlUIntToLongPtr(
	__in UINT uOperand,
	__out LONG_PTR * plResult)
{
	*plResult = uOperand;
	return STATUS_SUCCESS;
}
#else
#define RtlUIntToLongPtr   RtlUIntToLong
#endif

//
// UINT -> ptrdiff_t conversion
//
#define RtlUIntToPtrdiffT  RtlUIntToIntPtr

//
// UINT -> SSIZE_T conversion
//
#define RtlUIntToSSIZET    RtlUIntToLongPtr

//
// UINT32 -> CHAR conversion
//
#define RtlUInt32ToChar    RtlUIntToChar

//
// UINT32 -> INT8 conversion
//
#define RtlUInt32ToInt8    RtlUIntToInt8

//
// UINT32 -> UCHAR conversion
//
#define RtlUInt32ToUChar   RtlUIntToUChar

//
// UINT32 -> UINT8 conversion
//
#define RtlUInt32ToUInt8   RtlUIntToUInt8

//
// UINT32 -> BYTE conversion
//
#define RtlUInt32ToByte    RtlUInt32ToUInt8

//
// UINT32 -> SHORT conversion
//
#define RtlUInt32ToShort   RtlUIntToShort

//
// UINT32 -> INT16 conversion
//
#define RtlUInt32ToInt16   RtlUIntToShort

//
// UINT32 -> USHORT conversion
//
#define RtlUInt32ToUShort  RtlUIntToUShort

//
// UINT32 -> UINT16 conversion
//
#define RtlUInt32ToUInt16  RtlUIntToUShort

//
// UINT32 -> WORD conversion
//
#define RtlUInt32ToWord    RtlUIntToUShort

//
// UINT32 -> INT conversion
//
#define RtlUInt32ToInt RtlUIntToInt

//
// UINT32 -> INT_PTR conversion
//
#define RtlUInt32ToIntPtr  RtlUIntToIntPtr

//
// UINT32 -> INT32 conversion
//
#define RtlUInt32ToInt32   RtlUIntToInt

//
// UINT32 -> LONG conversion
//
#define RtlUInt32ToLong    RtlUIntToLong

//
// UINT32 -> LONG_PTR conversion
//
#define RtlUInt32ToLongPtr RtlUIntToLongPtr

//
// UINT32 -> ptrdiff_t conversion
//
#define RtlUInt32ToPtrdiffT    RtlUIntToPtrdiffT

//
// UINT32 -> SSIZE_T conversion
//
#define RtlUInt32ToSSIZET  RtlUIntToSSIZET

//
// UINT_PTR -> INT8 conversion
//
__inline
NTSTATUS
RtlUIntPtrToInt8(
	__in UINT_PTR uOperand,
	__out INT8 * pi8Result)
{
	NTSTATUS status;

	if (uOperand <= INT8_MAX)
	{
		*pi8Result = (INT8)uOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pi8Result = INT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT_PTR -> UCHAR conversion
//
__inline
NTSTATUS
RtlUIntPtrToUChar(
	__in UINT_PTR uOperand,
	__out UCHAR * pch)
{
	NTSTATUS status;

	if (uOperand <= 255)
	{
		*pch = (UCHAR)uOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT_PTR -> CHAR conversion
//
#ifdef _CHAR_UNSIGNED
__forceinline
NTSTATUS
RtlUIntPtrToChar(
	__in UINT_PTR uOperand,
	__out CHAR * pch)
{
	return RtlUIntPtrToUChar(uOperand, (UCHAR*)pch);
}
#else
__inline
NTSTATUS
RtlUIntPtrToChar(
	__in UINT_PTR uOperand,
	__out CHAR * pch)
{
	NTSTATUS status;

	if (uOperand <= 127)
	{
		*pch = (CHAR)uOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif // _CHAR_UNSIGNED

//
// UINT_PTR -> UINT8 conversion
//
__inline
NTSTATUS
RtlUIntPtrToUInt8(
	__in UINT_PTR uOperand,
	__out UINT8 * pu8Result)
{
	NTSTATUS status;

	if (uOperand <= UINT8_MAX)
	{
		*pu8Result = (UINT8)uOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pu8Result = UINT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT_PTR -> BYTE conversion
//
#define RtlUIntPtrToByte   RtlUIntPtrToUInt8

//
// UINT_PTR -> SHORT conversion
//
__inline
NTSTATUS
RtlUIntPtrToShort(
	__in UINT_PTR uOperand,
	__out SHORT * psResult)
{
	NTSTATUS status;

	if (uOperand <= SHORT_MAX)
	{
		*psResult = (SHORT)uOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*psResult = SHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT_PTR -> INT16 conversion
//
__inline
NTSTATUS
RtlUIntPtrToInt16(
	__in UINT_PTR uOperand,
	__out INT16 * pi16Result)
{
	NTSTATUS status;

	if (uOperand <= INT16_MAX)
	{
		*pi16Result = (INT16)uOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pi16Result = INT16_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT_PTR -> USHORT conversion
//
__inline
NTSTATUS
RtlUIntPtrToUShort(
	__in UINT_PTR uOperand,
	__out USHORT * pusResult)
{
	NTSTATUS status;

	if (uOperand <= USHORT_MAX)
	{
		*pusResult = (USHORT)uOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pusResult = USHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT_PTR -> UINT16 conversion
//
__inline
NTSTATUS
RtlUIntPtrToUInt16(
	__in UINT_PTR uOperand,
	__out UINT16 * pu16Result)
{
	NTSTATUS status;

	if (uOperand <= UINT16_MAX)
	{
		*pu16Result = (UINT16)uOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pu16Result = UINT16_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT_PTR -> WORD conversion
//
#define RtlUIntPtrToWord   RtlUIntPtrToUShort

//
// UINT_PTR -> INT conversion
//
__inline
NTSTATUS
RtlUIntPtrToInt(
	__in UINT_PTR uOperand,
	__out INT * piResult)
{
	NTSTATUS status;

	if (uOperand <= INT_MAX)
	{
		*piResult = (INT)uOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*piResult = INT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT_PTR -> INT32 conversion
//
#define RtlUIntPtrToInt32  RtlUIntPtrToInt

//
// UINT_PTR -> INT_PTR conversion
//
__inline
NTSTATUS
RtlUIntPtrToIntPtr(
	__in UINT_PTR uOperand,
	__out INT_PTR * piResult)
{
	NTSTATUS status;

	if (uOperand <= INT_PTR_MAX) //-V547
	{
		*piResult = (INT_PTR)uOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*piResult = INT_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT_PTR -> UINT conversion
//
#ifdef _WIN64
#define RtlUIntPtrToUInt   RtlULongLongToUInt
#else
__inline
NTSTATUS
RtlUIntPtrToUInt(
	__in UINT_PTR uOperand,
	__out UINT * puResult)
{
	*puResult = (UINT)uOperand;
	return STATUS_SUCCESS;
}
#endif

//
// UINT_PTR -> UINT32 conversion
//
#define RtlUIntPtrToUInt32 RtlUIntPtrToUInt

//
// UINT_PTR -> LONG conversion
//
__inline
NTSTATUS
RtlUIntPtrToLong(
	__in UINT_PTR uOperand,
	__out LONG * plResult)
{
	NTSTATUS status;

	if (uOperand <= LONG_MAX)
	{
		*plResult = (LONG)uOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*plResult = LONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT_PTR -> LONG_PTR conversion
//
__inline
NTSTATUS
RtlUIntPtrToLongPtr(
	__in UINT_PTR uOperand,
	__out LONG_PTR * plResult)
{
	NTSTATUS status;

	if (uOperand <= LONG_PTR_MAX) //-V547
	{
		*plResult = (LONG_PTR)uOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*plResult = LONG_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT_PTR -> ULONG conversion
//
#ifdef _WIN64
#define RtlUIntPtrToULong  RtlULongLongToULong
#else
__inline
NTSTATUS
RtlUIntPtrToULong(
	__in UINT_PTR uOperand,
	__out ULONG * pulResult)
{
	*pulResult = (ULONG)uOperand;
	return STATUS_SUCCESS;
}
#endif

//
// UINT_PTR -> DWORD conversion
//
#define RtlUIntPtrToDWord  RtlUIntPtrToULong

//
// UINT_PTR -> LONGLONG conversion
//
#ifdef _WIN64
#define RtlUIntPtrToLongLong   RtlULongLongToLongLong
#else
__inline
NTSTATUS
RtlUIntPtrToLongLong(
	__in UINT_PTR uOperand,
	__out LONGLONG * pllResult)
{
	*pllResult = (LONGLONG)uOperand;
	return STATUS_SUCCESS;
}
#endif

//
// UINT_PTR -> LONG64 conversion
//
#define RtlUIntPtrToLong64 RtlUIntPtrToLongLong

//
// UINT_PTR -> RtlINT64 conversion
//
#define RtlUIntPtrToInt64  RtlUIntPtrToLongLong

//
// UINT_PTR -> ptrdiff_t conversion
//
#define RtlUIntPtrToPtrdiffT   RtlUIntPtrToIntPtr

//
// UINT_PTR -> SSIZE_T conversion
//
#define RtlUIntPtrToSSIZET RtlUIntPtrToLongPtr

//
// LONG -> INT8 conversion
//
__inline
NTSTATUS
RtlLongToInt8(
	__in LONG lOperand,
	__out INT8 * pi8Result)
{
	NTSTATUS status;

	if ((lOperand >= INT8_MIN) && (lOperand <= INT8_MAX))
	{
		*pi8Result = (INT8)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pi8Result = INT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONG -> UCHAR conversion
//
__inline
NTSTATUS
RtlLongToUChar(
	__in LONG lOperand,
	__out UCHAR * pch)
{
	NTSTATUS status;

	if ((lOperand >= 0) && (lOperand <= 255))
	{
		*pch = (UCHAR)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONG -> CHAR conversion
//
#ifdef _CHAR_UNSIGNED
__forceinline
NTSTATUS
RtlLongToChar(
	__in LONG lOperand,
	__out CHAR * pch)
{
	return RtlLongToUChar(lOperand, (UCHAR*)pch);
}
#else
__inline
NTSTATUS
RtlLongToChar(
	__in LONG lOperand,
	__out CHAR * pch)
{
	NTSTATUS status;

	if ((lOperand >= -128) && (lOperand <= 127))
	{
		*pch = (CHAR)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif // _CHAR_UNSIGNED

//
// LONG -> UINT8 conversion
//
__inline
NTSTATUS
RtlLongToUInt8(
	__in LONG lOperand,
	__out UINT8 * pui8Result)
{
	NTSTATUS status;

	if ((lOperand >= 0) && (lOperand <= UINT8_MAX))
	{
		*pui8Result = (UINT8)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pui8Result = UINT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONG -> BYTE conversion
//
#define RtlLongToByte  RtlLongToUInt8

//
// LONG -> SHORT conversion
//
__inline
NTSTATUS
RtlLongToShort(
	__in LONG lOperand,
	__out SHORT * psResult)
{
	NTSTATUS status;

	if ((lOperand >= SHORT_MIN) && (lOperand <= SHORT_MAX))
	{
		*psResult = (SHORT)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*psResult = SHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONG -> INT16 conversion
//
#define RtlLongToInt16 RtlLongToShort

//
// LONG -> USHORT conversion
//
__inline
NTSTATUS
RtlLongToUShort(
	__in LONG lOperand,
	__out USHORT * pusResult)
{
	NTSTATUS status;

	if ((lOperand >= 0) && (lOperand <= USHORT_MAX))
	{
		*pusResult = (USHORT)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pusResult = USHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONG -> UINT16 conversion
//
#define RtlLongToUInt16    RtlLongToUShort

//
// LONG -> WORD conversion
//
#define RtlLongToWord  RtlLongToUShort

//
// LONG -> INT conversion
//
__inline
NTSTATUS
RtlLongToInt(
	__in LONG lOperand,
	__out INT * piResult)
{
	// we have C_ASSERT's above that ensures that this assignment is ok
	*piResult = (INT)lOperand;
	return STATUS_SUCCESS;
}

//
// LONG -> INT32 conversion
//
#define RtlLongToInt32 RtlLongToInt

//
// LONG -> INT_PTR conversion
//
#ifdef _WIN64
__inline
NTSTATUS
RtlLongToIntPtr(
	__in LONG lOperand,
	__out INT_PTR * piResult)
{
	*piResult = lOperand;
	return STATUS_SUCCESS;
}
#else
#define RtlLongToIntPtr    RtlLongToInt
#endif

//
// LONG -> UINT conversion
//
__inline
NTSTATUS
RtlLongToUInt(
	__in LONG lOperand,
	__out UINT * puResult)
{
	NTSTATUS status;

	if (lOperand >= 0)
	{
		*puResult = (UINT)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*puResult = UINT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONG -> UINT32 conversion
//
#define RtlLongToUInt32    RtlLongToUInt

//
// LONG -> UINT_PTR conversion
//
#ifdef _WIN64
__inline
NTSTATUS
RtlLongToUIntPtr(
	__in LONG lOperand,
	__out UINT_PTR * puResult)
{
	NTSTATUS status;

	if (lOperand >= 0)
	{
		*puResult = (UINT_PTR)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*puResult = UINT_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#else
#define RtlLongToUIntPtr   RtlLongToUInt
#endif

//
// LONG -> ULONG conversion
//
__inline
NTSTATUS
RtlLongToULong(
	__in LONG lOperand,
	__out ULONG * pulResult)
{
	NTSTATUS status;

	if (lOperand >= 0)
	{
		*pulResult = (ULONG)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pulResult = ULONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONG -> ULONG_PTR conversion
//
#ifdef _WIN64
__inline
NTSTATUS
RtlLongToULongPtr(
	__in LONG lOperand,
	__out ULONG_PTR * pulResult)
{
	NTSTATUS status;

	if (lOperand >= 0)
	{
		*pulResult = (ULONG_PTR)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pulResult = ULONG_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#else
#define RtlLongToULongPtr  RtlLongToULong
#endif

//
// LONG -> DWORD conversion
//
#define RtlLongToDWord RtlLongToULong

//
// LONG -> DWORD_PTR conversion
//
#define RtlLongToDWordPtr  RtlLongToULongPtr

//
// LONG -> ULONGLONG conversion
//
__inline
NTSTATUS
RtlLongToULongLong(
	__in LONG lOperand,
	__out ULONGLONG * pullResult)
{
	NTSTATUS status;

	if (lOperand >= 0)
	{
		*pullResult = (ULONGLONG)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pullResult = ULONGLONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONG -> DWORDLONG conversion
//
#define RtlLongToDWordLong RtlLongToULongLong

//
// LONG -> ULONG64 conversion
//
#define RtlLongToULong64   RtlLongToULongLong

//
// LONG -> DWORD64 conversion
//
#define RtlLongToDWord64   RtlLongToULongLong

//
// LONG -> UINT64 conversion
//
#define RtlLongToUInt64    RtlLongToULongLong

//
// LONG -> ptrdiff_t conversion
//
#define RtlLongToPtrdiffT  RtlLongToIntPtr

//
// LONG -> size_t conversion
//
#define RtlLongToSizeT RtlLongToUIntPtr

//
// LONG -> SIZE_T conversion
//
#define RtlLongToSIZET RtlLongToULongPtr

//
// LONG_PTR -> INT8 conversion
//
__inline
NTSTATUS
RtlLongPtrToInt8(
	__in LONG_PTR lOperand,
	__out INT8 * pi8Result)
{
	NTSTATUS status;

	if ((lOperand >= INT8_MIN) && (lOperand <= INT8_MAX))
	{
		*pi8Result = (INT8)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pi8Result = INT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONG_PTR -> UCHAR conversion
//
__inline
NTSTATUS
RtlLongPtrToUChar(
	__in LONG_PTR lOperand,
	__out UCHAR * pch)
{
	NTSTATUS status;

	if ((lOperand >= 0) && (lOperand <= 255))
	{
		*pch = (UCHAR)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONG_PTR -> CHAR conversion
//
#ifdef _CHAR_UNSIGNED
__forceinline
NTSTATUS
RtlLongPtrToChar(
	__in LONG_PTR lOperand,
	__out CHAR * pch)
{
	return RtlLongPtrToUChar(lOperand, (UCHAR*)pch);
}
#else
__inline
NTSTATUS
RtlLongPtrToChar(
	__in LONG_PTR lOperand,
	__out CHAR * pch)
{
	NTSTATUS status;

	if ((lOperand >= -128) && (lOperand <= 127))
	{
		*pch = (CHAR)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif

//
// LONG_PTR -> UINT8 conversion
//
__inline
NTSTATUS
RtlLongPtrToUInt8(
	__in LONG_PTR lOperand,
	__out UINT8 * pui8Result)
{
	NTSTATUS status;

	if ((lOperand >= 0) && (lOperand <= UINT8_MAX))
	{
		*pui8Result = (UINT8)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pui8Result = UINT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONG_PTR -> BYTE conversion
//
#define RtlLongPtrToByte   RtlLongPtrToUInt8

//
// LONG_PTR -> SHORT conversion
//
__inline
NTSTATUS
RtlLongPtrToShort(
	__in LONG_PTR lOperand,
	__out SHORT * psResult)
{
	NTSTATUS status;

	if ((lOperand >= SHORT_MIN) && (lOperand <= SHORT_MAX))
	{
		*psResult = (SHORT)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*psResult = SHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONG_PTR -> INT16 conversion
//
#define RtlLongPtrToInt16  RtlLongPtrToShort

//
// LONG_PTR -> USHORT conversion
//
__inline
NTSTATUS
RtlLongPtrToUShort(
	__in LONG_PTR lOperand,
	__out USHORT * pusResult)
{
	NTSTATUS status;

	if ((lOperand >= 0) && (lOperand <= USHORT_MAX))
	{
		*pusResult = (USHORT)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pusResult = USHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONG_PTR -> UINT16 conversion
//
#define RtlLongPtrToUInt16 RtlLongPtrToUShort

//
// LONG_PTR -> WORD conversion
//
#define RtlLongPtrToWord   RtlLongPtrToUShort

//
// LONG_PTR -> INT conversion
//
#ifdef _WIN64
#define RtlLongPtrToInt    RtlLongLongToInt
#else
__inline
NTSTATUS
RtlLongPtrToInt(
	__in LONG_PTR lOperand,
	__out INT * piResult)
{
	// we have C_ASSERT's above that ensures that this assignment is ok
	*piResult = (INT)lOperand;
	return STATUS_SUCCESS;
}
#endif

//
// LONG_PTR -> INT32 conversion
//
#define RtlLongPtrToInt32  RtlLongPtrToInt

//
// LONG_PTR -> INT_PTR conversion
//
__inline
NTSTATUS
RtlLongPtrToIntPtr(
	__in LONG_PTR lOperand,
	__out INT_PTR * piResult)
{
	// we have C_ASSERT's above that ensures that this assignment is ok
	*piResult = (INT_PTR)lOperand;
	return STATUS_SUCCESS;
}

//
// LONG_PTR -> UINT conversion
//
#ifdef _WIN64
#define RtlLongPtrToUInt   RtlLongLongToUInt
#else
__inline
NTSTATUS
RtlLongPtrToUInt(
	__in LONG_PTR lOperand,
	__out UINT * puResult)
{
	NTSTATUS status;

	if (lOperand >= 0)
	{
		*puResult = (UINT)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*puResult = UINT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif

//
// LONG_PTR -> UINT32 conversion
//
#define RtlLongPtrToUInt32 RtlLongPtrToUInt

//
// LONG_PTR -> UINT_PTR conversion
//
__inline
NTSTATUS
RtlLongPtrToUIntPtr(
	__in LONG_PTR lOperand,
	__out UINT_PTR * puResult)
{
	NTSTATUS status;

	if (lOperand >= 0)
	{
		*puResult = (UINT_PTR)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*puResult = UINT_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONG_PTR -> LONG conversion
//
#ifdef _WIN64
#define RtlLongPtrToLong   RtlLongLongToLong
#else
__inline
NTSTATUS
RtlLongPtrToLong(
	__in LONG_PTR lOperand,
	__out LONG * plResult)
{
	*plResult = (LONG)lOperand;
	return STATUS_SUCCESS;
}
#endif

//
// LONG_PTR -> ULONG conversion
//
#ifdef _WIN64
#define RtlLongPtrToULong  RtlLongLongToULong
#else
__inline
NTSTATUS
RtlLongPtrToULong(
	__in LONG_PTR lOperand,
	__out ULONG * pulResult)
{
	NTSTATUS status;

	if (lOperand >= 0)
	{
		*pulResult = (ULONG)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pulResult = ULONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif

//
// LONG_PTR -> ULONG_PTR conversion
//
__inline
NTSTATUS
RtlLongPtrToULongPtr(
	__in LONG_PTR lOperand,
	__out ULONG_PTR * pulResult)
{
	NTSTATUS status;

	if (lOperand >= 0)
	{
		*pulResult = (ULONG_PTR)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pulResult = ULONG_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONG_PTR -> DWORD conversion
//
#define RtlLongPtrToDWord  RtlLongPtrToULong

//
// LONG_PTR -> DWORD_PTR conversion
//
#define RtlLongPtrToDWordPtr   RtlLongPtrToULongPtr

//
// LONG_PTR -> ULONGLONG conversion
//
__inline
NTSTATUS
RtlLongPtrToULongLong(
	__in LONG_PTR lOperand,
	__out ULONGLONG * pullResult)
{
	NTSTATUS status;

	if (lOperand >= 0)
	{
		*pullResult = (ULONGLONG)lOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pullResult = ULONGLONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONG_PTR -> DWORDLONG conversion
//
#define RtlLongPtrToDWordLong  RtlLongPtrToULongLong

//
// LONG_PTR -> ULONG64 conversion
//
#define RtlLongPtrToULong64    RtlLongPtrToULongLong

//
// LONG_PTR -> DWORD64 conversion
//
#define RtlLongPtrToDWord64    RtlLongPtrToULongLong

//
// LONG_PTR -> UINT64 conversion
//
#define RtlLongPtrToUInt64 RtlLongPtrToULongLong

//
// LONG_PTR -> size_t conversion
//
#define RtlLongPtrToSizeT  RtlLongPtrToUIntPtr

//
// LONG_PTR -> SIZE_T conversion
//
#define RtlLongPtrToSIZET  RtlLongPtrToULongPtr

//
// ULONG -> INT8 conversion
//
__inline
NTSTATUS
RtlULongToInt8(
	__in ULONG ulOperand,
	__out INT8 * pi8Result)
{
	NTSTATUS status;

	if (ulOperand <= INT8_MAX)
	{
		*pi8Result = (INT8)ulOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pi8Result = INT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONG -> UCHAR conversion
//
__inline
NTSTATUS
RtlULongToUChar(
	__in ULONG ulOperand,
	__out UCHAR * pch)
{
	NTSTATUS status;

	if (ulOperand <= 255)
	{
		*pch = (UCHAR)ulOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONG -> CHAR conversion
//
#ifdef _CHAR_UNSIGNED
__forceinline
NTSTATUS
RtlULongToChar(
	__in ULONG ulOperand,
	__out CHAR * pch)
{
	return RtlULongToUChar(ulOperand, (UCHAR*)pch);
}
#else
__inline
NTSTATUS
RtlULongToChar(
	__in ULONG ulOperand,
	__out CHAR * pch)
{
	NTSTATUS status;

	if (ulOperand <= 127)
	{
		*pch = (CHAR)ulOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif // _CHAR_UNSIGNED

//
// ULONG -> UINT8 conversion
//
__inline
NTSTATUS
RtlULongToUInt8(
	__in ULONG ulOperand,
	__out UINT8 * pui8Result)
{
	NTSTATUS status;

	if (ulOperand <= UINT8_MAX)
	{
		*pui8Result = (UINT8)ulOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pui8Result = UINT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONG -> BYTE conversion
//
#define RtlULongToByte RtlULongToUInt8

//
// ULONG -> SHORT conversion
//
__inline
NTSTATUS
RtlULongToShort(
	__in ULONG ulOperand,
	__out SHORT * psResult)
{
	NTSTATUS status;

	if (ulOperand <= SHORT_MAX)
	{
		*psResult = (SHORT)ulOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*psResult = SHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONG -> INT16 conversion
//
#define RtlULongToInt16    RtlULongToShort

//
// ULONG -> USHORT conversion
//
__inline
NTSTATUS
RtlULongToUShort(
	__in ULONG ulOperand,
	__out USHORT * pusResult)
{
	NTSTATUS status;

	if (ulOperand <= USHORT_MAX)
	{
		*pusResult = (USHORT)ulOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pusResult = USHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONG -> UINT16 conversion
//
#define RtlULongToUInt16   RtlULongToUShort

//
// ULONG -> WORD conversion
//
#define RtlULongToWord RtlULongToUShort

//
// ULONG -> INT conversion
//
__inline
NTSTATUS
RtlULongToInt(
	__in ULONG ulOperand,
	__out INT * piResult)
{
	NTSTATUS status;

	if (ulOperand <= INT_MAX)
	{
		*piResult = (INT)ulOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*piResult = INT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONG -> INT32 conversion
//
#define RtlULongToInt32    RtlULongToInt

//
// ULONG -> INT_PTR conversion
//
#ifdef _WIN64
__inline
NTSTATUS
RtlULongToIntPtr(
	__in ULONG ulOperand,
	__out INT_PTR * piResult)
{
	*piResult = (INT_PTR)ulOperand;
	return STATUS_SUCCESS;
}
#else
#define RtlULongToIntPtr   RtlULongToInt
#endif

//
// ULONG -> UINT conversion
//
__inline
NTSTATUS
RtlULongToUInt(
	__in ULONG ulOperand,
	__out UINT * puResult)
{
	// we have C_ASSERT's above that ensures that this assignment is ok
	*puResult = (UINT)ulOperand;
	return STATUS_SUCCESS;
}

//
// ULONG -> UINT32 conversion
//
#define RtlULongToUInt32   RtlULongToUInt

//
// ULONG -> UINT_PTR conversion
//
#ifdef _WIN64
__inline
NTSTATUS
RtlULongToUIntPtr(
	__in LONG ulOperand,
	__out UINT_PTR * puiResult)
{
	*puiResult = (UINT_PTR)ulOperand;
	return STATUS_SUCCESS;
}
#else
#define RtlULongToUIntPtr  RtlULongToUInt
#endif

//
// ULONG -> LONG conversion
//
__inline
NTSTATUS
RtlULongToLong(
	__in ULONG ulOperand,
	__out LONG * plResult)
{
	NTSTATUS status;

	if (ulOperand <= LONG_MAX)
	{
		*plResult = (LONG)ulOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*plResult = LONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONG -> LONG_PTR conversion
//
#ifdef _WIN64
__inline
NTSTATUS
RtlULongToLongPtr(
	__in ULONG ulOperand,
	__out LONG_PTR * plResult)
{
	*plResult = (LONG_PTR)ulOperand;
	return STATUS_SUCCESS;
}
#else
#define RtlULongToLongPtr  RtlULongToLong
#endif

//
// ULONG -> ptrdiff_t conversion
//
#define RtlULongToPtrdiffT RtlULongToIntPtr

//
// ULONG -> SSIZE_T conversion
//
#define RtlULongToSSIZET   RtlULongToLongPtr

//
// ULONG_PTR -> INT8 conversion
//
__inline
NTSTATUS
RtlULongPtrToInt8(
	__in ULONG_PTR ulOperand,
	__out INT8 * pi8Result)
{
	NTSTATUS status;

	if (ulOperand <= INT8_MAX)
	{
		*pi8Result = (INT8)ulOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pi8Result = INT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONG_PTR -> UCHAR conversion
//
__inline
NTSTATUS
RtlULongPtrToUChar(
	__in ULONG_PTR ulOperand,
	__out UCHAR * pch)
{
	NTSTATUS status;

	if (ulOperand <= 255)
	{
		*pch = (UCHAR)ulOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONG_PTR -> CHAR conversion
//
#ifdef _CHAR_UNSIGNED
__forceinline
NTSTATUS
RtlULongPtrToChar(
	__in LONG_PTR ulOperand,
	__out CHAR * pch)
{
	return RtlULongPtrToUChar(ulOperand, (UCHAR*)pch);
}
#else
__inline
NTSTATUS
RtlULongPtrToChar(
	__in ULONG_PTR ulOperand,
	__out CHAR * pch)
{
	NTSTATUS status;

	if (ulOperand <= 127)
	{
		*pch = (CHAR)ulOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif // _CHAR_UNSIGNED

//
// ULONG_PTR -> UINT8 conversion
//
__inline
NTSTATUS
RtlULongPtrToUInt8(
	__in ULONG_PTR ulOperand,
	__out UINT8 * pui8Result)
{
	NTSTATUS status;

	if (ulOperand <= UINT8_MAX)
	{
		*pui8Result = (UINT8)ulOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pui8Result = UINT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONG_PTR -> BYTE conversion
//
#define RtlULongPtrToByte  RtlULongPtrToUInt8

//
// ULONG_PTR -> SHORT conversion
//
__inline
NTSTATUS
RtlULongPtrToShort(
	__in ULONG_PTR ulOperand,
	__out SHORT * psResult)
{
	NTSTATUS status;

	if (ulOperand <= SHORT_MAX)
	{
		*psResult = (SHORT)ulOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*psResult = SHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONG_PTR -> INT16 conversion
//
#define RtlULongPtrToInt16 RtlULongPtrToShort

//
// ULONG_PTR -> USHORT conversion
//
__inline
NTSTATUS
RtlULongPtrToUShort(
	__in ULONG_PTR ulOperand,
	__out USHORT * pusResult)
{
	NTSTATUS status;

	if (ulOperand <= USHORT_MAX)
	{
		*pusResult = (USHORT)ulOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pusResult = USHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONG_PTR -> UINT16 conversion
//
#define RtlULongPtrToUInt16    RtlULongPtrToUShort

//
// ULONG_PTR -> WORD conversion
//
#define RtlULongPtrToWord  RtlULongPtrToUShort

//
// ULONG_PTR -> INT conversion
//
__inline
NTSTATUS
RtlULongPtrToInt(
	__in ULONG_PTR ulOperand,
	__out INT * piResult)
{
	NTSTATUS status;

	if (ulOperand <= INT_MAX)
	{
		*piResult = (INT)ulOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*piResult = INT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONG_PTR -> INT32 conversion
//
#define RtlULongPtrToInt32 RtlULongPtrToInt

//
// ULONG_PTR -> INT_PTR conversion
//
__inline
NTSTATUS
RtlULongPtrToIntPtr(
	__in ULONG_PTR ulOperand,
	__out INT_PTR * piResult)
{
	NTSTATUS status;

	if (ulOperand <= INT_PTR_MAX) //-V547
	{
		*piResult = (INT_PTR)ulOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*piResult = INT_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONG_PTR -> UINT conversion
//
#ifdef _WIN64
#define RtlULongPtrToUInt  RtlULongLongToUInt
#else
__inline
NTSTATUS
RtlULongPtrToUInt(
	__in ULONG_PTR ulOperand,
	__out UINT * puResult)
{
	// we have C_ASSERT's above that ensures that this assignment is ok
	*puResult = (UINT)ulOperand;
	return STATUS_SUCCESS;
}
#endif

//
// ULONG_PTR -> UINT32 conversion
//
#define RtlULongPtrToUInt32    RtlULongPtrToUInt

//
// ULONG_PTR -> UINT_PTR conversion
//
__inline
NTSTATUS
RtlULongPtrToUIntPtr(
	__in ULONG_PTR ulOperand,
	__out UINT_PTR * puResult)
{
	*puResult = (UINT_PTR)ulOperand;
	return STATUS_SUCCESS;
}

//
// ULONG_PTR -> LONG conversion
//
__inline
NTSTATUS
RtlULongPtrToLong(
	__in ULONG_PTR ulOperand,
	__out LONG * plResult)
{
	NTSTATUS status;

	if (ulOperand <= LONG_MAX)
	{
		*plResult = (LONG)ulOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*plResult = LONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONG_PTR -> LONG_PTR conversion
//
__inline
NTSTATUS
RtlULongPtrToLongPtr(
	__in ULONG_PTR ulOperand,
	__out LONG_PTR * plResult)
{
	NTSTATUS status;

	if (ulOperand <= LONG_PTR_MAX) //-V547
	{
		*plResult = (LONG_PTR)ulOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*plResult = LONG_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONG_PTR -> ULONG conversion
//
#ifdef _WIN64
#define RtlULongPtrToULong RtlULongLongToULong
#else
__inline
NTSTATUS
RtlULongPtrToULong(
	__in ULONG_PTR ulOperand,
	__out ULONG * pulResult)
{
	*pulResult = (ULONG)ulOperand;
	return STATUS_SUCCESS;
}
#endif

//
// ULONG_PTR -> DWORD conversion
//
#define RtlULongPtrToDWord RtlULongPtrToULong

//
// ULONG_PTR -> LONGLONG conversion
//
#ifdef _WIN64
#define RtlULongPtrToLongLong  RtlULongLongToLongLong
#else
__inline
NTSTATUS
RtlULongPtrToLongLong(
	__in ULONG_PTR ulOperand,
	__out LONGLONG * pllResult)
{
	*pllResult = (LONGLONG)ulOperand;
	return STATUS_SUCCESS;
}
#endif

//
// ULONG_PTR -> LONG64 conversion
//
#define RtlULongPtrToLong64    RtlULongPtrToLongLong

//
// ULONG_PTR -> RtlINT64
//
#define RtlULongPtrToInt64 RtlULongPtrToLongLong

//
// ULONG_PTR -> ptrdiff_t conversion
//
#define RtlULongPtrToPtrdiffT  RtlULongPtrToIntPtr

//
// ULONG_PTR -> SSIZE_T conversion
//
#define RtlULongPtrToSSIZET    RtlULongPtrToLongPtr

//
// DWORD -> INT8 conversion
//
#define RtlDWordToInt8 RtlULongToInt8

//
// DWORD -> CHAR conversion
//
#define RtlDWordToChar RtlULongToChar

//
// DWORD -> UCHAR conversion
//
#define RtlDWordToUChar    RtlULongToUChar

//
// DWORD -> UINT8 conversion
//
#define RtlDWordToUInt8    RtlULongToUInt8

//
// DWORD -> BYTE conversion
//
#define RtlDWordToByte RtlULongToUInt8

//
// DWORD -> SHORT conversion
//
#define RtlDWordToShort    RtlULongToShort

//
// DWORD -> INT16 conversion
//
#define RtlDWordToInt16    RtlULongToShort

//
// DWORD -> USHORT conversion
//
#define RtlDWordToUShort   RtlULongToUShort

//
// DWORD -> UINT16 conversion
//
#define RtlDWordToUInt16   RtlULongToUShort

//
// DWORD -> WORD conversion
//
#define RtlDWordToWord RtlULongToUShort

//
// DWORD -> INT conversion
//
#define RtlDWordToInt  RtlULongToInt

//
// DWORD -> INT32 conversion
//
#define RtlDWordToInt32    RtlULongToInt

//
// DWORD -> INT_PTR conversion
//
#define RtlDWordToIntPtr   RtlULongToIntPtr

//
// DWORD -> UINT conversion
//
#define RtlDWordToUInt RtlULongToUInt

//
// DWORD -> UINT32 conversion
//
#define RtlDWordToUInt32   RtlULongToUInt

//
// DWORD -> UINT_PTR conversion
//
#define RtlDWordToUIntPtr  RtlULongToUIntPtr

//
// DWORD -> LONG conversion
//
#define RtlDWordToLong RtlULongToLong

//
// DWORD -> LONG_PTR conversion
//
#define RtlDWordToLongPtr  RtlULongToLongPtr

//
// DWORD -> ptrdiff_t conversion
//
#define RtlDWordToPtrdiffT RtlULongToIntPtr

//
// DWORD -> SSIZE_T conversion
//
#define RtlDWordToSSIZET   RtlULongToLongPtr

//
// DWORD_PTR -> INT8 conversion
//
#define RtlDWordPtrToInt8  RtlULongPtrToInt8

//
// DWORD_PTR -> UCHAR conversion
//
#define RtlDWordPtrToUChar RtlULongPtrToUChar

//
// DWORD_PTR -> CHAR conversion
//
#define RtlDWordPtrToChar  RtlULongPtrToChar

//
// DWORD_PTR -> UINT8 conversion
//
#define RtlDWordPtrToUInt8 RtlULongPtrToUInt8

//
// DWORD_PTR -> BYTE conversion
//
#define RtlDWordPtrToByte  RtlULongPtrToUInt8

//
// DWORD_PTR -> SHORT conversion
//
#define RtlDWordPtrToShort RtlULongPtrToShort

//
// DWORD_PTR -> INT16 conversion
//
#define RtlDWordPtrToInt16 RtlULongPtrToShort

//
// DWORD_PTR -> USHORT conversion
//
#define RtlDWordPtrToUShort    RtlULongPtrToUShort

//
// DWORD_PTR -> UINT16 conversion
//
#define RtlDWordPtrToUInt16    RtlULongPtrToUShort

//
// DWORD_PTR -> WORD conversion
//
#define RtlDWordPtrToWord  RtlULongPtrToUShort

//
// DWORD_PTR -> INT conversion
//
#define RtlDWordPtrToInt   RtlULongPtrToInt

//
// DWORD_PTR -> INT32 conversion
//
#define RtlDWordPtrToInt32 RtlULongPtrToInt

//
// DWORD_PTR -> INT_PTR conversion
//
#define RtlDWordPtrToIntPtr    RtlULongPtrToIntPtr

//
// DWORD_PTR -> UINT conversion
//
#define RtlDWordPtrToUInt  RtlULongPtrToUInt

//
// DWORD_PTR -> UINT32 conversion
//
#define RtlDWordPtrToUInt32    RtlULongPtrToUInt

//
// DWODR_PTR -> UINT_PTR conversion
//
#define RtlDWordPtrToUIntPtr   RtlULongPtrToUIntPtr

//
// DWORD_PTR -> LONG conversion
//
#define RtlDWordPtrToLong  RtlULongPtrToLong

//
// DWORD_PTR -> LONG_PTR conversion
//
#define RtlDWordPtrToLongPtr   RtlULongPtrToLongPtr

//
// DWORD_PTR -> ULONG conversion
//
#define RtlDWordPtrToULong RtlULongPtrToULong

//
// DWORD_PTR -> DWORD conversion
//
#define RtlDWordPtrToDWord RtlULongPtrToULong

//
// DWORD_PTR -> LONGLONG conversion
//
#define RtlDWordPtrToLongLong  RtlULongPtrToLongLong

//
// DWORD_PTR -> LONG64 conversion
//
#define RtlDWordPtrToLong64    RtlULongPtrToLongLong

//
// DWORD_PTR -> RtlINT64 conversion
//
#define RtlDWordPtrToInt64 RtlULongPtrToLongLong

//
// DWORD_PTR -> ptrdiff_t conversion
//
#define RtlDWordPtrToPtrdiffT  RtlULongPtrToIntPtr

//
// DWORD_PTR -> SSIZE_T conversion
//
#define RtlDWordPtrToSSIZET    RtlULongPtrToLongPtr

//
// LONGLONG -> INT8 conversion
//
__inline
NTSTATUS
RtlLongLongToInt8(
	__in LONGLONG llOperand,
	__out INT8 * pi8Result)
{
	NTSTATUS status;

	if ((llOperand >= INT8_MIN) && (llOperand <= INT8_MAX))
	{
		*pi8Result = (INT8)llOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pi8Result = INT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONGLONG -> UCHAR conversion
//
__inline
NTSTATUS
RtlLongLongToUChar(
	__in LONGLONG llOperand,
	__out UCHAR * pch)
{
	NTSTATUS status;

	if ((llOperand >= 0) && (llOperand <= 255))
	{
		*pch = (UCHAR)llOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONGLONG -> CHAR conversion
//
#ifdef _CHAR_UNSIGNED
__forceinline
NTSTATUS
RtlLongLongToChar(
	__in LONGLONG llOperand,
	__out CHAR * pch)
{
	return RtlLongLongToUChar(llOperand, (UCHAR*)pch);
}
#else
__inline
NTSTATUS
RtlLongLongToChar(
	__in LONGLONG llOperand,
	__out CHAR * pch)
{
	NTSTATUS status;

	if ((llOperand >= 0) && (llOperand <= 127))
	{
		*pch = (CHAR)llOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif

//
// LONGLONG -> UINT8 conversion
//
__inline
NTSTATUS
RtlLongLongToUInt8(
	__in LONGLONG llOperand,
	__out UINT8 * pu8Result)
{
	NTSTATUS status;

	if ((llOperand >= 0) && (llOperand <= UINT8_MAX))
	{
		*pu8Result = (UINT8)llOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pu8Result = UINT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONGLONG -> BYTE conversion
//
#define RtlLongLongToByte  RtlLongLongToUInt8

//
// LONGLONG -> SHORT conversion
//
__inline
NTSTATUS
RtlLongLongToShort(
	__in LONGLONG llOperand,
	__out SHORT * psResult)
{
	NTSTATUS status;

	if ((llOperand >= SHORT_MIN) && (llOperand <= SHORT_MAX))
	{
		*psResult = (SHORT)llOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*psResult = SHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONGLONG -> INT16 conversion
//
#define RtlLongLongToInt16 RtlLongLongToShort

//
// LONGLONG -> USHORT conversion
//
__inline
NTSTATUS
RtlLongLongToUShort(
	__in LONGLONG llOperand,
	__out USHORT * pusResult)
{
	NTSTATUS status;

	if ((llOperand >= 0) && (llOperand <= USHORT_MAX))
	{
		*pusResult = (USHORT)llOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pusResult = USHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONGLONG -> UINT16 conversion
//
#define RtlLongLongToUInt16    RtlLongLongToUShort

//
// LONGLONG -> WORD conversion
//
#define RtlLongLongToWord  RtlLongLongToUShort

//
// LONGLONG -> INT conversion
//
__inline
NTSTATUS
RtlLongLongToInt(
	__in LONGLONG llOperand,
	__out INT * piResult)
{
	NTSTATUS status;

	if ((llOperand >= INT_MIN) && (llOperand <= INT_MAX))
	{
		*piResult = (INT)llOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*piResult = INT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONGLONG -> INT32 conversion
//
#define RtlLongLongToInt32 RtlLongLongToInt

//
// LONGLONG -> INT_PTR conversion
//
#ifdef _WIN64
__inline
NTSTATUS
RtlLongLongToIntPtr(
	__in LONGLONG llOperand,
	__out INT_PTR * piResult)
{
	*piResult = llOperand;
	return STATUS_SUCCESS;
}
#else
#define RtlLongLongToIntPtr   RtlLongLongToInt
#endif

//
// LONGLONG -> UINT conversion
//
__inline
NTSTATUS
RtlLongLongToUInt(
	__in LONGLONG llOperand,
	__out UINT * puResult)
{
	NTSTATUS status;

	if ((llOperand >= 0) && (llOperand <= UINT_MAX))
	{
		*puResult = (UINT)llOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*puResult = UINT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONGLONG -> UINT32 conversion
//
#define RtlLongLongToUInt32    RtlLongLongToUInt

//
// LONGLONG -> UINT_PTR conversion
//
#ifdef _WIN64
#define RtlLongLongToUIntPtr  RtlLongLongToULongLong
#else
#define RtlLongLongToUIntPtr  RtlLongLongToUInt
#endif

//
// LONGLONG -> LONG conversion
//
__inline
NTSTATUS
RtlLongLongToLong(
	__in LONGLONG llOperand,
	__out LONG * plResult)
{
	NTSTATUS status;

	if ((llOperand >= LONG_MIN) && (llOperand <= LONG_MAX))
	{
		*plResult = (LONG)llOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*plResult = LONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONGLONG -> LONG_PTR conversion
//
#ifdef _WIN64
__inline
NTSTATUS
RtlLongLongToLongPtr(
	__in LONGLONG llOperand,
	__out LONG_PTR * plResult)
{
	*plResult = (LONG_PTR)llOperand;
	return STATUS_SUCCESS;
}
#else
#define RtlLongLongToLongPtr  RtlLongLongToLong
#endif

//
// LONGLONG -> ULONG conversion
//
__inline
NTSTATUS
RtlLongLongToULong(
	__in LONGLONG llOperand,
	__out ULONG * pulResult)
{
	NTSTATUS status;

	if ((llOperand >= 0) && (llOperand <= ULONG_MAX))
	{
		*pulResult = (ULONG)llOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pulResult = ULONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONGLONG -> ULONG_PTR conversion
//
#ifdef _WIN64
#define RtlLongLongToULongPtr RtlLongLongToULongLong
#else
#define RtlLongLongToULongPtr RtlLongLongToULong
#endif

//
// LONGLONG -> DWORD conversion
//
#define RtlLongLongToDWord    RtlLongLongToULong

//
// LONGLONG -> DWORD_PTR conversion
//
#define RtlLongLongToDWordPtr RtlLongLongToULongPtr

//
// LONGLONG -> ULONGLONG conversion
//
__inline
NTSTATUS
RtlLongLongToULongLong(
	__in LONGLONG llOperand,
	__out ULONGLONG * pullResult)
{
	NTSTATUS status;

	if (llOperand >= 0)
	{
		*pullResult = (ULONGLONG)llOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pullResult = ULONGLONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// LONGLONG -> DWORDLONG conversion
//
#define RtlLongLongToDWordLong RtlLongLongToULongLong

//
// LONGLONG -> ULONG64 conversion
//
#define RtlLongLongToULong64   RtlLongLongToULongLong

//
// LONGLONG -> DWORD64 conversion
//
#define RtlLongLongToDWord64   RtlLongLongToULongLong

//
// LONGLONG -> UINT64 conversion
//
#define RtlLongLongToUInt64    RtlLongLongToULongLong

//
// LONGLONG -> ptrdiff_t conversion
//
#define RtlLongLongToPtrdiffT RtlLongLongToIntPtr

//
// LONGLONG -> size_t conversion
//
#define RtlLongLongToSizeT    RtlLongLongToUIntPtr

//
// LONGLONG -> SSIZE_T conversion
//
#define RtlLongLongToSSIZET   RtlLongLongToLongPtr

//
// LONGLONG -> SIZE_T conversion
//
#define RtlLongLongToSIZET    RtlLongLongToULongPtr

//
// LONG64 -> CHAR conversion
//
#define RtlLong64ToChar    RtlLongLongToChar

//
// LONG64 -> INT8 conversion
//
#define RtlLong64ToInt8    RtlLongLongToInt8

//
// LONG64 -> UCHAR conversion
//
#define RtlLong64ToUChar   RtlLongLongToUChar

//
// LONG64 -> UINT8 conversion
//
#define RtlLong64ToUInt8   RtlLongLongToUInt8

//
// LONG64 -> BYTE conversion
//
#define RtlLong64ToByte    RtlLongLongToUInt8

//
// LONG64 -> SHORT conversion
//
#define RtlLong64ToShort   RtlLongLongToShort

//
// LONG64 -> INT16 conversion
//
#define RtlLong64ToInt16   RtlLongLongToShort

//
// LONG64 -> USHORT conversion
//
#define RtlLong64ToUShort  RtlLongLongToUShort

//
// LONG64 -> UINT16 conversion
//
#define RtlLong64ToUInt16  RtlLongLongToUShort

//
// LONG64 -> WORD conversion
//
#define RtlLong64ToWord    RtlLongLongToUShort

//
// LONG64 -> INT conversion
//
#define RtlLong64ToInt RtlLongLongToInt

//
// LONG64 -> INT32 conversion
//
#define RtlLong64ToInt32   RtlLongLongToInt

//
// LONG64 -> INT_PTR conversion
//
#define RtlLong64ToIntPtr  RtlLongLongToIntPtr

//
// LONG64 -> UINT conversion
//
#define RtlLong64ToUInt    RtlLongLongToUInt

//
// LONG64 -> UINT32 conversion
//
#define RtlLong64ToUInt32  RtlLongLongToUInt

//
// LONG64 -> UINT_PTR conversion
//
#define RtlLong64ToUIntPtr RtlLongLongToUIntPtr

//
// LONG64 -> LONG conversion
//
#define RtlLong64ToLong    RtlLongLongToLong

//
// LONG64 -> LONG_PTR conversion
//
#define RtlLong64ToLongPtr RtlLongLongToLongPtr

//
// LONG64 -> ULONG conversion
//
#define RtlLong64ToULong   RtlLongLongToULong

//
// LONG64 -> ULONG_PTR conversion
//
#define RtlLong64ToULongPtr    RtlLongLongToULongPtr

//
// LONG64 -> DWORD conversion
//
#define RtlLong64ToDWord   RtlLongLongToULong

//
// LONG64 -> DWORD_PTR conversion
//
#define RtlLong64ToDWordPtr    RtlLongLongToULongPtr

//
// LONG64 -> ULONGLONG conversion
//
#define RtlLong64ToULongLong   RtlLongLongToULongLong

//
// LONG64 -> ptrdiff_t conversion
//
#define RtlLong64ToPtrdiffT    RtlLongLongToIntPtr

//
// LONG64 -> size_t conversion
//
#define RtlLong64ToSizeT   RtlLongLongToUIntPtr

//
// LONG64 -> SSIZE_T conversion
//
#define RtlLong64ToSSIZET  RtlLongLongToLongPtr

//
// LONG64 -> SIZE_T conversion
//
#define RtlLong64ToSIZET   RtlLongLongToULongPtr

//
// RtlINT64 -> CHAR conversion
//
#define RtlInt64ToChar RtlLongLongToChar

//
// RtlINT64 -> INT8 conversion
//
#define RtlInt64ToInt8 RtlLongLongToInt8

//
// RtlINT64 -> UCHAR conversion
//
#define RtlInt64ToUChar    RtlLongLongToUChar

//
// RtlINT64 -> UINT8 conversion
//
#define RtlInt64ToUInt8    RtlLongLongToUInt8

//
// RtlINT64 -> BYTE conversion
//
#define RtlInt64ToByte RtlLongLongToUInt8

//
// RtlINT64 -> SHORT conversion
//
#define RtlInt64ToShort    RtlLongLongToShort

//
// RtlINT64 -> INT16 conversion
//
#define RtlInt64ToInt16    RtlLongLongToShort

//
// RtlINT64 -> USHORT conversion
//
#define RtlInt64ToUShort   RtlLongLongToUShort

//
// RtlINT64 -> UINT16 conversion
//
#define RtlInt64ToUInt16   RtlLongLongToUShort

//
// RtlINT64 -> WORD conversion
//
#define RtlInt64ToWord RtlLongLongToUShort

//
// RtlINT64 -> INT conversion
//
#define RtlInt64ToInt  RtlLongLongToInt

//
// RtlINT64 -> INT32 conversion
//
#define RtlInt64ToInt32    RtlLongLongToInt

//
// RtlINT64 -> INT_PTR conversion
//
#define RtlInt64ToIntPtr   RtlLongLongToIntPtr

//
// RtlINT64 -> UINT conversion
//
#define RtlInt64ToUInt RtlLongLongToUInt

//
// RtlINT64 -> UINT32 conversion
//
#define RtlInt64ToUInt32   RtlLongLongToUInt

//
// RtlINT64 -> UINT_PTR conversion
//
#define RtlInt64ToUIntPtr  RtlLongLongToUIntPtr

//
// RtlINT64 -> LONG conversion
//
#define RtlInt64ToLong RtlLongLongToLong

//
// RtlINT64 -> LONG_PTR conversion
//
#define RtlInt64ToLongPtr  RtlLongLongToLongPtr

//
// RtlINT64 -> ULONG conversion
//
#define RtlInt64ToULong    RtlLongLongToULong

//
// RtlINT64 -> ULONG_PTR conversion
//
#define RtlInt64ToULongPtr RtlLongLongToULongPtr

//
// RtlINT64 -> DWORD conversion
//
#define RtlInt64ToDWord    RtlLongLongToULong

//
// RtlINT64 -> DWORD_PTR conversion
//
#define RtlInt64ToDWordPtr RtlLongLongToULongPtr

//
// RtlINT64 -> ULONGLONG conversion
//
#define RtlInt64ToULongLong    RtlLongLongToULongLong

//
// RtlINT64 -> DWORDLONG conversion
//
#define RtlInt64ToDWordLong    RtlLongLongToULongLong

//
// RtlINT64 -> ULONG64 conversion
//
#define RtlInt64ToULong64  RtlLongLongToULongLong

//
// RtlINT64 -> DWORD64 conversion
//
#define RtlInt64ToDWord64  RtlLongLongToULongLong

//
// RtlINT64 -> UINT64 conversion
//
#define RtlInt64ToUInt64   RtlLongLongToULongLong

//
// RtlINT64 -> ptrdiff_t conversion
//
#define RtlInt64ToPtrdiffT RtlLongLongToIntPtr

//
// RtlINT64 -> size_t conversion
//
#define RtlInt64ToSizeT    RtlLongLongToUIntPtr

//
// RtlINT64 -> SSIZE_T conversion
//
#define RtlInt64ToSSIZET   RtlLongLongToLongPtr

//
// RtlINT64 -> SIZE_T conversion
//
#define RtlInt64ToSIZET    RtlLongLongToULongPtr

//
// ULONGLONG -> INT8 conversion
//
__inline
NTSTATUS
RtlULongLongToInt8(
	__in ULONGLONG ullOperand,
	__out INT8 * pi8Result)
{
	NTSTATUS status;

	if (ullOperand <= INT8_MAX)
	{
		*pi8Result = (INT8)ullOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pi8Result = INT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONGLONG -> UCHAR conversion
//
__inline
NTSTATUS
RtlULongLongToUChar(
	__in ULONGLONG ullOperand,
	__out UCHAR * pch)
{
	NTSTATUS status;

	if (ullOperand <= 255)
	{
		*pch = (UCHAR)ullOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONGLONG -> CHAR conversion
//
#ifdef _CHAR_UNSIGNED
__forceinline
NTSTATUS
RtlULongLongToChar(
	__in ULONGLONG ullOperand,
	__out CHAR * pch)
{
	return RtlULongLongToUChar(ullOperand, (UCHAR*)pch);
}
#else
__inline
NTSTATUS
RtlULongLongToChar(
	__in ULONGLONG ullOperand,
	__out CHAR * pch)
{
	NTSTATUS status;

	if (ullOperand <= 127)
	{
		*pch = (CHAR)ullOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pch = '\0';
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif

//
// ULONGLONG -> UINT8 conversion
//
__inline
NTSTATUS
RtlULongLongToUInt8(
	__in ULONGLONG ullOperand,
	__out UINT8 * pu8Result)
{
	NTSTATUS status;

	if (ullOperand <= UINT8_MAX)
	{
		*pu8Result = (UINT8)ullOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pu8Result = UINT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONGLONG -> BYTE conversion
//
#define RtlULongLongToByte RtlULongLongToUInt8

//
// ULONGLONG -> SHORT conversion
//
__inline
NTSTATUS
RtlULongLongToShort(
	__in ULONGLONG ullOperand,
	__out SHORT * psResult)
{
	NTSTATUS status;

	if (ullOperand <= SHORT_MAX)
	{
		*psResult = (SHORT)ullOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*psResult = SHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONGLONG -> INT16 conversion
//
#define RtlULongLongToInt16    RtlULongLongToShort

//
// ULONGLONG -> USHORT conversion
//
__inline
NTSTATUS
RtlULongLongToUShort(
	__in ULONGLONG ullOperand,
	__out USHORT * pusResult)
{
	NTSTATUS status;

	if (ullOperand <= USHORT_MAX)
	{
		*pusResult = (USHORT)ullOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pusResult = USHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONGLONG -> UINT16 conversion
//
#define RtlULongLongToUInt16   RtlULongLongToUShort

//
// ULONGLONG -> WORD conversion
//
#define RtlULongLongToWord RtlULongLongToUShort

//
// ULONGLONG -> INT conversion
//
__inline
NTSTATUS
RtlULongLongToInt(
	__in ULONGLONG ullOperand,
	__out INT * piResult)
{
	NTSTATUS status;

	if (ullOperand <= INT_MAX)
	{
		*piResult = (INT)ullOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*piResult = INT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONGLONG -> INT32 conversion
//
#define RtlULongLongToInt32    RtlULongLongToInt

//
// ULONGLONG -> INT_PTR conversion
//
#ifdef _WIN64
#define RtlULongLongToIntPtr   RtlULongLongToLongLong
#else
#define RtlULongLongToIntPtr   RtlULongLongToInt
#endif

//
// ULONGLONG -> UINT conversion
//
__inline
NTSTATUS
RtlULongLongToUInt(
	__in ULONGLONG ullOperand,
	__out UINT * puResult)
{
	NTSTATUS status;

	if (ullOperand <= UINT_MAX)
	{
		*puResult = (UINT)ullOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*puResult = UINT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONGLONG -> UINT32 conversion
//
#define RtlULongLongToUInt32   RtlULongLongToUInt

//
// ULONGLONG -> UINT_PTR conversion
//
#ifdef _WIN64
__inline
NTSTATUS
RtlULongLongToUIntPtr(
	__in ULONGLONG ullOperand,
	__out UINT_PTR * puResult)
{
	*puResult = ullOperand;
	return STATUS_SUCCESS;
}
#else
#define RtlULongLongToUIntPtr  RtlULongLongToUInt
#endif

//
// ULONGLONG -> LONG conversion
//
__inline
NTSTATUS
RtlULongLongToLong(
	__in ULONGLONG ullOperand,
	__out LONG * plResult)
{
	NTSTATUS status;

	if (ullOperand <= LONG_MAX)
	{
		*plResult = (LONG)ullOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*plResult = LONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONGLONG -> LONG_PTR conversion
//
__inline
NTSTATUS
RtlULongLongToLongPtr(
	__in ULONGLONG ullOperand,
	__out LONG_PTR * plResult)
{
	NTSTATUS status;

	if (ullOperand <= LONG_PTR_MAX)
	{
		*plResult = (LONG_PTR)ullOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*plResult = LONG_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONGLONG -> ULONG conversion
//
__inline
NTSTATUS
RtlULongLongToULong(
	__in ULONGLONG ullOperand,
	__out ULONG * pulResult)
{
	NTSTATUS status;

	if (ullOperand <= ULONG_MAX)
	{
		*pulResult = (ULONG)ullOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pulResult = ULONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONGLONG -> ULONG_PTR conversion
//
#ifdef _WIN64
__inline
NTSTATUS
RtlULongLongToULongPtr(
	__in ULONGLONG ullOperand,
	__out ULONG_PTR * pulResult)
{
	*pulResult = ullOperand;
	return STATUS_SUCCESS;
}
#else
#define RtlULongLongToULongPtr RtlULongLongToULong
#endif

//
// ULONGLONG -> DWORD conversion
//
#define RtlULongLongToDWord    RtlULongLongToULong

//
// ULONGLONG -> DWORD_PTR conversion
//
#define RtlULongLongToDWordPtr RtlULongLongToULongPtr

//
// ULONGLONG -> LONGLONG conversion
//
__inline
NTSTATUS
RtlULongLongToLongLong(
	__in ULONGLONG ullOperand,
	__out LONGLONG * pllResult)
{
	NTSTATUS status;

	if (ullOperand <= LONGLONG_MAX)
	{
		*pllResult = (LONGLONG)ullOperand;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pllResult = LONGLONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONGLONG -> RtlINT64 conversion
//
#define RtlULongLongToInt64    RtlULongLongToLongLong

//
// ULONGLONG -> LONG64 conversion
//
#define RtlULongLongToLong64   RtlULongLongToLongLong

//
// ULONGLONG -> ptrdiff_t conversion
//
#define RtlULongLongToPtrdiffT RtlULongLongToIntPtr

//
// ULONGLONG -> size_t conversion
//
#define RtlULongLongToSizeT    RtlULongLongToUIntPtr

//
// ULONGLONG -> SSIZE_T conversion
//
#define RtlULongLongToSSIZET   RtlULongLongToLongPtr

//
// ULONGLONG -> SIZE_T conversion
//
#define RtlULongLongToSIZET    RtlULongLongToULongPtr

//
// DWORDLONG -> CHAR conversion
//
#define RtlDWordLongToChar RtlULongLongToChar

//
// DWORDLONG -> INT8 conversion
//
#define RtlDWordLongToInt8 RtlULongLongToInt8

//
// DWORDLONG -> UCHAR conversion
//
#define RtlDWordLongToUChar    RtlULongLongToUChar

//
// DWORDLONG -> UINT8 conversion
//
#define RtlDWordLongToUInt8    RtlULongLongToUInt8

//
// DWORDLONG -> BYTE conversion
//
#define RtlDWordLongToByte RtlULongLongToUInt8

//
// DWORDLONG -> SHORT conversion
//
#define RtlDWordLongToShort    RtlULongLongToShort

//
// DWORDLONG -> INT16 conversion
//
#define RtlDWordLongToInt16    RtlULongLongToShort

//
// DWORDLONG -> USHORT conversion
//
#define RtlDWordLongToUShort   RtlULongLongToUShort

//
// DWORDLONG -> UINT16 conversion
//
#define RtlDWordLongToUInt16   RtlULongLongToUShort

//
// DWORDLONG -> WORD conversion
//
#define RtlDWordLongToWord RtlULongLongToUShort

//
// DWORDLONG -> INT conversion
//
#define RtlDWordLongToInt  RtlULongLongToInt

//
// DWORDLONG -> INT32 conversion
//
#define RtlDWordLongToInt32    RtlULongLongToInt

//
// DWORDLONG -> INT_PTR conversion
//
#define RtlDWordLongToIntPtr   RtlULongLongToIntPtr

//
// DWORDLONG -> UINT conversion
//
#define RtlDWordLongToUInt RtlULongLongToUInt

//
// DWORDLONG -> UINT32 conversion
//
#define RtlDWordLongToUInt32   RtlULongLongToUInt

//
// DWORDLONG -> UINT_PTR conversion
//
#define RtlDWordLongToUIntPtr  RtlULongLongToUIntPtr

//
// DWORDLONG -> LONG conversion
//
#define RtlDWordLongToLong RtlULongLongToLong

//
// DWORDLONG -> LONG_PTR conversion
//
#define RtlDWordLongToLongPtr  RtlULongLongToLongPtr

//
// DWORDLONG -> ULONG conversion
//
#define RtlDWordLongToULong    RtlULongLongToULong

//
// DWORDLONG -> ULONG_PTR conversion
//
#define RtlDWordLongToULongPtr RtlULongLongToULongPtr

//
// DWORDLONG -> DWORD conversion
//
#define RtlDWordLongToDWord    RtlULongLongToULong

//
// DWORDLONG -> DWORD_PTR conversion
//
#define RtlDWordLongToDWordPtr RtlULongLongToULongPtr

//
// DWORDLONG -> LONGLONG conversion
//
#define RtlDWordLongToLongLong RtlULongLongToLongLong

//
// DWORDLONG -> LONG64 conversion
//
#define RtlDWordLongToLong64   RtlULongLongToLongLong

//
// DWORDLONG -> RtlINT64 conversion
//
#define RtlDWordLongToInt64    RtlULongLongToLongLong

//
// DWORDLONG -> ptrdiff_t conversion
//
#define RtlDWordLongToPtrdiffT RtlULongLongToIntPtr

//
// DWORDLONG -> size_t conversion
//
#define RtlDWordLongToSizeT    RtlULongLongToUIntPtr

//
// DWORDLONG -> SSIZE_T conversion
//
#define RtlDWordLongToSSIZET   RtlULongLongToLongPtr

//
// DWORDLONG -> SIZE_T conversion
//
#define RtlDWordLongToSIZET    RtlULongLongToULongPtr

//
// ULONG64 -> CHAR conversion
//
#define RtlULong64ToChar   RtlULongLongToChar

//
// ULONG64 -> INT8 conversion
//
#define RtlULong64ToInt8   RtlULongLongToInt8

//
// ULONG64 -> UCHAR conversion
//
#define RtlULong64ToUChar  RtlULongLongToUChar

//
// ULONG64 -> UINT8 conversion
//
#define RtlULong64ToUInt8  RtlULongLongToUInt8

//
// ULONG64 -> BYTE conversion
//
#define RtlULong64ToByte   RtlULongLongToUInt8

//
// ULONG64 -> SHORT conversion
//
#define RtlULong64ToShort  RtlULongLongToShort

//
// ULONG64 -> INT16 conversion
//
#define RtlULong64ToInt16  RtlULongLongToShort

//
// ULONG64 -> USHORT conversion
//
#define RtlULong64ToUShort RtlULongLongToUShort

//
// ULONG64 -> UINT16 conversion
//
#define RtlULong64ToUInt16 RtlULongLongToUShort

//
// ULONG64 -> WORD conversion
//
#define RtlULong64ToWord   RtlULongLongToUShort

//
// ULONG64 -> INT conversion
//
#define RtlULong64ToInt    RtlULongLongToInt

//
// ULONG64 -> INT32 conversion
//
#define RtlULong64ToInt32  RtlULongLongToInt

//
// ULONG64 -> INT_PTR conversion
//
#define RtlULong64ToIntPtr RtlULongLongToIntPtr

//
// ULONG64 -> UINT conversion
//
#define RtlULong64ToUInt   RtlULongLongToUInt

//
// ULONG64 -> UINT32 conversion
//
#define RtlULong64ToUInt32 RtlULongLongToUInt

//
// ULONG64 -> UINT_PTR conversion
//
#define RtlULong64ToUIntPtr    RtlULongLongToUIntPtr

//
// ULONG64 -> LONG conversion
//
#define RtlULong64ToLong   RtlULongLongToLong

//
// ULONG64 -> LONG_PTR conversion
//
#define RtlULong64ToLongPtr    RtlULongLongToLongPtr

//
// ULONG64 -> ULONG conversion
//
#define RtlULong64ToULong  RtlULongLongToULong

//
// ULONG64 -> ULONG_PTR conversion
//
#define RtlULong64ToULongPtr   RtlULongLongToULongPtr

//
// ULONG64 -> DWORD conversion
//
#define RtlULong64ToDWord  RtlULongLongToULong

//
// ULONG64 -> DWORD_PTR conversion
//
#define RtlULong64ToDWordPtr   RtlULongLongToULongPtr

//
// ULONG64 -> LONGLONG conversion
//
#define RtlULong64ToLongLong   RtlULongLongToLongLong

//
// ULONG64 -> LONG64 conversion
//
#define RtlULong64ToLong64 RtlULongLongToLongLong

//
// ULONG64 -> RtlINT64 conversion
//
#define RtlULong64ToInt64  RtlULongLongToLongLong

//
// ULONG64 -> ptrdiff_t conversion
//
#define RtlULong64ToPtrdiffT   RtlULongLongToIntPtr

//
// ULONG64 -> size_t conversion
//
#define RtlULong64ToSizeT  RtlULongLongToUIntPtr

//
// ULONG64 -> SSIZE_T conversion
//
#define RtlULong64ToSSIZET RtlULongLongToLongPtr

//
// ULONG64 -> SIZE_T conversion
//
#define RtlULong64ToSIZET  RtlULongLongToULongPtr

//
// DWORD64 -> CHAR conversion
//
#define RtlDWord64ToChar   RtlULongLongToChar

//
// DWORD64 -> INT8 conversion
//
#define RtlDWord64ToInt8   RtlULongLongToInt8

//
// DWORD64 -> UCHAR conversion
//
#define RtlDWord64ToUChar  RtlULongLongToUChar

//
// DWORD64 -> UINT8 conversion
//
#define RtlDWord64ToUInt8  RtlULongLongToUInt8

//
// DWORD64 -> BYTE conversion
//
#define RtlDWord64ToByte   RtlULongLongToUInt8

//
// DWORD64 -> SHORT conversion
//
#define RtlDWord64ToShort  RtlULongLongToShort

//
// DWORD64 -> INT16 conversion
//
#define RtlDWord64ToInt16  RtlULongLongToShort

//
// DWORD64 -> USHORT conversion
//
#define RtlDWord64ToUShort RtlULongLongToUShort

//
// DWORD64 -> UINT16 conversion
//
#define RtlDWord64ToUInt16 RtlULongLongToUShort

//
// DWORD64 -> WORD conversion
//
#define RtlDWord64ToWord   RtlULongLongToUShort

//
// DWORD64 -> INT conversion
//
#define RtlDWord64ToInt    RtlULongLongToInt

//
// DWORD64 -> INT32 conversion
//
#define RtlDWord64ToInt32  RtlULongLongToInt

//
// DWORD64 -> INT_PTR conversion
//
#define RtlDWord64ToIntPtr RtlULongLongToIntPtr

//
// DWORD64 -> UINT conversion
//
#define RtlDWord64ToUInt   RtlULongLongToUInt

//
// DWORD64 -> UINT32 conversion
//
#define RtlDWord64ToUInt32 RtlULongLongToUInt

//
// DWORD64 -> UINT_PTR conversion
//
#define RtlDWord64ToUIntPtr    RtlULongLongToUIntPtr

//
// DWORD64 -> LONG conversion
//
#define RtlDWord64ToLong   RtlULongLongToLong

//
// DWORD64 -> LONG_PTR conversion
//
#define RtlDWord64ToLongPtr    RtlULongLongToLongPtr

//
// DWORD64 -> ULONG conversion
//
#define RtlDWord64ToULong  RtlULongLongToULong

//
// DWORD64 -> ULONG_PTR conversion
//
#define RtlDWord64ToULongPtr   RtlULongLongToULongPtr

//
// DWORD64 -> DWORD conversion
//
#define RtlDWord64ToDWord  RtlULongLongToULong

//
// DWORD64 -> DWORD_PTR conversion
//
#define RtlDWord64ToDWordPtr   RtlULongLongToULongPtr

//
// DWORD64 -> LONGLONG conversion
//
#define RtlDWord64ToLongLong   RtlULongLongToLongLong

//
// DWORD64 -> LONG64 conversion
//
#define RtlDWord64ToLong64 RtlULongLongToLongLong

//
// DWORD64 -> RtlINT64 conversion
//
#define RtlDWord64ToInt64  RtlULongLongToLongLong

//
// DWORD64 -> ptrdiff_t conversion
//
#define RtlDWord64ToPtrdiffT   RtlULongLongToIntPtr

//
// DWORD64 -> size_t conversion
//
#define RtlDWord64ToSizeT  RtlULongLongToUIntPtr

//
// DWORD64 -> SSIZE_T conversion
//
#define RtlDWord64ToSSIZET RtlULongLongToLongPtr

//
// DWORD64 -> SIZE_T conversion
//
#define RtlDWord64ToSIZET  RtlULongLongToULongPtr

//
// UINT64 -> CHAR conversion
//
#define RtlUInt64ToChar    RtlULongLongToChar

//
// UINT64 -> INT8 conversion
//
#define RtlUInt64ToInt8    RtlULongLongToInt8

//
// UINT64 -> UCHAR conversion
//
#define RtlUInt64ToUChar   RtlULongLongToUChar

//
// UINT64 -> UINT8 conversion
//
#define RtlUInt64ToUInt8   RtlULongLongToUInt8

//
// UINT64 -> BYTE conversion
//
#define RtlUInt64ToByte    RtlULongLongToUInt8

//
// UINT64 -> SHORT conversion
//
#define RtlUInt64ToShort   RtlULongLongToShort

//
// UINT64 -> INT16 conversion
//
//
#define RtlUInt64ToInt16   RtlULongLongToShort

//
// UINT64 -> USHORT conversion
//
#define RtlUInt64ToUShort  RtlULongLongToUShort

//
// UINT64 -> UINT16 conversion
//
#define RtlUInt64ToUInt16  RtlULongLongToUShort

//
// UINT64 -> WORD conversion
//
#define RtlUInt64ToWord    RtlULongLongToUShort

//
// UINT64 -> INT conversion
//
#define RtlUInt64ToInt RtlULongLongToInt

//
// UINT64 -> INT32 conversion
//
#define RtlUInt64ToInt32   RtlULongLongToInt

//
// UINT64 -> INT_PTR conversion
//
#define RtlUInt64ToIntPtr  RtlULongLongToIntPtr

//
// UINT64 -> UINT conversion
//
#define RtlUInt64ToUInt    RtlULongLongToUInt

//
// UINT64 -> UINT32 conversion
//
#define RtlUInt64ToUInt32  RtlULongLongToUInt

//
// UINT64 -> UINT_PTR conversion
//
#define RtlUInt64ToUIntPtr RtlULongLongToUIntPtr

//
// UINT64 -> LONG conversion
//
#define RtlUInt64ToLong    RtlULongLongToLong

//
// UINT64 -> LONG_PTR conversion
//
#define RtlUInt64ToLongPtr RtlULongLongToLongPtr

//
// UINT64 -> ULONG conversion
//
#define RtlUInt64ToULong   RtlULongLongToULong

//
// UINT64 -> ULONG_PTR conversion
//
#define RtlUInt64ToULongPtr    RtlULongLongToULongPtr

//
// UINT64 -> DWORD conversion
//
#define RtlUInt64ToDWord   RtlULongLongToULong

//
// UINT64 -> DWORD_PTR conversion
//
#define RtlUInt64ToDWordPtr    RtlULongLongToULongPtr

//
// UINT64 -> LONGLONG conversion
//
#define RtlUInt64ToLongLong    RtlULongLongToLongLong

//
// UINT64 -> LONG64 conversion
//
#define RtlUInt64ToLong64  RtlULongLongToLongLong

//
// UINT64 -> RtlINT64 conversion
//
#define RtlUInt64ToInt64   RtlULongLongToLongLong

//
// UINT64 -> ptrdiff_t conversion
//
#define RtlUInt64ToPtrdiffT    RtlULongLongToIntPtr

//
// UINT64 -> size_t conversion
//
#define RtlUInt64ToSizeT   RtlULongLongToUIntPtr

//
// UINT64 -> SSIZE_T conversion
//
#define RtlUInt64ToSSIZET  RtlULongLongToLongPtr

//
// UINT64 -> SIZE_T conversion
//
#define RtlUInt64ToSIZET  RtlULongLongToULongPtr

//
// ptrdiff_t -> CHAR conversion
//
#define RtlPtrdiffTToChar  RtlIntPtrToChar

//
// ptrdiff_t -> INT8 conversion
//
#define RtlPtrdiffTToInt8  RtlIntPtrToInt8

//
// ptrdiff_t -> UCHAR conversion
//
#define RtlPtrdiffTToUChar RtlIntPtrToUChar

//
// ptrdiff_t -> UINT8 conversion
//
#define RtlPtrdiffTToUInt8 RtlIntPtrToUInt8

//
// ptrdiff_t -> BYTE conversion
//
#define RtlPtrdiffTToByte  RtlIntPtrToUInt8

//
// ptrdiff_t -> SHORT conversion
//
#define RtlPtrdiffTToShort RtlIntPtrToShort

//
// ptrdiff_t -> INT16 conversion
//
#define RtlPtrdiffTToInt16 RtlIntPtrToShort

//
// ptrdiff_t -> USHORT conversion
//
#define RtlPtrdiffTToUShort    RtlIntPtrToUShort

//
// ptrdiff_t -> UINT16 conversion
//
#define RtlPtrdiffTToUInt16    RtlIntPtrToUShort

//
// ptrdiff_t -> WORD conversion
//
#define RtlPtrdiffTToWord  RtlIntPtrToUShort

//
// ptrdiff_t -> INT conversion
//
#define RtlPtrdiffTToInt   RtlIntPtrToInt

//
// ptrdiff_t -> INT32 conversion
//
#define RtlPtrdiffTToInt32 RtlIntPtrToInt

//
// ptrdiff_t -> UINT conversion
//
#define RtlPtrdiffTToUInt  RtlIntPtrToUInt

//
// ptrdiff_t -> UINT32 conversion
//
#define RtlPtrdiffTToUInt32    RtlIntPtrToUInt

//
// ptrdiff_t -> UINT_PTR conversion
//
#define RtlPtrdiffTToUIntPtr   RtlIntPtrToUIntPtr

//
// ptrdiff_t -> LONG conversion
//
#define RtlPtrdiffTToLong  RtlIntPtrToLong

//
// ptrdiff_t -> LONG_PTR conversion
//
#define RtlPtrdiffTToLongPtr   RtlIntPtrToLongPtr

//
// ptrdiff_t -> ULONG conversion
//
#define RtlPtrdiffTToULong RtlIntPtrToULong

//
// ptrdiff_t -> ULONG_PTR conversion
//
#define RtlPtrdiffTToULongPtr  RtlIntPtrToULongPtr

//
// ptrdiff_t -> DWORD conversion
//
#define RtlPtrdiffTToDWord RtlIntPtrToULong

//
// ptrdiff_t -> DWORD_PTR conversion
//
#define RtlPtrdiffTToDWordPtr  RtlIntPtrToULongPtr

//
// ptrdiff_t -> ULONGLONG conversion
//
#define RtlPtrdiffTToULongLong RtlIntPtrToULongLong

//
// ptrdiff_t -> DWORDLONG conversion
//
#define RtlPtrdiffTToDWordLong RtlIntPtrToULongLong

//
// ptrdiff_t -> ULONG64 conversion
//
#define RtlPtrdiffTToULong64   RtlIntPtrToULongLong

//
// ptrdiff_t -> DWORD64 conversion
//
#define RtlPtrdiffTToDWord64   RtlIntPtrToULongLong

//
// ptrdiff_t -> UINT64 conversion
//
#define RtlPtrdiffTToUInt64    RtlIntPtrToULongLong

//
// ptrdiff_t -> size_t conversion
//
#define RtlPtrdiffTToSizeT RtlIntPtrToUIntPtr

//
// ptrdiff_t -> SIZE_T conversion
//
#define RtlPtrdiffTToSIZET RtlIntPtrToULongPtr

//
// size_t -> INT8 conversion
//
#define RtlSizeTToInt8 RtlUIntPtrToInt8

//
// size_t -> UCHAR conversion
//
#define RtlSizeTToUChar    RtlUIntPtrToUChar

//
// size_t -> CHAR conversion
//
#define RtlSizeTToChar RtlUIntPtrToChar

//
// size_t -> UINT8 conversion
//
#define RtlSizeTToUInt8    RtlUIntPtrToUInt8

//
// size_t -> BYTE conversion
//
#define RtlSizeTToByte RtlUIntPtrToUInt8

//
// size_t -> SHORT conversion
//
#define RtlSizeTToShort    RtlUIntPtrToShort

//
// size_t -> INT16 conversion
//
#define RtlSizeTToInt16    RtlUIntPtrToShort

//
// size_t -> USHORT conversion
//
#define RtlSizeTToUShort   RtlUIntPtrToUShort

//
// size_t -> UINT16 conversion
//
#define RtlSizeTToUInt16   RtlUIntPtrToUShort

//
// size_t -> WORD
//
#define RtlSizeTToWord RtlUIntPtrToUShort

//
// size_t -> INT conversion
//
#define RtlSizeTToInt  RtlUIntPtrToInt

//
// size_t -> INT32 conversion
//
#define RtlSizeTToInt32    RtlUIntPtrToInt

//
// size_t -> INT_PTR conversion
//
#define RtlSizeTToIntPtr   RtlUIntPtrToIntPtr

//
// size_t -> UINT conversion
//
#define RtlSizeTToUInt RtlUIntPtrToUInt

//
// size_t -> UINT32 conversion
//
#define RtlSizeTToUInt32   RtlUIntPtrToUInt

//
// size_t -> LONG conversion
//
#define RtlSizeTToLong RtlUIntPtrToLong

//
// size_t -> LONG_PTR conversion
//
#define RtlSizeTToLongPtr  RtlUIntPtrToLongPtr

//
// size_t -> ULONG conversion
//
#define RtlSizeTToULong    RtlUIntPtrToULong

//
// size_t -> DWORD conversion
//
#define RtlSizeTToDWord    RtlUIntPtrToULong

//
// size_t -> LONGLONG conversion
//
#define RtlSizeTToLongLong RtlUIntPtrToLongLong

//
// size_t -> LONG64 conversion
//
#define RtlSizeTToLong64   RtlUIntPtrToLongLong

//
// size_t -> RtlINT64
//
#define RtlSizeTToInt64    RtlUIntPtrToLongLong

//
// size_t -> ptrdiff_t conversion
//
#define RtlSizeTToPtrdiffT RtlUIntPtrToIntPtr

//
// size_t -> SSIZE_T conversion
//
#define RtlSizeTToSSIZET   RtlUIntPtrToLongPtr

//
// SSIZE_T -> INT8 conversion
//
#define RtlSSIZETToInt8    RtlLongPtrToInt8

//
// SSIZE_T -> UCHAR conversion
//
#define RtlSSIZETToUChar   RtlLongPtrToUChar

//
// SSIZE_T -> CHAR conversion
//
#define RtlSSIZETToChar    RtlLongPtrToChar

//
// SSIZE_T -> UINT8 conversion
//
#define RtlSSIZETToUInt8   RtlLongPtrToUInt8

//
// SSIZE_T -> BYTE conversion
//
#define RtlSSIZETToByte    RtlLongPtrToUInt8

//
// SSIZE_T -> SHORT conversion
//
#define RtlSSIZETToShort   RtlLongPtrToShort

//
// SSIZE_T -> INT16 conversion
//
#define RtlSSIZETToInt16   RtlLongPtrToShort

//
// SSIZE_T -> USHORT conversion
//
#define RtlSSIZETToUShort  RtlLongPtrToUShort

//
// SSIZE_T -> UINT16 conversion
//
#define RtlSSIZETToUInt16  RtlLongPtrToUShort

//
// SSIZE_T -> WORD conversion
//
#define RtlSSIZETToWord    RtlLongPtrToUShort

//
// SSIZE_T -> INT conversion
//
#define RtlSSIZETToInt RtlLongPtrToInt

//
// SSIZE_T -> INT32 conversion
//
#define RtlSSIZETToInt32   RtlLongPtrToInt

//
// SSIZE_T -> INT_PTR conversion
//
#define RtlSSIZETToIntPtr  RtlLongPtrToIntPtr

//
// SSIZE_T -> UINT conversion
//
#define RtlSSIZETToUInt    RtlLongPtrToUInt

//
// SSIZE_T -> UINT32 conversion
//
#define RtlSSIZETToUInt32  RtlLongPtrToUInt

//
// SSIZE_T -> UINT_PTR conversion
//
#define RtlSSIZETToUIntPtr RtlLongPtrToUIntPtr

//
// SSIZE_T -> LONG conversion
//
#define RtlSSIZETToLong    RtlLongPtrToLong

//
// SSIZE_T -> ULONG conversion
//
#define RtlSSIZETToULong   RtlLongPtrToULong

//
// SSIZE_T -> ULONG_PTR conversion
//
#define RtlSSIZETToULongPtr    RtlLongPtrToULongPtr

//
// SSIZE_T -> DWORD conversion
//
#define RtlSSIZETToDWord   RtlLongPtrToULong

//
// SSIZE_T -> DWORD_PTR conversion
//
#define RtlSSIZETToDWordPtr    RtlLongPtrToULongPtr

//
// SSIZE_T -> ULONGLONG conversion
//
#define RtlSSIZETToULongLong   RtlLongPtrToULongLong

//
// SSIZE_T -> DWORDLONG conversion
//
#define RtlSSIZETToDWordLong   RtlLongPtrToULongLong

//
// SSIZE_T -> ULONG64 conversion
//
#define RtlSSIZETToULong64 RtlLongPtrToULongLong

//
// SSIZE_T -> DWORD64 conversion
//
#define RtlSSIZETToDWord64 RtlLongPtrToULongLong

//
// SSIZE_T -> UINT64 conversion
//
#define RtlSSIZETToUInt64  RtlLongPtrToULongLong

//
// SSIZE_T -> size_t conversion
//
#define RtlSSIZETToSizeT   RtlLongPtrToUIntPtr

//
// SSIZE_T -> SIZE_T conversion
//
#define RtlSSIZETToSIZET   RtlLongPtrToULongPtr

//
// SIZE_T -> INT8 conversion
//
#define RtlSIZETToInt8 RtlULongPtrToInt8

//
// SIZE_T -> UCHAR conversion
//
#define RtlSIZETToUChar    RtlULongPtrToUChar

//
// SIZE_T -> CHAR conversion
//
#define RtlSIZETToChar RtlULongPtrToChar

//
// SIZE_T -> UINT8 conversion
//
#define RtlSIZETToUInt8    RtlULongPtrToUInt8

//
// SIZE_T -> BYTE conversion
//
#define RtlSIZETToByte RtlULongPtrToUInt8

//
// SIZE_T -> SHORT conversion
//
#define RtlSIZETToShort    RtlULongPtrToShort

//
// SIZE_T -> INT16 conversion
//
#define RtlSIZETToInt16    RtlULongPtrToShort

//
// SIZE_T -> USHORT conversion
//
#define RtlSIZETToUShort   RtlULongPtrToUShort

//
// SIZE_T -> UINT16 conversion
//
#define RtlSIZETToUInt16   RtlULongPtrToUShort

//
// SIZE_T -> WORD
//
#define RtlSIZETToWord RtlULongPtrToUShort

//
// SIZE_T -> INT conversion
//
#define RtlSIZETToInt  RtlULongPtrToInt

//
// SIZE_T -> INT32 conversion
//
#define RtlSIZETToInt32    RtlULongPtrToInt

//
// SIZE_T -> INT_PTR conversion
//
#define RtlSIZETToIntPtr   RtlULongPtrToIntPtr

//
// SIZE_T -> UINT conversion
//
#define RtlSIZETToUInt RtlULongPtrToUInt

//
// SIZE_T -> UINT32 conversion
//
#define RtlSIZETToUInt32   RtlULongPtrToUInt

//
// SIZE_T -> UINT_PTR conversion
//
#define RtlSIZETToUIntPtr  RtlULongPtrToUIntPtr

//
// SIZE_T -> LONG conversion
//
#define RtlSIZETToLong RtlULongPtrToLong

//
// SIZE_T -> LONG_PTR conversion
//
#define RtlSIZETToLongPtr  RtlULongPtrToLongPtr

//
// SIZE_T -> ULONG conversion
//
#define RtlSIZETToULong    RtlULongPtrToULong

//
// SIZE_T -> DWORD conversion
//
#define RtlSIZETToDWord    RtlULongPtrToULong

//
// SIZE_T -> LONGLONG conversion
//
#define RtlSIZETToLongLong RtlULongPtrToLongLong

//
// SIZE_T -> LONG64 conversion
//
#define RtlSIZETToLong64   RtlULongPtrToLongLong

//
// SIZE_T -> RtlINT64
//
#define RtlSIZETToInt64    RtlULongPtrToLongLong

//
// SIZE_T -> ptrdiff_t conversion
//
#define RtlSIZETToPtrdiffT RtlULongPtrToIntPtr

//
// SIZE_T -> SSIZE_T conversion
//
#define RtlSIZETToSSIZET   RtlULongPtrToLongPtr

//=============================================================================
// Addition functions
//=============================================================================

//
// UINT8 addition
//
__inline
NTSTATUS
RtlUInt8Add(
	__in UINT8 u8Augend,
	__in UINT8 u8Addend,
	__out UINT8 * pu8Result)
{
	NTSTATUS status;

	if (((UINT8)(u8Augend + u8Addend)) >= u8Augend)
	{
		*pu8Result = (UINT8)(u8Augend + u8Addend);
		status = STATUS_SUCCESS;
	}
	else
	{
		*pu8Result = UINT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// USHORT addition
//
__inline
NTSTATUS
RtlUShortAdd(
	__in USHORT usAugend,
	__in USHORT usAddend,
	__out USHORT * pusResult)
{
	NTSTATUS status;

	if (((USHORT)(usAugend + usAddend)) >= usAugend)
	{
		*pusResult = (USHORT)(usAugend + usAddend);
		status = STATUS_SUCCESS;
	}
	else
	{
		*pusResult = USHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT16 addition
//
#define RtlUInt16Add   RtlUShortAdd

//
// WORD addtition
//
#define RtlWordAdd     RtlUShortAdd

//
// UINT addition
//
__inline
NTSTATUS
RtlUIntAdd(
	__in UINT uAugend,
	__in UINT uAddend,
	__out UINT * puResult)
{
	NTSTATUS status;

	if ((uAugend + uAddend) >= uAugend)
	{
		*puResult = (uAugend + uAddend);
		status = STATUS_SUCCESS;
	}
	else
	{
		*puResult = UINT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT32 addition
//
#define RtlUInt32Add   RtlUIntAdd

//
// UINT_PTR addition
//
#ifdef _WIN64
#define RtlUIntPtrAdd      RtlULongLongAdd
#else
__inline
NTSTATUS
RtlUIntPtrAdd(
	__in UINT_PTR uAugend,
	__in UINT_PTR uAddend,
	__out UINT_PTR * puResult)
{
	NTSTATUS status;

	if ((uAugend + uAddend) >= uAugend)
	{
		*puResult = (uAugend + uAddend);
		status = STATUS_SUCCESS;
	}
	else
	{
		*puResult = UINT_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif // _WIN64

//
// ULONG addition
//
__inline
NTSTATUS
RtlULongAdd(
	__in ULONG ulAugend,
	__in ULONG ulAddend,
	__out ULONG * pulResult)
{
	NTSTATUS status;

	if ((ulAugend + ulAddend) >= ulAugend)
	{
		*pulResult = (ulAugend + ulAddend);
		status = STATUS_SUCCESS;
	}
	else
	{
		*pulResult = ULONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONG_PTR addition
//
#ifdef _WIN64
#define RtlULongPtrAdd     RtlULongLongAdd
#else
__inline
NTSTATUS
RtlULongPtrAdd(
	__in ULONG_PTR ulAugend,
	__in ULONG_PTR ulAddend,
	__out ULONG_PTR * pulResult)
{
	NTSTATUS status;

	if ((ulAugend + ulAddend) >= ulAugend)
	{
		*pulResult = (ulAugend + ulAddend);
		status = STATUS_SUCCESS;
	}
	else
	{
		*pulResult = ULONG_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif // _WIN64

//
// DWORD addition
//
#define RtlDWordAdd        RtlULongAdd

//
// DWORD_PTR addition
//
#ifdef _WIN64
#define RtlDWordPtrAdd     RtlULongLongAdd
#else
__inline
NTSTATUS
RtlDWordPtrAdd(
	__in DWORD_PTR dwAugend,
	__in DWORD_PTR dwAddend,
	__out DWORD_PTR * pdwResult)
{
	NTSTATUS status;

	if ((dwAugend + dwAddend) >= dwAugend)
	{
		*pdwResult = (dwAugend + dwAddend);
		status = STATUS_SUCCESS;
	}
	else
	{
		*pdwResult = DWORD_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif // _WIN64

//
// size_t addition
//
__inline
NTSTATUS
RtlSizeTAdd(
	__in size_t Augend,
	__in size_t Addend,
	__out size_t * pResult)
{
	NTSTATUS status;

	if ((Augend + Addend) >= Augend)
	{
		*pResult = (Augend + Addend);
		status = STATUS_SUCCESS;
	}
	else
	{
		*pResult = SIZE_T_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// SIZE_T addition
//
#ifdef _WIN64
#define RtlSIZETAdd      RtlULongLongAdd
#else
__inline
NTSTATUS
RtlSIZETAdd(
	__in SIZE_T Augend,
	__in SIZE_T Addend,
	__out SIZE_T * pResult)
{
	NTSTATUS status;

	if ((Augend + Addend) >= Augend)
	{
		*pResult = (Augend + Addend);
		status = STATUS_SUCCESS;
	}
	else
	{
		*pResult = _SIZE_T_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif // _WIN64

//
// ULONGLONG addition
//
__inline
NTSTATUS
RtlULongLongAdd(
	__in ULONGLONG ullAugend,
	__in ULONGLONG ullAddend,
	__out ULONGLONG * pullResult)
{
	NTSTATUS status;

	if ((ullAugend + ullAddend) >= ullAugend)
	{
		*pullResult = (ullAugend + ullAddend);
		status = STATUS_SUCCESS;
	}
	else
	{
		*pullResult = ULONGLONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// DWORDLONG addition
//
#define RtlDWordLongAdd    RtlULongLongAdd

//
// ULONG64 addition
//
#define RtlULong64Add  RtlULongLongAdd

//
// DWORD64 addition
//
#define RtlDWord64Add  RtlULongLongAdd

//
// UINT64 addition
//
#define RtlUInt64Add   RtlULongLongAdd

//=============================================================================
// Subtraction functions
//=============================================================================

//
// UINT8 subtraction
//
__inline
NTSTATUS
RtlUInt8Sub(
	__in UINT8 u8Minuend,
	__in UINT8 u8Subtrahend,
	__out UINT8 * pu8Result)
{
	NTSTATUS status;

	if (u8Minuend >= u8Subtrahend)
	{
		*pu8Result = (UINT8)(u8Minuend - u8Subtrahend);
		status = STATUS_SUCCESS;
	}
	else
	{
		*pu8Result = UINT8_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// USHORT subtraction
//
__inline
NTSTATUS
RtlUShortSub(
	__in USHORT usMinuend,
	__in USHORT usSubtrahend,
	__out USHORT * pusResult)
{
	NTSTATUS status;

	if (usMinuend >= usSubtrahend)
	{
		*pusResult = (USHORT)(usMinuend - usSubtrahend);
		status = STATUS_SUCCESS;
	}
	else
	{
		*pusResult = USHORT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT16 subtraction
//
#define RtlUInt16Sub   RtlUShortSub

//
// WORD subtraction
//
#define RtlWordSub     RtlUShortSub

//
// UINT subtraction
//
__inline
NTSTATUS
RtlUIntSub(
	__in UINT uMinuend,
	__in UINT uSubtrahend,
	__out UINT * puResult)
{
	NTSTATUS status;

	if (uMinuend >= uSubtrahend)
	{
		*puResult = (uMinuend - uSubtrahend);
		status = STATUS_SUCCESS;
	}
	else
	{
		*puResult = UINT_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// UINT32 subtraction
//
#define RtlUInt32Sub   RtlUIntSub

//
// UINT_PTR subtraction
//
#ifdef _WIN64
#define RtlUIntPtrSub  RtlULongLongSub
#else
__inline
NTSTATUS
RtlUIntPtrSub(
	__in UINT_PTR uMinuend,
	__in UINT_PTR uSubtrahend,
	__out UINT_PTR * puResult)
{
	NTSTATUS status;

	if (uMinuend >= uSubtrahend)
	{
		*puResult = (uMinuend - uSubtrahend);
		status = STATUS_SUCCESS;
	}
	else
	{
		*puResult = UINT_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif // _WIN64

//
// ULONG subtraction
//
__inline
NTSTATUS
RtlULongSub(
	__in ULONG ulMinuend,
	__in ULONG ulSubtrahend,
	__out ULONG * pulResult)
{
	NTSTATUS status;

	if (ulMinuend >= ulSubtrahend)
	{
		*pulResult = (ulMinuend - ulSubtrahend);
		status = STATUS_SUCCESS;
	}
	else
	{
		*pulResult = ULONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// ULONG_PTR subtraction
//
#ifdef _WIN64
#define RtlULongPtrSub RtlULongLongSub
#else
__inline
NTSTATUS
RtlULongPtrSub(
	__in ULONG_PTR ulMinuend,
	__in ULONG_PTR ulSubtrahend,
	__out ULONG_PTR * pulResult)
{
	NTSTATUS status;

	if (ulMinuend >= ulSubtrahend)
	{
		*pulResult = (ulMinuend - ulSubtrahend);
		status = STATUS_SUCCESS;
	}
	else
	{
		*pulResult = ULONG_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif // _WIN64

//
// DWORD subtraction
//
#define RtlDWordSub        RtlULongSub

//
// DWORD_PTR subtraction
//
#ifdef _WIN64
#define RtlDWordPtrSub     RtlULongLongSub
#else
__inline
NTSTATUS
RtlDWordPtrSub(
	__in DWORD_PTR dwMinuend,
	__in DWORD_PTR dwSubtrahend,
	__out DWORD_PTR * pdwResult)
{
	NTSTATUS status;

	if (dwMinuend >= dwSubtrahend)
	{
		*pdwResult = (dwMinuend - dwSubtrahend);
		status = STATUS_SUCCESS;
	}
	else
	{
		*pdwResult = DWORD_PTR_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif // _WIN64

//
// size_t subtraction
//
__inline
NTSTATUS
RtlSizeTSub(
	__in size_t Minuend,
	__in size_t Subtrahend,
	__out size_t * pResult)
{
	NTSTATUS status;

	if (Minuend >= Subtrahend)
	{
		*pResult = (Minuend - Subtrahend);
		status = STATUS_SUCCESS;
	}
	else
	{
		*pResult = SIZE_T_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// SIZE_T subtraction
//
#ifdef _WIN64
#define RtlSIZETSub    RtlULongLongSub
#else
__inline
NTSTATUS
RtlSIZETSub(
	__in SIZE_T Minuend,
	__in SIZE_T Subtrahend,
	__out SIZE_T * pResult)
{
	NTSTATUS status;

	if (Minuend >= Subtrahend)
	{
		*pResult = (Minuend - Subtrahend);
		status = STATUS_SUCCESS;
	}
	else
	{
		*pResult = _SIZE_T_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}
#endif // _WIN64

//
// ULONGLONG subtraction
//
__inline
NTSTATUS
RtlULongLongSub(
	__in ULONGLONG ullMinuend,
	__in ULONGLONG ullSubtrahend,
	__out ULONGLONG * pullResult)
{
	NTSTATUS status;

	if (ullMinuend >= ullSubtrahend)
	{
		*pullResult = (ullMinuend - ullSubtrahend);
		status = STATUS_SUCCESS;
	}
	else
	{
		*pullResult = ULONGLONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}

	return status;
}

//
// DWORDLONG subtraction
//
#define RtlDWordLongSub    RtlULongLongSub

//
// ULONG64 subtraction
//
#define RtlULong64Sub  RtlULongLongSub

//
// DWORD64 subtraction
//
#define RtlDWord64Sub  RtlULongLongSub

//
// UINT64 subtraction
//
#define RtlUInt64Sub   RtlULongLongSub

//=============================================================================
// Multiplication functions
//=============================================================================

//
// UINT8 multiplication
//
__inline
NTSTATUS
RtlUInt8Mult(
	__in UINT8 u8Multiplicand,
	__in UINT8 u8Multiplier,
	__out UINT8 * pu8Result)
{
	UINT uResult = ((UINT)u8Multiplicand) * ((UINT)u8Multiplier);

	return RtlUIntToUInt8(uResult, pu8Result);
}

//
// USHORT multiplication
//
__inline
NTSTATUS
RtlUShortMult(
	__in USHORT usMultiplicand,
	__in USHORT usMultiplier,
	__out USHORT * pusResult)
{
	ULONG ulResult = ((ULONG)usMultiplicand) * ((ULONG)usMultiplier);

	return RtlULongToUShort(ulResult, pusResult);
}

//
// UINT16 multiplication
//
#define RtlUInt16Mult  RtlUShortMult

//
// WORD multiplication
//
#define RtlWordMult    RtlUShortMult

//
// UINT multiplication
//
__inline
NTSTATUS
RtlUIntMult(
	__in UINT uMultiplicand,
	__in UINT uMultiplier,
	__out UINT * puResult)
{
	ULONGLONG ull64Result = UInt32x32To64(uMultiplicand, uMultiplier);

	return RtlULongLongToUInt(ull64Result, puResult);
}

//
// UINT32 multiplication
//
#define RtlUInt32Mult  RtlUIntMult

//
// UINT_PTR multiplication
//
#ifdef _WIN64
#define RtlUIntPtrMult     RtlULongLongMult
#else
__inline
NTSTATUS
RtlUIntPtrMult(
	__in UINT_PTR uMultiplicand,
	__in UINT_PTR uMultiplier,
	__out UINT_PTR * puResult)
{
	ULONGLONG ull64Result = UInt32x32To64(uMultiplicand, uMultiplier);

	return RtlULongLongToUIntPtr(ull64Result, puResult);
}
#endif // _WIN64

//
// ULONG multiplication
//
__inline
NTSTATUS
RtlULongMult(
	__in ULONG ulMultiplicand,
	__in ULONG ulMultiplier,
	__out ULONG * pulResult)
{
	ULONGLONG ull64Result = UInt32x32To64(ulMultiplicand, ulMultiplier);

	return RtlULongLongToULong(ull64Result, pulResult);
}

//
// ULONG_PTR multiplication
//
#ifdef _WIN64
#define RtlULongPtrMult    RtlULongLongMult
#else
__inline
NTSTATUS
RtlULongPtrMult(
	__in ULONG_PTR ulMultiplicand,
	__in ULONG_PTR ulMultiplier,
	__out ULONG_PTR * pulResult)
{
	ULONGLONG ull64Result = UInt32x32To64(ulMultiplicand, ulMultiplier);

	return RtlULongLongToULongPtr(ull64Result, pulResult);
}
#endif // _WIN64

//
// DWORD multiplication
//
#define RtlDWordMult       RtlULongMult

//
// DWORD_PTR multiplication
//
#ifdef _WIN64
#define RtlDWordPtrMult    RtlULongLongMult
#else
__inline
NTSTATUS
RtlDWordPtrMult(
	__in DWORD_PTR dwMultiplicand,
	__in DWORD_PTR dwMultiplier,
	__out DWORD_PTR * pdwResult)
{
	ULONGLONG ull64Result = UInt32x32To64(dwMultiplicand, dwMultiplier);

	return RtlULongLongToDWordPtr(ull64Result, pdwResult);
}
#endif // _WIN64

//
// size_t multiplication
//

#ifdef _WIN64
#define RtlSizeTMult       RtlULongLongMult
#else
__inline
NTSTATUS
RtlSizeTMult(
	__in size_t Multiplicand,
	__in size_t Multiplier,
	__out size_t * pResult)
{
	ULONGLONG ull64Result = UInt32x32To64(Multiplicand, Multiplier);

	return RtlULongLongToSizeT(ull64Result, pResult);
}
#endif // _WIN64

//
// SIZE_T multiplication
//
#ifdef _WIN64
#define RtlSIZETMult       RtlULongLongMult
#else
__inline
NTSTATUS
RtlSIZETMult(
	__in SIZE_T Multiplicand,
	__in SIZE_T Multiplier,
	__out SIZE_T * pResult)
{
	ULONGLONG ull64Result = UInt32x32To64(Multiplicand, Multiplier);

	return RtlULongLongToSIZET(ull64Result, pResult);
}
#endif // _WIN64

//
// ULONGLONG multiplication
//
__inline
NTSTATUS
RtlULongLongMult(
	__in ULONGLONG ullMultiplicand,
	__in ULONGLONG ullMultiplier,
	__out ULONGLONG * pullResult)
{
	NTSTATUS status;
#ifdef _AMD64_
	ULONGLONG u64ResultHigh;
	ULONGLONG u64ResultLow;

	u64ResultLow = UnsignedMultiply128(ullMultiplicand, ullMultiplier, &u64ResultHigh);
	if (u64ResultHigh == 0)
	{
		*pullResult = u64ResultLow;
		status = STATUS_SUCCESS;
	}
	else
	{
		*pullResult = ULONGLONG_ERROR;
		status = STATUS_INTEGER_OVERFLOW;
	}
#else
	// 64x64 into 128 is like 32.32 x 32.32.
	//
	// a.b * c.d = a*(c.d) + .b*(c.d) = a*c + a*.d + .b*c + .b*.d
	// back in non-decimal notation where A=a*2^32 and C=c*2^32:
	// A*C + A*d + b*C + b*d
	// So there are four components to add together.
	//   result = (a*c*2^64) + (a*d*2^32) + (b*c*2^32) + (b*d)
	//
	// a * c must be 0 or there would be bits in the high 64-bits
	// a * d must be less than 2^32 or there would be bits in the high 64-bits
	// b * c must be less than 2^32 or there would be bits in the high 64-bits
	// then there must be no overflow of the resulting values summed up.

	ULONG dw_a;
	ULONG dw_b;
	ULONG dw_c;
	ULONG dw_d;
	ULONGLONG ad = 0;
	ULONGLONG bc = 0;
	ULONGLONG bd = 0;
	ULONGLONG ullResult = 0;

	status = STATUS_INTEGER_OVERFLOW;

	dw_a = (ULONG)(ullMultiplicand >> 32);
	dw_c = (ULONG)(ullMultiplier >> 32);

	// common case -- if high dwords are both zero, no chance for overflow
	if ((dw_a == 0) && (dw_c == 0))
	{
		dw_b = (DWORD)ullMultiplicand;
		dw_d = (DWORD)ullMultiplier;

		*pullResult = (((ULONGLONG)dw_b) * (ULONGLONG)dw_d);
		status = STATUS_SUCCESS;
	}
	else
	{
		// a * c must be 0 or there would be bits set in the high 64-bits
		if ((dw_a == 0) ||
			(dw_c == 0))
		{
			dw_d = (DWORD)ullMultiplier;

			// a * d must be less than 2^32 or there would be bits set in the high 64-bits
			ad = (((ULONGLONG)dw_a) * (ULONGLONG)dw_d);
			if ((ad & 0xffffffff00000000) == 0)
			{
				dw_b = (DWORD)ullMultiplicand;

				// b * c must be less than 2^32 or there would be bits set in the high 64-bits
				bc = (((ULONGLONG)dw_b) * (ULONGLONG)dw_c);
				if ((bc & 0xffffffff00000000) == 0)
				{
					// now sum them all up checking for overflow.
					// shifting is safe because we already checked for overflow above
					if (NT_SUCCESS(RtlULongLongAdd(bc << 32, ad << 32, &ullResult)))
					{
						// b * d
						bd = (((ULONGLONG)dw_b) * (ULONGLONG)dw_d);

						if (NT_SUCCESS(RtlULongLongAdd(ullResult, bd, &ullResult)))
						{
							*pullResult = ullResult;
							status = STATUS_SUCCESS;
						}
					}
				}
			}
		}
	}

	if (!NT_SUCCESS(status))
	{
		*pullResult = ULONGLONG_ERROR;
	}
#endif // _AMD64_

	return status;
}

//
// DWORDLONG multiplication
//
#define RtlDWordLongMult   RtlULongLongMult

//
// ULONG64 multiplication
//
#define RtlULong64Mult RtlULongLongMult

//
// DWORD64 multiplication
//
#define RtlDWord64Mult RtlULongLongMult

//
// UINT64 multiplication
//
#define RtlUInt64Mult  RtlULongLongMult

#endif // _NTINTSAFE_H_INCLUDED_
