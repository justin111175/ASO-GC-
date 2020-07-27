#include "Puyo.h"
#include <Dxlib.h> 
#include <utility>
#include "common/_debug/_DeBugConOut.h"
#include "common/TimeCount.h"
#include "SceneMng.h"


Puyo::Puyo(Vector2&& pos, PuyoID id)
{

	pos_ = pos;
	_size = { 48,48 };
	id_ = id;
	Init();
}

Puyo::~Puyo()
{
}

void Puyo::Draw(Vector2 offset)
{
	//DrawCircle(pos_.x+_size.x / 2+offset.x, pos_.y+ _size.y / 2, _size.x/2, color_, true);
	DrawOval(pos_.x+_size.x / 2+offset.x,
		pos_.y+ _size.y / 2 + 24*_cnt,
		_size.x/2,
		(_size.y/2)-12*_cnt, color_, true);

}

bool Puyo::Run(void)
{

	if (_dropCnt < _dropInt)
	{
		_dropCnt++;
		return false;
	}
	else
	{
		pos_.y++;
		return true;
	}
	


}

void Puyo::Move(InputID id)
{
	if (_state == PuyoState::—Ž‚¿‚é)
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

		default:
			break;
		}
	}

}

bool Puyo::SetPData(DirBit dirbit)
{
	_pData._bit = dirbit;
	return true;
}

const PuyoState& Puyo::State(void)
{
	return _state;
}

bool Puyo::_State(PuyoState state)
{
	
	_state = state;
	return true;

}

const Vector2& Puyo::Pos(void)
{
	return pos_;
}

bool Puyo::Pos(Vector2 pos)
{
	pos_ = pos;
	return true;
}

const PuyoID& Puyo::ID(void)
{
	return id_;
}




const bool& Puyo::Alive(void)
{
	return _alive;
}

bool Puyo::Alive(bool flag)
{
	return _alive=flag;
}

void Puyo::puyo(double no)
{


	_cnt =no;




}

const Vector2 Puyo::GetGrid(int size)
{

	
	return Vector2{ pos_.x / size,(pos_.y) / size };
}

void Puyo::Init(void)
{
	_alive = true;
	_dropCnt = 0;
	_dropInt = 20;
	_state = PuyoState::—Ž‚¿‚é;
	_cnt = 0;
	switch (id_)
	{
	case PuyoID::Ô:
		color_ = 0xFF0000;
		break;
	case PuyoID::—Î:
		color_ = 0x00FF00;
		break;
	case PuyoID::Â:
		color_ = 0x0000FF;
		break;
	case PuyoID::‰©:
		color_ = 0xFFFF00;
		break;
	case PuyoID::Ž‡:
		color_ = 0xFF00FF;
		break;
	default:
		break;
	}



}



