#pragma once
#include "Dxlib.h"
#include "Vector2.h"
#include <memory>
#include "_DeBugConOut.h"
#include "TimeCount.h"
#include "State.h"
#include "BaseScene.h"

#define IpSceneMng SceneMng::GetInstance()

class SceneMng
{
public:
	static SceneMng& GetInstance(void)							// �Q�b�g-�C���X�^���X
	{
		Create();
		return *sInstance;
	}
	static void Create()
	{
		if (sInstance == nullptr)
		{
			sInstance = new SceneMng();
		}
	}
	static void Destroy()
	{
		if (sInstance != nullptr)
		{
			delete sInstance;
		}
		sInstance = nullptr;
	}
	void Run(void);

	const Vector2 ScreenSize;									// �S��ʃT�C�Y

	const int frames(void)const;								// �t���[�����̃Q�b�g�֐�

private:
	static SceneMng* sInstance;									// �C���X�^���X
	// �X�}�[�g�|�C���^
	
	unique_Base _activeScene;									// ��Scene�̈ʒu

	int _frames;												// �t���[����

	~SceneMng();
	SceneMng();
	bool SysInit(void);

};

