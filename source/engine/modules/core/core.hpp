#pragma once
#include <cstdint>

#define RAY_VERSION_MAJOR "0"
#define RAY_VERSION_MINOR "1"
#define RAY_VERSION_PATCH "0"

#define RAY_VERSION_CODENAME "Primal"

#define BIT(value) 1 << value

#ifdef _DEBUG
#define RAY_DEBUG
#endif

#ifdef _DEVELOPMENT
#define RAY_DEVELOPMENT
#endif

#ifdef _RELEASE
#define RAY_RELEASE
#endif

#ifdef __clang__
#define RAY_COMPILER_CLANG
#endif //__clang__

#ifdef __GNUC__
	#define RAY_COMPILER_GCC
#endif //__GNUC__

#ifdef __INTEL_COMPILER
#define RAY_COMPILER_INTEL
#endif //__INTEL_COMPILER

#ifdef _MSC_VER
#define RAY_COMPILER_MSVC
#endif //_MSC_VER

#ifdef __APPLE__
#error "Ray Engine doesn't support Apple Platforms!"
#endif // __APPLE__

#ifdef __ANDROID__
#error "Ray Engine doesn't support Android Platform!"
#endif //__ANDROID__

#ifdef _WIN32
	#ifdef _WIN64
		#define RAY_PLATFORM_WIN64
	#else
		#define RAY_PLATFORM_WIN32
	#endif //_WIN64
#endif //_WIN32

#if defined (RAY_PLATFORM_WIN32) || defined (RAY_PLATFORM_WIN64)
#define RAY_PLATFORM_WIN
#endif 

#ifdef __linux__
	#define RAY_PLATFORM_LINUX
#endif //__linux__

#ifdef _DEBUG
	#define RAY_ASSERT(cond, msg) // assert(cond && msg);
#else
	#define RAY_ASSERT(cond, msg)
#endif

#if defined(RAY_DEBUG) || defined(RAY_DEVELOPMENT)
#define RAY_DLLEXPORTS __declspec(dllexport)
#define RAY_DLLIMPORT __declspec(dllimport)
#define RAY_STATIC 0
#else
#define RAY_DLLEXPORTS
#define RAY_DLLIMPORT
#define RAY_STATIC 1
#endif

#ifdef RAY_BUILD_CORE
#define RAY_CORE_API RAY_DLLEXPORTS
#else
#define RAY_CORE_API RAY_DLLIMPORT
#endif

/* 
	On windows, remove the dependecy on api-ms-win-crt-*.dll libraries,
	as well as support OS < 10 
*/
#ifdef RAY_PLATFORM_WIN
#pragma comment(lib, "mincore_downlevel.lib")
#endif

// Misc defines
#define DEPRECATED(version, message) [[deprecated(message)]]

// Warnings stuff
#pragma warning(disable: 4251)
#pragma warning(disable: 4275) // C4275: non dll-interface struct 'A' used as base for dll-interface class 'B'
#pragma warning(disable: 4595) // C4595: 'operator xxx': non-member operator new or delete functions may not be declared inline


// Types
using u8  = uint8_t;  using s8  = int8_t;
using u16 = uint16_t; using s16 = int16_t;
using u32 = uint32_t; using s32 = int32_t;
using u64 = uint64_t; using s64 = int64_t;

using pcstr = const char*;
using pcwstr = const wchar_t*;
