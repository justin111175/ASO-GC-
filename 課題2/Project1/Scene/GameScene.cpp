#include <Dxlib.h>
#include <_DeBugConOut.h>
#include "GameScene.h"
#include "TitleScene.h"
#include <Vector2.h>
#include "SceneMng.h"
#include <utility>
#include "EffectMng.h"
#include "../common/ImageMng.h"
GameScene::GameScene()
{	


	playerState.emplace_back(std::make_unique<State>(std::move (Vector2(450, 50)), std::move(Vector2(700+450, 722))));
	
	IpImageMng.GetID("space", "image/space.png", { 570,40 }, { 1,1 });

	cnt_ = 0;

}


GameScene::~GameScene()
{

}

unique_Base GameScene::Update(unique_Base own)
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

	DrawGraph(0, 0, IMAGE_ID("BG")[0], true);

	
	DrawGraph(0, 0, playerState[0]->GetScreenId(), true);


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

void GameScene::ObjDraw(void)
{
}

