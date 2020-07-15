#pragma once
#include <map>
#include <iostream>
#include "Vector2.h"
#include <string>
#include <Vector>
#include <memory>
#include "MouseCtl.h"
#include "CardServer.h"
#include <functional>
#include "Sound.h"

enum class PayType
{
	CASH,		//現金
	CARD,		//ICカード
	MAX
};

using MapInt = std::map<int, int>;
using VecInt = std::vector<int>;
using sharedMouse = std::shared_ptr<MouseCtl>;

using DataInt = std::map<PayType, std::pair<MapInt, MapInt>>;
using SetPay = std::map<PayType, std::function<bool(int type)>>;

#define LpTicketMachine TicketMachine::GetInstance()

class TicketMachine
{
public:
	//静的シングルドン
	static TicketMachine& GetInstance()
	{
		static TicketMachine s_Instance;
		return s_Instance;
	}
	void Run(void);
	void Draw(void);									// チケット処理画面表示

	
	bool Init(sharedMouse mouse);

	//ゲット関数
	const bool paySuccessGet(void) const;				// 支払い成功かどうかフラグをとる
	VecInt& GetMoneyType(void);							// 支払いお金種類をとる
	PayType& GetPayType(void);							// 支払い方法

	DataInt& GetData(void);
	SetPay& GetInsert(void);
	const char* _bug;									// バッグ状態の文字
	
	// 方法1：
	//bool InsertCash(int cash);						// 入金受付処理
	//bool InsertCard(void);							// 入金受付処理

private:

	// 描画
	void BugDraw(void);									// バッグ用画面表示
	void DrawBtn(void);									// ボタン用画面表示

	bool PayCash(void);									// キャシューで支払い関数
	bool PayCard(void);									// カードで支払い関数
	void Clear(void);									// 要らないデータを消す
	bool Cancel(void);									// キャンセル処理、初期化


	// 基本的な初期化変数
	const Vector2 screen_size;							// 画面のサイズ
	const Vector2 money_size;							// お金のサイズ
	
	const int font_size;								// フォントのサイズ
	const Vector2 pay_btn_size;							// 支払いボタンのサイズ
	
	const int comment_offsetY;
	const Vector2 draw_offset;							// 文字描画オフセット
	
	const int price_cash;								// 現金の場合の価額
	const int price_card;								// 電子マネーの場合の価額

	DataInt cashData;									// 現金とおつりの資料
	PairInt& cardData= LpCardServer.GetCardState();		// かード資料

	PayType payType;									// 支払方法
	bool paySuccess;									// 払ったかどうか管理
	
	sharedMouse mouse;									// マウス

	std::map<std::string, Vector2> btnPos;				// 決済（受け取り）ボタンの表示の位置
	std::map<std::string, std::string> btnKey;			// 表示するボタンへのキー（image）
	std::map<std::string, int> image;					// 画像ハンドル
	SetPay insert;

	VecInt moneyType;									// 支払い手法

	// 関数化用
	std::map<PayType, std::function<void(void)>> draw;	// 描画関数化	
	std::map<PayType, std::function<void(void)>> pay;	// 支払い関数化	
	std::map<PayType, std::function<void(void)>> cancel;// 支払い関数化	

	// 関数化変数の初期化
	bool InitDraw(void);								// 描画関数初期化
	bool InitPay(void);									// 支払い関数初期化
	bool InitCancel(void);								// キャンセル関数初期化
	bool InitInsert(void);
	
	TicketMachine();
	~TicketMachine();
};


