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
			{InputID::Right, PAD_INPUT_RIGHT}

		};

	}
	else
	{
		_inputTbl= {
			{InputID::Up, PAD_INPUT_UP},
			{InputID::Down, PAD_INPUT_DOWN},
			{InputID::Left, PAD_INPUT_LEFT},
			{InputID::Right, PAD_INPUT_RIGHT}

		};

	}

	return true;
}

void PadInput::Updata(void)
{

	
	_keyData[0]= GetJoypadInputState(DX_INPUT_PAD1);
	//_keyData[1]= GetJoypadInputState(DX_INPUT_PAD2);

	for (auto id : InputID())
	{
		_data[0][id][static_cast<int>(Trg::Old)] = _data[0][id][static_cast<int>(Trg::Now)];
		_data[0][id][static_cast<int>(Trg::Now)] = _keyData[0] & _inputTbl[id];
		
		//_data[1][id][static_cast<int>(Trg::Old)] = _data[1][id][static_cast<int>(Trg::Now)];
		//_data[1][id][static_cast<int>(Trg::Now)] = _keyData[1] & _inputTbl[id];

	}


}
