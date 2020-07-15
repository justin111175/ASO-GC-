#pragma once
#include <map>
#include <iostream> 
#define _timeCount TimeCount::GetInstance()

class TimeCount
{
public:
	// �ÓI�V���O���h��
	static TimeCount& GetInstance()
	{
		static TimeCount s_Instance;
		return s_Instance;
	}
	void Run(void);
	bool Set(const char* string,bool flag, double cnt);			// �v�Z�̕ϐ����A�X�E�B�b�`�A���b

	const bool GetFlag(const char* string);						// �t���O��Ԃ��Ƃ�
private:
	TimeCount();
	~TimeCount();

	std::map<std::string, bool> _flag;							// �v�Z���邩�ǂ����t���O
	std::map<std::string, double> _cnt;							// �v�Z�p�ϐ�
};

