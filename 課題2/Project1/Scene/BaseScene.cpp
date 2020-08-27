#include "BaseScene.h"
#include "SceneMng.h"
#include "DxLib.h"
#include "../common/ImageMng.h"

BaseScene::BaseScene()
{

	IpImageMng.GetID("black", "image/black.png");							//枠:わく- 物の輪郭や範囲
	IpImageMng.GetID("white", "image/white.png");
	_fadeScrID = MakeScreen(IpSceneMng.ScreenSize.x, IpSceneMng.ScreenSize.y, false);
	FadeInit("white");

}

BaseScene::~BaseScene()
{
}





void BaseScene::FadeInit(std::string fadeType)
{
	GetDrawScreenGraph(0, 0, IpSceneMng.ScreenSize.x, IpSceneMng.ScreenSize.y, _fadeScrID);
	IpSceneMng._blendCnt = 200;
	_fadeType = fadeType;
}

bool BaseScene::FadeUpdate(void)
{
	if (IpSceneMng._blendCnt)
	{
		IpSceneMng._blendCnt -= 5;
	}

	return IpSceneMng._blendCnt;
}

void BaseScene::GameMeanDraw(void)
{

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
}

void BaseScene::MeanCtl(void)
{
	(*controller[conType::Pad])();

	for (auto data : controller[conType::Pad]->GetCntData(0))
	{
		if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
		{
			if (!meanFlag_)
			{
				if (data.first == InputID::Mean)
				{
					meanFlag_ = true;
				}
			}
			else
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
					meanFlag_ = false;
					break;
				case InputID::Btn1:
					switch (gameMean_)
					{
					case GameMean::タイトルに戻る:
						sceneFlag_ = true;
						break;
					case GameMean::ゲームに戻る:
						_timeCount.Set("待つ", true, 1);
						meanFlag_ = false;
						break;
					case GameMean::ゲーム終了:
						DxLib_End();
						break;

					}

				}
			}




		}
	}

}

