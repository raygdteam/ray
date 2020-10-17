#pragma once

#include "defines.hpp"

namespace header
{
    struct riff
    {
        char riff_header[4];
        int wav_size;
        char wave_header[4];

        char fmt_header[3];
        int fmt_chunk_size;
        short audio_format;
        short channels;
        int sample_rate;
        int byte_rate;
        short alignment;
        short bit_depth;

        char data_header[4];
        int data_bytes;
    };
}
