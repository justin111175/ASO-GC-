#pragma once
#include "BaseScene.h"
#include "common/Input/Controller.h"

class GameOver :
    public BaseScene
{
public:
	GameOver();
	~GameOver();
	unique_Base Update(unique_Base own) override;					// çXêV

	std::map<conType, std::unique_ptr<Controller>> controller;

private:

	void Draw(void);
};

