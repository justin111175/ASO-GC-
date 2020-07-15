#include "TimeCount.h"

void TimeCount::Run(void)
{
	auto type = [&](const char* string) {	
		// フラフの種類がtrueの場合は
		if (_flag[string])
		{
			//もし0より大きい場合、引く
			if (_cnt[string] > 0)
			{
				_cnt[string]--;
			}
			else
			{
				// じゃない場合、falseにする
				_flag[string] = false;
			}
		}
	};
	type("エラー");
}

bool TimeCount::Set(const char* string,bool flag, double cnt)
{
	_flag[string] = flag;
	// cnt(何秒)が60をかけ→_cnt(何フレーム)
	_cnt[string] = cnt*60;
	return true;
}

const bool TimeCount::GetFlag(const char* string)
{
	return _flag[string];
}

TimeCount::TimeCount()
{
	_flag.try_emplace("エラー", false);
	_cnt.try_emplace("エラー", 0);
	
}

TimeCount::~TimeCount()
{
}
