#pragma once
#include "Controller.h"




class PadInput :
    public Controller
{

	conType GetConType(void) override;
	bool SetUp(int no) override;
	void Updata(void) override;
private:
	//std::map<Player_No,int> _keyData;
	int _keyData[2];
	

	std::map<InputID, int> _inputTbl;

};


