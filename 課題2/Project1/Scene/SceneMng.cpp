#include "SceneMng.h"
#include "common/Vector2.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "common/TimeCount.h"
#include <time.h>
#include "common/_debug/_DebugDispOut.h"
#include "EffectMng.h"
#include <EffekseerForDxlib.h>

SceneMng* SceneMng::sInstance = nullptr;


Vector2 testA = { 40,60 };
void SceneMng::Run(void)
{
	if (!SysInit())
	{
		//AST();
	}

	_activeScene = std::make_unique<TitleScene>();

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
	SetWindowText("1916229_YANG CHIHCHENG �Ղ�Ղ�");											// �^�C�g���o�[
	SetGraphMode(ScreenSize.x, ScreenSize.y, 16);
	ChangeWindowMode(true);												// ture:window false:�t���X�N���[��
	
	
	
	if (DxLib_Init() == -1)												// DX�����u�����̏�����
	{
		return false;
	}

	_dbgSetup(ScreenSize.x, ScreenSize.y,215);												// debug�p�F�̓����x

	srand((unsigned)time(NULL));										// ���Ԃ��痐������
	
	IpEffect.Init(Vector2(ScreenSize.x, ScreenSize.y));

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
