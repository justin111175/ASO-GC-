#include <Dxlib.h>
#include "MouseCtl.h"
#include "_DebugConOut.h"

MouseCtl::MouseCtl()
{
	mouseData = 0;
	mouseDataOld = 0;
	TRACE("MouseCtの生成\n");
}

MouseCtl::~MouseCtl()
{
	TRACE("MouseCtの廃棄\n");

}

bool MouseCtl::GetClickTrg(void)
{
	return (((mouseData & (~mouseDataOld)) & MOUSE_INPUT_LEFT) != 0);
}

bool MouseCtl::GetClicking(void)
{
	return (((mouseData & (mouseDataOld)) & MOUSE_INPUT_LEFT) != 0);
}

const Vector2& MouseCtl::GetPos(void)
{
	return pos;
}

void MouseCtl::Update(void)
{

	// 座標
	GetMousePoint(&pos.x, &pos.y);
	// クリック状態
	mouseDataOld = mouseData;
	mouseData=GetMouseInput();

}
