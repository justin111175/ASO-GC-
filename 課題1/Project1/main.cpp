#include <Dxlib.h>
#include "Myself.h"



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Myself::Destroy();											// �u���O�����̏I��
	Myself::GetInstance().Run();								

	return 0;
}


