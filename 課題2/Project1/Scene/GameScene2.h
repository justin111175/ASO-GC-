#pragma once


#include "BaseScene.h"
#include "State.h"
#include <memory>
#include <Vector>

class GameScene2 :
	public BaseScene
{
public:
	GameScene2();
	~GameScene2();
	unique_Base Update(unique_Base own) override;					// 更新
	void BaseDraw(void) override;					// 更新

private:
	int cnt_;
	void Draw(void);

	std::vector <std::unique_ptr<State>> playerState;

};