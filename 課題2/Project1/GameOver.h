#pragma once
#include "BaseScene.h"
class GameOver :
    public BaseScene
{
public:
	GameOver();
	~GameOver();
	unique_Base Update(unique_Base own) override;					// XV


private:

	void Draw(void);
};

