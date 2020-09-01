#include "CheckScene.h"
#include <Dxlib.h>
#include "../common/Input/PadInput.h"
#include "../common/Input/KeyInput.h"
#include "SceneMng.h"
#include "TitleScene.h"
#include "MeanScene.h"
CheckScene::CheckScene(unique_Base own)
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
	checkMeanID_ = CheckMean::ÇÕÇ¢;

}

CheckScene::~CheckScene()
{
}

unique_Base CheckScene::Update(unique_Base own)
{

	if (cnt_ < 125)
	{
		cnt_ += 5;
	}
	else
	{
		for (auto&& conType_ : conType())
		{
			if (CheckCtl(conType_))
			{
				if (key_ == 1)
				{
					switch (checkMeanID_)
					{
					case CheckMean::ÇÕÇ¢:
						return std::make_unique<TitleScene>();
						break;
					case CheckMean::Ç¢Ç¢Ç¶:
						return std::make_unique<MeanScene>(std::move(childScene_));
						break;
					};
				}
				else
				{
					return std::make_unique<MeanScene>(std::move(childScene_));

				}



			}

		}


	}

	BaseDraw();


	return std::move(own);
}

void CheckScene::BaseDraw(void)
{

	ClsDrawScreen();
	childScene_->BaseDraw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cnt_);
	DrawBox(0, 0, IpSceneMng.ScreenSize.x, IpSceneMng.ScreenSize.y, 0x000000, true);

	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
	
	if (cnt_ >= 125)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 220);
		DrawBox(400, 200, 900, 500, 0x000000, true);
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
		DrawBoxAA(400, 200, 900, 500, 0xFFFFFF, false, 3.0f);

		SetFontSize(50);
		DrawString(400, 200, "ämîFÇ≈Ç∑Ç©ÅH", 0xFFFFFF);
		DrawBoxAA(400, 200, 900, 250, 0xFFFFFF, false, 3.0f);

		SetFontSize(30);

		DrawBox(420, 270 + static_cast<int>(checkMeanID_) * 50, 660, 310 + static_cast<int>(checkMeanID_) * 50, 0x000000, true);
		DrawBoxAA(420, 270 + static_cast<int>(checkMeanID_) * 50, 660, 310 + static_cast<int>(checkMeanID_) * 50, 0xFFFFFF, false, 3.0f);

		DrawString(420, 270, "ÇÕÇ¢", 0xFFFFFF);
		DrawString(420, 320, "Ç¢Ç¢Ç¶", 0xFFFFFF);

	}

	ScreenFlip();
	
}

bool CheckScene::CheckCtl(conType input)
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
					if (checkMeanID_ <= CheckMean::ÇÕÇ¢)
					{
						checkMeanID_ = CheckMean::Ç¢Ç¢Ç¶;
					}
					else
					{
						checkMeanID_ = (CheckMean)(static_cast<int>(checkMeanID_) - 1);

					}
					break;
				case InputID::Down:
					if (checkMeanID_ >= CheckMean::Ç¢Ç¢Ç¶)
					{
						checkMeanID_ = CheckMean::ÇÕÇ¢;
					}
					else
					{
						checkMeanID_ = (CheckMean)(static_cast<int>(checkMeanID_) + 1);

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
