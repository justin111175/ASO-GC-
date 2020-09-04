#include "MeanScene.h"
#include "GameScene.h"
#include <Dxlib.h>
#include "TitleScene.h"
#include "../common/Input/PadInput.h"
#include "../common/Input/KeyInput.h"
#include "CheckScene.h"
#include "SceneMng.h"
MeanScene::MeanScene(unique_Base own)
{
    childScene_ = std::move(own);
	for (int i = 0; i < 2; i++)
	{
		controller[i].try_emplace(conType::Key, std::make_unique<KeyInput>());
		controller[i][conType::Key]->SetUp(i);
		controller[i].try_emplace(conType::Pad, std::make_unique<PadInput>());
		controller[i][conType::Pad]->SetUp(i);

	}
	cnt_ = 0;

}

MeanScene::~MeanScene()
{
}

unique_Base MeanScene::Update(unique_Base own)
{

	if (cnt_ < 125)
	{
		cnt_+=5;
		
	}
	else
	{
		for (auto&& conType_ : conType())
		{
			if (MeanCtl(conType_))
			{
				if (key_ == 1)
				{
					switch (gameMean_)
					{
					case GameMean::�^�C�g���ɖ߂�:
						return std::make_unique<CheckScene>(std::move(childScene_));
						break;
					case GameMean::�Q�[���ɖ߂�:
						_timeCount.Set("�҂�", true, 1);
						return childScene_->Update(std::move(childScene_));
						break;
					case GameMean::�Q�[���I��:
						DxLib_End();
						break;
					}
				}
				else
				{
					return childScene_->Update(std::move(childScene_));

				}

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

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cnt_);
	DrawBox(0, 0, IpSceneMng.ScreenSize.x, IpSceneMng.ScreenSize.y, 0x000000, true);
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);



	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);
	DrawBox(400, 200, 900, 500, 0x000000, true);
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
	DrawBoxAA(400.0f, 200.0f, 900.0f, 500.0f, 0xFFFFFF, false, 3.0f);

	SetFontSize(50);
	DrawString(400, 200, "���j���[", 0xFFFFFF);
	DrawBoxAA(400.0f, 200.0f, 900.0f, 250.0f, 0xFFFFFF, false, 3.0f);

	SetFontSize(30);

	DrawBox(420, 270 + static_cast<int>(gameMean_) * 50, 660, 310 + static_cast<int>(gameMean_) * 50, 0x000000, true);
	DrawBoxAA(420.0f, 270.0f + static_cast<int>(gameMean_) * 50, 660.0f, 310.0f + static_cast<int>(gameMean_) * 50, 0xFFFFFF, false, 3.0f);

	DrawString(420, 270, "�^�C�g���ɖ߂�", 0xFFFFFF);
	DrawString(420, 320, "�Q�[���ɖ߂�", 0xFFFFFF);
	DrawString(420, 370, "�Q�[���I��", 0xFFFFFF);
	ScreenFlip();
}

bool MeanScene::MeanCtl(conType input)
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
				case InputID::Btn1:
					key_ = 1;
					return true;
					break;

				case InputID::Btn2:
					key_ = 2;
					return true;
					break;
				}
			}
		}
	}
	return false;

}


