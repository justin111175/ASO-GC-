#include "MeanScene.h"
#include "GameScene.h"
#include <Dxlib.h>
#include "TitleScene.h"
#include "../common/Input/PadInput.h"


MeanScene::MeanScene(unique_Base own)
{
    childScene_ = std::move(own);
	controller.try_emplace(conType::Pad, std::make_unique<PadInput>());
	controller[conType::Pad]->SetUp(0);

}

MeanScene::~MeanScene()
{
}

unique_Base MeanScene::Update(unique_Base own)
{
	(*controller[conType::Pad])();

	for (auto data : controller[conType::Pad]->GetCntData(0))
	{
		if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
		{

			switch (data.first)
			{
			case InputID::Up:
				if (gameMean_ <= GameMean::タイトルに戻る)
				{
					gameMean_ = GameMean::ゲーム終了;
				}
				else
				{
					gameMean_ = (GameMean)(static_cast<int>(gameMean_) - 1);

				}
				break;
			case InputID::Down:
				if (gameMean_ >= GameMean::ゲーム終了)
				{
					gameMean_ = GameMean::タイトルに戻る;
				}
				else
				{
					gameMean_ = (GameMean)(static_cast<int>(gameMean_) + 1);

				}
				break;
			case InputID::Btn2:
				return childScene_->Update(std::move(childScene_));
				break;
			case InputID::Btn1:
				switch (gameMean_)
				{
				case GameMean::タイトルに戻る:
					return std::make_unique<TitleScene>();
					break;
				case GameMean::ゲームに戻る:
					_timeCount.Set("待つ", true, 1);
					return childScene_->Update(std::move(childScene_));
					break;
				case GameMean::ゲーム終了:
					DxLib_End();
					break;
				}
				break;			
			}		
		}
	}
	BaseDraw();
	return std::move(own);
}



void MeanScene::BaseDraw(void)
{
	ClsDrawScreen();
	childScene_->BaseDraw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);
	DrawBox(400, 200, 900, 500, 0x000000, true);
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
	DrawBoxAA(400, 200, 900, 500, 0xFFFFFF, false, 3.0f);

	SetFontSize(50);
	DrawString(400, 200, "メニュー", 0xFFFFFF);
	DrawBoxAA(400, 200, 900, 250, 0xFFFFFF, false, 3.0f);

	SetFontSize(30);

	DrawBox(420, 270 + static_cast<int>(gameMean_) * 50, 660, 310 + static_cast<int>(gameMean_) * 50, 0x000000, true);
	DrawBoxAA(420, 270 + static_cast<int>(gameMean_) * 50, 660, 310 + static_cast<int>(gameMean_) * 50, 0xFFFFFF, false, 3.0f);

	DrawString(420, 270, "タイトルに戻る", 0xFFFFFF);
	DrawString(420, 320, "ゲームに戻る", 0xFFFFFF);
	DrawString(420, 370, "ゲーム終了", 0xFFFFFF);
	ScreenFlip();
}


