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

	bool TestCtl(conType input);
	void Init(void);
	TitleMean meanID_;
	void Draw(void);
	bool Flag;
	int cnt_;
};

