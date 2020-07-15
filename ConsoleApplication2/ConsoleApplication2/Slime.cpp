#include "Slime.h"

Slime::Slime()
{

	std::cout << "スライムを生成\n" << std::endl;

	HP = 100;
}

Slime::~Slime()
{

	std::cout << "スライムを廃棄\n" << std::endl;

}

void Slime::AddHP(int num)
{

	HP += num * 2;
}
//オペレータ（演算子）のオーバーロード