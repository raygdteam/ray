#include "audio/audio.hpp"

extern "C" {
#include <kernel/kernel_file.h>
}

header::riff* audio_manager::load(const char* filename)
{
	void* buffer;
	unsigned long numBytes = 0;

	krnlFileRead(krnlFileOpen(filename, 0), &buffer, numBytes);
	header::riff* wav = (header::riff*)buffer;
	return wav;
}
