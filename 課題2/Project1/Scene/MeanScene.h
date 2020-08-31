#pragma once
#include "BaseScene.h"
#include <memory>

enum class GameMean
{
	�^�C�g���ɖ߂�,
	�Q�[���ɖ߂�,
	�Q�[���I��
};

class MeanScene :
    public BaseScene
{
public:

	MeanScene(unique_Base own);
	~MeanScene();
	unique_Base Update(unique_Base own) override;					// �X�V
	void BaseDraw(void) override;									// �`��
private:
	GameMean gameMean_;
	unique_Base childScene_;										// �q�V�[��
};

