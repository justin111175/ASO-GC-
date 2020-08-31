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
#include "GameOverScene.h"
GameScene::GameScene()
{


	playerState.emplace_back(std::make_unique<State>(std::move(Vector2(450, 50)), std::move(Vector2(700 + 450, 800))));

	IpImageMng.GetID("space", "image/space.png", { 570,40 }, { 1,1 });
	IpImageMng.GetID("X", "image/X.png", { 57,57 }, { 1,1 });

	controller.try_emplace(conType::Pad, std::make_unique<PadInput>());

	controller[conType::Pad]->SetUp(0);

	cnt_ = 0;
	gameMean_ = GameMean::ƒ^ƒCƒgƒ‹‚É–ß‚é;

	overFlag_ = false;
}


GameScene::~GameScene()
{

}

unique_Base GameScene::Update(unique_Base own)
{

	Draw();

	if (CheckHitKey(KEY_INPUT_F1))
	{
		return std::make_unique<MeanScene>(std::move(own));

	}
	if (!FadeUpdate())
	{

		for (auto&& state : playerState)
		{
			if (!_timeCount.GetFlag("‘Ò‚Â"))
			{
				state->Run();

			}

			if (state->overFlag_)
			{
				return std::make_unique<GameOverScene>(std::move(own), playerState.size());
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



void GameScene::BaseDraw(void)
{

	for (auto&& state : playerState)
	{
		state->Draw();
	}

	SetDrawScreen(DX_SCREEN_BACK);

	DrawGraph(0, 0, IMAGE_ID("BG")[0], true);


	DrawGraph(0, 0, playerState[0]->GetScreenId(), true);

	//if (overFlag_)
	//{

	//	SetFontSize(50);

	//	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);
	//	DrawBox(0, 0, IpSceneMng.ScreenSize.x, IpSceneMng.ScreenSize.y, 0x000000, true);
	//	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

	//	DrawString(515, 300, "GAME OVER", 0xFFFFFF, 0x000000);


	//	SetDrawBlendMode(DX_BLENDMODE_ALPHA, sin((double)IpSceneMng.frames() / 10) * 250);
	//	DrawGraph(350, 500, IMAGE_ID("space")[0], true);

	//	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
	//}

}

void GameScene::Draw(void)
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




