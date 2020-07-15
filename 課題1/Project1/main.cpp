#include <Dxlib.h>
#include "Myself.h"



int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Myself::Destroy();											// ƒuƒƒOƒ‰ƒ€‚ÌI—¹
	Myself::GetInstance().Run();								

	return 0;
}


