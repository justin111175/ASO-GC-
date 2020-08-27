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

GameScene2::GameScene2()
{


	playerState.emplace_back(std::make_unique<State>(std::move(Vector2(100, 50)), std::move(Vector2(700 + 100, 722))));

	playerState.emplace_back(std::make_unique<State>(std::move(Vector2(800, 50)), std::move(Vector2(700 + 800, 722))));
	
	controller.try_emplace(conType::Pad, std::make_unique<PadInput>());

	controller[conType::Pad]->SetUp(0);

	cnt_ = 100;
	IpImageMng.GetID("space", "image/space.png", { 570,40 }, { 1,1 });
	IpImageMng.GetID("�����摜", "image/win.png", { 260,150 }, { 1,2 });

	gameMean_ = GameMean::�^�C�g���ɖ߂�;
	meanFlag_ = false;
	sceneFlag_ = false;

}


GameScene2::~GameScene2()
{

}

unique_Base GameScene2::Update(unique_Base own)
{


	Draw();
	MeanCtl();

	// ���Ԏ��Q�ƁAconst�Q�ƁAshared_ptr�O�̕��@������
	if (!FadeUpdate())
	{
		if (!meanFlag_)
		{
			for (auto&& state : playerState)
			{

				if (!_timeCount.GetFlag("�҂�"))
				{
					state->Run();

				}

				if (state->sceneFlag_)
				{
					return std::make_unique<TitleScene>();

				}
				if (state->overFlag_)
				{
					overFlag_ = true;
				}
				if (overFlag_)
				{
					state->overFlag_ = true;
				}
			}
		}
	}

	if (sceneFlag_)
	{
		return std::make_unique<TitleScene>();


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

		//DrawString(515, 300, "GAME OVER", 0xFFFFFF, 0x000000);

		for (auto&& state : playerState)
		{
			DrawGraph(state->_offset.x+60 , state->_offset.y+200, IMAGE_ID("�����摜")[static_cast<int>(state->winFlag_)], true);

		}



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