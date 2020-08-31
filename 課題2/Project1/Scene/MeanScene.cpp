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
				if (gameMean_ <= GameMean::�^�C�g���ɖ߂�)
				{
					gameMean_ = GameMean::�Q�[���I��;
				}
				else
				{
					gameMean_ = (GameMean)(static_cast<int>(gameMean_) - 1);

				}
				break;
			case InputID::Down:
				if (gameMean_ >= GameMean::�Q�[���I��)
				{
					gameMean_ = GameMean::�^�C�g���ɖ߂�;
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
				case GameMean::�^�C�g���ɖ߂�:
					return std::make_unique<TitleScene>();
					break;
				case GameMean::�Q�[���ɖ߂�:
					_timeCount.Set("�҂�", true, 1);
					return childScene_->Update(std::move(childScene_));
					break;
				case GameMean::�Q�[���I��:
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
	DrawString(400, 200, "���j���[", 0xFFFFFF);
	DrawBoxAA(400, 200, 900, 250, 0xFFFFFF, false, 3.0f);

	SetFontSize(30);

	DrawBox(420, 270 + static_cast<int>(gameMean_) * 50, 660, 310 + static_cast<int>(gameMean_) * 50, 0x000000, true);
	DrawBoxAA(420, 270 + static_cast<int>(gameMean_) * 50, 660, 310 + static_cast<int>(gameMean_) * 50, 0xFFFFFF, false, 3.0f);

	DrawString(420, 270, "�^�C�g���ɖ߂�", 0xFFFFFF);
	DrawString(420, 320, "�Q�[���ɖ߂�", 0xFFFFFF);
	DrawString(420, 370, "�Q�[���I��", 0xFFFFFF);
	ScreenFlip();
}


