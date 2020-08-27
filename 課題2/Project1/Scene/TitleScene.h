#pragma once
#include "BaseScene.h"
#include "common/Input/Controller.h"

enum class TitleMean
{
	一人プレイ,
	二人プレイ,
	ゲームオーバー
};

class TitleScene :
    public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	unique_Base Update(unique_Base own) override;					// 更新
	void BaseDraw(void) override;					// 更新



private:
	void Init(void);
	TitleMean meanID_;

	void MeanCtl(void);
	void Draw(void);
	bool Flag;
	int cnt_;
};

