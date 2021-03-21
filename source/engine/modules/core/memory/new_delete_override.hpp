#pragma once
#include <core/memory/memory_debug.hpp>

#ifndef DEBUG_MEMORY
#ifdef __cplusplus
#if defined(_RELEASE) && !defined(RAY_BUILD_CORE)
#else
#include <new>
#include "mimalloc/mimalloc.h"

inline void operator delete(void* p) noexcept { mi_free(p); };
inline void operator delete[](void* p) noexcept { mi_free(p); };

inline void* operator new(std::size_t n) noexcept(false) { return mi_new(n); }
inline void* operator new[](std::size_t n) noexcept(false) { return mi_new(n); }

inline void* operator new  (std::size_t n, const std::nothrow_t& tag) noexcept { (void)(tag); return mi_new_nothrow(n); }
inline void* operator new[](std::size_t n, const std::nothrow_t& tag) noexcept { (void)(tag); return mi_new_nothrow(n); }

#if (__cplusplus >= 201402L || _MSC_VER >= 1916)
inline void operator delete  (void* p, std::size_t n) noexcept { mi_free_size(p, n); };
inline void operator delete[](void* p, std::size_t n) noexcept { mi_free_size(p, n); };
#endif

#if (__cplusplus > 201402L || defined(__cpp_aligned_new))
inline void operator delete  (void* p, std::align_val_t al) noexcept { mi_free_aligned(p, static_cast<size_t>(al)); }
inline void operator delete[](void* p, std::align_val_t al) noexcept { mi_free_aligned(p, static_cast<size_t>(al)); }
inline void operator delete  (void* p, std::size_t n, std::align_val_t al) noexcept { mi_free_size_aligned(p, n, static_cast<size_t>(al)); };
inline void operator delete[](void* p, std::size_t n, std::align_val_t al) noexcept { mi_free_size_aligned(p, n, static_cast<size_t>(al)); };

inline void* operator new(std::size_t n, std::align_val_t al)   noexcept(false) { return mi_new_aligned(n, static_cast<size_t>(al)); }
inline void* operator new[](std::size_t n, std::align_val_t al) noexcept(false) { return mi_new_aligned(n, static_cast<size_t>(al)); }
inline void* operator new  (std::size_t n, std::align_val_t al, const std::nothrow_t&) noexcept { return mi_new_aligned_nothrow(n, static_cast<size_t>(al)); }
inline void* operator new[](std::size_t n, std::align_val_t al, const std::nothrow_t&) noexcept { return mi_new_aligned_nothrow(n, static_cast<size_t>(al)); }
#endif

#endif
#endif
#endif
