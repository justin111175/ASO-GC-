#include "Slime.h"

Slime::Slime()
{

	std::cout << "�X���C���𐶐�\n" << std::endl;

	HP = 100;
}

Slime::~Slime()
{

	std::cout << "�X���C����p��\n" << std::endl;

}

void Slime::AddHP(int num)
{

	HP += num * 2;
}
//�I�y���[�^�i���Z�q�j�̃I�[�o�[���[�h