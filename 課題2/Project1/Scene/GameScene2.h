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
	unique_Base Update(unique_Base own) override;					// �X�V
	void BaseDraw(void) override;									// �`��

private:
	void Draw(void);

	std::vector <std::unique_ptr<State>> playerState;

};