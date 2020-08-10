#include "NextPuyo.h"
#include <Dxlib.h>

NextPuyo::NextPuyo(Vector2& nextPos, int haveCount, int drawCount)
{
	//screenID = MakeScreen(96, 48*4, true);
	nextPuyoList_.clear();
	drawCount_ = drawCount;
	nextPos_ = nextPos;
	Add(haveCount);

}

NextPuyo::~NextPuyo()
{
}

const NextList& NextPuyo::Get(void)
{
	return nextPuyoList_;
}

void NextPuyo::Draw(Vector2 offest)
{
	int count = drawCount_;
	//auto beforID = GetDrawScreen();
	//SetDrawScreen(screenID);
	//ClsDrawScreen();
	for (auto puyo : nextPuyoList_)
	{
		puyo.first->Draw(offest);
		puyo.second->Draw(offest);

		count--;
		if (count <= 0)
		{
			break;
		}
	}
	//SetDrawScreen(beforID);
	//DrawGraph(nextPos_.x, nextPos_.y, screenID, true);
}

pairPuyo NextPuyo::pickUp(void)
{
	auto data = *(nextPuyoList_.begin());
	nextPuyoList_.erase(nextPuyoList_.begin());
	Add(1);
	return data;

}

bool NextPuyo::Add(int no)
{
	while (no > 0)
	{

		auto id1= static_cast<PuyoID>((rand() % (static_cast<int>(PuyoID::Max) - 3)) + 1);
		auto id2= static_cast<PuyoID>((rand() % (static_cast<int>(PuyoID::Max) - 3)) + 1);
		nextPuyoList_.push_back(
			{
				std::make_shared<Puyo>(Vector2{0,0},id1),
				std::make_shared<Puyo>(Vector2{0,48},id2)
			}
		);
		no--;

	}

	Vector2 pos1 = { 48*8,0 };
	Vector2 pos2 = { 48*8,48 };
	Vector2 offset = { 48,24 };
	for (auto puyoPair : nextPuyoList_)
	{
		puyoPair.first->Pos(std::move(pos1));
		puyoPair.second->Pos(std::move(pos2));
		pos1 += offset;
		pos2 += offset;
	}
	return true;
}
