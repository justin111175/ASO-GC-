#include "SceneMng.h"
#include "common/Vector2.h"
#include "GameScene.h"


SceneMng* SceneMng::sInstance = nullptr;


Vector2 testA = { 40,60 };
void SceneMng::Run(void)
{
	if (!SysInit())
	{
		AST();
	}

	_activeScene = std::make_unique<GameScene>();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{

		_activeScene = (*_activeScene).Update(std::move(_activeScene));


		_frames++;
		_timeCount.Run();
	}

}

const int SceneMng::frames(void) const
{
    return _frames;
}




bool SceneMng::SysInit(void)
{
	SetWindowText("ぷよぷよ");											// タイトルバー
	SetGraphMode(ScreenSize.x, ScreenSize.y, 16);
	ChangeWindowMode(true);												// ture:window false:フルスクリーン
	if (DxLib_Init() == -1)												// DXラリブラリの初期化
	{
		return false;
	}


	return true;

}


SceneMng::~SceneMng()
{	

}

SceneMng::SceneMng():
	ScreenSize(1280,800)
{
	_frames = 0;

}
