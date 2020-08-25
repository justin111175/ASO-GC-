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
	unique_Base Update(unique_Base own) override;					// �X�V


private:
	int cnt_;
	void Draw(void);
	void MeanDraw(void);

	void ObjDraw(void);
	std::vector <std::unique_ptr<State>> playerState;
	std::map<conType, std::unique_ptr<Controller>> controller;


};
