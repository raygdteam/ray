#include "audio/audio.hpp"

#include <audioclient.h>
#include <mmdeviceapi.h>

extern "C" {
#include <kernel/kernel_file.h>
#include <stdio.h>
}

AudioManager::AudioManager()
{
    this->HResult = CoInitialize(nullptr);

    this->AudioClient = 0;

    this->AudioRenderClient = 0;
}

header::riff* AudioManager::Load(const char* filename)
{
	palFileSysSwitchToExeDirectory();
	
	void* buffer = static_cast<void*>(new u8[4'608'116]); // hardcod3d
	// unsigned long numBytes = 0;

	void* file = krnlFileOpen(filename, 0);
	krnlFileRead(file, buffer, 4'608'116);

	return static_cast<header::riff*>(buffer);
}

void AudioManager::foo(header::riff* file)
{
    uint32_t numWavSamples = file->dataChunkSize / (file->numChannels * sizeof(uint16_t));
    uint16_t* wavSamples = &file->samples;

    IMMDeviceEnumerator* deviceEnumerator;
    this->HResult = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (LPVOID*)(&deviceEnumerator));

    IMMDevice* audioDevice;
    this->HResult = deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &audioDevice);

    deviceEnumerator->Release();

    this->HResult = audioDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, nullptr, (LPVOID*)(&this->AudioClient));

    audioDevice->Release();

    WAVEFORMATEX mixFormat = {}; // see WAVEFORMATEXTENSIBLE later
    mixFormat.wFormatTag = WAVE_FORMAT_PCM;
    mixFormat.nChannels = 2;
    mixFormat.nSamplesPerSec = file->sampleRate;
    mixFormat.wBitsPerSample = 16;
    mixFormat.nBlockAlign = (mixFormat.nChannels * mixFormat.wBitsPerSample) / 8;
    mixFormat.nAvgBytesPerSec = mixFormat.nSamplesPerSec * mixFormat.nBlockAlign;

    const int64_t REFTIMES_PER_SEC = 10000000; 

    REFERENCE_TIME requestedSoundBufferDuration = REFTIMES_PER_SEC * 2;

    DWORD initStreamFlags = (AUDCLNT_STREAMFLAGS_RATEADJUST
        | AUDCLNT_STREAMFLAGS_AUTOCONVERTPCM
        | AUDCLNT_STREAMFLAGS_SRC_DEFAULT_QUALITY);

    this->HResult = this->AudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, initStreamFlags, requestedSoundBufferDuration, 0, &mixFormat, nullptr);

    this->HResult = this->AudioClient->GetService(__uuidof(IAudioRenderClient), (LPVOID*)(&this->AudioRenderClient));

    UINT32 bufferSizeInFrames;
    this->HResult = this->AudioClient->GetBufferSize(&bufferSizeInFrames);

    this->HResult = this->AudioClient->Start();

    int wavPlaybackSample = 0;

    while (true)
    {
        UINT32 bufferPadding; // по нормальному bufferPadding не должен быть равен нулю (после GetCurrentPadding он остается неизменным, значит дело в audioclient)
        this->HResult = this->AudioClient->GetCurrentPadding(&bufferPadding);

        UINT32 soundBufferLatency = bufferSizeInFrames / 50;
        UINT32 numFramesToWrite = soundBufferLatency - bufferPadding;

        int16_t* buffer;
        this->HResult = this->AudioRenderClient->GetBuffer(numFramesToWrite, (BYTE**)(&buffer));

        for (UINT32 frameIndex = 0; frameIndex < numFramesToWrite; ++frameIndex)
        {
            *buffer++ = wavSamples[wavPlaybackSample]; // left
            *buffer++ = wavSamples[wavPlaybackSample]; // right

            ++wavPlaybackSample;
            wavPlaybackSample %= numWavSamples;
        }

        this->HResult = this->AudioRenderClient->ReleaseBuffer(numFramesToWrite, 0);
    }

    this->AudioClient->Stop();
    this->AudioClient->Release();

    this->AudioRenderClient->Release();
}
