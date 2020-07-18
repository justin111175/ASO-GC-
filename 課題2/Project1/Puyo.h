#pragma once
#include "common/Vector2.h"
#include <map>
#include <functional>
#include <memory>
#include "common/Input/InputID.h"

enum class PuyoID
{
	NON,
	ê‘,
	óŒ,
	ê¬,
	â©,
	éá,

	ï«,
	Max
};

enum class PuyoState
{
	óéÇøÇÈ,
	é~Ç‹ÇÈ,
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
	bool Run(void);
	void Move(InputID id);
	bool SetPData(DirBit dirbit);
	
	const PuyoState& State(void);
	bool _State(PuyoState state);
	
	bool Pos(Vector2 pos);

	const PuyoID& ID(void);

	const bool& Alive(void);
	bool Alive(bool flag);


	Permit_Data _pData;

	virtual const Vector2 GetGrid(int size);
private:
	bool _alive;

	int _dropCnt;
	int _dropInt;
	PuyoState _state;
	
	void Init(void);
	int color_;
	Vector2 pos_;
	Vector2 _size;

	PuyoID id_;

};

