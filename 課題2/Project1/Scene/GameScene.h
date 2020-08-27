#pragma once
#include "BaseScene.h"
#include "State.h"
#include <memory>
#include <Vector>

class GameScene :
	public BaseScene
{
public:
	GameScene();													
	~GameScene();
	unique_Base Update(unique_Base own) override;					// 更新
	void BaseDraw(void) override;					// 更新


private:
	int cnt_;
	void Draw(void);
	std::vector <std::unique_ptr<State>> playerState;
};
