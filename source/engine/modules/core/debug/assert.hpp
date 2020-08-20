#pragma once

#include "core/log/log.hpp"

#define stringize(argument) #argument

#ifdef NDEBUG
#define assert(condition, message)
#else
#define assert(condition, logger) condition ? logger->Log("[%s] assertion succeeded '%s'", stringize(__LINE__), #condition) : logger->Log("[%s] assertion failed '%s'", stringize(__LINE__), #condition)
#endif