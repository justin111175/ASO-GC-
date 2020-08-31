#pragma once
#include "BaseScene.h"
#include <memory>


class MeanScene :
    public BaseScene
{

public:

	MeanScene(unique_Base own);
	~MeanScene();
	unique_Base Update(unique_Base own) override;					// 更新
	void BaseDraw(void) override;					// 更新
private:
	unique_Base childScene_;

};

