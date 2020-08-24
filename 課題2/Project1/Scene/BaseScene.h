#pragma once
#include <memory>
#include <String>
// �N���X�̃v���g�^�C�v�錾
class BaseScene;


using unique_Base = std::unique_ptr<BaseScene>;							// �X�}�[�g�|�C���^-BaseScene

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene();
	virtual unique_Base Update(unique_Base own) = 0;					// �������z�֐�

protected:
	void FadeInit(std::string fadeType);								// Fade������
	bool FadeUpdate(void);												// Fade�X�V
	std::string _fadeType;												// Fade�^�C�v

private:
	int _fadeScrID;														// Fade�`��ID
};


