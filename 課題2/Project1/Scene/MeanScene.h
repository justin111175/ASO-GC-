#pragma once
#include "BaseScene.h"
#include <memory>

enum class GameMean
{
	タイトルに戻る,
	ゲームに戻る,
	ゲーム終了
};

class MeanScene :
    public BaseScene
{
public:

	MeanScene(unique_Base own);
	~MeanScene();
	unique_Base Update(unique_Base own) override;					// 更新
	void BaseDraw(void) override;									// 描画
private:
	GameMean gameMean_;
	unique_Base childScene_;										// 子シーン
};

