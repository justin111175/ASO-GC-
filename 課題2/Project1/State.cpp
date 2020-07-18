#include "State.h"
#include "DxLib.h"
#include <utility>
#include "common/Input/KeyInput.h"
#include "common/Input/PadInput.h"
#include "common/_debug/_DeBugConOut.h"
#include "SceneMng.h"
#include <functional>
#include <type_traits>

int State::_stateCount = 0;
State::State(Vector2&& _offset, Vector2&& _size):blockSize_(48),gridMax(8,14)
{
	_id = _stateCount;
	_stateCount++;
	
	// move:
	//		�����ŗ^����ꂽ�I�u�W�F�N�g�̎����\�[�X�����ӂփI�u�W�F�N�g�Ɉړ�����@�\��񋟂��܂��B
	//		���L�����ړ����܂��Bstd::move�́AC++11�Œǉ�����܂����Bstd::move�́A�L���X�g�̂P�ł��B
	State::_offset =std::move(_offset);
	State::_size = std::move(_size);

	Vector2 pos_ = { gridMax.x / 2 * blockSize_,0 };

	int id= (rand() % (static_cast<int>(PuyoID::Max)-2)+1);
	_puyo.emplace(_puyo.begin(), std::make_unique<Puyo>(std::move(pos_), static_cast<PuyoID>(id)));
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
	
	DrawBox(_offset.x, _offset.y, _offset.x+ gridMax.x*blockSize_, _offset.y+ gridMax.y*blockSize_, _color, true);


	for (int i = 0; i < (__int64)gridMax.x * gridMax.y; i++)
	{

		if (_dataBase[i] == PuyoID::��)
		{
			DrawBox(_offset.x + (i % gridMax.x) * blockSize_, _offset.y + (i / gridMax.x) * blockSize_,
				_offset.x + (1 + (i % gridMax.x)) * blockSize_, _offset.y + (1 + (i / gridMax.x)) * blockSize_, 0xFFFFFF, true);

		}

		if (_EraserdataBase[i] != PuyoID::NON)
		{
			DrawBox(_offset.x + (i % gridMax.x) * blockSize_, _offset.y + (i / gridMax.x) * blockSize_,
				_offset.x + (1 + (i % gridMax.x)) * blockSize_, _offset.y + (1 + (i / gridMax.x)) * blockSize_, 0xFF00FF, true);

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
	
	//_puyo[0]->_State(PuyoState::������);





	for (auto data : controller[conType::Key]->GetCntData())
	{
		if (data.first != InputID::Down)
		{
			if (data.second[static_cast<int>(Trg::Now)]&&!data.second[static_cast<int>(Trg::Old)])
			{
				auto pos = _puyo[0]->GetGrid(blockSize_);
				if (_data[pos.y][pos.x + 1] != PuyoID::NON)
				{
					_pData._bit.RIGHT = 0;
				}
				if (_data[pos.y][pos.x - 1] != PuyoID::NON)
				{
					_pData._bit.LEFT = 0;
				}

				_puyo[0]->SetPData(_pData._bit);
				_puyo[0]->Move(data.first);
			}
		}
		else
		{
			if (data.second[static_cast<int>(Trg::Now)] )
			{
				auto pos = _puyo[0]->GetGrid(blockSize_);

				if (_data[pos.y + 1][pos.x] != PuyoID::NON)
				{
					_pData._bit.DOWN = 0;
					_puyo[0]->_State(PuyoState::�~�܂�);
					break;
				}

				_puyo[0]->SetPData(_pData._bit);

				_puyo[0]->Move(data.first);
				
					
			}
		}


	}
		
	auto pos = _puyo[0]->GetGrid(blockSize_);

	if (_puyo[0]->State() == PuyoState::������)
	{
		if (_puyo[0]->Run())
		{
			if (_data[pos.y + 1][pos.x] != PuyoID::NON)
			{
				_puyo[0]->Pos(pos * blockSize_);
				_pData._bit.DOWN = 0;
				_puyo[0]->_State(PuyoState::�~�܂�);
			}
		}

		
	}
	else
	{
		_data[pos.y][pos.x] = _puyo[0]->ID();
		SetEraser();
		InstancePuyo();
	}









	//for (auto data : controller[conType::Pad]->GetCntData(_id))
	//{
	//	if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
	//	{
	//		for (auto&& puyo : _puyo)
	//		{
	//			auto pos = puyo->GetGrid(blockSize_);


	//			if (_data[pos.y][pos.x + 1] != PuyoID::NON)
	//			{
	//				_pData._bit.RIGHT = 0;
	//			}
	//			if (_data[pos.y][pos.x - 1] != PuyoID::NON)
	//			{
	//				_pData._bit.LEFT = 0;
	//			}
	//			if (_data[pos.y + 1][pos.x] != PuyoID::NON)
	//			{
	//				_pData._bit.DOWN = 0;
	//				_data[pos.y][pos.x] = PuyoID::��;

	//			}
	//			puyo->SetPData(_pData._bit);
	//			puyo->Move(data.first);


	//		}
	//	}
	//}

	(*controller[conType::Key])();





	_puyo.erase(std::remove_if(
		_puyo.begin(),
		_puyo.end(),
		[](std::unique_ptr<Puyo>& obj) {return !(obj)->Alive(); }), _puyo.end());

	

	//for (auto&& puyo : _puyo)
	//{
	//	auto a = std::remove_if(_puyo.begin(),_puyo.end(),
	//		[&]() {return !puyo->Alive(); });

	//	//_puyo.erase(a);

	//}
}

bool State::InstancePuyo(void)
{
	Vector2 pos_ = { gridMax.x / 2 * blockSize_,0 };
	int id = (rand() % (static_cast<int>(PuyoID::Max) - 2) + 1);
	_puyo.emplace(_puyo.begin(), std::make_unique<Puyo>(std::move(pos_), static_cast<PuyoID>(id)));


	return true;
}

void State::SetEraser(void)
{


	memset(_EraserdataBase.data(), 0, _EraserdataBase.size() * sizeof(PuyoID));

	auto pos = _puyo[0]->GetGrid(blockSize_);
	auto id = _puyo[0]->ID();
	int count = 0;

	std::function<void(PuyoID id, Vector2 vec)> DirCheck = [&](PuyoID id, Vector2 vec) {
		if (_data[vec.y][vec.x] == id)
		{
			if (_Eraserdata[vec.y][vec.x] == PuyoID::NON)
			{
				count++;
				_Eraserdata[vec.y][vec.x] = _data[vec.y][vec.x];

				DirCheck(id, { vec.x - 1,vec.y });
				DirCheck(id, { vec.x + 1,vec.y });
				DirCheck(id, { vec.x,vec.y - 1 });
				DirCheck(id, { vec.x,vec.y + 1 });
			}
		}

	};


	DirCheck(id, pos);


	if (count < 4)
	{
		//memset(_EraserdataBase.data(), 0, _EraserdataBase.size() * sizeof(PuyoID));

	}
	else
	{
		for (auto&& puyo : _puyo)
		{
			auto pos = puyo->GetGrid(blockSize_);
			if (_data[pos.y][pos.x] == _Eraserdata[pos.y][pos.x])
			{
				_data[pos.y][pos.x] = PuyoID::NON;
				puyo->Alive(false);
			}
		}
		memset(_EraserdataBase.data(), 0, _EraserdataBase.size() * sizeof(PuyoID));
	}







}



void State::Init(void)
{

	_color = 0x000033<<(16*_id);

	//controller = std::make_unique<KeyInput>();
	
	
	controller.try_emplace(conType::Key, std::make_unique<KeyInput>());
	controller.try_emplace(conType::Pad, std::make_unique<PadInput>());



	controller[conType::Key]->SetUp(_id);
	controller[conType::Pad]->SetUp(_id);




	_dataBase.resize((__int64)gridMax.x * gridMax.y);

	for (size_t no = 0; no < gridMax.y; no++)
	{
		_data.emplace_back(&_dataBase[no * gridMax.x]);
	}

	
	_EraserdataBase.resize((__int64)gridMax.x * gridMax.y);

	for (size_t no = 0; no < gridMax.y; no++)
	{
		_Eraserdata.emplace_back(&_EraserdataBase[no * gridMax.x]);
	}

	for (int x = 0; x < gridMax.x;x++)
	{
		for (int y = 0; y < gridMax.y; y++)
		{
			_data[y][x] = PuyoID::NON;

			_data[(__int64)gridMax.y-1][x] = PuyoID::��;
			_data[y][0] = PuyoID::��;
			_data[y][gridMax.x-1] = PuyoID::��;
		}
	}
	//dataBase[0] = 100;

	//TRACE("%d\n", dataBase[0]);

	screenID = MakeScreen(1280, 800, true);



}
