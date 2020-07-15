#include "Sound.h"
#include <DxLib.h>

const int Sound::soundGet(std::string string)
{
	return sound[string];
}

Sound::Sound()
{
	sound.try_emplace("�{�^��", LoadSoundMem("image/button.mp3"));
	sound.try_emplace("�G���[", LoadSoundMem("image/bug.mp3"));
}

Sound::~Sound()
{
}
