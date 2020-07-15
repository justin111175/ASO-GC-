#pragma once
#include "Vector2.h"

class MouseCtl
{
public:
	MouseCtl();
	~MouseCtl();

	bool GetClickTrg(void);					// クリックした瞬間の場合,true
	bool GetClicking(void);					// クリック中の場合,true
	const Vector2& GetPos(void);			// クリック位置を取得
	void Update(void);						// マウスの状態更新

private:
	Vector2 pos;							// マウス座標
	char mouseData;							// 現在のマウスクリック状態
	char mouseDataOld;						// 1フレーム前のマウスクリック状態

};

