#include "Myself.h"
#include <Dxlib.h>
#include "_DebugConOut.h"
#include "TicketMachine.h"
#include "MouseCtl.h"
#include "_DebugDispOut.h"
#include "_DebugConOut.h"
Myself* Myself::s_Instance = nullptr;

bool Myself::Run(void)
{
	
	if (!SysInit())
	{
		return false;
	}
	if (!MyInit())
	{
		return false;
	}
	if (!LpTicketMachine.Init(mouse))
	{
		return false;
	}
	Myself::GetInstance();
	TicketMachine::GetInstance();

	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		mouse->Update();

		if (!LpTicketMachine.paySuccessGet())
		{
			//�N���b�N�������u�ԁA�G���[�̌v�Z�t���O���t�@���X�̏ꍇ
			if (mouse->GetClickTrg()&&!_timeCount.GetFlag("�G���["))
			{
				Vector2 pos = mouse->GetPos();
				//�����蔻��̈ʒu
				if (pos.x < money_size.x)
				{
					if (pos.y < static_cast<int>(moneyType.size()) * money_size.y)
					{
						int type = moneyType[pos.y / money_size.y];
						//�N���b�N�����̖�����0�ȏ�A�����ē����I����ĂȂ��ꍇ
						if ((cash[type] > 0)&& !LpTicketMachine.paySuccessGet())
						{
							//�w�肵�������̖���������
							if (LpTicketMachine.GetInsert()[PayType::CASH](type));
							{
								cash[type]--;
								PlaySoundMem(_sound.soundGet("�{�^��"), DX_PLAYTYPE_BACK);
							}
						}
						else
						{
							PlaySoundMem(_sound.soundGet("�G���["), DX_PLAYTYPE_BACK);
							_timeCount.Set("�G���[", true, 2);
							LpTicketMachine._bug = "����������Ȃ��A���邢�͌��ϊ����̂ɓ������܂�\n";
							TRACE("����������Ȃ��A���邢�͌��ϊ����̂ɓ������܂�");
						}
					}
					else
					{
						//�����͈̔�+1�̈ʒu�����傤�Ǔd�q�}�l�[
						if (pos.y < static_cast<int>(moneyType.size() + 1) * money_size.y)
						{
							PlaySoundMem(_sound.soundGet("�{�^��"), DX_PLAYTYPE_BACK);
							LpTicketMachine.GetInsert()[PayType::CARD](NULL);
						}
					}
				}

			}
		}

		//auto moneyLine = 0;
			//if (mouse->GetClickTrg())
			//{
			//	//for (auto cashData : cash)
			//	//{
			//	//	//���W�ʒu������̌����̉摜�̈ʒu
			//	//	if ((mouse->GetPos().x - money_size.x / 2) * (mouse->GetPos().x - money_size.x / 2) +
			//	//		(mouse->GetPos().y - (money_size.y * moneyLine +money_size.y / 2)) * (mouse->GetPos().y - (money_size.y * moneyLine +money_size.y / 2))
			//	//		<= (money_size.y / 2) * (money_size.y / 2))
			//	//	{
			//	//		//�����̖�����0�ł͂Ȃ��ꍇ
			//	//		if (cashData.second > 0)
			//	//		{
			//	//			//���̋����̎�ނ̖���������
			//	//			cash[cashData.first]--;
			//	//		}
			//	//	}
			//	//	moneyLine++;
			//	//}
			//}
		//}

		LpTicketMachine.Run();
		
		_timeCount.Run();
		SetDrawScreen(DX_SCREEN_BACK);										// �G�����o�b�N�o�b�t�@�ɐݒ� ���̉��(�\������Ă��Ȃ���ʁj
		ClsDrawScreen();
		Draw();
		ScreenFlip();
	}
	return true;
}

bool Myself::MergeCash(MapInt& changeCash)
{
	//���@1�F
	//auto type = LpTicketMachine.GetMoneyType();
	//for (int i = 0; i < type.size(); i++)
	//{
	//	if (changeCash.count(type[i]))
	//	{
	//		cash[type[i]] += changeCash[type[i]];
	//	}
	//}

	//���@2�F
	for (auto cashData : changeCash)
	{
		cash[cashData.first] += cashData.second;
	}

	return true;
}

void Myself::Draw(void)
{	
	auto moneyLine = 0;
	SetFontSize(font_size);

	//�͈�FOR��
	for (auto cashData : cash)
	{
		//�����̖���
		DrawFormatString(money_size.x+25, money_size.y* moneyLine +money_size.y/3, GetColor(255, 255, 255), "%2d ��", cashData.second);
		moneyLine++;
	}

	//�d�؂��
	DrawLine(0, money_size.y * (static_cast<int>(moneyType.size()) + 2), screen_size.x, money_size.y * (static_cast<int>(moneyType.size()) + 2), GetColor(255, 255, 255),true);
	DrawLine(money_size.x * 2, 0, money_size.x * 2, money_size.y * (static_cast<int>(moneyType.size()) + 2), GetColor(255, 255, 255), true);
	
	//�`�P�b�g�p�`��
	LpTicketMachine.Draw();
}

bool Myself::SysInit(void)
{
	SetMainWindowText("1916229_�ۑ�1");
	SetGraphMode(screen_size.x, screen_size.y, 16);											// ��ʃ��[�h�̕ύX 800*600�h�b�g�A�U�T�T�R�U�F���[�h�ɐݒ�
	ChangeWindowMode(true);		// ture:window false:�t���X�N���[��


	if (DxLib_Init() == -1)												// DX�����u�����̏�����
	{
		return false;
	}
	return true;
}

bool Myself::MyInit(void)
{

	//���@1�F
	//	�錾�̒��g�̍쐬�𓯎��ɂ���ꍇ
	//	std::unique_ptr<MouseCtl> mouse(new MouseCtl());
	
	//���@2�F
	//	.�̓|�C���^�[���A�N�Z�X
	//	�|���̓����o�[���A�N�Z�X
	
	//	reset
	//�@�@	���\�[�X�̏��L����������A�V���ȃ��\�[�X�̏��L����ݒ肷��B
	//mouse.reset();
	//mouse.reset(new MouseCtl());

	//���@3�F
	//	make_unique
	//	unique_ptr�I�u�W�F�N�g���\�z����B

	//	(1) : ��z��^T�̃R���X�g���N�^�������󂯎��Aunique_ptr<T>�I�u�W�F�N�g���\�z����B
	//	(2) : �z��^T�̗v�f�����󂯎��Aunique_ptr<T>�I�u�W�F�N�g���\�z����B
	//	(3) : (1)�ɔz��^���w�肳�ꂽ�ꍇ�ɁA������Ă��Ȃ��I�[�o�[���[�h�Ƃ��Đ錾�����B
	//mouse = std::make_unique<MouseCtl>();

	mouse = std::make_shared<MouseCtl>();

	if (!mouse)
	{
		return false;
	}

	// �ێ����錻����ǉ�-�P�ʁA�f�[�^
	cash.try_emplace(10, 15);
	cash.try_emplace(50, 3);
	cash.try_emplace(100, 2);
	cash.try_emplace(500, 1);
	cash.try_emplace(1000, 1);
	cash.try_emplace(5000, 1);
	cash.try_emplace(10000, 1);

	return true;
}

Myself::Myself() :
	screen_size{ 800,600 },
	money_size{100,50},
	font_size(18)
{
	//MyInit();
	mouse = nullptr;
	TRACE("Myself�𐶐�\n");

}

Myself::~Myself()
{
	//delete mouse;
	TRACE("Myself��p��\n");

}
