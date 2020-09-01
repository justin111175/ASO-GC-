#include "NextPuyo.h"
#include <Dxlib.h>
#include "Scene/SceneMng.h"



NextPuyo::NextPuyo(Vector2& nextPos, int haveCount, int drawCount,int ID)
{
	id_ = ID;

	nextPuyoList_.clear();
	drawCount_ = drawCount;
	nextPos_ = nextPos;
	Add(haveCount);
}

NextPuyo::~NextPuyo()
{
}

// ゲットnextぷよリスト
const NextList& NextPuyo::Get(void)
{
	return nextPuyoList_;
}

void NextPuyo::Draw(int color)
{
	int count = drawCount_;
	//Vector2 offset;
	if (id_ == 0)
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);
		DrawBox(nextPos_.x + 8 * 48, nextPos_.y + 48, nextPos_.x + 9 * 48, nextPos_.y + 3 * 48, color, true);
		DrawBox(nextPos_.x + 9 * 48, nextPos_.y + 48 + 24, nextPos_.x + 10 * 48, nextPos_.y + 3 * 48 + 24, color, true);

		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);



		DrawBoxAA(nextPos_.x + 8 * 48, nextPos_.y + 48, nextPos_.x + 9 * 48, nextPos_.y + 3 * 48, 0xFFFFFF, false, 3.0f);
		DrawBoxAA(nextPos_.x + 9 * 48, nextPos_.y + 48 + 24, nextPos_.x + 10 * 48, nextPos_.y + 3 * 48 + 24, 0xFFFFFF, false, 3.0f);
	}
	else
	{
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 125);
		DrawBox(nextPos_.x + 8 * 48, nextPos_.y + 48, nextPos_.x + 9 * 48, nextPos_.y + 3 * 48, color, true);
		DrawBox(nextPos_.x + 7 * 48, nextPos_.y + 48 + 24, nextPos_.x + 8 * 48, nextPos_.y + 3 * 48 + 24, color, true);

		SetDrawBlendMode(DX_BLENDGRAPHTYPE_NORMAL, 0);



		DrawBoxAA(nextPos_.x + 8 * 48, nextPos_.y + 48, nextPos_.x + 9 * 48, nextPos_.y + 3 * 48, 0xFFFFFF, false, 3.0f);
		DrawBoxAA(nextPos_.x + 7 * 48, nextPos_.y + 48 + 24, nextPos_.x + 8 * 48, nextPos_.y + 3 * 48 + 24, 0xFFFFFF, false, 3.0f);

	}


	for (auto puyo : nextPuyoList_)
	{
		puyo.first->Draw(nextPos_);
		puyo.second->Draw(nextPos_);

		count--;
		if (count <= 0)
		{
			break;
		}
	}
}

// 古いnextぷよ消す、新しいnextぷよ追加
pairPuyo NextPuyo::pickUp(void)
{
	auto data = *(nextPuyoList_.begin());
	nextPuyoList_.erase(nextPuyoList_.begin());
	Add(1);
	return data;
}

// nextぷよ追加
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

	Vector2 pos1 = { 48*8,48 };
	Vector2 pos2 = { 48*8,96 };
	Vector2 offset;
	
	if (id_==0)
	{
		offset = { 48,24 };
	}		
	else
	{
		offset = { -48,24 };

	}

	for (auto puyoPair : nextPuyoList_)
	{
		puyoPair.first->Pos(std::move(pos1));
		puyoPair.second->Pos(std::move(pos2));
		pos1 += offset;
		pos2 += offset;
	}
	return true;
}
