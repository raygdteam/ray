#include <audio/audio.hpp>

void __Playground_Nikita()
{
	audio_manager audio_manager;
	auto oof = audio_manager.load("StarWars60.wav");

	oof->audio_format = 0;
}
