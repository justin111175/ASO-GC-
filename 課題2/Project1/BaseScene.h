#pragma once
#include <memory>

// クラスのプロトタイプ宣言
class BaseScene;


using unique_Base = std::unique_ptr<BaseScene>;							// スマートポインタ-BaseScene

class BaseScene
{
public:
	BaseScene();														
	virtual ~BaseScene();
	virtual unique_Base Update(unique_Base own) = 0;					// 純粋仮想関数	
};


