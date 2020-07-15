#include "CardServer.h"


//現在の残高と引く額を取り得する関数（first：残高、sceond：引く額）
PairInt& CardServer::GetCardState(void)
{
	return cardData;
}

bool CardServer::Payment(int price)
{
	if (cardData.first >= price)
	{
		cardData.first -= price;
		cardData.second = price;
		return true;
	}
	return false;
}

CardServer::CardServer()
{
	cardData = { 1000,0 };
}

CardServer::~CardServer()
{
	cardData = { 0,0 };
}
