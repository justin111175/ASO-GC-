#include <Dxlib.h>
#include "MouseCtl.h"
#include "_DebugConOut.h"

MouseCtl::MouseCtl()
{
	mouseData = 0;
	mouseDataOld = 0;
	TRACE("MouseCt�̐���\n");
}

MouseCtl::~MouseCtl()
{
	TRACE("MouseCt�̔p��\n");

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

	// ���W
	GetMousePoint(&pos.x, &pos.y);
	// �N���b�N���
	mouseDataOld = mouseData;
	mouseData=GetMouseInput();

}
