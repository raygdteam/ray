#pragma once
#include <ray/type/extensions.hpp>

#define RAY_VERSION_MAJOR "0"
#define RAY_VERSION_MINOR "1"
#define RAY_VERSION_PATCH "0"

#define RAY_VERSION_CODENAME "Ray"

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
#error "Ray Engine don't support Apple Platforms!"
#endif // __APPLE__

#ifdef __ANDROID__
#error "Ray Engine don't support Android Platform!"
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

#if defined(RAY_BUILD_DEBUG) || defined(RAY_BUILD_DEVELOPMENT)
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

// Misc defines
#define DEPRECATED(version, message) [[deprecated(message)]]

// Warnings stuff
#pragma warning(disable: 4251)
