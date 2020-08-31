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
	const NextList& Get(void);						// �Q�b�gnext�Ղ惊�X�g
	void Draw(int color);
	pairPuyo pickUp(void);							// �Â�next�Ղ�����A�V����next�Ղ�ǉ�
	bool Add(int no);								// next�Ղ�ǉ�

private:
	Vector2 nextPos_;								// next�Ղ���W
	NextList nextPuyoList_;							// next�Ղ惊�X�g
	int drawCount_;									

	int id_;
};

