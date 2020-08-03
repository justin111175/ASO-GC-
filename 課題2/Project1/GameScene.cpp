#include <Dxlib.h>
#include <_DeBugConOut.h>
#include <GameScene.h>
#include <Vector2.h>
#include <SceneMng.h>
#include <utility>
#include "GameOver.h"
GameScene::GameScene()
{	


	playerState.emplace_back(std::make_unique<State>(std::move (Vector2(100, 100)), std::move(Vector2(1280, 800))/*,PlayerCtl::Player_1*/));
	//playerState.emplace_back(std::make_unique<State>(std::move (Vector2(600, 0)), std::move(Vector2(1280, 800))/*,PlayerCtl::Player_1*/));


}


GameScene::~GameScene()
{

}

unique_Base GameScene::Update(unique_Base own)
{
	Draw();

	// ���Ԏ��Q�ƁAconst�Q�ƁAshared_ptr�O�̕��@������
	for (auto&& state : playerState)
	{
		state->Run();
		if (state->OverFlag)
		{
			return std::make_unique<GameOver>();

		}

	}

	
	return std::move(own);
}

void GameScene::Draw(void)
{
	ClsDrawScreen();

	for (auto&& state : playerState)
	{
		state->Draw();
	}



	SetDrawScreen(DX_SCREEN_BACK);

	for (auto&& state : playerState)
	{
		DrawGraph(0, 0,state->GetScreenId(), true);
	}
	
	//DrawGraph(0, 300, playerState[0]->GetScreenId(), true);
	//DrawGraph(0, 100, playerState[1]->GetScreenId(), true);

	ScreenFlip();

}

