#include "GameOver.h"
#include <Dxlib.h>
#include "common/Input/KeyInput.h"
#include "common/Input/PadInput.h"
#include "GameScene.h"
GameOver::GameOver()
{
	controller.try_emplace(conType::Key, std::make_unique<KeyInput>());
	controller.try_emplace(conType::Pad, std::make_unique<PadInput>());

	for (int i = 0; i < 2; i++)
	{
		controller[conType::Key]->SetUp(i);
		controller[conType::Pad]->SetUp(i);

	}

}

GameOver::~GameOver()
{
}

unique_Base GameOver::Update(unique_Base own)
{
    Draw();

	(*controller[conType::Key])();
	for (auto data : controller[conType::Key]->GetCntData())
	{
		switch (data.first)
		{
		case InputID::Btn1:
			if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
			{
				return std::make_unique<GameScene>();
			}
			break;
		}
	}
	(*controller[conType::Pad])();

	for (int i = 0; i < 2; i++)
	{
		for (auto data : controller[conType::Pad]->GetCntData(i))
		{
			switch (data.first)
			{
			case InputID::Btn1:
				if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
				{
					return std::make_unique<GameScene>();
				}
				break;
			}
		}
	}




    return std::move(own);
}

void GameOver::Draw(void)
{
	ClsDrawScreen();

	DrawString(400, 400, "GAMEOVER", 0xFFFFFF, true);
	SetFontSize(40);
	ScreenFlip();
}
