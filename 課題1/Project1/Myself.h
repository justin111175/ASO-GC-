#pragma once
#include <mutex>
#include "Vector2.h"
#include <map>
#include <memory>
#include "TicketMachine.h"
#include <Vector>
#include "Sound.h"
#include "TimeCount.h"

#define _myself Myself::GetInstance()

class MouseCtl;

class Myself
{
public:
	//動的シングルドン
	static Myself& GetInstance()
	{
		//once_flag:
		//				once_flagは、一度だけ指定された処理を呼び出すcall_once()関数で、
		//				読んだらtrue 読んでなかったらfalse
		static std::once_flag once;
		//call_once:
		//				指定された関数を一度だけ呼び出す。
		//				この関数は主に、複数スレッド間で共通使用するデータの初期化処理などで使用する。

		std::call_once(once, Create);
		Create();
		return *s_Instance;
	}
	static void Create()
	{
		if (s_Instance==nullptr)
		{
			s_Instance = new Myself();
		}
	}
	static void Destroy()
	{
		delete s_Instance;
		s_Instance = nullptr;
	}
	bool Run(void);
	bool MergeCash(MapInt& changeCash);							// 合計金額

private:
	void Draw(void);											// 描画
	
	// 初期化
	bool SysInit(void);
	bool MyInit(void);
	
	std::shared_ptr<MouseCtl> mouse;							// マウスの共有

	MapInt cash;												// 持っているお金（first:主類、second：枚数）
	const VecInt& moneyType = LpTicketMachine.GetMoneyType();	// お金のタイプ

	PayType& payType = LpTicketMachine.GetPayType();			// 支払い方法

	const Vector2 screen_size;									// 画面のサイズ
	const Vector2 money_size;									// お金のサイズ
	const int font_size;										// フォントのサイズ

	//unique_ptr
	//		指定されたリソースへのポインタの所有権(ownership)を唯一(unique)持っているように振舞うスマートポインタである。auto_ptrを置き換えるものとしてC++に追加された。
	//      コピー不可能なクラスである。代わりにムーブによって所有権を他のunique_ptr へ譲渡することができる。また、shared_ptrはunique_ptrを受け取るムーブコンストラクタとムーブ代入演算子を持つ。
	
	//unique_ptr　	→　unique_ptrのメンバー
	// ↑			→　unique_ptrのメンバーの一つとして、MouseCtlを管理するポインター	→	MouseCtlのメンバー
	//mouse

	static Myself* s_Instance;
	Myself();
	~Myself();
};

