#include "audio/audio.hpp"

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
	header::riff* wav = (header::riff*)buffer;
	return wav;
}
