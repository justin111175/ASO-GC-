#include "State.h"
#include "DxLib.h"
#include <utility>
#include "common/Input/KeyInput.h"
#include "common/Input/PadInput.h"
#include "common/_debug/_DeBugConOut.h"


int State::_stateCount = 0;
State::State(Vector2&& _offset, Vector2&& _size)
{


	_id = _stateCount;
	_stateCount++;
	//palyerDate.try_emplace(PlayerState::MAX, offset, size);
	
	// move:
	//		引数で与えられたオブジェクトの持つリソースを左辺へオブジェクトに移動する機能を提供します。
	//		所有権を移動します。std::moveは、C++11で追加されました。std::moveは、キャストの１つです。
	State::_offset =std::move(_offset);
	State::_size = std::move(_size);

	_pos = _offset;

	
	_puyo =std::make_unique<Puyo>(std::move(_pos), PuyoID::Red);
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

	for (int x = 0; x < CHIP_MAX_X; x++)
	{
		for (int y = 0; y < CHIP_MAX_Y; y++)
		{
			DrawBox(_offset.x + x * CHIP_SIZE, _offset.y + y * CHIP_SIZE,
				_offset.x + (1 + x) * CHIP_SIZE, _offset.y + (1 + y) * CHIP_SIZE, 0xFFFFFF, false);

		}
	}


	_puyo->Draw();
	




}

void State::Run(void)
{
	P_Data _pData;
	_pData._bit = { 1,1,1,1};

	for (auto data : controller[conType::Key]->GetCntData())
	{
		if (data.second[static_cast<int>(Trg::Now)]&&!data.second[static_cast<int>(Trg::Old)])
		{
			_puyo->SetPData(_pData._bit);
			_puyo->Move(data.first);
		}
	}

	for (auto data : controller[conType::Pad]->GetCntData(_id))
	{
		if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
		{
			_puyo->Move(data.first);
		}
	}

	_puyo->Run();

	(*controller[conType::Key])();
	(*controller[conType::Pad])();




}

void State::CtlInit(void)
{


}

void State::Init(void)
{
	CtlInit();

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

	dataBase.resize(CHIP_MAX_Y * CHIP_MAX_X);
	for (int no = 0; no < CHIP_MAX_Y; no++)
	{
		data.emplace_back(&dataBase[(__int64)no * CHIP_MAX_X]);
	}
	//dataBase[0] = 100;

	//TRACE("%d\n", dataBase[0]);

	screenID = MakeScreen(1280, 800, true);



}
