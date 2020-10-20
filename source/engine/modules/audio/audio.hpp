#pragma once

#include <audio/audio_def.hpp>

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
        short num_channels;
        int sample_rate;
        int byte_rate;
        short sample_alignment;
        short bit_depth;

        char data_header[4];
        int data_bytes;
    };
}

#include <audioclient.h>

class RAY_AUDIO_API AudioManager
{
public:
    AudioManager();

    header::riff* Load(const char* filename);

    void foo(header::riff* file);

private:
    IAudioClient* AudioClient;
    IAudioRenderClient* AudioRenderClient;
};
