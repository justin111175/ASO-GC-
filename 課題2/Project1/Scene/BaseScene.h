#pragma once
#include <memory>
#include <String>
#include "../common/Input/Controller.h"
// �N���X�̃v���g�^�C�v�錾
class BaseScene;


using unique_Base = std::unique_ptr<BaseScene>;							// �X�}�[�g�|�C���^-BaseScene


enum class GameMean
{
	�^�C�g���ɖ߂�,
	�Q�[���ɖ߂�,
	�Q�[���I��
};

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual unique_Base Update(unique_Base own) = 0;					// �������z�֐�

protected:
	void FadeInit(std::string fadeType);								// Fade������
	bool FadeUpdate(void);												// Fade�X�V
	void GameMeanDraw(void);
	void MeanCtl(void);

	std::string _fadeType;												// Fade�^�C�v
	GameMean gameMean_;
	
	bool meanFlag_;
	bool sceneFlag_;
	
	std::map<conType, std::unique_ptr<Controller>> controller;
	bool overFlag_;
private:

	int _fadeScrID;														// Fade�`��ID
};


