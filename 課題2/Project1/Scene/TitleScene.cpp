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
		MeanCtl();

		if (Flag)
		{
			switch (meanID_)
			{
			case TitleMean::��l�v���C:
				return std::make_unique<GameScene>();

				break;
			case TitleMean::��l�v���C:
				return std::make_unique<GameScene2>();

				break;
			default:
				break;
			}

		}
	}
			
	





	return std::move(own);
}



void TitleScene::Init(void)
{
	//controller.try_emplace(conType::Pad, std::make_unique<PadInput>());
	cnt_ = 0;


	controller.emplace(conType::Key, std::make_unique<KeyInput>());
	controller[conType::Key]->SetUp(0);
	
	//controller[conType::Pad]->SetUp(_id);

	IpImageMng.GetID("�}�[�N", "image/mark.png", { 45,45 }, { 1,1 });
	IpImageMng.GetID("BG", "image/BG.png", { 1280,800 }, { 1,1 });
	IpImageMng.GetID("Frame", "image/frame.png", { 1280,800 }, { 1,1 });
	IpImageMng.GetID("Main", "image/main.png", { 720,720 }, { 1,1 });
	meanID_ = TitleMean::��l�v���C;
	Flag = false;

}

void TitleScene::MeanCtl(void)
{
	for (auto data : controller[conType::Key]->GetCntData())
	{
		if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
		{
			switch (data.first)
			{
			case InputID::Up:

				if (meanID_ <= TitleMean::��l�v���C)
				{
					meanID_ = TitleMean::�Q�[���I�[�o�[;
				}
				else
				{
					meanID_ = (TitleMean)(static_cast<int>(meanID_) - 1);

				}
				break;
			case InputID::Down:
				if (meanID_ >= TitleMean::�Q�[���I�[�o�[)
				{
					meanID_ = TitleMean::��l�v���C;
				}
				else
				{
					meanID_ = (TitleMean)(static_cast<int>(meanID_) + 1);

				}
				break;
			case InputID::Btn1:
				switch (meanID_)
				{
				case TitleMean::�Q�[���I�[�o�[:
					DxLib_End();
					break;
				default:
					Flag = true;
					break;
				}
				break;
			}
		}

	}
	(*controller[conType::Key])();


}

void TitleScene::Draw(void)
{
	ClsDrawScreen();

	DrawGraph(0,0, IMAGE_ID("BG")[0], true);
	DrawGraph(0,0, IMAGE_ID("Frame")[0], true);
	
	DrawRotaGraph3(400,0,0,0,1.0f,0.8f,0, IMAGE_ID("Main")[0], true);


	ChangeFontType(DX_FONTTYPE_EDGE);

	SetFontSize(60);
	DrawString(400, 200, "�Ղ�Ղ�ۑ�", 0xFFFFFF);
	
	SetFontSize(30);

	DrawString(450, 400, "��l�v���C", 0xFFFFFF);
	DrawString(450, 450, "��l�v���C", 0xFFFFFF);
	DrawString(450, 500, "�Q�[���I��", 0xFFFFFF);

	DrawGraph(400+sin((double)IpSceneMng.frames()/5)*5, 393+ static_cast<int>(meanID_)*50, IMAGE_ID("�}�[�N")[0], true);
	if (IpSceneMng._blendCnt)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA,IpSceneMng._blendCnt);
		DrawGraph(0, 0, IpImageMng.GetID(_fadeType)[0], true);
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

	}

	ScreenFlip();

}
