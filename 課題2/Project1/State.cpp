#include "State.h"
#include "DxLib.h"
#include <utility>
#include "common/Input/KeyInput.h"
#include "common/Input/PadInput.h"
#include "common/_debug/_DeBugConOut.h"

int State::_stateCount = 0;
State::State(Vector2&& _offset, Vector2&& _size):blockSize_(48),gridMax(8,14)
{
	_id = _stateCount;
	_stateCount++;
	
	// move:
	//		引数で与えられたオブジェクトの持つリソースを左辺へオブジェクトに移動する機能を提供します。
	//		所有権を移動します。std::moveは、C++11で追加されました。std::moveは、キャストの１つです。
	State::_offset =std::move(_offset);
	State::_size = std::move(_size);

	Vector2 pos_ = { gridMax.x / 2 * blockSize_,0 };

	_puyo.emplace(_puyo.begin(), std::make_unique<Puyo>(std::move(pos_), PuyoID::Red));
	Init();

}

State::~State()
{
	_stateCount--;

}

int State::GetScreenId(void)
{
	return screenID;
}

void State::Draw(void)
{

	SetDrawScreen(screenID);
	
	ClsDrawScreen();
	
	DrawBox(_offset.x, _offset.y, _offset.x+512, _offset.y+512, _color, true);


	for (int i = 0; i < (__int64)gridMax.x * gridMax.y; i++)
	{

		if (_dataBase[i] == PuyoID::Wall)
		{
			DrawBox(_offset.x + (i % gridMax.x) * blockSize_, _offset.y + (i / gridMax.x) * blockSize_,
				_offset.x + (1 + (i % gridMax.x)) * blockSize_, _offset.y + (1 + (i / gridMax.x)) * blockSize_, 0xFFFFFF, true);

		}
		DrawBox(_offset.x + (i% gridMax.x) * blockSize_, _offset.y + (i / gridMax.x) * blockSize_,
			_offset.x + (1 + (i % gridMax.x)) * blockSize_, _offset.y + (1 + (i / gridMax.x)) * blockSize_, 0xFFFFFF, false);

	}
	for (auto&& puyo : _puyo)
	{
		puyo->Draw(_offset);

	}


}

void State::Run(void)
{
	Permit_Data _pData;
	_pData._bit = { 1,1,1,1};
	


	for (auto data : controller[conType::Key]->GetCntData())
	{
		if (data.second[static_cast<int>(Trg::Now)]&&!data.second[static_cast<int>(Trg::Old)])
		{
			for (auto&& puyo : _puyo)
			{
				auto pos = puyo->GetGrid(blockSize_);

				if ((_pData._bit.DOWN ==1)&& puyo->State()== PuyoState::落ちる)
				{
					if (_data[pos.y][pos.x + 1] != PuyoID::NON)
					{
						_pData._bit.RIGHT = 0;
					}
					if (_data[pos.y][pos.x - 1] != PuyoID::NON)
					{
						_pData._bit.LEFT = 0;
					}
					if (_data[pos.y + 1][pos.x] != PuyoID::NON)
					{
						_pData._bit.DOWN = 0;
						puyo->_State(PuyoState::止まる);
						_data[pos.y][pos.x] = PuyoID::Red;
						break;
					}
				}
				//else
				//{
				//	InstancePuyo();
				//	break;
				//}


				puyo->SetPData(_pData._bit);
				puyo->Move(data.first);




				


			}

		}
	}
	/*InstancePuyo();*/

	for (auto&& puyo : _puyo)
	{
		auto pos = puyo->GetGrid(blockSize_);

		if ((_pData._bit.DOWN == 0)&& puyo->State() == PuyoState::止まる)
		{
			InstancePuyo();
				
			break;

		}
	}

	


	for (auto data : controller[conType::Pad]->GetCntData(_id))
	{
		if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
		{
			for (auto&& puyo : _puyo)
			{
				auto pos = puyo->GetGrid(blockSize_);


				if (_data[pos.y][pos.x + 1] != PuyoID::NON)
				{
					_pData._bit.RIGHT = 0;
				}
				if (_data[pos.y][pos.x - 1] != PuyoID::NON)
				{
					_pData._bit.LEFT = 0;
				}
				if (_data[pos.y + 1][pos.x] != PuyoID::NON)
				{
					_pData._bit.DOWN = 0;
					_data[pos.y][pos.x] = PuyoID::Red;

				}
				puyo->SetPData(_pData._bit);
				puyo->Move(data.first);


			}
		}
	}

	(*controller[conType::Key])();
	(*controller[conType::Pad])();




}

bool State::InstancePuyo(void)
{
	Vector2 pos_ = { gridMax.x / 2 * blockSize_,0 };

	_puyo.emplace(_puyo.begin(), std::make_unique<Puyo>(std::move(pos_), PuyoID::Red));


	return true;
}



void State::Init(void)
{

	_color = 0x000033<<(16*_id);

	//controller = std::make_unique<KeyInput>();
	
	
	controller.try_emplace(conType::Key, std::make_unique<KeyInput>());
	controller.try_emplace(conType::Pad, std::make_unique<PadInput>());



	controller[conType::Key]->SetUp(_id);
	controller[conType::Pad]->SetUp(_id);


	//dataBase.resize( CHIP_MAX_Y*CHIP_MAX_X);
	//data.resize(CHIP_MAX_Y);
	//for (int no = 0; no < CHIP_MAX_Y; no++)
	//{
	//	data[no] = &dataBase[(__int64)no * CHIP_MAX_X];
	//}

	_dataBase.resize((__int64)gridMax.x * gridMax.y);
	for (size_t no = 0; no < gridMax.y; no++)
	{
		_data.emplace_back(&_dataBase[no * gridMax.x]);
	}

	for (int x = 0; x < gridMax.x;x++)
	{
		for (int y = 0; y < gridMax.y; y++)
		{
			_data[y][x] = PuyoID::NON;
			_data[(__int64)gridMax.y-1][x] = PuyoID::Wall;
			_data[y][0] = PuyoID::Wall;
			_data[y][gridMax.x-1] = PuyoID::Wall;
		}
	}
	//dataBase[0] = 100;

	//TRACE("%d\n", dataBase[0]);

	screenID = MakeScreen(1280, 800, true);



}
