#include "Ojyama.h"
#include <Dxlib.h>

Ojyama::Ojyama(Vector2&& pos, PuyoID id)
{
	pos_ = pos;
	id_ = id;
	size_ = { 48,48 };
	_alive = true;

}

Ojyama::~Ojyama()
{
}

bool Ojyama::Run(int no)
{
//	if()
	if (_pData._bit.DOWN)
	{
		pos_.y += no;
	}
	return true;
}

void Ojyama::Draw(Vector2 offset)
{	

	DrawOval(pos_.x + size_.x / 2 + offset.x,
		pos_.y + size_.y / 2  + offset.y,
		size_.x / 2,
		(size_.y / 2), 0xFFFFFF, true);

}

bool Ojyama::SetPData(DirBit dirbit)
{
	_pDataOld = _pData;
	_pData._bit = dirbit;
	return true;
}

const Vector2 Ojyama::GetGrid(int size)
{
	return Vector2{ pos_.x / size,(pos_.y) / size };
}

const PuyoID& Ojyama::ID(void)
{
	PuyoID id = PuyoID::お邪魔ぷよ;
	return id;
}

bool Ojyama::Pos(Vector2 pos)
{
	pos_ = pos;
	return true;
}

const bool& Ojyama::Alive(void)
{
	return _alive;
	// TODO: return ステートメントをここに挿入します
}

bool Ojyama::Alive(bool flag)
{
	
	return _alive= flag;
}
