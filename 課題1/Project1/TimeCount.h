#pragma once
#include <map>
#include <iostream> 
#define _timeCount TimeCount::GetInstance()

class TimeCount
{
public:
	// 静的シングルドン
	static TimeCount& GetInstance()
	{
		static TimeCount s_Instance;
		return s_Instance;
	}
	void Run(void);
	bool Set(const char* string,bool flag, double cnt);			// 計算の変数名、スウィッチ、何秒

	const bool GetFlag(const char* string);						// フラグ状態をとる
private:
	TimeCount();
	~TimeCount();

	std::map<std::string, bool> _flag;							// 計算するかどうかフラグ
	std::map<std::string, double> _cnt;							// 計算用変数
};

