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
	static SceneMng& GetInstance(void)							// ゲット-インスタンス
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

	const Vector2 ScreenSize;									// 全画面サイズ

	const int frames(void)const;								// フレーム数のゲット関数

private:
	static SceneMng* sInstance;									// インスタンス
	// スマートポインタ
	
	unique_Base _activeScene;									// 今Sceneの位置

	int _frames;												// フレーム数

	~SceneMng();
	SceneMng();
	bool SysInit(void);

};

