#include "audio/audio.hpp"

extern "C"
{
#include <kernel\kernel_file.h>
}

AudioManager::AudioManager()
{
	CoInitialize(NULL);
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

	auto file_data = malloc(file_size);

	if (krnlFileRead(file_handle, file_data, file_size) == HFILE_ERROR)
		free(file_data); // don't forget about file_handle and file_size!

	krnlFileClose(file_handle);

	return static_cast<header::riff*>(file_data);
}

void AudioManager::Play(header::riff* wave)
{
	/* in the future all this should be moved to the constructor */

	CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&this->DeviceEnumerator);

	this->DeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &this->EndPoint);

	this->EndPoint->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&this->AudioClient);

	this->AudioClient->GetMixFormat(&this->DeviceFormat);

	this->AudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, 1000000 /* number of microseconds in a second */, 0, this->DeviceFormat, NULL);

	// this->AudioClient->GetBufferSize(&);

	this->AudioClient->GetService(__uuidof(IAudioRenderClient), (void**)&this->AudioRenderClient);

	/* in the future all this should be moved to the constructor */
}
