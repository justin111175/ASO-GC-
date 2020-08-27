#include <Dxlib.h>
#include <_DeBugConOut.h>
#include "GameScene.h"
#include "TitleScene.h"
#include <Vector2.h>
#include "SceneMng.h"
#include <utility>
#include "EffectMng.h"
#include "../common/ImageMng.h"
#include "../common/Input/PadInput.h"
#include "MeanScene.h"

GameScene::GameScene()
{


	playerState.emplace_back(std::make_unique<State>(std::move(Vector2(450, 50)), std::move(Vector2(700 + 450, 722))));

	IpImageMng.GetID("space", "image/space.png", { 570,40 }, { 1,1 });

	controller.try_emplace(conType::Pad, std::make_unique<PadInput>());

	controller[conType::Pad]->SetUp(0);

	cnt_ = 0;
	gameMean_ = GameMean::タイトルに戻る;
	meanFlag_ = false;
	sceneFlag_ = false;
	overFlag_ = false;
}


GameScene::~GameScene()
{

}

unique_Base GameScene::Update(unique_Base own)
{

	// 無返事参照、const参照、shared_ptr三つの方法がある

	Draw();
	MeanCtl();

	if (!FadeUpdate())
	{
		if (!meanFlag_)
		{
			for (auto&& state : playerState)
			{
				if (!_timeCount.GetFlag("待つ"))
				{
					state->Run();

				}

				if (state->overFlag_)
				{
					overFlag_ = true;
				}
				if (state->sceneFlag_)
				{
					return std::make_unique<TitleScene>();

				}

			}
		}
	}

	if (CheckHitKey(KEY_INPUT_F1))
	{
		return std::make_unique<MeanScene>(std::move(own));

	}

	if (sceneFlag_)
	{
		return std::make_unique<TitleScene>();
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



	if (overFlag_)
	{

		SetFontSize(50);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);
		DrawBox(0,0,IpSceneMng.ScreenSize.x,IpSceneMng.ScreenSize.y, 0x000000, true);
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

		DrawString(515, 300, "GAME OVER", 0xFFFFFF, 0x000000);
		//DrawGraph(_offset.x+60 , _offset.y+200, overImage_[static_cast<int>(winFlag_)], true);


		SetDrawBlendMode(DX_BLENDMODE_ALPHA, sin((double)IpSceneMng.frames() / 10) * 250);
		DrawGraph(350, 500, IMAGE_ID("space")[0], true);

		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
	}

	
	
	if (meanFlag_)
	{
		GameMeanDraw();
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


