#include "BaseScene.h"
#include "SceneMng.h"
#include "DxLib.h"
#include "../common/ImageMng.h"

BaseScene::BaseScene()
{

	IpImageMng.GetID("black", "image/black.png");							//˜g:‚í‚­- •¨‚Ì—ÖŠs‚â”ÍˆÍ
	IpImageMng.GetID("white", "image/white.png");
	_fadeScrID = MakeScreen(IpSceneMng.ScreenSize.x, IpSceneMng.ScreenSize.y, false);
	FadeInit("white");

}

BaseScene::~BaseScene()
{
}





void BaseScene::FadeInit(std::string fadeType)
{
	GetDrawScreenGraph(0, 0, IpSceneMng.ScreenSize.x, IpSceneMng.ScreenSize.y, _fadeScrID);
	IpSceneMng._blendCnt = 200;
	_fadeType = fadeType;
}

bool BaseScene::FadeUpdate(void)
{
	if (IpSceneMng._blendCnt)
	{
		IpSceneMng._blendCnt -= 5;
	}

	return IpSceneMng._blendCnt;
}

