#pragma once

#pragma warning(push)
// This internal implementation file which can use internal API functions
#pragma warning(disable: 4996)

/// <summary>
/// Non recommended function
/// </summary>
#define DeprecatedCall(Why) __declspec(deprecated(Why))

/// <summary>
/// Internal API, using directly is forbidden
/// </summary>
#define InternalApiCall __declspec(deprecated("Internal API"))

/// <summary>
/// Marking only for virtual class using MSVC extension
/// </summary>
#define VirtualClass class __declspec(novtable) 

/// <summary>
/// Marking only for virtual structure using MSVC extension
/// </summary>
#define VirtualStruct struct __declspec(novtable) 

/// <summary>
/// Marking interface class using MSVC extension
/// </summary>
#define interface __interface

/// <summary>
/// Static variable constructor signature
/// </summary>
// ReSharper disable once CppUnnamedNamespaceInHeaderFile
namespace {
    typedef void (__cdecl* PF)();
}

/// <summary>
/// Static variable destructor signature
/// </summary>
extern int __cdecl __RegisterStaticDtors(PF pf);

#pragma warning(push)
#pragma warning(disable: 4075)

#pragma init_seg(".INIT$m", __RegisterStaticDtors)

#pragma comment(linker, "/MERGE:.INIT=.xdata") 
#define __INIT__
#pragma warning(pop)

/// <summary>
/// Exports but within kernel. Substitude function body instead of call
/// </summary>
/// <param name = "FuncSig">Function signature</param>
/// <param name = "Body">Function body</param>
#define OptimizeExport(FunctSig, Body) __forceinline FunctSig Body

#define EXPAND_VA_ARGS(x) x

/// <summary>
/// A readonly property
/// </summary>
#define __Property1(Getter) __declspec( property( get=Getter) )

/// <summary>
/// A read and write property
/// </summary>
#define __Property2(Getter, Setter) __declspec( property( get=Getter, put=Setter ) )

#define __Property_GET_MACRO(_1,_2,NAME,...) NAME

/// <summary>
/// A read property; read and write if only "getter" in given; write if it is "getter" and "setter"
/// </summary>
/// <param name = "__VA_ARGS__">"getter" or "getter" and "setter"</param>
#define Property(...) EXPAND_VA_ARGS(__Property_GET_MACRO(__VA_ARGS__, __Property2, __Property1)(__VA_ARGS__))

/// <summary>
/// Informs that function/method result can be ignored
/// </summary>
#define UnusedData [[maybe_unused]]

/// <summary>
/// Informs that function/method result can be ignored
/// </summary>
#define MaybeUnused [[maybe_unused]]

/// <summary>
/// Informs that function/method result can not be ignored
/// </summary>
#define NoDiscard [[nodiscard]]

/// <summary>
/// Indent relatively to the beginning of structure to the field
/// </summary>
/// <param name = "s">Structure type</param>
/// <param name = "m">Field name</param>
/// <returns>"SizeT" - a number which is point to the field indent in bytes relatively to the beginning of structure</returns>
#define StructOffsetOfMember(s,m) \
    __builtin_offsetof(s, m)

/// <summary>
/// Forced lambda code insertion "#define InlineLambda" [[msvs::forceinline]]
/// </summary>
#define InlineLambda [[msvc::forceinline]]

/// <summary>
/// Forced function code insertion
/// </summary>
#define ForceInline __forceinline

/// <summary>
/// A guarantee for the compiler to the uniqueness of the variable address
/// </summary>
#define Restrict __restrict

/// <summary>
/// Arrow function without arguments
/// </summary>
#define __ArrowFunct1(ret) \
    []() InlineLambda { return ret; }

/// <summary>
/// Arrow function with one argument
/// </summary>
#define __ArrowFunct2(x1, ret) \
    [](auto x1) InlineLambda { return ret; }

/// <summary>
/// Arrow function with two arguments
/// </summary>
#define __ArrowFunct3(x1, x2, ret) \
    [](auto x1, auto x2) InlineLambda { return ret; }

/// <summary>
/// Arrow function with three arguments
/// </summary>
#define __ArrowFunct4(x1, x2, x3, ret) \
    [](auto x1, auto x2, auto x3) InlineLambda { return ret; }

/// <summary>
/// Arrow function with four arguments
/// </summary>
#define __ArrowFunct5(x1, x2, x3, x4, ret) \
    [](auto x1, auto x2, auto x3, auto x4) InlineLambda { return ret; }

/// <summary>
/// Arrow function with five arguments
/// </summary>
#define __ArrowFunct6(x1, x2 ,x3, x4, x5, ret) \
    [](auto x1, auto x2, auto x3, auto x4, auto x5) InlineLambda { return ret; }

/// <summary>
/// Arrow function with six arguments
/// </summary>
#define __ArrowFunct7(x1, x2, x3, x4, x5, x6, ret) \
    [](auto x1, auto x2, auto x3, auto x4, auto x5, auto x6) InlineLambda { return ret; }

/// <summary>
/// Arrow function with seven arguments
/// </summary>
#define __ArrowFunct8(x1, x2, x3, x4, x5, x6, x7, ret) \
    [](auto x1, auto x2, auto x3, auto x4, auto x5, auto x6, auto x7) InlineLambda { return ret; }

/// <summary>
/// Arrow function with eight arguments
/// </summary>
#define __ArrowFunct9(x1, x2, x3, x4 ,x5, x6, x7, x8, ret) \
    [](auto x1, auto x2, auto x3, auto x4, auto x5, auto x6, auto x7, auto x8) InlineLambda { return ret; }

/// <summary>
/// Arrow function with nine arguments
/// </summary>
#define __ArrowFunct10(x1, x2, x3, x4, x5, x6, x7, x8, x9, ret) \
    [](auto x1, auto x2, auto x3, auto x4, auto x5, auto x6, auto x7, auto x8, auto x9) InlineLambda { return ret; }

/// <summary>
/// Arrow function with ten arguments
/// </summary>
#define __ArrowFunct11(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, ret) \
    [](auto x1, auto x2, auto x3, auto x4, auto x5, auto x6, auto x7, auto x8, auto x9, auto x10) InlineLambda { return ret; }

/// <summary>
/// An expression for defining the "define" overloading
/// </summary>
#define __ArrowFunct_GET_MACRO(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11, NAME,...) NAME

/// <summary>
/// Arrow function
/// </summary>
/// <param name = "__VA_ARGS__">An exspression and arguments</param>
#define ArrowFunct(...) EXPAND_VA_ARGS( \
    __ArrowFunct_GET_MACRO(__VA_ARGS__, \
    __ArrowFunct11,                     \
    __ArrowFunct10,                     \
    __ArrowFunct9,                      \
    __ArrowFunct8,                      \
    __ArrowFunct7,                      \
    __ArrowFunct6,                      \
    __ArrowFunct5,                      \
    __ArrowFunct4,                      \
    __ArrowFunct3,                      \
    __ArrowFunct2,                      \
    __ArrowFunct1)                      \
    (__VA_ARGS__))

/// <summary>
/// Arrow function without arguments
/// </summary>
#define __ArrowFunctWithCapture1(ret) \
    [&]() InlineLambda { return ret; }

/// <summary>
/// Arrow function with one argument
/// </summary>
#define __ArrowFunctWithCapture2(x1, ret) \
    [&](auto x1) InlineLambda { return ret; }

/// <summary>
/// Arrow function with two arguments
/// </summary>
#define __ArrowFunctWithCapture3(x1, x2, ret) \
    [&](auto x1, auto x2) InlineLambda { return ret; }

/// <summary>
/// Arrow function with three arguments
/// </summary>
#define __ArrowFunctWithCapture4(x1, x2, x3, ret) \
    [&](auto x1, auto x2, auto x3) InlineLambda { return ret; }

/// <summary>
/// Arrow function with four arguments
/// </summary>
#define __ArrowFunctWithCapture5(x1, x2, x3, x4, ret) \
    [&](auto x1, auto x2, auto x3, auto x4) InlineLambda { return ret; }

/// <summary>
/// Arrow function with five arguments
/// </summary>
#define __ArrowFunctWithCapture6(x1, x2 ,x3, x4, x5, ret) \
    [&](auto x1, auto x2, auto x3, auto x4, auto x5) InlineLambda { return ret; }

/// <summary>
/// Arrow function with six arguments
/// </summary>
#define __ArrowFunctWithCapture7(x1, x2, x3, x4, x5, x6, ret) \
    [&](auto x1, auto x2, auto x3, auto x4, auto x5, auto x6) InlineLambda { return ret; }

/// <summary>
/// Arrow function with seven arguments
/// </summary>
#define __ArrowFunctWithCapture8(x1, x2, x3, x4, x5, x6, x7, ret) \
    [&](auto x1, auto x2, auto x3, auto x4, auto x5, auto x6, auto x7) InlineLambda { return ret; }

/// <summary>
/// Arrow function with eight arguments
/// </summary>
#define __ArrowFunctWithCapture9(x1, x2, x3, x4 ,x5, x6, x7, x8, ret) \
    [&](auto x1, auto x2, auto x3, auto x4, auto x5, auto x6, auto x7, auto x8) InlineLambda { return ret; }

/// <summary>
/// Arrow function with nine arguments
/// </summary>
#define __ArrowFunctWithCapture10(x1, x2, x3, x4, x5, x6, x7, x8, x9, ret) \
    [&](auto x1, auto x2, auto x3, auto x4, auto x5, auto x6, auto x7, auto x8, auto x9) InlineLambda { return ret; }

/// <summary>
/// Arrow function with ten arguments
/// </summary>
#define __ArrowFunctWithCapture11(x1, x2, x3, x4, x5, x6, x7, x8, x9, x10, ret) \
    [&](auto x1, auto x2, auto x3, auto x4, auto x5, auto x6, auto x7, auto x8, auto x9, auto x10) InlineLambda { return ret; }

/// <summary>
/// An expression for defining the "define" overloading
/// </summary>
#define __ArrowFunctWithCapture_GET_MACRO(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,_11, NAME,...) NAME

/// <summary>
/// Arrow function with variable capturing
/// </summary>
/// <param name = "__VA_ARGS__">An exspression and arguments</param>
#define ArrowFunctWithCapture(...) EXPAND_VA_ARGS( \
    __ArrowFunctWithCapture_GET_MACRO(__VA_ARGS__, \
    __ArrowFunctWithCapture11,                     \
    __ArrowFunctWithCapture10,                     \
    __ArrowFunctWithCapture9,                      \
    __ArrowFunctWithCapture8,                      \
    __ArrowFunctWithCapture7,                      \
    __ArrowFunctWithCapture6,                      \
    __ArrowFunctWithCapture5,                      \
    __ArrowFunctWithCapture4,                      \
    __ArrowFunctWithCapture3,                      \
    __ArrowFunctWithCapture2,                      \
    __ArrowFunctWithCapture1)                      \
    (__VA_ARGS__))

/// <summary>
/// Converting char to wchar_t
/// </summary>
#define _L(x)  __L(x)
#define __L(x)  L##x

/// <summary>
/// Exception localization macros
/// </summary>
#define SourcePosition {_L(__FILE__), (Cardinal::SizeT) __LINE__}

/// <summary> 
/// Use for definition code block in which some warnings are disabled 
/// </summary>
#define Unsafe(...) { do {              \
    __pragma(warning(push));          \
    __pragma(warning(disable: 4996)); \
    __VA_ARGS__;                      \
    __pragma(warning(pop));           \
} while(false); }

#define __ELEVENTH_ARGUMENT( \
a1, a2, a3, a4, a5, a6, a7, a8, a9, a10,  \
a11, a12, a13, a14, a15, a16, a17, a18, a19, a20,  \
a21, a22, a23, a24, a25, a26, a27, a28, a29, a30,  \
a31, a32, a33, a34, a35, a36, a37, a38, a39, a40,  \
a41, a42, a43, a44, a45, a46, a47, a48, a49, a50,  \
a51, a52, a53, a54, a55, a56, a57, a58, a59, a60,  \
a61, a62, a63, a64, a65, a66, ...) a66

/// <summary> 
/// Calculate parameter pack size in C defines
/// </summary>
#define COUNT_ARGUMENTS(...) __ELEVENTH_ARGUMENT(dummy, ## __VA_ARGS__,  \
64, 63, 62, 61, 60,                                                      \
59, 58, 57, 56, 55, 54, 53, 52, 51, 50,                                  \
49, 48, 47, 46, 45, 44, 43, 42, 41, 40,                                  \
39, 38, 37, 36, 35, 34, 33, 32, 31, 30,                                  \
29, 28, 27, 26, 25, 24, 23, 22, 21, 20,                                  \
19, 18, 17, 16, 15, 14, 13, 12, 11, 10,                                  \
9, 8, 7, 6, 5, 4, 3, 2, 1, 0) 

/// <summary> 
/// Helper for converting token to string
/// </summary>
#define __VAL(str) #str

/// <summary> 
/// Convert token to string
/// </summary>
#define __TOSTRING(str) __VAL(str)

#pragma warning(pop)

/// <summary>
/// If "RefCountClass" is inserted into a class body, the given class automatically got reference count
/// </summary>
/// <param name = "ClassName">Class name</param>
/// <param name = "This">A pointer in which is inserted a copy data, must contain a variable "RefCount" of "RefCountT" type</param>
/// <remarks>
/// "Define" adds a constructor, addition operator, moving operator and destructor
/// </remarks>
#define RefCountClass(ClassName, This)                                                                                                                                                    \
/**<summary>Copying constructor</summary><param name = "Orig">Original</param>*/                                                                                                          \
__forceinline ClassName(const ClassName & Orig) :This(Orig.This) { if(This != nullptr) ++This->RefCount; };                                                                               \
/**<summary>Moving constructor</summary><param name = "Orig">Original</param>*/                                                                                                           \
__forceinline ClassName(ClassName && From) : This(From.This) { From.This = nullptr; };                                                                                                    \
/**<summary>Copying operator</summary><param name = "Orig">Original</param><returns>Reference to the current object</returns>*/                                                           \
__forceinline ClassName & operator=(const ClassName & Orig){this->~ClassName(); this->ClassName::ClassName(Orig); return *this; };                                                        \
/**<summary>Moving operator</summary><param name = "Orig">Original</param><returns>Reference to the current object</returns>*/                                                            \
__forceinline ClassName & operator=(ClassName && From){if(this == &From) return *this; this->~ClassName(); this->ClassName::ClassName(Cardinal::MoveRValue(From)); return *this; }; \
/**<summary>Checks if objects are references on same data objects</summary>*/                                                                                                             \
/**<returns>True if objects are references on same data objects</returns>*/                                                                                                               \
__forceinline bool IsSameAs(const ClassName& other) const { return this->This == other.This; }                                                                                            \
/**<summary>Destructor</summary>*/                                                                                                                                                        \
__forceinline virtual ~ClassName(){if (This != nullptr) {if (This->RefCount.DecrementAndCheckIsZero()) { this->Dispose(); delete This; } This = nullptr;}}