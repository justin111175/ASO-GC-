#include <Dxlib.h>
#include "KeyInput.h"
#include "InputID.h"
conType KeyInput::GetConType(void)
{
	return conType::Key;
}

bool KeyInput::SetUp(int no)
{
	if (no == 0)
	{
		_inputTbl = {
			{InputID::Up,KEY_INPUT_W},
			{InputID::Down,KEY_INPUT_S},
			{InputID::Left,KEY_INPUT_A},
			{InputID::Right,KEY_INPUT_D},
			{InputID::Btn1,KEY_INPUT_V}

		};


	}
	else
	{		
		_inputTbl = {
			{InputID::Up,KEY_INPUT_UP},
			{InputID::Down,KEY_INPUT_DOWN},
			{InputID::Left,KEY_INPUT_LEFT},
			{InputID::Right,KEY_INPUT_RIGHT},	
			{InputID::Btn1,KEY_INPUT_M}

		};
	}




	return true;
}

void KeyInput::Updata(void)
{

	GetHitKeyStateAll(_keyData.data());
	
	for (auto id : InputID())
	{
		_dataKey[id][static_cast<int>(Trg::Old)] = _dataKey[id][static_cast<int>(Trg::Now)];
		_dataKey[id][static_cast<int>(Trg::Now)] = _keyData[_inputTbl[id]];
	}

}
