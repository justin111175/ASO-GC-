#pragma once
#include "common/Vector2.h"
#include <String>
#include <map>
#include <list>

#define IpEffect EffectMng::GetInstance()

class EffectMng
{
public:
	static EffectMng& GetInstance()
	{
		static EffectMng s_Instance;
		return s_Instance;
	}

	void Updata(void);
	void Draw(void);
	bool StopAll(void);
	bool Play(std::string name, const Vector2& pos, Vector2 offest);
	bool Init(Vector2&& size);

private:
	int GetHandle(std::string name);
	std::map<std::string, int> handles;
	std::list<int> playList;
	EffectMng();
	~EffectMng();
};