#include "Monster.h"


//ポリモーフィズム
//constの変数だけはここで初期化しかできない
Monster::Monster():HP(0),atk(0)
{

	std::cout << "モンスタークラスを生成\n" << std::endl;
	//HP = 0;
}

Monster::~Monster()
{
	std::cout << "モンスタークラスを廃棄\n" << std::endl;


}

int Monster::GetHP(void)
{
	return HP;
}

//void Monster::AddHP(int num)
//{
//	HP += num;
//}

void Monster::AddHP(void)
{
	HP++;
}
