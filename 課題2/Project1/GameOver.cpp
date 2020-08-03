#include "GameOver.h"
#include <Dxlib.h>
GameOver::GameOver()
{

}

GameOver::~GameOver()
{
}

unique_Base GameOver::Update(unique_Base own)
{
    Draw();
    return std::move(own);
}

void GameOver::Draw(void)
{
	ClsDrawScreen();

	DrawString(400, 400, "GAMEOVER", 0xFFFFFF, true);
	SetFontSize(40);
	ScreenFlip();
}
