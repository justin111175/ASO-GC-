#pragma once
#include <memory>
#include <String>
#include "../common/Input/Controller.h"

// クラスのプロトタイプ宣言
class BaseScene;
using unique_Base = std::unique_ptr<BaseScene>;							// スマートポインタ-BaseScene



class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual unique_Base Update(unique_Base own) = 0;					// 純粋仮想関数

	virtual void BaseDraw(void) = 0;					

protected:
	void FadeInit(std::string fadeType);								// Fade初期化
	bool FadeUpdate(void);												// Fade更新

	std::string _fadeType;												// Fadeタイプ
	
	bool meanFlag_;
	bool sceneFlag_;
	
	std::map<conType, std::unique_ptr<Controller>> controller;
	bool overFlag_;
private:

	int _fadeScrID;														// Fade描画ID
};


