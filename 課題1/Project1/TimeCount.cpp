#include "TimeCount.h"

void TimeCount::Run(void)
{
	auto type = [&](const char* string) {	
		// �t���t�̎�ނ�true�̏ꍇ��
		if (_flag[string])
		{
			//����0���傫���ꍇ�A����
			if (_cnt[string] > 0)
			{
				_cnt[string]--;
			}
			else
			{
				// ����Ȃ��ꍇ�Afalse�ɂ���
				_flag[string] = false;
			}
		}
	};
	type("�G���[");
}

bool TimeCount::Set(const char* string,bool flag, double cnt)
{
	_flag[string] = flag;
	// cnt(���b)��60��������_cnt(���t���[��)
	_cnt[string] = cnt*60;
	return true;
}

const bool TimeCount::GetFlag(const char* string)
{
	return _flag[string];
}

TimeCount::TimeCount()
{
	_flag.try_emplace("�G���[", false);
	_cnt.try_emplace("�G���[", 0);
	
}

TimeCount::~TimeCount()
{
}
