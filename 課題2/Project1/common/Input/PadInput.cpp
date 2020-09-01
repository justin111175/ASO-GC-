#include <Dxlib.h>
#include "PadInput.h"

conType PadInput::GetConType(void)
{
	return conType::Pad;
}

bool PadInput::SetUp(int no)
{
	if (no == 0)
	{

		_inputTbl= {
			{InputID::Up, PAD_INPUT_UP},
			{InputID::Down, PAD_INPUT_DOWN},
			{InputID::Left, PAD_INPUT_LEFT},
			{InputID::Right, PAD_INPUT_RIGHT},
			{InputID::Btn1,PAD_INPUT_1},
			{InputID::Btn2,PAD_INPUT_2},
			{InputID::Btn3,PAD_INPUT_3},
			{InputID::Mean,PAD_INPUT_8}

		};

	}
	else
	{
		_inputTbl= {
			{InputID::Up, PAD_INPUT_UP},
			{InputID::Down, PAD_INPUT_DOWN},
			{InputID::Left, PAD_INPUT_LEFT},
			{InputID::Right, PAD_INPUT_RIGHT},
			{InputID::Btn1,PAD_INPUT_1},
			{InputID::Btn2,PAD_INPUT_2},
			{InputID::Btn3,PAD_INPUT_3},
			{InputID::Mean,PAD_INPUT_8}

		};

	}

	return true;
}

void PadInput::Updata(void)
{
		_keyData[0]= GetJoypadInputState(DX_INPUT_PAD1);
		_keyData[1]= GetJoypadInputState(DX_INPUT_PAD2);
	
	for (auto id : InputID())
	{
		for (int i = 0; i < 2; i++)
		{
			_data[i][id][static_cast<int>(Trg::Old)] = _data[i][id][static_cast<int>(Trg::Now)];
			_data[i][id][static_cast<int>(Trg::Now)] = _keyData[i] & _inputTbl[id];
		

		}
	}



}
