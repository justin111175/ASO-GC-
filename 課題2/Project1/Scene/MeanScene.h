#pragma once
#include "BaseScene.h"
#include <memory>


class MeanScene :
    public BaseScene
{

public:

	MeanScene(unique_Base own);
	~MeanScene();
	unique_Base Update(unique_Base own) override;					// çXêV


private:
	GameScene childScene_;
	void Draw(void);



};

