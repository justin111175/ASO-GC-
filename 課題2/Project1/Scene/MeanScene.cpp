#include "MeanScene.h"
#include "GameScene.h"
#include <Dxlib.h>



MeanScene::MeanScene(unique_Base own)
{
    childScene_ = std::move(own);

}

MeanScene::~MeanScene()
{
}

unique_Base MeanScene::Update(unique_Base own)
{
	if (CheckHitKey(KEY_INPUT_F2))
	{
		return;
	}

	Draw();

	return std::move(own);
}

void MeanScene::Draw(void)
{
	ClsDrawScreen();
	
	//childScene_->Update(std::move(childScene_));
	DrawString(200, 200, "TEST", 0xFFFFFF);
	ScreenFlip();

}
