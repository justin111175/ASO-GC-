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
	CASH,		//����
	CARD,		//IC�J�[�h
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
	//�ÓI�V���O���h��
	static TicketMachine& GetInstance()
	{
		static TicketMachine s_Instance;
		return s_Instance;
	}
	void Run(void);
	void Draw(void);									// �`�P�b�g������ʕ\��

	
	bool Init(sharedMouse mouse);

	//�Q�b�g�֐�
	const bool paySuccessGet(void) const;				// �x�����������ǂ����t���O���Ƃ�
	VecInt& GetMoneyType(void);							// �x����������ނ��Ƃ�
	PayType& GetPayType(void);							// �x�������@

	DataInt& GetData(void);
	SetPay& GetInsert(void);
	const char* _bug;									// �o�b�O��Ԃ̕���
	
	// ���@1�F
	//bool InsertCash(int cash);						// ������t����
	//bool InsertCard(void);							// ������t����

private:

	// �`��
	void BugDraw(void);									// �o�b�O�p��ʕ\��
	void DrawBtn(void);									// �{�^���p��ʕ\��

	bool PayCash(void);									// �L���V���[�Ŏx�����֐�
	bool PayCard(void);									// �J�[�h�Ŏx�����֐�
	void Clear(void);									// �v��Ȃ��f�[�^������
	bool Cancel(void);									// �L�����Z�������A������


	// ��{�I�ȏ������ϐ�
	const Vector2 screen_size;							// ��ʂ̃T�C�Y
	const Vector2 money_size;							// �����̃T�C�Y
	
	const int font_size;								// �t�H���g�̃T�C�Y
	const Vector2 pay_btn_size;							// �x�����{�^���̃T�C�Y
	
	const int comment_offsetY;
	const Vector2 draw_offset;							// �����`��I�t�Z�b�g
	
	const int price_cash;								// �����̏ꍇ�̉��z
	const int price_card;								// �d�q�}�l�[�̏ꍇ�̉��z

	DataInt cashData;									// �����Ƃ���̎���
	PairInt& cardData= LpCardServer.GetCardState();		// ���[�h����

	PayType payType;									// �x�����@
	bool paySuccess;									// ���������ǂ����Ǘ�
	
	sharedMouse mouse;									// �}�E�X

	std::map<std::string, Vector2> btnPos;				// ���ρi�󂯎��j�{�^���̕\���̈ʒu
	std::map<std::string, std::string> btnKey;			// �\������{�^���ւ̃L�[�iimage�j
	std::map<std::string, int> image;					// �摜�n���h��
	SetPay insert;

	VecInt moneyType;									// �x������@

	// �֐����p
	std::map<PayType, std::function<void(void)>> draw;	// �`��֐���	
	std::map<PayType, std::function<void(void)>> pay;	// �x�����֐���	
	std::map<PayType, std::function<void(void)>> cancel;// �x�����֐���	

	// �֐����ϐ��̏�����
	bool InitDraw(void);								// �`��֐�������
	bool InitPay(void);									// �x�����֐�������
	bool InitCancel(void);								// �L�����Z���֐�������
	bool InitInsert(void);
	
	TicketMachine();
	~TicketMachine();
};


