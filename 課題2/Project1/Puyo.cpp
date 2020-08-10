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
		pos_.y+ _size.y/2  +24* puyoPos_+ offset.y,
		_size.x/2,
		(_size.y/2), color_, true);
	
	//ç∂
	if (!_drawData._bit.LEFT)
	{
		DrawBox(pos_.x + offset.x, 
			pos_.y + 24 * sin(_cnt / 10.0) + offset.y,
			pos_.x + _size.x / 2 + offset.x, 
			pos_.y + _size.y + 24 * sin(_cnt / 10.0) + offset.y, color_, true);
	}
	//âE
	if (!_drawData._bit.RIGHT)
	{
		DrawBox(pos_.x + _size.x/2 + offset.x, 
			pos_.y + 24 * sin(_cnt / 10.0) + offset.y,
			pos_.x + _size.x  + offset.x, 
			pos_.y + _size.y + 24 * sin(_cnt / 10.0) + offset.y, color_, true);
	}

	//è„
	if (!_drawData._bit.UP )
	{
		DrawBox(pos_.x + offset.x, 
			pos_.y + 24 * sin(_cnt / 10.0) + offset.y,
			pos_.x + _size.x + offset.x, 
			pos_.y + _size.y / 2 + 24 * sin(_cnt / 10.0) + offset.y, color_, true);
	}
	//â∫
	if (!_drawData._bit.DOWN)
	{
		DrawBox(pos_.x + offset.x,
			pos_.y + _size.y / 2 + 24 * sin(_cnt / 10.0) + offset.y,
			pos_.x + _size.x + offset.x,
			pos_.y + _size.y + 24 * sin(_cnt / 10.0) + offset.y, color_, true);
	}
}

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

		default:
			break;
		}
	

}

bool Puyo::SetPData(DirBit dirbit)
{
	_pData._bit = dirbit;
	return true;
}

bool Puyo::SetDrawData(DirBit& dirbit)
{
	_drawData._bit = dirbit;
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

bool Puyo::Cnt(double cnt)
{
	_cnt = cnt;
	puyoPos_ = sin(_cnt / 10.0);

	return true;
}

bool Puyo::puyo(void)
{

	if (test_ == test::puyo)
	{
		if (_cnt < 30)
		{
			_cnt += 2; 
			puyoPos_ = sin(_cnt / 10.0);
			return false;

		}
		else
		{
			test_ == test::stay;
			return true;
		}

	}


	return false;

}

const Vector2 Puyo::GetGrid(int size)
{

	
	return Vector2{ pos_.x / size,(pos_.y) / size };
}

void Puyo::Init(void)
{
	_drawData._bit = { 1,1,1,1 };
	test_ = test::stay;
	_alive = true;
	_dropCnt = 0;
	_dropInt = 20;
	puyoPos_ = 0;
	_cnt = 0;
	switch (id_)
	{
	case PuyoID::ê‘:
		color_ = 0xFF0000;
		break;
	case PuyoID::óŒ:
		color_ = 0x00FF00;
		break;
	case PuyoID::ê¬:
		color_ = 0x0000FF;
		break;
	case PuyoID::â©:
		color_ = 0xFFFF00;
		break;
	case PuyoID::éá:
		color_ = 0xFF00FF;
		break;
	default:
		break;
	}



}



