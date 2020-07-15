#pragma once

#ifdef _DEBUG
#include <memory>
#define AST() {\
	CHAR ast_mes[256]; \
	wsprintf(ast_mes, "%s %d�s��\0", __FILE__, __LINE__); \
	MessageBox(0, ast_mes, "���ĕ\��", MB_OK); \
	}

#define TRACE(fmt,...) printf(fmt,__VA_ARGS__)
#else
#define AST()
#endif

#ifdef _DEBUG
class _DeBugConOut
{
public:
	// �V���O���g��
	static _DeBugConOut &GetInstance(void)
	{
		return *sInstance;
	}
private:
	// �֐��I�u�W�F�N�g
	struct _DeBugConOutDeleter
	{
		void operator()(_DeBugConOut*_debugConOut)const
		{
			delete _debugConOut;
		}
	};
	
	_DeBugConOut();
	~_DeBugConOut();

	FILE* _debugFp = nullptr;
	
	static std::unique_ptr<_DeBugConOut, _DeBugConOutDeleter> sInstance;
};

#else
#define AST()
#define TRACE(fmt, ...)
#endif