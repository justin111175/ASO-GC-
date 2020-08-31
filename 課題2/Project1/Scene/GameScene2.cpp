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
#include "MeanScene.h"
#include "GameOverScene.h"

// 二人プレイモード
GameScene2::GameScene2()
{
	playerState.emplace_back(std::make_unique<State>(std::move(Vector2(100, 50)), std::move(Vector2(700 + 100, 800))));

	playerState.emplace_back(std::make_unique<State>(std::move(Vector2(800, 50)), std::move(Vector2(700 + 800, 800))));
	
	controller.try_emplace(conType::Pad, std::make_unique<PadInput>());

	controller[conType::Pad]->SetUp(0);

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

				if (state->sceneFlag_)
				{
					return std::make_unique<TitleScene>();

				}
				if (state->overFlag_)
				{
					return std::make_unique<GameOverScene>(std::move(own), playerState.size());
				}

			}
		}
	}
	(*controller[conType::Pad])();

	for (auto data : controller[conType::Pad]->GetCntData(0))
	{
		if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
		{

			if (data.first == InputID::Mean)
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

	//DrawGraph(0, 0, playerState[0]->GetScreenId(), true);

	//DrawGraph(0, 0, playerState[1]->GetScreenId(), true);

	for (auto&& state : playerState)
	{
		if (state->overFlag_)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, sin((double)IpSceneMng.frames() / 10) * 250);
			DrawGraph(350, 500, IMAGE_ID("space")[0], true);

			SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
		}
	}

	if (overFlag_)
	{

		SetFontSize(50);

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);
		DrawBox(0, 0, IpSceneMng.ScreenSize.x, IpSceneMng.ScreenSize.y, 0x000000, true);
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

		for (auto&& state : playerState)
		{
			DrawGraph(state->_offset.x + 60, state->_offset.y + 200, IMAGE_ID("勝負画像")[static_cast<int>(state->winFlag_)], true);

		}

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, sin((double)IpSceneMng.frames() / 10) * 250);
		DrawGraph(350, 500, IMAGE_ID("space")[0], true);

		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
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