#include "SceneMng.h"
#include "common/Vector2.h"
#include "GameScene.h"
#include "common/TimeCount.h"
#include <time.h>
#include "common/_debug/_DebugDispOut.h"


SceneMng* SceneMng::sInstance = nullptr;


Vector2 testA = { 40,60 };
void SceneMng::Run(void)
{
	if (!SysInit())
	{
		//AST();
	}

	_activeScene = std::make_unique<GameScene>();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		_dbgAddDraw();

		_activeScene = (*_activeScene).Update(std::move(_activeScene));
		_timeCount.Run();

		_frames++;
	}

}

const int SceneMng::frames(void) const
{
    return _frames;
}




bool SceneMng::SysInit(void)
{
	SetWindowText("�Ղ�Ղ�");											// �^�C�g���o�[
	SetGraphMode(ScreenSize.x, ScreenSize.y, 16);
	ChangeWindowMode(true);												// ture:window false:�t���X�N���[��
	if (DxLib_Init() == -1)												// DX�����u�����̏�����
	{
		return false;
	}
	_dbgSetup(215,215,215);												// debug�p�F�̓����x

	srand((unsigned)time(NULL));										// ���Ԃ��痐������

	return true;

}


SceneMng::~SceneMng()
{	

}

SceneMng::SceneMng():
	ScreenSize(1280,800)
{
	_frames = 0;

}
