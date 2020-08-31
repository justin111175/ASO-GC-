#include "GameOverScene.h"
#include "../common/Input/PadInput.h"
#include "TitleScene.h"
#include <Dxlib.h>
#include "SceneMng.h"
#include "../common/ImageMng.h"





GameOverScene::GameOverScene(unique_Base own, int no)
{
	childScene_ = std::move(own);

	controller.try_emplace(conType::Pad, std::make_unique<PadInput>());

	controller[conType::Pad]->SetUp(0);
	cnt_ = 0;
	playerSize_ = no;
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
		(*controller[conType::Pad])();

		for (auto data : controller[conType::Pad]->GetCntData(0))
		{
			switch (data.first)
			{
			case InputID::Btn1:
				if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
				{
					return std::make_unique<TitleScene>();
				}
				break;
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



	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cnt_);
	DrawBox(0, 0, IpSceneMng.ScreenSize.x, IpSceneMng.ScreenSize.y, 0x000000, true);

	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
	if (cnt_ >= 125)
	{
		//if (state_->winFlag_)
		//{
		//	SetFontSize(50);
		//	DrawString(515, 300, "GAME CLEAR", 0xFFFFFF, 0x000000);

		//}
		//else
		//{			
		if (playerSize_ == 1)
		{
			SetFontSize(50);
			DrawString(515, 300, "GAME OVER", 0xFFFFFF, 0x000000);

		}
		else
		{
			DrawString(300, 300, "TEST  2", 0xFFFFFF, 0x000000);

		}


		SetDrawBlendMode(DX_BLENDMODE_ALPHA, sin((double)IpSceneMng.frames() / 10) * 250);
		DrawGraph(350, 500, IMAGE_ID("space")[0], true);

		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

	}
	
	ScreenFlip();

}


