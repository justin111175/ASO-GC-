#include "Monster.h"


//�|�����[�t�B�Y��
//const�̕ϐ������͂����ŏ����������ł��Ȃ�
Monster::Monster():HP(0),atk(0)
{

	std::cout << "�����X�^�[�N���X�𐶐�\n" << std::endl;
	//HP = 0;
}

Monster::~Monster()
{
	std::cout << "�����X�^�[�N���X��p��\n" << std::endl;


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
