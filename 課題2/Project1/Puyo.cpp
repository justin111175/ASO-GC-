#include "Puyo.h"
#include <Dxlib.h> 
#include <utility>
#include "common/_debug/_DeBugConOut.h"
#include "common/TimeCount.h"
#include "./Scene/SceneMng.h"


Puyo::Puyo(Vector2&& pos, PuyoID id)
{
	pos_ = pos;
	id_ = id;
	Init();
}

Puyo::~Puyo()
{
}

// ぷよ描画表示
void Puyo::Draw(Vector2 offset)
{
	// ぷよ本体
	DrawOval(pos_.x+_size.x / 2+offset.x,
		pos_.y+ _size.y/2  +24* puyoPos_ + offset.y,
		_size.x/2,
		(_size.y/2) , color_, true);
	
	// むよんー＞左
	if (_drawData._bit.LEFT)
	{
		DrawBox(pos_.x + offset.x, 
			pos_.y + 24 * puyoPos_ + offset.y,
			pos_.x + _size.x / 2 + offset.x, 
			pos_.y + _size.y + 24 * puyoPos_ + offset.y, color_, true);
	}
	// むよんー＞右
	if (_drawData._bit.RIGHT)
	{
		DrawBox(pos_.x + _size.x/2 + offset.x, 
			pos_.y + 24 * puyoPos_ + offset.y,
			pos_.x + _size.x  + offset.x, 
			pos_.y + _size.y + 24 * puyoPos_ + offset.y, color_, true);
	}

	// むよんー＞上
	if (_drawData._bit.UP )
	{
		DrawBox(pos_.x + offset.x, 
			pos_.y + 24 * puyoPos_ + offset.y,
			pos_.x + _size.x + offset.x, 
			pos_.y + _size.y / 2 + 24 * puyoPos_ + offset.y, color_, true);
	}
	// むよんー＞下
	if (_drawData._bit.DOWN)
	{
		DrawBox(pos_.x + offset.x,
			pos_.y + _size.y / 2 + 24 * puyoPos_ + offset.y,
			pos_.x + _size.x + offset.x,
			pos_.y + _size.y + 24 * puyoPos_ + offset.y, color_, true);
	}
}

// 更新（落ちる速度）
bool Puyo::Run(int no)
{
	if (_dropCnt < _dropInt)
	{
		_dropCnt++;
		return false;
	}
	else
	{
		pos_.y+=no;
		return true;
	}
}

// ぷよ移動操作
void Puyo::Move(InputID id)
{
		switch (id)
		{
		case InputID::Up:
			if (_pData._bit.UP)
			{
				pos_.y -= _size.y;

			}
			break;
		case InputID::Down:

			if (_pData._bit.DOWN)
			{
				pos_.y += 4;

			}

			break;
		case InputID::Left:
			if (_pData._bit.LEFT)
			{
				pos_.x -= _size.x;
			}
			break;
		case InputID::Right:
			if (_pData._bit.RIGHT)
			{
				pos_.x += _size.x;
			}
			break;
		}
}

// 移動制限
bool Puyo::SetPData(DirBit dirbit)
{
	_pDataOld= _pData;
	_pData._bit = dirbit;
	return true;
}

// むにょん描画制御
bool Puyo::SetDrawData(DirBit& dirbit)
{
	_drawData._bit = dirbit;
	return true;
}

// ゲットPos
const Vector2& Puyo::Pos(void)
{
	return pos_;
}

// Posセット
bool Puyo::Pos(Vector2 pos)
{
	pos_ = pos;
	return true;
}

// ゲットぷよID
const PuyoID& Puyo::ID(void)
{
	return id_;
}

// ゲット生きるかどうか
const bool& Puyo::Alive(void)
{
	return _alive;
}

// 生きるかどうかセット
bool Puyo::Alive(bool flag)
{
	return _alive=flag;
}

// ぷよん用カンドセット
bool Puyo::Cnt(double cnt)
{
	_cnt = cnt;
	puyoPos_ = sin(_cnt / 10.0);

	return true;
}
// ぷよんする
bool Puyo::puyo(void)
{
	if (puyoMode_ == PuyonState::puyo)
	{
		//ぷよモードがぷよんの場合、ぷよん開始
		if (puyoFlag_)
		{
			if (_cnt <= 30)
			{
				_cnt += 2; 
				puyoPos_ = sin(_cnt / 10.0);
				return false;
			}
			else
			{
				puyoMode_ = PuyonState::stay;
				puyoFlag_ = false;
				_cnt = 0;
				puyoPos_ = sin(_cnt / 10.0);
				return false;
			}
		}
		else
		{
			return true;
		}

	}
	else
	{
		return true;

	}
}

// ぷよのマス目をとる
const Vector2 Puyo::GetGrid(int size)
{
	return Vector2{ pos_.x / size,(pos_.y) / size };
}

int Puyo::SetPuyoCnt(int cnt)
{
	return puyoCnt_ = cnt;
}

void Puyo::Init(void)
{
	_drawData._bit = { 0,0,0,0 };
	puyoCnt_ = 1;

	puyoMode_ = PuyonState::stay;

	_alive = true;
	_dropCnt = 0;
	_dropInt = 20;
	puyoPos_ = 0;
	_cnt = 0;
	_size = { 48,48 };
	puyoFlag_ = false;

	switch (id_)
	{
	case PuyoID::赤:
		color_ = 0xFF0000;
		break;
	case PuyoID::緑:
		color_ = 0x00FF00;
		break;
	case PuyoID::青:
		color_ = 0x0000FF;
		break;
	case PuyoID::黄:
		color_ = 0xFFFF00;
		break;
	case PuyoID::紫:
		color_ = 0xFF00FF;
		break;
	}
}



