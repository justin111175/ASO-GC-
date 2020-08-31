#include "State.h"
#include "DxLib.h"
#include <utility>
#include "common/Input/KeyInput.h"
#include "common/Input/PadInput.h"
#include "common/_debug/_DeBugConOut.h"
#include "./Scene/SceneMng.h"
#include <functional>
#include <type_traits>
#include "EffectMng.h"
#include "common/ImageMng.h"

int State::_stateCount = 0;
State::State(Vector2&& _offset, Vector2&& _size):blockSize_(48),gridMax(8,14)
{
	_id = _stateCount;
	_stateCount++;
	
	tagetID = 0;
	dropSpeed_ = 1;

	winFlag_ = true;

	// move:
	//		引数で与えられたオブジェクトの持つリソースを左辺へオブジェクトに移動する機能を提供します。
	//		所有権を移動します。std::moveは、C++11で追加されました。std::moveは、キャストの１つです。
	State::_offset =std::move(_offset);
	State::_size = std::move(_size);
	
	// プレイのIDによって、nextぷよの位置を設定
	if (_id == 0)
	{
		auto pos = Vector2(_offset);
		nextPuyo_ = std::make_unique<NextPuyo>(pos, 3, 2,_id);
		name_ = "プレイ1";
	}
	else
	{
		auto pos = Vector2(_offset.x- blockSize_*9, _offset.y);
		nextPuyo_ = std::make_unique<NextPuyo>(pos, 3, 2,_id);
		name_ = "プレイ2";

	}

	// 初期化
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
		
	ObjDraw();

	// スコア表示する
	if (!_timeCount.GetFlag("連鎖"))
	{
		// 連鎖モードじゃない時、スコアを表示する
		number_.Draw({ _offset.x+ blockSize_*7,_offset.y+ blockSize_*13+10 }, { 1.0f,1.0f }, scoreOld_);

	}
	else
	{
		// 連鎖モードの時、100＊連鎖数を表示する
		number_.Draw({ _offset.x + blockSize_ * 7 + 10,_offset.y + blockSize_ * 13 + 5 }, { 1.0f,1.0f }, rennsaCnt_);
		DrawRotaGraph3(_offset.x + blockSize_ * 5 + 10, _offset.y + blockSize_ * 13 + 5,0,0,1.0f,0.9f,0, IMAGE_ID("X")[0], true);
		number_.Draw({ _offset.x + blockSize_ * 3 + 10,_offset.y + blockSize_ * 13 + 5 }, { 1.0f,1.0f }, 100);
	}

}

// ゲーム画面
void State::ObjDraw(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);
	DrawBox(_offset.x, _offset.y, _offset.x + (gridMax.x) * blockSize_, _offset.y + (gridMax.y) * blockSize_, _color2, true);
	SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);

	DrawBoxAA(_offset.x, _offset.y, _offset.x + (gridMax.x) * blockSize_, _offset.y + (gridMax.y) * blockSize_, 0xFFFFFF, false, 3.0f);

	DrawBox(_offset.x + blockSize_, _offset.y + blockSize_, _offset.x + (gridMax.x - 1) * blockSize_, _offset.y + (gridMax.y - 1) * blockSize_, _color, true);

	DrawBoxAA(_offset.x + blockSize_, _offset.y + blockSize_, _offset.x + (gridMax.x - 1) * blockSize_, _offset.y + (gridMax.y - 1) * blockSize_, 0xFFFFFF, false, 3.0f);

	SetFontSize(30);

	DrawString(_offset.x+10, _offset.y+5, name_, 0xFFFFFF);
	for (auto&& puyo : _puyo)
	{
		puyo->Draw(_offset);
		DrawString(_offset.x + _puyo[0]->Pos().x, _offset.y + _puyo[0]->Pos().y, "0", 0xFFFFFF, true);
		DrawString(_offset.x + _puyo[1]->Pos().x, _offset.y + _puyo[1]->Pos().y, "1", 0xFFFFFF, true);
	}
	//for (int x = 1; x < gridMax.x - 1; x++)
	//{
	//	for (int y = 1; y < gridMax.y - 1; y++)
	//	{
	//		if (_data[y][x] == PuyoID::壁)
	//		{
	//			DrawBox(_offset.x + x * blockSize_, _offset.y + y * blockSize_,
	//				_offset.x + (1 + x) * blockSize_, _offset.y + (1 + y) * blockSize_, 0xFFFFFF, true);
	//		}
	//		if (_Eraserdata[y][x]!= PuyoID::NON)
	//		{
	//			DrawBox(_offset.x +x * blockSize_, _offset.y + y * blockSize_,
	//				_offset.x + (1 + x) * blockSize_, _offset.y + (1 + y) * blockSize_, 0xFF00FF, true);
	//		}
	//		if (_data[y][x] != PuyoID::NON)
	//		{
	//			DrawFormatString(_offset.x + x * blockSize_, _offset.y + (y)*blockSize_, 0xFFFFF, "%d\n", static_cast<int>(_data[y][x]));
	//		}
	//		if (_Puyodata[y][x] != PuyoID::NON)
	//		{
	//			DrawFormatString(_offset.x + x * blockSize_, _offset.y + (y)*blockSize_, 0xFFFFF, "%d\n", static_cast<int>(_Puyodata[y][x]));
	//		}
	//		DrawBox(_offset.x + x * blockSize_, _offset.y + y * blockSize_,
	//			_offset.x + (1 + x) * blockSize_, _offset.y + (1 + y) * blockSize_, 0xFFFFFF, false);
	//	}
	//}

	// nextぷよ表示する
	nextPuyo_->Draw( _color2);

	// お邪魔ぷよ表示する
	for (auto&& ojyama : _ojyama)
	{
		ojyama->Draw(_offset);
	}

	SetFontSize(50);

	if (rennsaFlag_)
	{
		// 連鎖の時、連鎖数表示する
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, _timeCount.GetCnt("連鎖"));
		DrawFormatString(_offset.x + blockSize_ * 3, _offset.y + blockSize_ * 6, 0xFFFFFF, "%d連鎖", rennsaCnt_);
		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);
	}
}



void State::Run(void)
{
	// ゲーム終わってないとき
	if (!overFlag_)
	{		
		// ぷよの処理モードを流す
		puyoMode_[puyomode_]();
		// 連鎖数が1以上、連鎖モード（表示用）じゃない時、連鎖数を０にする
		if (rennsaCnt_ >1&&!_timeCount.GetFlag("連鎖"))
		{
			rennsaCnt_ = 0;
		}
		// 連鎖モードの時、連鎖計算（表示用）が０以下の時、連鎖モードをfalseにする

		if (rennsaFlag_)
		{
			if (_timeCount.GetCnt("連鎖") <= 0)
			{
				rennsaFlag_ = false;
			}
		}
		// 連鎖モード（表示用）の時1フレーム前のスコア＝スコア
		if (_timeCount.GetFlag("連鎖"))
		{
			scoreOld_ = score_;
		}
		else
		{
			// 1フレーム前のスコアとスコア同じじゃない場合、1フレーム前のスコアを１たす
			if (score_ != scoreOld_)
			{
				{
					scoreOld_++;

				}
			}
		}
	}
}

const int State::GetID(void)
{
	return _id;
}

void State::playerCtl(void)
{
	_pData._bit = { 1,1,1,1 };
	// ぷよの方向変換関数
	auto Rotemove = [&](sharePuyo& puyo0, sharePuyo& puyo1, InputID id) {
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

	};
	
	// キーボードでする場合
	(*controller[conType::Key])();
	for (auto data : controller[conType::Key]->GetCntData())
	{
		switch (data.first)
		{
		case InputID::Down:
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
			break;
		case InputID::テスト用:
			if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
			{
				InstanceOjyamapuyo();
			}
			break;
		default:
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
			break;
		}

	}

	// コントロールでする場合

	(*controller[conType::Pad])();
	for (auto data : controller[conType::Pad]->GetCntData(_id))
	{
		switch (data.first)
		{
		case InputID::Down:

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
			break;
		case InputID::Btn3:
			if (data.second[static_cast<int>(Trg::Now)] && !data.second[static_cast<int>(Trg::Old)])
			{
				puyomode_ = PuyoMode::直接落下;
				dropSpeed_ = 16;

			}
			break;
		default:
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
			break;
		}
	}
}

// ぷよの下のマスチェック
bool State::downCheck(sharePuyo& puyo)
{
	bool flag = true;

	// ぷよのマス位置をとる
	auto pos = puyo->GetGrid(blockSize_);
	//ぷよのマス位置の下がNONじゃない場合
	if (_data[(__int64)pos.y + 1][pos.x] != PuyoID::NON)
	{
		//ぷよのマス位置が壁じゃない場合、位置修正、下移動フラグ、コマID設定
		if(_data[(__int64)pos.y][pos.x] != PuyoID::壁)
		{		
			puyo->Pos(pos * blockSize_);
			_pData._bit.DOWN = 0;
			_data[pos.y][pos.x] = puyo->ID();
		}
	}
	else
	{
		flag = false;
	}

	return flag;
	
}

// インスタンスぷよ
bool State::InstancePuyo(void)
{
	auto pickupPuyo = nextPuyo_->pickUp();
	pickupPuyo.first->Pos(Vector2{ gridMax.x / 2 * blockSize_,blockSize_ });
	pickupPuyo.second->Pos(Vector2{ gridMax.x / 2 * blockSize_,blockSize_*1 });
	

	Vector2 pos_ = { gridMax.x / 2 * blockSize_,blockSize_ * 1 };
	_puyo.emplace(_puyo.begin(), std::make_shared<Puyo>(std::move(pos_), pickupPuyo.first->ID()));
	pos_ = { gridMax.x / 2 * blockSize_,blockSize_*2 };
	_puyo.emplace(_puyo.begin(), std::make_shared<Puyo>(std::move(pos_), pickupPuyo.second->ID()));


	return true;
}

// インスタンスお邪魔ぷよ
bool State::InstanceOjyamapuyo(void)
{
	Vector2 pos_ = { gridMax.x / 2 * blockSize_,blockSize_ };
	_ojyama.emplace(_ojyama.begin(), std::make_shared<Ojyama>(std::move(pos_), PuyoID::お邪魔ぷよ));
	return true;

}

// 消すぷよチェック
bool State::SetEraser(PuyoID id,Vector2 pos)
{
	memset(_EraserdataBase.data(), 0, _EraserdataBase.size() * sizeof(PuyoID));
	int count = 0;
	//　再帰関数
	std::function<void(PuyoID id, Vector2 vec)> DirCheck = [&](PuyoID id, Vector2 vec) {
		// マスIDと指定ぷよIDと同じ場合
		if (_data[vec.y][vec.x] == id)
		{
			// 消すマスがNONの場合、マスカンド＋１、四方向再帰
			if (_Eraserdata[vec.y][vec.x] == PuyoID::NON)
			{
				count++;
				if (_data[vec.y][vec.x + 1] == PuyoID::お邪魔ぷよ)
				{
					_Eraserdata[vec.y][vec.x + 1] = _data[vec.y][vec.x + 1];

				}
				if (_data[vec.y][vec.x - 1] == PuyoID::お邪魔ぷよ)
				{
					_Eraserdata[vec.y][vec.x - 1] = _data[vec.y][vec.x - 1];

				}
				if (_data[vec.y + 1][vec.x] == PuyoID::お邪魔ぷよ)
				{
					_Eraserdata[vec.y + 1][vec.x] = _data[vec.y + 1][vec.x];

				}
				if (_data[vec.y - 1][vec.x] == PuyoID::お邪魔ぷよ)
				{
					_Eraserdata[vec.y - 1][vec.x] = _data[vec.y - 1][vec.x];

				}

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
		// マスカンドが4より小さい場合、消すマスをリセット
		memset(_EraserdataBase.data(), 0, _EraserdataBase.size() * sizeof(PuyoID));
		return false;

	}
	else
	{
		// マスカンドが4以上場合
		for (auto&& puyo : _puyo)
		{
			auto pos = puyo->GetGrid(blockSize_);
			// マスデータと消すマスデータと同じ場合、ぷよ消す、マス設定、スコア設定
			if (_data[pos.y][pos.x] == _Eraserdata[pos.y][pos.x])
			{
				IpEffect.Play("puyo1", puyo->Pos(), _offset);
				_data[pos.y][pos.x] = PuyoID::NON;
				puyo->Alive(false);
				score_ = score_ + 10+(rennsaCnt_)*100;
			}
		}

		for (auto&& ojyama : _ojyama)
		{
			auto pos = ojyama->GetGrid(blockSize_);
			if (_data[pos.y][pos.x] == _Eraserdata[pos.y][pos.x])
			{
				_data[pos.y][pos.x] = PuyoID::NON;
				ojyama->Alive(false);
			}
		}
		return true;
	}
}

// ぷよんするかどうかチェック
bool State::PuyoCheck(Vector2 pos)
{
	int count = 0;
	// 再帰関数
	std::function<void(Vector2 pos)> PunyonCheck = [&](Vector2 pos) {
		// 指定したマスが壁じゃない場合
		if (_data[pos.y][pos.x] != PuyoID::壁)
		{
			// 指定したマスがNONじゃない場合
			if (_data[pos.y][pos.x] != PuyoID::NON)
			{
				// 指定したぷよから計算始めます、下３個目まで、ぷよん設定する
				if (count <= 3)
				{
					count++;
					_Puyodata[pos.y][pos.x] = _data[pos.y][pos.x];
					PunyonCheck({ pos.x,pos.y + 1 });
				}
			}
		}
	};

	PunyonCheck(pos);
	// 全部回す
	for (auto&& puyo : _puyo)
	{
		auto pos = puyo->GetGrid(blockSize_);
		// ぷよんデータがNONじゃない場合、ぷよん設定する
		if (_Puyodata[pos.y][pos.x] != PuyoID::NON)
		{

			puyo->puyoMode_ = PuyonState::puyo;
			puyo->puyoFlag_ = true;
		}
		else
		{
			// 止まる
			puyo->puyoMode_ = PuyonState::stay;
			puyo->puyoFlag_ = false;
		}
	}

	return true;

}

// ぷよ消す
void State::delPuyo(void)
{
	// remove_if：条件を満たす要素を除ける。
	// erase：指定した要素を削除する。
	_puyo.erase(std::remove_if(
		_puyo.begin(),
		_puyo.end(),
		[](sharePuyo& obj) {return !(obj)->Alive(); }), _puyo.end());
	
	_ojyama.erase(std::remove_if(
		_ojyama.begin(),
		_ojyama.end(),
		[](sharedOjyama& obj) {return !(obj)->Alive(); }), _ojyama.end());

}

void State::Init(void)
{
	_color = 0x000033<<(16*_id);
	_color2 = 0x0000AA<<(16*_id);
	
	number_.Init();

	Vector2 pos_ = { gridMax.x / 2 * blockSize_,blockSize_ };
	int id = (rand() % (static_cast<int>(PuyoID::Max) - 3) + 1);
	_puyo.emplace(_puyo.begin(), std::make_shared<Puyo>(std::move(pos_), static_cast<PuyoID>(id)));

	pos_ = { gridMax.x / 2 * blockSize_,blockSize_ * 2 };
	id = (rand() % (static_cast<int>(PuyoID::Max) - 3) + 1);
	_puyo.emplace(_puyo.begin(), std::make_shared<Puyo>(std::move(pos_), static_cast<PuyoID>(id)));

	overFlag_ = false;

	controller.try_emplace(conType::Key, std::make_unique<KeyInput>());
	controller.try_emplace(conType::Pad, std::make_unique<PadInput>());

	controller[conType::Key]->SetUp(_id);
	controller[conType::Pad]->SetUp(_id);

	score_ = 0;
	rennsaCnt_ = 0;
	sceneFlag_ = false;
	rennsaFlag_ = false;

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

	_PuyodataBase.resize((__int64)gridMax.x * gridMax.y);

	for (size_t no = 0; no < gridMax.y; no++)
	{
		_Puyodata.emplace_back(&_PuyodataBase[no * gridMax.x]);
	}

	for (int x = 0; x < gridMax.x; x++)
	{
		for (int y = 0; y < gridMax.y; y++)
		{
			_data[y][x] = PuyoID::NON;
			_data[0][x] = PuyoID::壁;
			_data[(__int64)gridMax.y - 1][x] = PuyoID::壁;
			_data[y][0] = PuyoID::壁;
			_data[y][gridMax.x - 1] = PuyoID::壁;
		}
	}


	screenID = MakeScreen(_size.x, _size.y, true);

	puyoMode_.try_emplace(PuyoMode::落下, [&]() {
		// 操作
		playerCtl();
		TRACE("落下\n");

		// ぷよ全部回す
		std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharePuyo& puyo) {
			// 落ちる
			puyo->Run(dropSpeed_);
			{
				// 落ちるかどうかチェック
				if (downCheck(puyo))
				{
					// ぷよの下にはぷよあるいは壁がある場合、ぷよんの設定
					auto pos = puyo->GetGrid(blockSize_);
					_pData._bit = { 0,0,0,0 };
					puyo->SetPData(_pData._bit);
					// 落ちっていたぷよ
					if (!puyo->_pData._bit.DOWN && puyo->_pDataOld._bit.DOWN)
					{
						dropSpeed_ = 16;
						PuyoCheck(pos);
						puyomode_ = PuyoMode::ぷよ;
					}
				}
			}
		});
	});

	puyoMode_.try_emplace(PuyoMode::直接落下, [&]() {
		TRACE("直接落下\n");

		// ぷよ全部回す
		std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharePuyo& puyo) {
			// 落ちる
			puyo->Run(dropSpeed_);
			{
				// 落ちるかどうかチェック
				if (downCheck(puyo))
				{
					// ぷよの下にはぷよあるいは壁がある場合、ぷよんの設定
					auto pos = puyo->GetGrid(blockSize_);
					_pData._bit = { 0,0,0,0 };
					puyo->SetPData(_pData._bit);
					// 落ちっていたぷよ
					if (!puyo->_pData._bit.DOWN && puyo->_pDataOld._bit.DOWN)
					{
						dropSpeed_ = 16;
						PuyoCheck(pos);
						puyomode_ = PuyoMode::ぷよ;
					}
				}
			}
		});

		// 落ちっていたぷよがない場合、全部のぷよをもう一度確認、大丈夫です。ならぷよんモードに行く
		bool Flag = true;
		std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharePuyo& puyo) {

			return Flag &= downCheck(puyo);
		});

		if (Flag)
		{
			puyomode_ = PuyoMode::ぷよ;
		}
	});

	puyoMode_.try_emplace(PuyoMode::連鎖, [&]() {
		TRACE("連鎖\n");
		// お邪魔ぷよの下チェックのラムダ式関数
		auto Check = [&](sharedOjyama& ojyama) {
			auto pos = ojyama->GetGrid(blockSize_);
			if (_data[(__int64)pos.y + 1][pos.x] != PuyoID::NON)
			{
				ojyama->Pos(pos * blockSize_);
				_pData._bit.DOWN = 0;
				_data[pos.y][pos.x] = PuyoID::お邪魔ぷよ;
			}
			else
			{
				return false;
			}
		};
		// 連鎖の時、ちょっと待つ
		if (_timeCount.GetFlag("連鎖落下"))
		{
			return;
		}

		bool rennsaFlag = true;

		//std::for_each(_ojyama.rbegin(), _ojyama.rend(), [&](sharedOjyama& ojyama) {

		//	return rennsaFlag &= Check(ojyama);
		//});

		//std::for_each(_ojyama.rbegin(), _ojyama.rend(), [&](sharedOjyama& ojyama) {

		//	if (!ojyama->Run(dropSpeed_))
		//	{
		//		rennsaFlag = false;
		//	}
		//});

		// 全部のぷよ下チェックがtrueなら、消すモードに行く、一つがfalseなら、また直接落下モードに行く
		std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharePuyo& puyo) {
			return rennsaFlag &= downCheck(puyo);

		});

		if (rennsaFlag)
		{
			puyomode_ = PuyoMode::消す;
		}
		else
		{
			puyomode_ = PuyoMode::直接落下;
		}
	
	});


	puyoMode_.try_emplace(PuyoMode::ぷよ, [&]() {
		TRACE("ぷよ\n");

		// ぷよんが終わるかどうかの確認
		std::function<bool (sharePuyo& puyo)> PuyonState = [&](sharePuyo& puyo) {
			auto pos = puyo->GetGrid(blockSize_);
			// ぷよの下が壁じゃない場合
			if (_data[(__int64)pos.y + 1][pos.x] != PuyoID::壁)
			{
				// ぷよの状態がぷよんなら（停止状態じゃない場合）
				if (puyo->puyoFlag_)
				{
					// ぷよん開始
					if (puyo->puyo())
					{
						// ぷよん終わった
						return true;
					}
				}
				else
				{
					// ぷよは停止状態の場合）
					return true;
				}
			}
			else
			{
				// ぷよの下が壁の場合
				return true;
			}
			// 何もしない
			return  false;
		};

		bool Flag = true;
		bool drop = false;
		// ぷよ全部回す
		std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharePuyo& puyo) {
			// 二つ目のぷよがまた落ちっている場合
			if (!downCheck(puyo))
			{
				puyo->Run(dropSpeed_);
				drop = true;
			}
			else
			{
				// ぷよんチェック
				auto pos = puyo->GetGrid(blockSize_);
				_pData._bit = { 0,0,0,0 };
				puyo->SetPData(_pData._bit);
				if (!puyo->_pData._bit.DOWN && puyo->_pDataOld._bit.DOWN)
				{
					PuyoCheck(pos);
				}
			}

			return Flag &= PuyonState(puyo);
		});

		// ぷよんが終わる場合
		if (Flag)
		{
			// ぷよん用チェック保存をリセットする
			memset(_PuyodataBase.data(), 0, _PuyodataBase.size() * sizeof(PuyoID));
			// もし全員落ちって、ぷよんもおわったら、むにょんモードに行く
			if(!drop)
			{
				puyomode_ = PuyoMode::むにょん;
			}
		}
	});

	puyoMode_.try_emplace(PuyoMode::むにょん, [&]() {
		TRACE("むにょん\n");

		// 指定方向のマスのIDをチェック
		auto dirCheck = [&](PuyoID id, Vector2 vec) {

			return (id == _data[vec.y][vec.x]);
		};
		// 指定ぷよの四方向チェック、trueの場合むにょんを表示する
		auto check = [&](sharePuyo& puyo) {
			_drawData._bit = { 0,0,0,0 };
			auto Pos = puyo->GetGrid(blockSize_);
			_drawData._bit.RIGHT = dirCheck(puyo->ID(), { Pos.x + 1,Pos.y });
			_drawData._bit.LEFT = dirCheck(puyo->ID(), { Pos.x - 1,Pos.y });
			_drawData._bit.UP = dirCheck(puyo->ID(), { Pos.x ,Pos.y - 1 });
			_drawData._bit.DOWN = dirCheck(puyo->ID(), { Pos.x ,Pos.y + 1 });
			puyo->SetDrawData(_drawData._bit);
			return true;
		};
		bool flag = true;
		// 全部のぷよを回して、ぷよ位置を修正して、四方向をチェック
		std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharePuyo& puyo) {
			puyo->Cnt(0);
			auto Pos = puyo->GetGrid(blockSize_);
			puyo->Pos(Pos* blockSize_);
			return flag &= check(puyo);

		});
		// もし全部チェック終わったら、連鎖モードに戻る
		if (flag)
		{
			puyomode_ = PuyoMode::連鎖;
		}
	});
	puyoMode_.try_emplace(PuyoMode::消す, [&]() {
		TRACE("消す\n");

		bool Flag = true;
		// ぷよ全部回す
		std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharePuyo& puyo) {
			auto pos = puyo->GetGrid(blockSize_);
			auto id = puyo->ID();
			// 消すぷよ確認
			if (SetEraser(id, pos))
			{
				// trueの場合ぷよがいるマスがNONにする、そして、コントロールを振動する
				std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharePuyo& puyo) {

					auto pos = puyo->GetGrid(blockSize_);

					_data[pos.y][pos.x] = PuyoID::NON;

					StartJoypadVibration(DX_INPUT_PAD1, 1000, 200);


				});

				std::for_each(_ojyama.rbegin(), _ojyama.rend(), [&](sharedOjyama& ojyama) {

					auto pos = ojyama->GetGrid(blockSize_);
					_data[pos.y][pos.x] = PuyoID::NON;
					StartJoypadVibration(DX_INPUT_PAD1, 1000, 200);
				});
			}
		});

		// 死んだぷよをlistから消す
		delPuyo();

		// 消した後に、もう一度確認
		std::for_each(_puyo.rbegin(), _puyo.rend(), [&](sharePuyo& puyo) {

			return Flag &= downCheck(puyo);

		});
		
		// ぷよ全部が止まる場合
		if (Flag)
		{
			puyomode_ = PuyoMode::お邪魔ぷよ落下;
		}
		else
		{
			// また落ちる場合、連鎖数＋１、連鎖開始して、直接落下モードに戻る
			rennsaCnt_++;
			if ((rennsaCnt_ > 1))
			{
				_timeCount.Set("連鎖", true, 5);
				rennsaFlag_ = true;

			}
			_timeCount.Set("連鎖落下", true, 2);
			puyomode_ = PuyoMode::直接落下;
		}

	
	
	});
	puyoMode_.try_emplace(PuyoMode::お邪魔ぷよ落下, [&]() {


		auto Check = [&](sharedOjyama& ojyama) {
				
			auto pos = ojyama->GetGrid(blockSize_);
			if (_data[(__int64)pos.y + 1][pos.x] != PuyoID::NON)
			{			
				ojyama->Pos(pos* blockSize_);

				_pData._bit.DOWN = 0;

				_data[pos.y][pos.x] = PuyoID::お邪魔ぷよ;

			}
			else
			{
				return false;
			}
		};


		bool rennsaFlag = true;

		std::for_each(_ojyama.rbegin(), _ojyama.rend(), [&](sharedOjyama& ojyama) {

			return rennsaFlag &= Check(ojyama);
		});

		std::for_each(_ojyama.rbegin(), _ojyama.rend(), [&](sharedOjyama& ojyama) {

			if (!ojyama->Run(8))
			{
				rennsaFlag = false;
			}




		});

		std::for_each(_ojyama.rbegin(), _ojyama.rend(), [&](sharedOjyama& ojyama) {

			return rennsaFlag &= Check(ojyama);
		});


		if (rennsaFlag)
		{

			puyomode_ = PuyoMode::オーバーチェック;

		}





	});

	puyoMode_.try_emplace(PuyoMode::オーバーチェック, [&]() {
		TRACE("オーバーチェック\n");

		bool Flag = false;

		// 全部ぷよを確認（上から）、もし一行目にぷよがある場合ループを止まって、Flagがfalseにする
		std::for_each(_puyo.begin(), _puyo.end(), [&](sharePuyo& puyo)
		{
			//puyo->puyoMode_ = PuyonState::stay;
			auto pos = puyo->GetGrid(blockSize_);
			if (pos.y <= 1)
			{
				return Flag = true;
			}
		});

		// もしflaseの場合、新しいぷよをつくり、連鎖数０にして、落下モードに戻って、ゲーム続ける
		if (!Flag)
		{
			InstancePuyo();
			if (rennsaCnt_ > 0 && !_timeCount.GetFlag("連鎖"))
			{
				rennsaCnt_ = 0;
			}
			dropSpeed_ = 1;

			puyomode_ = PuyoMode::落下;

		}
		else
		{
			//ゲームオーバー
			winFlag_ = false;
			overFlag_ = true;
		}
	});
}
