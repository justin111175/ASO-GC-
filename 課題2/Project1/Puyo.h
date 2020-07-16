#pragma once
#include "common/Vector2.h"
#include <map>
#include <functional>
#include <memory>
#include "common/Input/InputID.h"

enum class PuyoID
{
	NON,
	Red,
	Wall,
	Max
};

enum class PuyoState
{
	—Ž‚¿‚é,
	Ž~‚Ü‚é,
	MAX
};

struct DirBit
{
	unsigned int UP : 1;
	unsigned int RIGHT : 1;
	unsigned int DOWN: 1;
	unsigned int LEFT : 1;

};

union Permit_Data
{
	DirBit _bit;
	unsigned int _flag;
};


class Puyo
{
public:
	Puyo(Vector2&& pos, PuyoID id);
	~Puyo();

	void Draw(Vector2 offset);
	void Run(void);
	void Move(InputID id);
	bool SetPData(DirBit dirbit);
	
	const PuyoState& State(void);
	bool _State(PuyoState state);

	virtual const Vector2 GetGrid(int size);
private:
	
	 PuyoState _state;
	Permit_Data _pData;
	void Init(void);
	Vector2 pos_;
	Vector2 _size;
};

