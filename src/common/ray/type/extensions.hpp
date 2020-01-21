#ifndef __RAY_TYPE_EXTENSIONS_HPP__
#define __RAY_TYPE_EXTENSIONS_HPP__

#include <cstdint>
#include <string>

typedef int8_t		s8;
typedef int16_t		s16;
typedef int32_t		s32;
typedef int64_t		s64;
typedef uint8_t		u8;
typedef uint16_t	u16;
typedef uint32_t	u32;
typedef uint64_t	u64;

typedef const char* pcstr;

#ifdef UNICODE
using ray_string = std::wstring;
#else
using ray_string = std::string;
#endif // UNICODE


#endif