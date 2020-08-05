#pragma once
#include "Puyo.h"


class Ojyama
{
public:
	Ojyama(Vector2&& pos, PuyoID id);
	~Ojyama();
	bool Run(int no);
	void Draw(void);

	virtual const Vector2 GetGrid(int size);

	const PuyoID& ID(void);
	bool Pos(Vector2 pos);
	Permit_Data _pData;
	Permit_Data _drawData;
	
	const bool& Alive(void);
	bool Alive(bool flag);

private:
	Vector2 pos_;
	PuyoID id_;
	Vector2 size_;

	bool _alive;

};

