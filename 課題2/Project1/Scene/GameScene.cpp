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

GameScene::GameScene()
{	


	playerState.emplace_back(std::make_unique<State>(std::move (Vector2(450, 50)), std::move(Vector2(700+450, 722))));
	
	IpImageMng.GetID("space", "image/space.png", { 570,40 }, { 1,1 });
	
	controller.try_emplace(conType::Pad, std::make_unique<PadInput>());

	controller[conType::Pad]->SetUp(0);

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
			if (!state->meanFlag_)
			{
				state->Run();

			}

			
			if (state->sceneFlag_)
			{
				return std::make_unique<TitleScene>();

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
		if (state->overFlag_)
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, sin((double)IpSceneMng.frames() / 10) * 250);
			DrawGraph(350, 500, IMAGE_ID("space")[0], true);

			SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
		}
		if (state->meanFlag_)
		{
			MeanDraw();
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

void GameScene::MeanDraw(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);
	DrawBox(400,200,900,500, 0x000000, true);
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
	DrawBoxAA(400, 200, 900, 500, 0xFFFFFF, false, 3.0f);

	SetFontSize(50);
	DrawString(400,200, "メニュー", 0xFFFFFF);
	DrawBoxAA(400, 200, 900, 250, 0xFFFFFF, false, 3.0f);

	SetFontSize(30);

	DrawBox(420, 270, 660, 320, 0x000000, true);
	DrawBoxAA(420, 270, 660, 320, 0xFFFFFF, false, 3.0f);

	DrawString(420, 270, "タイトルに戻る", 0xFFFFFF);
	DrawString(420, 320, "ゲームに戻る", 0xFFFFFF);
	DrawString(420, 370, "ゲーム終了", 0xFFFFFF);
}

void GameScene::ObjDraw(void)
{
}

