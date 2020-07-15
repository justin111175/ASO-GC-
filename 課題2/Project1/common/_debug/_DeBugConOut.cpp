#ifdef _DEBUG
#include <Windows.h>
#include "_DeBugConOut.h"

std::unique_ptr<_DeBugConOut, _DeBugConOut::_DeBugConOutDeleter> _DeBugConOut::sInstance(new _DeBugConOut);

_DeBugConOut::_DeBugConOut()
{
	AllocConsole();
	freopen_s(&_debugFp, "CONOUT$", "w", stdout);
	freopen_s(&_debugFp, "CONIN$", "r", stdin);
}

_DeBugConOut::~_DeBugConOut()
{

	FreeConsole();
}

#endif