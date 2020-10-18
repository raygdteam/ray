#include "audio/audio.hpp"

#include <audioclient.h>
#include <mmdeviceapi.h>

extern "C" {
#include <kernel/kernel_file.h>
}

header::riff* audio_manager::load(const char* filename)
{
	palFileSysSwitchToExeDirectory();
	
	void* buffer = static_cast<void*>(new u8[sizeof(header::riff)]);
	// unsigned long numBytes = 0;

	void* file = krnlFileOpen(filename, 0);
	krnlFileRead(file, buffer, sizeof(header::riff));

	return static_cast<header::riff*>(buffer);
}

void audio_manager::foo(header::riff* file)
{
    uint32_t numWavSamples = file->dataChunkSize / (file->numChannels * sizeof(uint16_t));
    uint16_t* wavSamples = &file->samples;

    HRESULT hr = CoInitializeEx(nullptr, COINIT_SPEED_OVER_MEMORY);

    IMMDeviceEnumerator* deviceEnumerator;
    hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (LPVOID*)(&deviceEnumerator));

    IMMDevice* audioDevice;
    hr = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &audioDevice);

    deviceEnumerator->Release();

    IAudioClient* audioClient;
    hr = audioDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (LPVOID*)(&audioClient));

    audioDevice->Release();

    WAVEFORMATEX* defaultMixFormat = NULL;
    hr = audioClient->GetMixFormat(&defaultMixFormat);

    WAVEFORMATEX mixFormat = {};
    mixFormat.wFormatTag = WAVE_FORMAT_PCM;
    mixFormat.nChannels = 2;
    mixFormat.nSamplesPerSec = file->sampleRate;
    mixFormat.wBitsPerSample = 16;
    mixFormat.nBlockAlign = (mixFormat.nChannels * mixFormat.wBitsPerSample) / 8;
    mixFormat.nAvgBytesPerSec = mixFormat.nSamplesPerSec * mixFormat.nBlockAlign;

    const int64_t REFTIMES_PER_SEC = 10000000; // hundred nanoseconds
    REFERENCE_TIME requestedSoundBufferDuration = REFTIMES_PER_SEC * 2;
    DWORD initStreamFlags = (AUDCLNT_STREAMFLAGS_RATEADJUST
        | AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM
        | AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY);
    hr = audioClient->Initialize(AUDCLNT_SHAREMODE_SHARED,
        initStreamFlags,
        requestedSoundBufferDuration,
        0, &mixFormat, nullptr);

    IAudioRenderClient* audioRenderClient;
    hr = audioClient->GetService(__uuidof(IAudioRenderClient), (LPVOID*)(&audioRenderClient));

    UINT32 bufferSizeInFrames;
    hr = audioClient->GetBufferSize(&bufferSizeInFrames);

    hr = audioClient->Start();

    int wavPlaybackSample = 0;

    while (true)
    {
        // Padding is how much valid data is queued up in the sound buffer
        // if there's enough padding then we could skip writing more data
        UINT32 bufferPadding;
        hr = audioClient->GetCurrentPadding(&bufferPadding);

        UINT32 soundBufferLatency = bufferSizeInFrames / 50;
        UINT32 numFramesToWrite = soundBufferLatency - bufferPadding;

        int16_t* buffer;
        hr = audioRenderClient->GetBuffer(numFramesToWrite, (BYTE**)(&buffer));

        for (UINT32 frameIndex = 0; frameIndex < numFramesToWrite; ++frameIndex)
        {
            *buffer++ = wavSamples[wavPlaybackSample]; // left
            *buffer++ = wavSamples[wavPlaybackSample]; // right

            ++wavPlaybackSample;
            wavPlaybackSample %= numWavSamples;
        }

        hr = audioRenderClient->ReleaseBuffer(numFramesToWrite, 0);
    }

    audioClient->Stop();
    audioClient->Release();
    audioRenderClient->Release();
}
