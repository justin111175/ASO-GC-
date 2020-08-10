#include "EffectMng.h"

#include "DxLib.h"

#include <EffekseerForDxlib.h>

void EffectMng::Updata(void)
{
	UpdateEffekseer2D();
	auto itr = std::remove_if(playList.begin(), playList.end(), [](int handle) {return !IsEffekseer2DEffectPlaying(handle); });
	playList.erase(itr, playList.end());

}

void EffectMng::Draw(void)
{
	DrawEffekseer2D();
}

bool EffectMng::StopAll(void)
{
	for (auto handle : playList)
	{
		StopEffekseer2DEffect(handle);
	}


	return false;
}

bool EffectMng::Play(std::string name, const Vector2& pos,Vector2 offest)
{

	playList.push_front(PlayEffekseer2DEffect(GetHandle(name)));
	SetPosPlayingEffekseer2DEffect(*(playList.begin()), static_cast<float>(pos.x)+offest.x+24, static_cast<float>(pos.y) + offest.y+24,0);

	return true;
}

bool EffectMng::Init(Vector2&& size)
{
	if (Effekseer_Init(8000) == -1)
	{
		return false;
	}
	SetChangeScreenModeGraphicsSystemResetFlag(FALSE);
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	Effekseer_Set2DSetting(size.x, size.y);
	SetUseZBuffer3D(TRUE);
	SetWriteZBuffer3D(TRUE);



	return true;
}

int EffectMng::GetHandle(std::string name)
{
	handles.try_emplace(name, LoadEffekseerEffect(("effect/" + name + ".efk").c_str(), 10.0f));
	return handles[name];
}

EffectMng::EffectMng()
{
}

EffectMng::~EffectMng()
{
}
