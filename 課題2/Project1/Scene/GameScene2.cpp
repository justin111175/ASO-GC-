#include "GameScene2.h"
#include <Dxlib.h>
#include <_DeBugConOut.h>
#include "TitleScene.h"
#include <Vector2.h>
#include "SceneMng.h"
#include <utility>
#include "EffectMng.h"
#include "../common/ImageMng.h"

GameScene2::GameScene2()
{


	playerState.emplace_back(std::make_unique<State>(std::move(Vector2(100, 50)), std::move(Vector2(700 + 100, 722))));

	playerState.emplace_back(std::make_unique<State>(std::move(Vector2(800, 50)), std::move(Vector2(700 + 800, 722))));

	cnt_ = 100;
	IpImageMng.GetID("space", "image/space.png", { 570,40 }, { 1,1 });

}


GameScene2::~GameScene2()
{

}

unique_Base GameScene2::Update(unique_Base own)
{
	Draw();

	// 無返事参照、const参照、shared_ptr三つの方法がある
	if (!FadeUpdate())
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




	IpEffect.Updata();




	return std::move(own);
}

void GameScene2::Draw(void)
{
	ClsDrawScreen();
	DrawGraph(0, 0, IMAGE_ID("BG")[0], true);

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

	for (auto&& state : playerState)
	{
		if (state->OverFlag)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, sin((double)IpSceneMng.frames() / 10) * 250);
			DrawGraph(350, 500, IMAGE_ID("space")[0], true);

			SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
		}
	}
	IpEffect.Draw();


	if (IpSceneMng._blendCnt)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, IpSceneMng._blendCnt);
		DrawGraph(0, 0, IpImageMng.GetID(_fadeType)[0], true);
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
	}
	ScreenFlip();

}