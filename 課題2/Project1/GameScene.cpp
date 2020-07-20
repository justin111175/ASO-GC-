#include <Dxlib.h>
#include <_DeBugConOut.h>
#include <GameScene.h>
#include <Vector2.h>
#include <SceneMng.h>
#include <utility>

GameScene::GameScene()
{	


	playerState.emplace_back(std::make_unique<State>(std::move (Vector2(0, 0)), std::move(Vector2(1280, 800))/*,PlayerCtl::Player_1*/));
//	playerState.emplace_back(std::make_unique<State>(std::move (Vector2(600, 0)), std::move(Vector2(1280, 800))/*,PlayerCtl::Player_1*/));


}


GameScene::~GameScene()
{

}

unique_Base GameScene::Update(unique_Base own)
{
	Draw();

	// 無返事参照、const参照、shared_ptr三つの方法がある
	for (auto&& state : playerState)
	{
		state->Run();

	}


	return std::move(own);
}

void GameScene::Draw(void)
{
	ClsDrawScreen();

	for (auto&& state : playerState)
	{
		state->Draw();
	}



	SetDrawScreen(DX_SCREEN_BACK);

	for (auto&& state : playerState)
	{
		DrawGraph(0, 0,state->GetScreenId(), true);
	}
	
	//DrawGraph(0, 300, playerState[0]->GetScreenId(), true);
	//DrawGraph(0, 100, playerState[1]->GetScreenId(), true);

	ScreenFlip();

}

