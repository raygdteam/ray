#pragma once

#include <audio/audio_def.hpp>

namespace header
{
    struct riff
    {
        // riff chunk
        char riff_header[4];
        int wave_size;
        char wave_header[4];

        // fmt chunk
        char fmt_header[3];
        int fmt_chunk_size;
        short audio_format;
        short num_channels;
        int sample_rate;
        int byte_rate;
        short sample_alignment;
        short bit_depth;

        // data chunk
        char data_header[4];
        int data_bytes;
        short samples;
    };
}

#include <mmdeviceapi.h>
#include <audioclient.h>

class RAY_AUDIO_API AudioManager
{
public:
    AudioManager();
    ~AudioManager();

    header::riff* Load(const char* filename);

    void Play(header::riff* wave);

private:
    IMMDeviceEnumerator* DeviceEnumerator;
    IMMDevice* EndPoint;

    WAVEFORMATEX DeviceFormat = {}; // see WAVEFORMATEXTENSIBLE later.

    IAudioClient* AudioClient;
    IAudioRenderClient* AudioRenderClient;
};
