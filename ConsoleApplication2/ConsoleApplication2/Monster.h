#pragma once
#include <iostream>
class Monster
{
public:
	Monster();				//コンストラクタ

	virtual ~Monster();
	int GetHP(void);

	//オーバーロード
	virtual void AddHP(int num)=0;
	void AddHP(void);

private:
	const int atk;
protected:
	int HP;

};


//			public		private		protected
//基底		〇			〇			○
//配生		〇			×			○
//外部		〇			×			×
