#pragma once
#include "Vector2.h"

class MouseCtl
{
public:
	MouseCtl();
	~MouseCtl();

	bool GetClickTrg(void);					// �N���b�N�����u�Ԃ̏ꍇ,true
	bool GetClicking(void);					// �N���b�N���̏ꍇ,true
	const Vector2& GetPos(void);			// �N���b�N�ʒu���擾
	void Update(void);						// �}�E�X�̏�ԍX�V

private:
	Vector2 pos;							// �}�E�X���W
	char mouseData;							// ���݂̃}�E�X�N���b�N���
	char mouseDataOld;						// 1�t���[���O�̃}�E�X�N���b�N���

};

