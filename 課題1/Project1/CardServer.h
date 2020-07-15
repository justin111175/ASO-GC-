#pragma once
#include <utility>

#define LpCardServer CardServer::GetInstance()

using PairInt = std::pair<int, int>;

class CardServer
{
public:
	// �ÓI�V���O���h��
	static CardServer& GetInstance()
	{
		static CardServer s_Instance;
		return s_Instance;
	}
	
	bool Payment(int price);					// �J�[�h�x����
	PairInt& GetCardState(void);				// �J�[�h�f�[�^�����
private:

	
	PairInt cardData;							// �J�[�h�̃f�[�^�ifirst�F�c���Asceond�F�����z�j
	CardServer();
	~CardServer();

};

