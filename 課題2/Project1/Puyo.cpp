#include "Puyo.h"
#include <Dxlib.h> 
#include <utility>
#include "common/_debug/_DeBugConOut.h"




Puyo::Puyo(Vector2&& pos, PuyoID id)
{
	_pos = std::move(pos + Vector2(24.0,24.0));
	_size = { 48,48 };
	Init();
}

Puyo::~Puyo()
{
}

void Puyo::Draw(void)
{
	DrawCircle(_pos.x, _pos.y, 24, 0xFFFFFF, true);

}

void Puyo::Run(void)
{


}

void Puyo::Move(InputID id)
{
	switch (id)
	{
	case InputID::Up:
		//TRACE("%d\n", _pos.x);

		break;
	case InputID::Down:
		if (_pData._bit.DOWN)
		{
			_pos.y += _size.y;

		}

		break;
	case InputID::Left:
		if (_pData._bit.LEFT)
		{
			_pos.x -= _size.x;
		}
		break;
	case InputID::Right:
		if (_pData._bit.RIGHT)
		{
			_pos.x += _size.x;
		}
		break;

	default:
		break;
	}
}

bool Puyo::SetPData(DirBit dirbit)
{
	_pData._bit = dirbit;
	return true;
}

void Puyo::Init(void)
{


}



