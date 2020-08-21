#pragma once
#include "BaseScene.h"
#include "common/Input/Controller.h"


class TitleScene :
    public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	unique_Base Update(unique_Base own) override;					// �X�V



private:
	void Draw(void);
	std::map<conType, std::unique_ptr<Controller>> controller;
	int cnt_;
};

