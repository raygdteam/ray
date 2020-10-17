#include "audio/audio.hpp"

extern "C" {
#include <kernel/kernel_file.h>
}

class audio_manager
{
public:
	RAY_AUDIO_API header::riff* load(const char* filename);
};

header::riff* audio_manager::load(const char* filename)
{
	return (header::riff*)krnlFileOpen(filename, 0);
}
