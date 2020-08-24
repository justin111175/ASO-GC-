#include "TimeCount.h"
#include "../Scene/SceneMng.h"

void TimeCount::Run(void)
{
	
	auto type = [&](const char* string) {	
		if (_flag[string])
		{
			if (_cnt[string] > 0)
			{
				_cnt[string]--;
			}
			else
			{
				_flag[string] = false;
			}
		}
	};
	
	type("������");

}

bool TimeCount::Set(const char* string,bool flag, double cnt)
{
	_flag[string] = flag;
	_cnt[string] = cnt*60;

	return true;
}

const bool TimeCount::GetFlag(const char* string)
{
	return _flag[string];
}

TimeCount::TimeCount()
{
	auto add = [&](std::string string) {
		_flag.try_emplace(string, false);
		_cnt.try_emplace(string, 0);

	};
	
	add("������");
}

TimeCount::~TimeCount()
{
}
