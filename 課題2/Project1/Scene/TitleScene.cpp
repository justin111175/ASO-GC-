#include "TitleScene.h"
#include <Dxlib.h>
#include"GameScene.h"
#include"GameScene2.h"
#include "common/Input/KeyInput.h"
#include "common/Input/PadInput.h"
#include "common/ImageMng.h"
#include "SceneMng.h"

TitleScene::TitleScene()
{

	Init();


}

TitleScene::~TitleScene()
{
}



unique_Base TitleScene::Update(unique_Base own)
{
    Draw();


	if (!FadeUpdate())
	{
		for (auto&& conType_ : conType())
		{
			if (TestCtl(conType_))
			{
				switch (meanID_)
				{
				case TitleMean::一人プレイ:
					return std::make_unique<GameScene>();
					break;
				case TitleMean::二人プレイ:
					return std::make_unique<GameScene2>();
					break;
				case TitleMean::ゲームオーバー:
					DxLib_End();
					break;

				}
			}
		}

	}
			


	return std::move(own);
}



void TitleScene::BaseDraw(void)
{
	
}



bool TitleScene::TestCtl(conType input)
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

					if (meanID_ <= TitleMean::一人プレイ)
					{
						meanID_ = TitleMean::ゲームオーバー;
					}
					else
					{
						meanID_ = (TitleMean)(static_cast<int>(meanID_) - 1);

					}
					break;
				case InputID::Down:
					if (meanID_ >= TitleMean::ゲームオーバー)
					{
						meanID_ = TitleMean::一人プレイ;
					}
					else
					{
						meanID_ = (TitleMean)(static_cast<int>(meanID_) + 1);

					}
					break;
				case InputID::Btn1:
					return true;

				}
			}
		}
	}
	return false;
}

void TitleScene::Init(void)
{
	//controller.try_emplace(conType::Pad, std::make_unique<PadInput>());
	cnt_ = 0;



	for (int i = 0; i < 2; i++)
	{	

		controller[i].try_emplace( conType::Key, std::make_unique<KeyInput>());
		controller[i][conType::Key]->SetUp(i);
		controller[i].try_emplace(conType::Pad, std::make_unique<PadInput>());
		controller[i][conType::Pad]->SetUp(i);
		
	}


	IpImageMng.GetID("マーク", "image/mark.png", { 45,45 }, { 1,1 });
	IpImageMng.GetID("BG", "image/BG.png", { 1280,800 }, { 1,1 });
	IpImageMng.GetID("Frame", "image/frame.png", { 1280,800 }, { 1,1 });
	IpImageMng.GetID("Main", "image/main.png", { 720,720 }, { 1,1 });
	


	meanID_ = TitleMean::一人プレイ;
	Flag = false;

}



void TitleScene::Draw(void)
{
	ClsDrawScreen();

	DrawGraph(0,0, IMAGE_ID("BG")[0], true);
	DrawGraph(0,0, IMAGE_ID("Frame")[0], true);
	
	DrawRotaGraph3(300,100,0,0,1.0f,0.8f,0, IMAGE_ID("Main")[0], true);


	ChangeFontType(DX_FONTTYPE_EDGE);

	SetFontSize(50);
	DrawString(500, 135, "ぷよぷよ課題", 0xFFFFFF);
	
	SetFontSize(30);

	DrawString(450, 250, "一人プレイ", 0xFFFFFF);
	DrawString(450, 300, "二人プレイ", 0xFFFFFF);
	DrawString(450, 350, "ゲーム終了", 0xFFFFFF);

	DrawGraph(400+sin((double)IpSceneMng.frames()/5)*5, 243+ static_cast<int>(meanID_)*50, IMAGE_ID("マーク")[0], true);


	if (IpSceneMng._blendCnt)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,IpSceneMng._blendCnt);
		DrawGraph(0, 0, IpImageMng.GetID(_fadeType)[0], true);
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

	}

	ScreenFlip();

}
