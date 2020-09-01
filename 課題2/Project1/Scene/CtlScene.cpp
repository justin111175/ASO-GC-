#include "CtlScene.h"
#include "common/Input/KeyInput.h"
#include "common/Input/PadInput.h"
#include <Dxlib.h>
#include "SceneMng.h"
#include "../common/ImageMng.h"
#include "TitleScene.h"
CtlScene::CtlScene()
{
	Init();
	cnt_ = 0;
}

CtlScene::CtlScene(int no)
{
	Init();
	cnt_ = no;
}

CtlScene::~CtlScene()
{
}

unique_Base CtlScene::Update(unique_Base own)
{
	BaseDraw();

	if (!FadeUpdate())
	{
		for (auto&& conType_ : conType())
		{
			if (cnt_ == 0)
			{
				if (Ctl(conType_))
				{
					return std::make_unique<CtlScene>(1);
				}
			}
			else
			{
				if (Ctl(conType_))
				{
					return std::make_unique<TitleScene>();
				}
			}


		}
	}



	return std::move(own);
}

void CtlScene::BaseDraw(void)
{
	ClsDrawScreen();

	DrawGraph(0, 0, IMAGE_ID("BG")[0], true);
	//DrawGraph(0, 0, IMAGE_ID("Frame")[0], true);
	ChangeFontType(DX_FONTTYPE_EDGE);

	SetFontSize(50);
	DrawString(50, 50, "ëÄçÏê‡ñæ", 0xFFFFFF, 0x000000);
	if (cnt_ == 0)
	{
		//SetFontSize(20);

		DrawGraph(500, 25, IMAGE_ID("2")[0], true);
		DrawGraph(150, 175, IMAGE_ID("1")[0], true);


	}
	else
	{
		DrawGraph(500, 25, IMAGE_ID("3")[0], true);
		DrawGraph(150, 175, IMAGE_ID("4")[0], true);
	}

	DrawString(250, 730, "É{É^Éì1ÇâüÇµÇƒÇ≠ÇæÇ≥Ç¢ÅB", 0xFFFFFF, 0x000000);


	if (IpSceneMng._blendCnt)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, IpSceneMng._blendCnt);
		DrawGraph(0, 0, IpImageMng.GetID(_fadeType)[0], true);
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

	}

	ScreenFlip();

}

bool CtlScene::Ctl(conType input)
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

void CtlScene::Init(void)
{
	IpImageMng.GetID("BG", "image/BG.png", { 1280,800 }, { 1,1 });
	IpImageMng.GetID("Frame", "image/frame.png", { 1280,800 }, { 1,1 });
	IpImageMng.GetID("1", "image/Key/1.png", { 983,530 }, { 1,1 });
	IpImageMng.GetID("2", "image/Key/2.png", { 441,150 }, { 1,1 });
	IpImageMng.GetID("3", "image/Key/3.png", { 216,150 }, { 1,1 });
	IpImageMng.GetID("4", "image/Key/4.png", { 734,530 }, { 1,1 });


	for (int i = 0; i < 2; i++)
	{
		controller[i].try_emplace(conType::Key, std::make_unique<KeyInput>());
		controller[i][conType::Key]->SetUp(i);
		controller[i].try_emplace(conType::Pad, std::make_unique<PadInput>());
		controller[i][conType::Pad]->SetUp(i);
	}


}
