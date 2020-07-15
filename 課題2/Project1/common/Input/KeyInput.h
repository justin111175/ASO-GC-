#pragma once
#include "Controller.h"
#include <array>

class KeyInput :
    public Controller
{

	conType GetConType(void) override;
	bool SetUp(int no) override;
	void Updata(void) override;
private:
	std::array<char, 256> _keyData;
	std::map<InputID, int> _inputTbl;

};

