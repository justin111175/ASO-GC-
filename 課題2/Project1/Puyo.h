#pragma once
#include "common/Vector2.h"
#include <map>
#include <functional>
#include <memory>
#include "common/Input/InputID.h"


enum class PuyoID
{
	Red,
	Max
};


struct DirBit
{
	unsigned int UP : 1;
	unsigned int RIGHT : 1;
	unsigned int DOWN: 1;
	unsigned int LEFT : 1;

};

union P_Data
{
	DirBit _bit;
	unsigned int _flag;
};


class Puyo
{
public:
	Puyo(Vector2&& pos, PuyoID id);
	~Puyo();

	void Draw(void);
	void Run(void);
	void Move(InputID id);
	bool SetPData(DirBit dirbit);
private:
	
	P_Data _pData;
	void Init(void);
	Vector2 _pos;
	Vector2 _size;
};

