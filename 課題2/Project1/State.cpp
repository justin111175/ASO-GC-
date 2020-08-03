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
	
	tagetID = 0;


	// move:
	//		引数で与えられたオブジェクトの持つリソースを左辺へオブジェクトに移動する機能を提供します。
	//		所有権を移動します。std::moveは、C++11で追加されました。std::moveは、キャストの１つです。
	State::_offset =std::move(_offset);
	State::_size = std::move(_size);

	Vector2 pos_ = { gridMax.x / 2 * blockSize_,blockSize_ };
	int id = (rand() % (static_cast<int>(PuyoID::Max) - 2) + 1);
	_puyo.emplace(_puyo.begin(), std::make_shared<Puyo>(std::move(pos_), static_cast<PuyoID>(id)));

	pos_ = { gridMax.x / 2 * blockSize_,blockSize_ * 2 };
	id = (rand() % (static_cast<int>(PuyoID::Max) - 2) + 1);
	_puyo.emplace(_puyo.begin(), std::make_shared<Puyo>(std::move(pos_), static_cast<PuyoID>(id)));
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


	for (int x=0;x< gridMax.x;x++)
	{
		for (int y=0; y< gridMax.y;y++)
		{
			//if (_data[y][x] == PuyoID::壁)
			//{
			//	DrawBox(_offset.x + x * blockSize_, _offset.y + y * blockSize_,
			//		_offset.x + (1 + x) * blockSize_, _offset.y + (1 + y) * blockSize_, 0xFFFFFF, true);

			//}

			if (_Eraserdata[y][x]!= PuyoID::NON)
			{
				DrawBox(_offset.x +x * blockSize_, _offset.y + y * blockSize_,
					_offset.x + (1 + x) * blockSize_, _offset.y + (1 + y) * blockSize_, 0xFF00FF, true);

			}
			if (_data[y][x] != PuyoID::NON)
			{
				DrawFormatString(_offset.x + x * blockSize_, (y) * blockSize_, 0xFFFFF, "%d\n",static_cast<int>(_data[y][x]));

			}

			DrawBox(_offset.x + x * blockSize_, _offset.y + y * blockSize_,
				_offset.x + (1 + x) * blockSize_, _offset.y + (1 + y) * blockSize_, 0xFFFFFF, false);
		}


	}
	for (auto&& puyo : _puyo)
	{
		puyo->Draw(_offset);
		DrawString(_offset.x + _puyo[0]->Pos().x, _puyo[0]->Pos().y, "0", 0xFFFFFF, true);
		DrawString(_offset.x + _puyo[1]->Pos().x, _puyo[1]->Pos().y, "1", 0xFFFFFF, true);
	}


}

void State::Run(void)
{

	puyoMode_[puyomode_]();


}

void State::playerCtl(void)
{
	_pData._bit = { 1,1,1,1 };


	(*controller[conType::Key])();
	auto Rotemove = [&](sharedPuyo& puyo0, sharedPuyo& puyo1, InputID id) {
		auto pos = _puyo[tagetID]->GetGrid(blockSize_);
		int CntPos;
		id == InputID::Btn1 ? CntPos = blockSize_ : CntPos = -blockSize_;
		if (id == InputID::Btn1)
		{
			if (puyo0->Pos().y > puyo1->Pos().y)
			{
				if (_data[pos.y][pos.x -1] != PuyoID::NON)
				{
					return false;
				}
				//左移動
				puyo1->Pos({ puyo0->Pos().x- CntPos, puyo0->Pos().y  });
				return true;	
			}
			if (puyo0->Pos().x < puyo1->Pos().x)
			{
				//上移動
				puyo1->Pos({ puyo0->Pos().x, puyo0->Pos().y - CntPos });

				std::swap(puyo0, puyo1);
				tagetID = tagetID ^ 1;
				return true;
			}
			if (puyo0->Pos().x > puyo1->Pos().x)
			{
				//下移動

				puyo1->Pos({ puyo0->Pos().x, puyo0->Pos().y + CntPos });
				std::swap(puyo0, puyo1);
				tagetID = tagetID ^ 1;

			
				return true;
			}	
			if (puyo0->Pos().y < puyo1->Pos().y)
			{
				//右移動
				if (_data[pos.y][pos.x + 1] != PuyoID::NON)
				{
					return false;
				}
				puyo1->Pos({ puyo0->Pos().x + CntPos, puyo0->Pos().y });
				return true;
			}
		}
		else
		{
			if (puyo0->Pos().y > puyo1->Pos().y)
			{

				if (_data[pos.y][pos.x + 1] != PuyoID::NON)
				{
					return false;
				}
				//右移動
				puyo1->Pos({ puyo0->Pos().x - CntPos, puyo0->Pos().y });
				return true;
			}
			if (puyo0->Pos().x < puyo1->Pos().x)
			{
				//下移動
				puyo1->Pos({ puyo0->Pos().x, puyo0->Pos().y - CntPos });

				std::swap(puyo0, puyo1);
				tagetID = tagetID ^ 1;
				return true;
			}
			if (puyo0->Pos().x > puyo1->Pos().x)
			{
				//上移動
				puyo1->Pos({ puyo0->Pos().x, puyo0->Pos().y + CntPos });
				std::swap(puyo0, puyo1);
				tagetID = tagetID ^ 1;


				return true;
			}
			if (puyo0->Pos().y < puyo1->Pos().y)
			{
				//左移動
				if (_data[pos.y][pos.x - 1] != PuyoID::NON)
				{
					return false;
				}
				puyo1->Pos({ puyo0->Pos().x + CntPos, puyo0->Pos().y });
				return true;
			}
		}

		//return false;

	};
	for (auto data : controller[conType::Key]->GetCntData())
	{
		if (data.first != InputID::Down)
		{	
			if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
			{
				if ((data.first != InputID::Btn1)&& (data.first != InputID::Btn2))
				{
					auto RLcheck = [&](int tagetID) {
						auto pos = _puyo[tagetID]->GetGrid(blockSize_);
						if (_data[pos.y][pos.x + 1] != PuyoID::NON)
						{
							_pData._bit.RIGHT = 0;
						}
						if (_data[pos.y][pos.x - 1] != PuyoID::NON)
						{
							_pData._bit.LEFT = 0;
						}

					};
					RLcheck(tagetID);
					RLcheck(tagetID ^ 1);
					for (int i=0;i<2;i++)
					{
						_puyo[i]->SetPData(_pData._bit);
						_puyo[i]->Move(data.first);
					}
				}
				else
				{
					Rotemove(_puyo[tagetID], _puyo[tagetID^1],data.first);
				}
			}
		}
		else
		{
			if (data.second[static_cast<int>(Trg::Now)])
			{
				for (int i = 0; i < 2; i++)
				{
					auto pos = _puyo[i]->GetGrid(blockSize_);

					if (_data[(_int64)pos.y + 1][pos.x] != PuyoID::NON)
					{
						_pData._bit.DOWN = 0;
						break;
					}
					_puyo[i]->SetPData(_pData._bit);
					_puyo[i]->Move(data.first);
				}
			}
		}
	}

	(*controller[conType::Pad])();

	for (auto data : controller[conType::Pad]->GetCntData(_id))
	{
		if (data.first != InputID::Down)
		{
			if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
			{
				if ((data.first != InputID::Btn1) && (data.first != InputID::Btn2))
				{
					auto RLcheck = [&](int tagetID) {
						auto pos = _puyo[tagetID]->GetGrid(blockSize_);
						if (_data[pos.y][pos.x + 1] != PuyoID::NON)
						{
							_pData._bit.RIGHT = 0;
						}
						if (_data[pos.y][pos.x - 1] != PuyoID::NON)
						{
							_pData._bit.LEFT = 0;
						}

					};
					RLcheck(tagetID);
					RLcheck(tagetID ^ 1);
					for (int i = 0; i < 2; i++)
					{
						_puyo[i]->SetPData(_pData._bit);
						_puyo[i]->Move(data.first);
					}
				}
				else
				{
					Rotemove(_puyo[tagetID], _puyo[tagetID ^ 1], data.first);
				}
			}
		}
		else
		{
			if (data.second[static_cast<int>(Trg::Now)])
			{
				for (int i = 0; i < 2; i++)
				{
					auto pos = _puyo[i]->GetGrid(blockSize_);

					if (_data[(_int64)pos.y + 1][pos.x] != PuyoID::NON)
					{
						_pData._bit.DOWN = 0;
						break;
					}
					_puyo[i]->SetPData(_pData._bit);
					_puyo[i]->Move(data.first);
				}
			}
		}
	}

}

bool State::downCheck(sharedPuyo& puyo)
{
	bool flag = true;

	auto pos = puyo->GetGrid(blockSize_);

	if (_data[(__int64)pos.y + 1][pos.x] != PuyoID::NON)
	{

		puyo->Pos(pos * blockSize_);
		_pData._bit.DOWN = 0;
		_data[pos.y][pos.x] = puyo->ID();

	}
	else
	{
		flag = false;
	}

	

	return flag;
	
}

bool State::InstancePuyo(void)
{
	Vector2 pos_ = { gridMax.x / 2 * blockSize_,0 };
	int id = (rand() % (static_cast<int>(PuyoID::Max) - 2) + 1);
	_puyo.emplace(_puyo.begin(), std::make_shared<Puyo>(std::move(pos_), static_cast<PuyoID>(id)));

	pos_ = { gridMax.x / 2 * blockSize_,blockSize_*1 };
	id = (rand() % (static_cast<int>(PuyoID::Max) - 2) + 1);
	_puyo.emplace(_puyo.begin(), std::make_shared<Puyo>(std::move(pos_), static_cast<PuyoID>(id)));


	return true;
}

bool State::SetEraser(PuyoID id,Vector2 pos)
{


	memset(_EraserdataBase.data(), 0, _EraserdataBase.size() * sizeof(PuyoID));
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
		memset(_EraserdataBase.data(), 0, _EraserdataBase.size() * sizeof(PuyoID));
		return false;

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

		return true;
	}




}

void State::delPuyo(void)
{

	_puyo.erase(std::remove_if(
		_puyo.begin(),
		_puyo.end(),
		[](sharedPuyo& obj) {return !(obj)->Alive(); }), _puyo.end());


}



void State::Init(void)
{





	_color = 0x000033<<(16*_id);
	//controller = std::make_unique<KeyInput>();
	OverFlag = false;

	controller.try_emplace(conType::Key, std::make_unique<KeyInput>());
	controller.try_emplace(conType::Pad, std::make_unique<PadInput>());



	controller[conType::Key]->SetUp(_id);
	controller[conType::Pad]->SetUp(_id);

	

	puyoMode_.try_emplace(PuyoMode::落下,/* Drop()*/[&]() {
		
		playerCtl();


		_puyo[tagetID]->Run(1);
		_puyo[tagetID^1]->Run(1);

		if (downCheck(_puyo[tagetID^1])|| downCheck(_puyo[tagetID]))
		{
			puyomode_ = PuyoMode::ぷよ;
		}




	});
	
	

	puyoMode_.try_emplace(PuyoMode::連鎖, [&]() {


			bool rennsaFlag = true;
			std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharedPuyo& puyo) {

				return rennsaFlag &= downCheck(puyo);
			});
			std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharedPuyo& puyo) {

				if (!puyo->Run(4))
				{
					rennsaFlag = false;
				}


			

			});
	
			std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharedPuyo& puyo) {

				return rennsaFlag &= downCheck(puyo);

			});

			if (rennsaFlag)
			{
				
				puyomode_ = PuyoMode::むにょん;

			}
			else
			{
				puyomode_ = PuyoMode::連鎖;
			}
	
	});


	puyoMode_.try_emplace(PuyoMode::ぷよ, [&]() {
	

		bool Flag = true;
		std::function<bool (sharedPuyo& puyo)> test = [&](sharedPuyo& puyo) {
			auto Pos = _puyo[tagetID]->GetGrid(blockSize_);
			//if (_data[Pos.y+1][Pos.x]!= PuyoID::NON)
			{
				puyo->test_ = test::puyo;
				if (puyo->puyo())
				{
					return true;
				}
			}

			return  false;


		};

		Flag &= (test(_puyo[tagetID])|| test(_puyo[tagetID^1]));


		if (Flag)
		{
			_puyo[tagetID]->Cnt(0);
			_puyo[tagetID^1]->Cnt(0);
			puyomode_ = PuyoMode::連鎖;
			
		}


	
	});

	puyoMode_.try_emplace(PuyoMode::むにょん, [&]() {
					
		bool flag = true;

		auto dirCheck = [&](PuyoID id, Vector2 vec) {

			return !(id == _data[vec.y][vec.x]);
		};
		auto check = [&](sharedPuyo& puyo) {
		
			_drawData._bit = { 1,1,1,1 };
			auto Pos = puyo->GetGrid(blockSize_);
			_drawData._bit.RIGHT = dirCheck(puyo->ID(), { Pos.x + 1,Pos.y });
			_drawData._bit.LEFT = dirCheck(puyo->ID(), { Pos.x - 1,Pos.y });
			_drawData._bit.UP = dirCheck(puyo->ID(), { Pos.x ,Pos.y - 1 });
			_drawData._bit.DOWN = dirCheck(puyo->ID(), { Pos.x ,Pos.y + 1 });

			puyo->SetDrawData(_drawData._bit);

			return true;
		
		};

		std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharedPuyo& puyo) {

			return flag &= check(puyo);

		});

		if (flag)
		{

			puyomode_ = PuyoMode::消す;

		}



	});
	puyoMode_.try_emplace(PuyoMode::消す, [&]() {
		
		bool Flag = true;

		std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharedPuyo& puyo) {
			auto pos = puyo->GetGrid(blockSize_);
			auto id = puyo->ID();

			//	if (rennsaFlag)
			{
				if (SetEraser(id, pos))
				{
					std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharedPuyo& puyo) {

						auto pos = puyo->GetGrid(blockSize_);

						_data[pos.y][pos.x] = PuyoID::NON;
						StartJoypadVibration(DX_INPUT_PAD1, 1000, 200);


					});
				}

			}
		});

		delPuyo();

		std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharedPuyo& puyo) {

			return Flag &= downCheck(puyo);

		});
		
		if (Flag)
		{
			puyomode_ = PuyoMode::オーバーチェック;

		}
		else
		{
			puyomode_ = PuyoMode::連鎖;

		}

	
	
	});

	puyoMode_.try_emplace(PuyoMode::オーバーチェック, [&]() {

		bool Flag = false;

		auto check = [&]() {
			if (_data[1][5]!=PuyoID::NON)
			{
				return  true;
			}
			return false;

		};


		std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharedPuyo& puyo) {
		//	for (int x = 0; x < gridMax.x; x++)
			{
				if (puyo->Pos().y < 50)
				{
					return Flag = true;
				}
			}
		});
		// Flag |= check();
		if (!Flag)
		{
			InstancePuyo();
			puyomode_ = PuyoMode::落下;

		}
		else
		{
			OverFlag = true;
		}



	});

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
			_data[0][x] = PuyoID::壁;
			_data[(__int64)gridMax.y-1][x] = PuyoID::壁;
			_data[y][0] = PuyoID::壁;
			_data[y][gridMax.x-1] = PuyoID::壁;


			//_data[y][x].reset();
			//_data[0][x](PuyoID::壁);
			//_d
			//_data[(__int64)gridMax.y - 1][x] = PuyoID::壁;
			//_data[y][0] = PuyoID::壁;
			//_data[y][gridMax.x - 1] = PuyoID::壁;
		}
	}
	//dataBase[0] = 100;

	//TRACE("%d\n", dataBase[0]);

	screenID = MakeScreen(1280, 800, true);



}
