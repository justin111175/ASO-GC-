#include "TicketMachine.h"
#include "Myself.h"
#include "_DebugConOut.h"
#include <Dxlib.h>
#include <algorithm>
#include "TimeCount.h"

//�ÓI�V���O���g��
//TicketMachine::s_Instance;

void TicketMachine::Run(void)
{
	Vector2 pos = mouse->GetPos();

	//�����_��
	// �{�^���̓����蔻��
	auto CheckBtn = [&](Vector2 btnPos) {
		if (((pos.x >= btnPos.x) && (pos.x < btnPos.x + pay_btn_size.x)) &&
			((pos.y >= btnPos.y) && (pos.y < btnPos.y + pay_btn_size.y)))
		{
			return true;
		}
		return false;
	};

	// �{�^���̕\���ϊ�
	auto BtnKey = [&](Vector2 btnPos,std::string& btnKey) {
		if (mouse->GetClickTrg())
		{
			if (mouse->GetClicking() &&(CheckBtn(btnPos)))
			{
				btnKey = "btn_push";
			}
			else
			{
				btnKey = "btn";
			}
		}
	};

	BtnKey(btnPos["�{�^��1"], btnKey["�{�^��1"]);
	BtnKey(btnPos["�{�^��2"], btnKey["�{�^��2"]);

	if (mouse->GetClickTrg() && CheckBtn(btnPos["�{�^��2"]))
	{
		// �x�����͂܂��������ĂȂ��ꍇ�A�L�����Z������
		if (!paySuccess)
		{
			Cancel();
		}
		else
		{
			PlaySoundMem(_sound.soundGet("�G���["), DX_PLAYTYPE_BACK);
			_timeCount.Set("�G���[", true, 2);
			_bug = "����Ƃ��Ă܂���\n";

			TRACE("����Ƃ��Ă܂���\n");
		}

	}

	if (mouse->GetClickTrg() && CheckBtn(btnPos["�{�^��1"]))
	{
		// �x�����͐��������ꍇ�A�L�����Z������
		if (paySuccess)
		{
			// ���ϊ���
			if(_myself.MergeCash(cashData[payType].second))
			{
				PlaySoundMem(_sound.soundGet("�{�^��"), DX_PLAYTYPE_BACK);
				Clear();
			}
		}
		else
		{
			// �x�������@��������A�x�����֐��֍s��
			if (pay.count(payType))
			{
				pay[payType]();
			}
			else
			{
				TRACE("���m�Ȏx������\n");
				_timeCount.Set("�G���[", true, 2);
				_bug = "���m�Ȏx������\n";
				PlaySoundMem(_sound.soundGet("�G���["), DX_PLAYTYPE_BACK);

			}
		}
	}
}

bool TicketMachine::Cancel(void)
{
	// �x�������@��������A�L�����Z���֐��֍s��
	if (cancel.count(payType))
	{
		cancel[payType]();
		return true;
	}
	return false;
}

void TicketMachine::Draw(void)
{		

	// �ؕ��̒l�i
	DrawString(screen_size.x / 2 - font_size, money_size.y / 2, "�ؕ��̉��z�@�����F130�~�@�d�q�}�l�[:124�~", GetColor(255, 255, 255));

	// �x���`��	
	// ���@�P�F
	if (draw.count(payType))
	{
		draw[payType]();
	}
	// ���@�Q�F
	//if (draw.find(payType) != draw.end())
	//{
	//	draw[payType]();
	//}

	// �{�^���`��
	DrawBtn();
	// �o�b�O�`��
	BugDraw();

}

void TicketMachine::BugDraw(void)
{
	// �G���[�v�Z�t���O��true�̎��A�\������
	if (_timeCount.GetFlag("�G���["))
	{
		DrawBox(200, 0, 800, 50*(static_cast<int>(moneyType.size())+2), GetColor(0, 0, 0), true);
		DrawBox(200, 0, 800, 50*(static_cast<int>(moneyType.size()) +2), GetColor(255, 255, 255), false);
		DrawString(200, 0, "�G���[�ł�", 0xFFFFFF, true);
		SetFontSize(20);
		DrawString(250, 100, _bug, 0xFFFFFF, true);
		DrawGraph(600, 200, image["�G���["], true);
	}
}

VecInt& TicketMachine::GetMoneyType(void)
{
	return moneyType;
}

bool TicketMachine::InitDraw(void)
{
	int moneyLine = 0;
	int totalMoney = 0;

	SetFontSize(font_size);

	// MAX
	draw.try_emplace(PayType::MAX, [&]() {
		int moneyLine = 0;
		int totalMoney = 0;

		SetFontSize(font_size);

		DrawGraph(0, 0, image["money"], true);

		DrawString(0,
			comment_offsetY + GetFontSize() / 2,
			"���̘g���̌�����IC�J�[�h��I�����ăN���b�N���ē������Ă��������B\n���������������猈�σ{�^���������Ă��������B",
			0xFFFFFF);
		
		//TRACE("Function��Draw::MAX\n");
	});
		
	//�J�[�h
	draw.try_emplace(PayType::CARD, [&]() {
		int moneyLine = 0;
		int totalMoney = 0;

		SetFontSize(font_size);

		DrawGraph(0, 0, image["act_cash"], true);

		if (paySuccess)
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2,
				"���ϊ���\nIC�J�[�h���o���ۂ͎󂯎��{�^���������Ă��������B", 0xFFFFFF);
			DrawString(draw_offset.x, draw_offset.y, "�d�q�}�l�[", 0xFFFFFF);
			DrawFormatString(draw_offset.x + GetFontSize(), draw_offset.y + GetFontSize(),
				0xFFFFFF, "  �c���@%d�~", cardData.first);
			DrawFormatString(draw_offset.x + GetFontSize(), draw_offset.y + GetFontSize() * 2,
				0xFFFFFF, "�����z�@%d�~", cardData.second);
		}
		else
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2,
				"���̘g����IC�J�[�h��I�����ăN���b�N�������Ă��������B\n�������������猈�σ{�^���������Ă��������B", 0xFFFFFF);
			DrawString(draw_offset.x, draw_offset.y, "�d�q�}�l�[", 0xFFFFFF);

			DrawFormatString(draw_offset.x + GetFontSize(), draw_offset.y + GetFontSize(),
				0xFFFFFF, "  �c���@%d�~", cardData.first);
			if (cardData.first < price_card)
			{
				DrawString(draw_offset.x, draw_offset.y + GetFontSize() * 3, "�c��������܂���", 0xFF0000, true);
			}

		}
		//TRACE("Function��Draw::Card\n");
	});
	
	//�L���V���[
	draw.try_emplace(PayType::CASH, [&]() {
		int moneyLine = 0;
		int totalMoney = 0;

		SetFontSize(font_size);

		DrawGraph(0, 0, image["act_cash"], true);

		if (paySuccess)
		{
			DrawString(0, comment_offsetY + GetFontSize() / 2,
				"���ϊ���\n������󂯎��ۂ͎󂯎��{�^���������Ă��������B", 0xFFFFFF);
			DrawString(draw_offset.x, draw_offset.y, "�������z", 0xFFFFFF);
			DrawString(draw_offset.x, draw_offset.y, "              ����", 0xFFFFFF);
			int moneyLine = 0;
			int totalMoney = 0;
			for (auto cashData : cashData[payType].first)
			{
				DrawFormatString(draw_offset.x + GetFontSize(), (draw_offset.y + GetFontSize()) + moneyLine * GetFontSize(), 0xFFFFFF, "%d�~", cashData.first);
				DrawFormatString(draw_offset.x + GetFontSize(), (draw_offset.y + GetFontSize()) + moneyLine * GetFontSize(), 0xFFFFFF, "�@�@�@�@ %5d��", cashData.second);
				moneyLine++;
				totalMoney += (cashData.first * cashData.second);
			}
			DrawFormatString(draw_offset.x, (draw_offset.y + GetFontSize() * 2) + moneyLine * GetFontSize(), 0xFFFFFF, "���v�����z�@%d�~", totalMoney);


			//
			DrawString(draw_offset.x * 2, draw_offset.y, "   �ޑK", 0xFFFFFF);
			DrawString(draw_offset.x * 2, draw_offset.y, "             ����", 0xFFFFFF);
			int changeLine = 0;

			for (auto cashData : cashData[payType].second)
			{
				DrawFormatString(draw_offset.x * 2, (draw_offset.y + GetFontSize()) + changeLine * GetFontSize(), 0xFFFFFF, "%5d�~", cashData.first);
				DrawFormatString(draw_offset.x * 2, (draw_offset.y + GetFontSize()) + changeLine * GetFontSize(), 0xFFFFFF, "%15d��", cashData.second);
				changeLine++;

			}
		}
		else
		{
			DrawString(0,
				comment_offsetY + GetFontSize() / 2,
				"���̘g���̌�����I�����ăN���b�N���ē������Ă��������B\n���������������猈�σ{�^���������Ă��������B",
				0xFFFFFF);
			DrawString(draw_offset.x, draw_offset.y, "�������z", 0xFFFFFF);
			DrawString(draw_offset.x, draw_offset.y, "           ����", 0xFFFFFF);


			for (auto moneyData : cashData[payType].first)
			{
				DrawFormatString(draw_offset.x + GetFontSize(), (draw_offset.y + GetFontSize()) + moneyLine * GetFontSize(),
					0xFFFFFF, "%d�~", moneyData.first);

				DrawFormatString(draw_offset.x + GetFontSize(), (draw_offset.y + GetFontSize()) + moneyLine * GetFontSize(),
					0xFFFFFF, "�@�@�@�@�@%d��", moneyData.second);
				moneyLine++;
				totalMoney += moneyData.first * moneyData.second;

			}

			DrawFormatString(draw_offset.x, (draw_offset.y + GetFontSize() * 2) + moneyLine * GetFontSize(),
				0xFFFFFF, "���v�����@%3d�~", totalMoney);


			if (totalMoney < price_card)
			{
				DrawString(draw_offset.x, (draw_offset.y + GetFontSize() * 3) + moneyLine * GetFontSize(),
					"���z������܂���", 0xFF0000);
			}
		}
		//TRACE("Function��Draw::Cash\n");
	});
	return true;
}

bool TicketMachine::InitPay(void)
{
	// �L���V���[
	pay.try_emplace(PayType::CASH, [&]() {
		if (PayCash())
		{
			//���ϐ���
			TRACE("���ϐ���\n");
		}
	});
	
	// �J�[�h
	pay.try_emplace(PayType::CARD, [&]() {
		if (PayCard())
		{
			//���ϐ���
			TRACE("���ϐ���\n");
		}
	});

	// MAX
	pay.try_emplace(PayType::MAX, [&]() {
		PlaySoundMem(_sound.soundGet("�G���["), DX_PLAYTYPE_BACK);
		_timeCount.Set("�G���[", true, 2);
		_bug = "�������ĂȂ��󋵂ŁA���σ{�^��\n";
		TRACE("�������ĂȂ��󋵂ŁA���σ{�^��")
	});

	return true;
}

bool TicketMachine::InitCancel(void)
{
	// �L���V���[
	cancel.try_emplace(PayType::CASH, [&]() {
		if (_myself.MergeCash(cashData[payType].first))
		{
			PlaySoundMem(_sound.soundGet("�{�^��"), DX_PLAYTYPE_BACK);
			TRACE("������Ԃ�\n");
			Clear();
		}
	});

	// �J�[�h
	cancel.try_emplace(PayType::CARD, [&]() {
		PlaySoundMem(_sound.soundGet("�{�^��"), DX_PLAYTYPE_BACK);
		Clear();
		TRACE("�J�[�h�L�����Z��\n");
	});	
	
	// MAX
	cancel.try_emplace(PayType::MAX, [&]() {
		if (cashData[payType].first.size() == 0)
		{
			PlaySoundMem(_sound.soundGet("�G���["), DX_PLAYTYPE_BACK);
			_timeCount.Set("�G���[", true, 2);
			_bug = "�������ĂȂ��󋵂ŁA�L�����Z������\n";
			TRACE("�������ĂȂ��󋵂ŁA�L�����Z������\n");
		}
	});

	return true;
}

bool TicketMachine::InitInsert(void)
{
	//���@2�F
	insert.try_emplace(PayType::CASH, [&](int type) {
		// �J�[�h�ŕ������A�����������ꍇ
		if (payType == PayType::CARD)
		{
			PlaySoundMem(_sound.soundGet("�G���["), DX_PLAYTYPE_BACK);
			_timeCount.Set("�G���[", true, 2);
			LpTicketMachine._bug = "�����ŕ��������ꍇ�́A�L�����Z�����Ă��������B\n";
			return false;
		}

		if (payType == PayType::MAX)
		{
			payType = PayType::CASH;
		}

		cashData[payType].first.try_emplace(type, 0);
		cashData[payType].first[type]++;
		return true;
	});
	
	insert.try_emplace(PayType::CARD, [&](int) {
		// �����ŕ������A�J�[�h�������ꍇ
		if (payType == PayType::CASH)
		{
			PlaySoundMem(_sound.soundGet("�G���["), DX_PLAYTYPE_BACK);
			_timeCount.Set("�G���[", true, 2);
			LpTicketMachine._bug = "�J�[�h�ŕ��������ꍇ�́A�L�����Z�����Ă��������B\n";
			return false;
		}

		if (payType == PayType::MAX)
		{
			payType = PayType::CARD;
		}
		cardData = LpCardServer.GetCardState();
		return true;

	});

	return true;
}

bool TicketMachine::PayCash(void)
{
	int totalCash = 0;
	auto tmp = cashData[payType].first;
	//�������z���v
	// ���@1
	for (auto Data : tmp)
	{
		totalCash += (Data.first * Data.second);
	}

	// ���@2
	//int totalCash = 0;
	//auto tmpCashData = cashData;
	//for (int j = 0; j < moneyType.size(); j++)
	//{
	//	try
	//	{
	//		totalCash += tmpCashData[moneyType[j]] * moneyType[j];
	//	}
	//	catch (...)
	//	{
	//		//�����Ȃ�
	//	}
	//}

	// ���@3
	//int totalCash = 0;
	//auto tmpCashData = cashData;
	//for (int j = 0; j < moneyType.size(); j++)
	//{
	//	if (tmpCashData.count(moneyType[j]))
	//	{
	//		totalCash += tmpCashData[moneyType[j]] * moneyType[j];
	//	}
	//}

	if (totalCash < price_cash)
	{
		//�������z�s��
		PlaySoundMem(_sound.soundGet("�G���["), DX_PLAYTYPE_BACK);
		_timeCount.Set("�G���[", true, 2);
		_bug = "�������z�s��\n";
		return false;
	}
	
	// ���@1
	int credit = price_cash;
	for (auto& Data : tmp)
	{
		while (Data.second)
		{
			credit -= Data.first;
			Data.second--;
			TRACE("�g��������:%d\n", Data.first);
			if (credit <= 0)
			{
				break;
			}
		}
		
		if (credit <= 0)
		{
			//�x�������I������̂ŁA����̏���
			cashData[payType].second = tmp;
			int change = -credit;
			//�@���@1�F
			//for (int j = moneyType.size() - 1; j >= 0; j--)
			//{
			//	while (change>=moneyType[j])
			//	{
			//		data[payType].second.try_emplace(moneyType[j], 0);
			//		data[payType].second[moneyType[j]]++;
			//		change -= moneyType[j];
			//	}
			//	if (change < 0)
			//	{
			//		TRACE("����ُ�\n");
			//		return false;
			//	}
			//	if (change == 0)
			//	{
			//		paySuccess = true;
			//		return true;
			//	}
			//}
			
			//���@2�F
			//  crbegin:
			//				�R���e�i���̍Ō�̗v�f���w���t�C�e���[�^��Ԃ�
			std::for_each(moneyType.crbegin(), moneyType.crend(), [&](int type) {
				while (change >= type)
				{
					cashData[payType].second.try_emplace(type, 0);
					cashData[payType].second[type]++;
					change -= type;
				}
			});
			if (change < 0)
			{
				TRACE("����ُ�\n");
				PlaySoundMem(_sound.soundGet("�G���["), DX_PLAYTYPE_BACK);
				_timeCount.Set("�G���[", true, 2);
				_bug = "����ُ�\n";
				return false;
			}
			if (change == 0)
			{
				TRACE("����ݒ芮��\n")
				PlaySoundMem(_sound.soundGet("�{�^��"), DX_PLAYTYPE_BACK);

				paySuccess = true;
				return true;
			}
			break;
		}
	}
	// ���@2
	//int credit = price_cash;
	//for (int j = 0; j < moneyType.size(); j++)
	//{
	//	if (cashData.count(moneyType[j]))
	//	{
	//		while (cashData[moneyType[j]] > 0)
	//		{
	//			credit -= moneyType[j];
	//			cashData[moneyType[j]]--;
	//			
	//			TRACE("�g��������:%d\n", moneyType[j]);
	//			if (credit <= 0)
	//			{
	//				break;
	//			}
	//		}
	//	}
	//}
	paySuccess = false;

	return false;
}

bool TicketMachine::PayCard(void)
{
	if (LpCardServer.Payment(price_card))
	{
		PlaySoundMem(_sound.soundGet("�{�^��"), DX_PLAYTYPE_BACK);
		paySuccess = true;
		return true;
	}
	PlaySoundMem(_sound.soundGet("�G���["), DX_PLAYTYPE_BACK);
	_timeCount.Set("�G���[", true, 2);
	_bug = "�J�[�h���s\n";
	return false;
}

void TicketMachine::Clear(void)
{
	btnKey["�{�^��1"] = "btn";
	btnKey["�{�^��2"] = "btn";

	paySuccess = false;

	payType = PayType::MAX;
	cashData.clear();
}

void TicketMachine::DrawBtn(void)
{
	SetFontSize(font_size * 2);

	std::string btnName = (paySuccess == false ? "  ���@��" : " �󂯎��");


	DrawGraph(btnPos["�{�^��1"].x, btnPos["�{�^��1"].y, image[btnKey["�{�^��1"]], true);
	DrawString(btnPos["�{�^��1"].x+ font_size / 2, btnPos["�{�^��1"].y + font_size / 2, btnName.c_str(), 0x000000);

	DrawGraph(btnPos["�{�^��2"].x, btnPos["�{�^��2"].y, image[btnKey["�{�^��2"]], true);
	DrawString(btnPos["�{�^��2"].x + font_size / 2, btnPos["�{�^��2"].y + font_size / 2, "�L�����Z��", 0x000000);
}

bool TicketMachine::Init(sharedMouse mouse)
{
	TicketMachine::mouse = mouse;
	//UI�z�u����Ă��邨����ǉ�
	if (moneyType.size()==0)
	{
		moneyType.emplace_back(10);
		moneyType.emplace_back(50);
		moneyType.emplace_back(100);
		moneyType.emplace_back(500);
		moneyType.emplace_back(1000);
		moneyType.emplace_back(5000);
		moneyType.emplace_back(10000);
	}

	//moneyType.resize(7);
	//moneyType[0] = 10;
	//moneyType[1] = 50;
	//moneyType[2] = 100;
	//moneyType[3] = 500;
	//moneyType[4] = 1000;
	//moneyType[5] = 5000;
	//moneyType[6] = 10000;
	
	//�摜�̓ǂݍ���
	image.try_emplace("money", LoadGraph("image/money.png"));
	image.try_emplace("act_cash", LoadGraph("image/active_cash.png"));
	image.try_emplace("act_card", LoadGraph("image/active_card.png"));

	image.try_emplace("btn", LoadGraph("image/btn.png"));
	image.try_emplace("btn_push", LoadGraph("image/btn_push.png"));
	
	image.try_emplace("�G���[", LoadGraph("image/bug.png"));

	btnPos.try_emplace("�{�^��1", (screen_size.x - money_size.x * 3-20) - pay_btn_size.x, money_size.y * static_cast<int>(moneyType.size()) );
	btnPos.try_emplace("�{�^��2", (screen_size.x - money_size.x +20) - pay_btn_size.x, money_size.y * static_cast<int>(moneyType.size()) );
	
	btnKey.try_emplace("�{�^��1", "btn");
	btnKey.try_emplace("�{�^��2", "btn");


	InitDraw();
	InitPay();
	InitCancel();
	InitInsert();
	return true;
}

//���@2�F
//bool TicketMachine::InsertCash(int cash)
//{
//	if (payType == PayType::MAX)
//	{
//		payType = PayType::CASH;
//	}
//
//
//	if (payType != PayType::CASH)
//	{
//		return false;
//	}
//
//	//try
//	//{
//	//	(cashData.at(cash))++;
//	//}
//	//catch (...)
//	//{
//	//	cashData.emplace(cash, 1);
//	//}
//
//	data[payType].first.try_emplace(cash, 0);
//	data[payType].first[cash]++;
//
//	return true;
//}
//
//bool TicketMachine::InsertCard(void)
//{
//	if (payType == PayType::MAX)
//	{
//		payType = PayType::CARD;
//	}
//	else
//	{
//		//�J�[�h����ь������������̏ꍇ�̂ݎ󂯕t����̂ŁA����ȊO�͏������Ȃ�
//		return false;
//	}
//
//	cardData = LpCardServer.GetCardState();
//	return true;
//}

const bool TicketMachine::paySuccessGet(void) const
{
	return paySuccess;
}

PayType& TicketMachine::GetPayType(void)
{
	return payType;
}

DataInt& TicketMachine::GetData(void)
{
	return cashData;
}

SetPay& TicketMachine::GetInsert(void)
{
	return insert;
}


TicketMachine::TicketMachine():
	screen_size{ 800,600 },
	money_size{ 100,50 },
	font_size(18),
	pay_btn_size{200,50},
	comment_offsetY(450),
	draw_offset{200,70},
	price_cash(130),
	price_card(124)
{
	Clear();
	TRACE("TicketMachine�𐶐�\n");
}

TicketMachine::~TicketMachine()
{
	TRACE("TicketMachine��p��\n");
}
