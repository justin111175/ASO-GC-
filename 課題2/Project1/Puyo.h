#pragma once
#include "common/Vector2.h"
#include <map>
#include <functional>
#include <memory>
#include "common/Input/InputID.h"


enum class PuyoID
{
	NON,
	��,
	��,
	��,
	��,
	��,
	���ז��Ղ�,
	��,
	Max
};

enum class PuyonState		// �Ղ���
{
	stay,					// �~�܂�
	puyo					// �Ղ��
};

struct DirBit
{
	unsigned int UP : 1;
	unsigned int RIGHT : 1;
	unsigned int DOWN: 1;
	unsigned int LEFT : 1;

};

union Permit_Data
{
	DirBit _bit;
	unsigned int _flag;
};


class Puyo
{
public:
	Puyo(Vector2&& pos, PuyoID id);
	~Puyo();
	PuyonState puyoMode_;
	void Draw(Vector2 offset);							// �Ղ�`��\��
	bool Run(int no);									// �X�V�i�����鑬�x�j
	void Move(InputID id);								// �Ղ�ړ�����
	bool SetPData(DirBit dirbit);						// �ړ�����
	
	bool SetDrawData(DirBit& dirbit);					// �ނɂ��`�搧��

	const Vector2& Pos(void);							// �Q�b�gPos
	bool Pos(Vector2 pos);								// Pos�Z�b�g

	const PuyoID& ID(void);								// �Q�b�g�Ղ�ID

	const bool& Alive(void);							// �Q�b�g�����邩�ǂ���
	bool Alive(bool flag);								// �����邩�ǂ����Z�b�g
	
	bool Cnt(double cnt);								// �Ղ��p�J���h�Z�b�g
	bool puyo(void);									// �Ղ�񂷂�

	Permit_Data _pData;
	Permit_Data _pDataOld;
	Permit_Data _drawData;

	virtual const Vector2 GetGrid(int size);			// �Ղ�̃}�X�ڂ��Ƃ�
	int SetPuyoCnt(int no);

	bool puyoFlag_;										
private:
	double _cnt;										// �Ղ��v�Z�p�J���h
	bool _alive;										// �����邩�ǂ����t���O

	int _dropCnt;										// ������J�n�O�̃J���h
	int _dropInt;										// ������J�n�O�̃J���h������
	
	void Init(void);									// ������
	int color_;											// �Ղ�ID�̐F
	Vector2 pos_;										// ���W
	Vector2 _size;										// �T�C�Y

	PuyoID id_;											// �Ղ�ID
	double puyoPos_;									// �Ղ��p�ړ����W
	
	int puyoCnt_;										
};

using sharePuyo = std::shared_ptr<Puyo>;
