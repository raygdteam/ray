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

	auto file_data = HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, file_size); // is this flag correct ?

	if (krnlFileRead(file_handle, file_data, file_size) == HFILE_ERROR)
		HeapFree(GetProcessHeap(), 0, file_data); // don't forget about file_handle and file_size!

	krnlFileClose(file_handle);

	return static_cast<header::riff*>(file_data);
}

void AudioManager::Play(header::riff* wave)
{
	/* in the future all this should be moved to the constructor */

	CoCreateInstance(CLSID_MMDeviceEnumerator, NULL, CLSCTX_ALL, IID_IMMDeviceEnumerator, (void**)&this->DeviceEnumerator);

	this->DeviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &this->Device);

	this->Device->Activate(IID_IAudioClient, CLSCTX_ALL, NULL, (void**)&this->AudioClient);

	this->AudioClient->GetMixFormat(&this->DeviceFormat);

	this->AudioClient->Initialize(AUDCLNT_SHAREMODE_SHARED, 0, 1000000 /* number of microseconds in a second */, 0, this->DeviceFormat, NULL);

	// this->AudioClient->GetBufferSize(&);

	this->AudioClient->GetService(IID_IAudioRenderClient, (void**)&this->AudioRenderClient);

	/* in the future all this should be moved to the constructor */
}
