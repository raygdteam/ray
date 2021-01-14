#include "common.hpp"

u32 Crc32(u8* buffer, u64 len)
{
    u32 hash = 0;

    for (u64 i = 0; i < len; ++i)
    {
        hash = _mm_crc32_u8(hash, buffer[i]);
    }

    return hash;
}
