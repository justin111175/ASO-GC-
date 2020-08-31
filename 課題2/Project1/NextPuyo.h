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
	const NextList& Get(void);						// ゲットnextぷよリスト
	void Draw(int color);
	pairPuyo pickUp(void);							// 古いnextぷよ消す、新しいnextぷよ追加
	bool Add(int no);								// nextぷよ追加

private:
	Vector2 nextPos_;								// nextぷよ座標
	NextList nextPuyoList_;							// nextぷよリスト
	int drawCount_;									

	int id_;
};

