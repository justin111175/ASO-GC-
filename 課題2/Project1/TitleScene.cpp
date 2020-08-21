#include "TitleScene.h"
#include <Dxlib.h>
#include"GameScene.h"
#include "common/Input/KeyInput.h"
#include "common/Input/PadInput.h"

TitleScene::TitleScene()
{

	//controller.try_emplace(conType::Pad, std::make_unique<PadInput>());
	cnt_ = 100;

	for (int i = 0; i < 2; i++)
	{
		controller.emplace(conType::Key, std::make_unique<KeyInput>());
		controller[conType::Key]->SetUp(i);

	}
	//controller[conType::Pad]->SetUp(_id);




}

TitleScene::~TitleScene()
{
}



unique_Base TitleScene::Update(unique_Base own)
{
    Draw();

	if (cnt_ <= 0)
	{
		if (CheckHitKey(KEY_INPUT_SPACE))
		{
			return std::make_unique<GameScene>();

		}

	}
	else
	{
		cnt_--;
	}

	return std::move(own);
}



void TitleScene::Draw(void)
{
	ClsDrawScreen();
	SetFontSize(60);
	DrawString(400, 400, "Title", 0xFFFFFF);

	ScreenFlip();

}
