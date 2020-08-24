#pragma once
#include "BaseScene.h"
#include "common/Input/Controller.h"

enum class TitleMean
{
	��l�v���C,
	��l�v���C,
	�Q�[���I�[�o�[
};

class TitleScene :
    public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	unique_Base Update(unique_Base own) override;					// �X�V
	


private:
	void Init(void);
	TitleMean meanID_;

	void MeanCtl(void);
	void Draw(void);
	bool Flag;
	std::map<conType, std::unique_ptr<Controller>> controller;
	int cnt_;
};

