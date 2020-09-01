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
			{InputID::Btn1,KEY_INPUT_C},
			{InputID::Btn2,KEY_INPUT_V},
			{InputID::Btn3,KEY_INPUT_B},
			{InputID::Mean,KEY_INPUT_SPACE}
		};
	}
	else
	{		
		_inputTbl = {
			{InputID::Up,KEY_INPUT_UP},
			{InputID::Down,KEY_INPUT_DOWN},
			{InputID::Left,KEY_INPUT_LEFT},
			{InputID::Right,KEY_INPUT_RIGHT},	
			{InputID::Btn1,KEY_INPUT_COMMA},
			{InputID::Btn2,KEY_INPUT_M},
			{InputID::Btn3,KEY_INPUT_N},
			{InputID::Mean,KEY_INPUT_SPACE}
		};

	}

	return true;
}

void KeyInput::Updata(void)
{
	GetHitKeyStateAll(_keyData[0].data());
	GetHitKeyStateAll(_keyData[1].data());

	for (auto id : InputID())
	{
		for (int i = 0; i < 2; i++)
		{
			_data[i][id][static_cast<int>(Trg::Old)] = _data[i][id][static_cast<int>(Trg::Now)];
			_data[i][id][static_cast<int>(Trg::Now)] = _keyData[i][_inputTbl[id]];
		}
	}
}
