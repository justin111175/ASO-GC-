#include "GameScene2.h"
#include <Dxlib.h>
#include <_DeBugConOut.h>
#include "TitleScene.h"
#include <Vector2.h>
#include "SceneMng.h"
#include <utility>
#include "EffectMng.h"
#include "../common/ImageMng.h"
#include "../common/Input/PadInput.h"
#include "../common/Input/KeyInput.h"
#include "MeanScene.h"
#include "GameOverScene.h"

// 二人プレイモード
GameScene2::GameScene2()
{
	playerState.emplace_back(std::make_unique<State>(std::move(Vector2(100, 50)), std::move(Vector2(700 + 100, 800))));

	playerState.emplace_back(std::make_unique<State>(std::move(Vector2(800, 50)), std::move(Vector2(700 + 800, 800))));
	
	for (int i = 0; i < 2; i++)
	{
		controller[i].try_emplace(conType::Key, std::make_unique<KeyInput>());
		controller[i][conType::Key]->SetUp(i);
		controller[i].try_emplace(conType::Pad, std::make_unique<PadInput>());
		controller[i][conType::Pad]->SetUp(i);

	}
	IpImageMng.GetID("space", "image/space.png", { 570,40 }, { 1,1 });
	IpImageMng.GetID("勝負画像", "image/win.png", { 260,150 }, { 1,2 });

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
					return std::make_unique<GameOverScene>(std::move(own), playerState);
				}
			}
		}
		for (auto&& conType_ : conType())
		{
			if (MeanCtl(conType_))
			{
				return std::make_unique<MeanScene>(std::move(own));
			}
		}
	}
	IpEffect.Updata();
	return std::move(own);
}



void GameScene2::BaseDraw(void)
{
	DrawGraph(0, 0, IMAGE_ID("BG")[0], true);


	for (auto&& state : playerState)
	{
		state->Draw();
	}




	SetDrawScreen(DX_SCREEN_BACK);

	for (auto&& state : playerState)
	{
		DrawGraph(0, 0,state->GetScreenId(), true);
	}


}

void GameScene2::Draw(void)
{
	ClsDrawScreen();


	BaseDraw();
	IpEffect.Draw();

	if (IpSceneMng._blendCnt)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, IpSceneMng._blendCnt);
		DrawGraph(0, 0, IpImageMng.GetID(_fadeType)[0], true);
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
	}
	ScreenFlip();

}