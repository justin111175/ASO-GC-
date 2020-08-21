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
	Ç®é◊ñÇÇ’ÇÊ,
	ï«,
	Max
};

enum class PuyonState
{
	stay,
	puyo
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
	PuyonState puyoMode_;
	void Draw(Vector2 offset);
	bool Run(int no);
	void Move(InputID id);
	bool SetPData(DirBit dirbit);
	
	bool SetDrawData(DirBit& dirbit);

	const Vector2& Pos(void);
	bool Pos(Vector2 pos);

	const PuyoID& ID(void);

	const bool& Alive(void);
	bool Alive(bool flag);
	
	bool Cnt(double cnt);
	bool puyo(void);

	Permit_Data _pData;
	Permit_Data _drawData;

	virtual const Vector2 GetGrid(int size);
	void SetPuyoCnt(void);
private:
	double _cnt;
	bool _alive;

	int _dropCnt;
	int _dropInt;
	
	void Init(void);
	int color_;
	Vector2 pos_;
	Vector2 _size;

	PuyoID id_;
	double puyoPos_;
	int puyoCnt_;
};

using sharePuyo = std::shared_ptr<Puyo>;
