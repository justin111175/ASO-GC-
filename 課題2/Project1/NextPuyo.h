#pragma once
#include "common/Vector2.h"
#include <memory>
#include <list>
#include "Puyo.h"

using pairPuyo = std::pair<sharePuyo, sharePuyo>;
using NextList = std::list<pairPuyo>;

class NextPuyo
{
public:
	NextPuyo(Vector2 &nextPos,int haveCount ,int drawCoun,int ID);
	~NextPuyo();
	const NextList& Get(void);
	void Draw(Vector2 offest, int color);
	pairPuyo pickUp(void);
	bool Add(int no);

private:
	Vector2 nextPos_;
	NextList nextPuyoList_;
	int drawCount_;
	int screenID;
	static int _stateCount;

	int id_;
};

