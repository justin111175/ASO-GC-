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
	//���I�V���O���h��
	static Myself& GetInstance()
	{
		//once_flag:
		//				once_flag�́A��x�����w�肳�ꂽ�������Ăяo��call_once()�֐��ŁA
		//				�ǂ񂾂�true �ǂ�łȂ�������false
		static std::once_flag once;
		//call_once:
		//				�w�肳�ꂽ�֐�����x�����Ăяo���B
		//				���̊֐��͎�ɁA�����X���b�h�Ԃŋ��ʎg�p����f�[�^�̏����������ȂǂŎg�p����B

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
	bool MergeCash(MapInt& changeCash);							// ���v���z

private:
	void Draw(void);											// �`��
	
	// ������
	bool SysInit(void);
	bool MyInit(void);
	
	std::shared_ptr<MouseCtl> mouse;							// �}�E�X�̋��L

	MapInt cash;												// �����Ă��邨���ifirst:��ށAsecond�F�����j
	const VecInt& moneyType = LpTicketMachine.GetMoneyType();	// �����̃^�C�v

	PayType& payType = LpTicketMachine.GetPayType();			// �x�������@

	const Vector2 screen_size;									// ��ʂ̃T�C�Y
	const Vector2 money_size;									// �����̃T�C�Y
	const int font_size;										// �t�H���g�̃T�C�Y

	//unique_ptr
	//		�w�肳�ꂽ���\�[�X�ւ̃|�C���^�̏��L��(ownership)��B��(unique)�����Ă���悤�ɐU�����X�}�[�g�|�C���^�ł���Bauto_ptr��u����������̂Ƃ���C++�ɒǉ����ꂽ�B
	//      �R�s�[�s�\�ȃN���X�ł���B����Ƀ��[�u�ɂ���ď��L���𑼂�unique_ptr �֏��n���邱�Ƃ��ł���B�܂��Ashared_ptr��unique_ptr���󂯎�郀�[�u�R���X�g���N�^�ƃ��[�u������Z�q�����B
	
	//unique_ptr�@	���@unique_ptr�̃����o�[
	// ��			���@unique_ptr�̃����o�[�̈�Ƃ��āAMouseCtl���Ǘ�����|�C���^�[	��	MouseCtl�̃����o�[
	//mouse

	static Myself* s_Instance;
	Myself();
	~Myself();
};

