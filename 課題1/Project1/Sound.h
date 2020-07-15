#pragma once
#include <map>
#include <string>

#define _sound Sound::GetInstance()

class Sound
{
public:
	static Sound& GetInstance()
	{
		static Sound s_Instance;
		return s_Instance;
	}
	const int soundGet(std::string sound);

private:
	std::map<std::string, int> sound;			// ‰¹—p•Ï”
	Sound();
	~Sound();

};

