#include "pch.hpp"

#include <memory>
#include <spdlog/spdlog.hpp>

void* operator new(size_t size)
{
    void* p = malloc(size);
    //spdlog::debug("memory: Allocated {}({} bytes)", p, size);
    return p;
}

void operator delete(void* p)
{
    //spdlog::debug("memory: freed {}", p);
    free(p);
}