#pragma once
#include "BaseScene.h"
#include <memory>


class MeanScene :
    public BaseScene
{

public:

	MeanScene(unique_Base own);
	~MeanScene();
	unique_Base Update(unique_Base own) override;					// �X�V
	void BaseDraw(void) override;					// �X�V
private:
	unique_Base childScene_;

};

