#include "GameOverScene.h"
#include "../common/Input/PadInput.h"
#include "../common/Input/KeyInput.h"
#include "TitleScene.h"
#include <Dxlib.h>
#include "SceneMng.h"
#include "../common/ImageMng.h"
#include <memory>

GameOverScene::GameOverScene(unique_Base own, std::vector <std::unique_ptr<State>>& playerState)
{
	childScene_ = std::move(own);

	playerState_ = std::move(playerState);

	playerSize_ = 2;
	Init();
}

GameOverScene::GameOverScene(unique_Base own)
{

	childScene_ = std::move(own);
	playerSize_ = 1;
	Init();
}

GameOverScene::~GameOverScene()
{
}

unique_Base GameOverScene::Update(unique_Base own)
{
	if (cnt_ < 125)
	{
		cnt_+=5;
	}
	else
	{

		for (auto&& conType_ : conType())
		{
			if (OverCtl(conType_))
			{
				return std::make_unique<TitleScene>();
			}
		}
	}

	BaseDraw();
	return std::move(own);
}

void GameOverScene::BaseDraw(void)
{
	ClsDrawScreen();
	childScene_->BaseDraw();
	for (auto&& state : playerState_)
	{
		state->ObjDraw();
	}

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cnt_);
	DrawBox(0, 0, IpSceneMng.ScreenSize.x, IpSceneMng.ScreenSize.y, 0x000000, true);

	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
	if (cnt_ >= 125)
	{	
		if (playerSize_ == 1)
		{
			SetFontSize(50);
			DrawString(515, 300, "GAME OVER", 0xFFFFFF, 0x000000);

		}
		else
		{

			//DrawString(300, 300, "TEST  Player1", 0xFFFFFF, 0x000000);
			//DrawString(900, 300, "TEST  Player2", 0xFFFFFF, 0x000000);
			for (auto&& state : playerState_)
			{
				DrawGraph(state->_offset.x + 60, state->_offset.y + 200, IMAGE_ID("èüïââÊëú")[static_cast<int>(!state->winFlag_)], true);

			}
		}
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, sin((double)IpSceneMng.frames() / 10) * 250);
		DrawString(350, 500, "É{É^Éì1ÇâüÇµÇƒÇ≠ÇæÇ≥Ç¢ÅB",0xFFFFFF, 0x000000);

		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

	}
	//playerState_[0]->Draw();


	ScreenFlip();

}

void GameOverScene::Init(void)
{
	for (int i = 0; i < 2; i++)
	{
		controller[i].try_emplace(conType::Key, std::make_unique<KeyInput>());
		controller[i][conType::Key]->SetUp(i);
		controller[i].try_emplace(conType::Pad, std::make_unique<PadInput>());
		controller[i][conType::Pad]->SetUp(i);
	}
	cnt_ = 0;


}

bool GameOverScene::OverCtl(conType input)
{
	for (int i = 0; i < 2; i++)
	{
		(*controller[i][input])();

		for (auto data : controller[i][input]->GetCntData(i))
		{
			if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
			{
				switch (data.first)
				{
				case InputID::Btn1:
					return true;

					break;
				}
			}
		}
	}
	return false;

}


