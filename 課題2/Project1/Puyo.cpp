#include "Puyo.h"
#include <Dxlib.h> 
#include <utility>
#include "common/_debug/_DeBugConOut.h"




Puyo::Puyo(Vector2&& pos, PuyoID id)
{

	pos_ = pos;
	_size = { 48,48 };
	Init();
}

Puyo::~Puyo()
{
}

void Puyo::Draw(Vector2 offset)
{
	DrawCircle(pos_.x+_size.x / 2+offset.x, pos_.y+ _size.y / 2, _size.x/2, 0xAA0000, true);

}

void Puyo::Run(void)
{
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
				pos_.y += _size.y;

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

const Vector2 Puyo::GetGrid(int size)
{

	
	return Vector2{ pos_.x / size,pos_.y / size };
}

void Puyo::Init(void)
{

	_state = PuyoState::—Ž‚¿‚é;
}



