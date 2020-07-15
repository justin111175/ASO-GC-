#pragma once
#include <iostream>
class Monster
{
public:
	Monster();				//�R���X�g���N�^

	virtual ~Monster();
	int GetHP(void);

	//�I�[�o�[���[�h
	virtual void AddHP(int num)=0;
	void AddHP(void);

private:
	const int atk;
protected:
	int HP;

};


//			public		private		protected
//���		�Z			�Z			��
//�z��		�Z			�~			��
//�O��		�Z			�~			�~
