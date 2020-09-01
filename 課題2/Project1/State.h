#pragma once
#include "common/Vector2.h"
#include <memory>
#include "Puyo.h"
#include <vector>
#include "common/Input/Controller.h"
#include "Ojyama.h"
#include "NextPuyo.h"
#include "common/Number.h"
#include "common/TimeCount.h"

enum class PuyoMode
{
	����,
	���ڗ���,
	�Ղ�,
	�ނɂ��,
	�A��,
	����,
	���ז��Ղ旎��,
	�I�[�o�[�`�F�b�N,
	MAX
};
enum class PlayState
{
	Win,
	Lose,
	MAX
};



using sharedOjyama = std::shared_ptr<Ojyama>;

class State
{
public:

	State(Vector2&& _offset, Vector2&& _size);
	~State();
	int GetScreenId(void);											// state�̉��ID
	void Draw(void);												// �Q�[���`��
	void Run(void);													// �X�V
	
	bool overFlag_;
	bool sceneFlag_;
	bool rennsaFlag_;
	bool winFlag_;
	
	Vector2 _offset;												// ���W

	const int GetID(void);
	void ObjDraw(void);												// ��l�̃Q�[���V�[��

private:
		
	PuyoMode puyomode_;												// �Ղ�̃��[�h		
	void playerCtl(conType input);											// �v���C���[����
	bool downCheck(sharePuyo& puyo);								// �Ղ�̉��̃}�X�`�F�b�N

	bool SetEraser(PuyoID id, Vector2 pos);							// �����Ղ�`�F�b�N
	
	bool PuyoCheck(Vector2 pos);									// �Ղ�񂷂邩�ǂ����`�F�b�N

	void delPuyo(void);												// �Ղ����
	bool InstancePuyo(void);										// �C���X�^���X�Ղ�

	bool InstanceOjyamapuyo(void);									// �C���X�^���X���ז��Ղ�


	Permit_Data _pData;
	Permit_Data _drawData;

	const char* name_;												// �v���C���[ID


	std::map<conType,std::unique_ptr<Controller>> controller[2];


	const int blockSize_;
	const Vector2 gridMax;


	std::vector<PuyoID> _dataBase;
	std::vector<PuyoID*> _data;

	std::vector<PuyoID> _EraserdataBase;
	std::vector<PuyoID*> _Eraserdata;
	
	std::vector<PuyoID> _PuyodataBase;
	std::vector<PuyoID*> _Puyodata;


	int screenID;
	void Init(void);

	Vector2 _size;
	static int _stateCount;
	int _id;
	int _color;
	int _color2;
	int tagetID;
	
	int score_;
	int scoreOld_;
	int rennsaCnt_;
	int dropSpeed_;
	Number number_;
protected:

	std::map<PuyoMode,std::function<void(void)>> puyoMode_;
	std::vector<sharePuyo> _puyo;
	std::unique_ptr<NextPuyo> nextPuyo_;
	std::vector<sharedOjyama> _ojyama;


};



