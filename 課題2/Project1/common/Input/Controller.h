#pragma once
#include <array>
#include <map>
#include "InputID.h"
//“ü—Í‘•’u
enum class conType
{
	Key,
	Mouse,
	Pad,
	Max
};

enum class Trg
{
	Now,
	Old,
	Max
};
enum class Player_No
{
	Player_1,
	Player_2,
	MAX
};


using TrgBool = std::array<bool, static_cast<size_t>(Trg::Max)>;
using CntData = std::map<InputID, TrgBool>;

struct Controller
{
	void operator()()
	{
		Updata();
	}
	
	const CntData& GetCntData(int no)
	{
		return _data[no];
	}	
	
	const CntData& GetCntData(void)
	{
		return _dataKey;
	}

	virtual conType GetConType(void) = 0;
	virtual bool SetUp(int no) = 0;
private:
	virtual void Updata(void) = 0;
protected:
	CntData _dataKey;
	
	CntData _data[2];

};

