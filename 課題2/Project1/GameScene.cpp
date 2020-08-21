#include <Dxlib.h>
#include <_DeBugConOut.h>
#include <GameScene.h>
#include "TitleScene.h"
#include <Vector2.h>
#include <SceneMng.h>
#include <utility>
#include "EffectMng.h"
GameScene::GameScene()
{	


	playerState.emplace_back(std::make_unique<State>(std::move (Vector2(100, 50)), std::move(Vector2(700+100, 722))));

	playerState.emplace_back(std::make_unique<State>(std::move (Vector2(800,50)), std::move(Vector2(700+800, 722))));

	cnt_ = 100;

}


GameScene::~GameScene()
{

}

unique_Base GameScene::Update(unique_Base own)
{
	Draw();

	// 無返事参照、const参照、shared_ptr三つの方法がある
	if (cnt_ <= 0)
	{
		for (auto&& state : playerState)
		{
			if (!state->OverFlag)
			{
				state->Run();

			}
			else
			{
				if (CheckHitKey(KEY_INPUT_SPACE))
				{
					return std::make_unique<TitleScene>();

				}
			}


		}
		if (playerState[0]->OverFlag)
		{
			playerState[1]->OverFlag = true;
		}
		if (playerState[1]->OverFlag)
		{
			playerState[0]->OverFlag = true;
		}
	}
	else
	{
		cnt_--;

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

