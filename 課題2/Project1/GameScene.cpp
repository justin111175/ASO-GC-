#include <Dxlib.h>
#include <_DeBugConOut.h>
#include <GameScene.h>
#include <Vector2.h>
#include <SceneMng.h>
#include <utility>
#include "GameOver.h"
#include "EffectMng.h"
GameScene::GameScene()
{	


	playerState.emplace_back(std::make_unique<State>(std::move (Vector2(100, 50)), std::move(Vector2(700+100, 722))));

	playerState.emplace_back(std::make_unique<State>(std::move (Vector2(800,50)), std::move(Vector2(700+800, 722))));


}


GameScene::~GameScene()
{

}

unique_Base GameScene::Update(unique_Base own)
{
	Draw();
	
	// 無返事参照、const参照、shared_ptr三つの方法がある
	for (auto&& state : playerState)
	{
		state->Run();
		if (state->OverFlag)
		{
			return std::make_unique<GameOver>();

		}

	}
	IpEffect.Updata();



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

	//for (auto&& state : playerState)
	//{
	//	DrawGraph(0, 0,state->GetScreenId(), true);
	//}
	
	DrawGraph(0, 0, playerState[0]->GetScreenId(), true);
	
	DrawGraph(0, 0, playerState[1]->GetScreenId(), true);
	
	IpEffect.Draw();

	ScreenFlip();

}

