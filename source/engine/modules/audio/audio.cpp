#include "audio/audio.hpp"

extern "C"
{
#include <kernel\kernel_file.h>
}

AudioManager::AudioManager()
{
	CoInitialize(NULL); // see CoInitializeEx later.

    CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (LPVOID*)(&this->DeviceEnumerator));

    this->DeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &this->EndPoint);

    this->DeviceEnumerator->Release();

    this->EndPoint->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (LPVOID*)(&this->AudioClient));

    this->EndPoint->Release();
}

AudioManager::~AudioManager()
{
	CoUninitialize();
}

header::riff* AudioManager::Load(const char* filename)
{
	palFileSysSwitchToExeDirectory();

	auto file_handle = krnlFileOpen(filename, 0);
	auto file_size = GetFileSize(file_handle, 0);

	auto file_data = malloc(file_size); // it needs an alternative.

	if (krnlFileRead(file_handle, file_data, file_size) == HFILE_ERROR)
		free(file_data); // don't forget about file_handle and file_size!

	krnlFileClose(file_handle);

	return static_cast<header::riff*>(file_data);
}

void AudioManager::Play(header::riff* wave)
{
    auto wave_samples = &wave->samples;

    this->DeviceFormat.wFormatTag = WAVE_FORMAT_PCM;
    this->DeviceFormat.nChannels = 2;
    this->DeviceFormat.nSamplesPerSec = wave->sample_rate;
    this->DeviceFormat.wBitsPerSample = 16;
    this->DeviceFormat.nBlockAlign = (this->DeviceFormat.nChannels * this->DeviceFormat.wBitsPerSample) / 8;
    this->DeviceFormat.nAvgBytesPerSec = this->DeviceFormat.nSamplesPerSec * this->DeviceFormat.nBlockAlign;

    /*REFERENCE_TIME device_period;
    this->AudioClient->GetDevicePeriod(&device_period, nullptr);*/

    this->AudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, AUDCLNT_STREAMFLAGS_RATEADJUST | AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM | AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY /* flags are needed, otherwise the function will not work. */, 10000000000 * 2, 0, &this->DeviceFormat, nullptr);

    this->AudioClient->GetService(__uuidof(IAudioRenderClient), (LPVOID*)(&this->AudioRenderClient));

    UINT32 bufferSizeInFrames;
    this->AudioClient->GetBufferSize(&bufferSizeInFrames);

    this->AudioClient->Start();

    int wave_playback_sample = 0;

    while (true) // sound playback will be endless
    {
        unsigned int buffer_padding;
        this->AudioClient->GetCurrentPadding(&buffer_padding);

        unsigned int frames2write = bufferSizeInFrames / 50 - buffer_padding;

        int16_t* buffer;
        this->AudioRenderClient->GetBuffer(frames2write, (BYTE**)(&buffer));

        for (unsigned int frame_index = 0; frame_index < frames2write; ++frame_index)
        {
            *buffer++ = wave_samples[wave_playback_sample]; // left
            *buffer++ = wave_samples[wave_playback_sample]; // right

            ++wave_playback_sample;
            wave_playback_sample %= wave->data_bytes / (wave->num_channels * sizeof(short));
        }

        this->AudioRenderClient->ReleaseBuffer(frames2write, 0);
    }

    this->AudioClient->Stop();
    this->AudioClient->Release();

    this->AudioRenderClient->Release();
}
