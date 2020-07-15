#pragma once
#include <utility>

#define LpCardServer CardServer::GetInstance()

using PairInt = std::pair<int, int>;

class CardServer
{
public:
	// 静的シングルドン
	static CardServer& GetInstance()
	{
		static CardServer s_Instance;
		return s_Instance;
	}
	
	bool Payment(int price);					// カード支払い
	PairInt& GetCardState(void);				// カードデータを取る
private:

	
	PairInt cardData;							// カードのデータ（first：残高、sceond：引く額）
	CardServer();
	~CardServer();

};

