#pragma once

#include <audio/audio_def.hpp>

namespace header
{
    struct riff
    {
        /*char riff_header[4];
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
        short samples;*/

        // RIFF Chunk
        uint32_t riffId;
        uint32_t riffChunkSize;
        uint32_t waveId;

        // fmt Chunk
        uint32_t fmtId;
        uint32_t fmtChunkSize;
        uint16_t formatCode;
        uint16_t numChannels;
        uint32_t sampleRate;
        uint32_t byteRate;
        uint16_t blockAlign;
        uint16_t bitsPerSample;
        // These are not present for PCM Wav Files
        // uint16_t cbSize;
        // uint16_t wValidBitsPerSample;
        // uint32_t dwChannelMask;
        // char subFormatGUID[16];

        // data Chunk
        uint32_t dataId;
        uint32_t dataChunkSize;
        uint16_t samples; // actual samples start here
    };
}

class RAY_AUDIO_API audio_manager
{
public:
    header::riff* load(const char* filename);

    void foo(header::riff* file);
};
