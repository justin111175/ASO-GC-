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
	落下,
	直接落下,
	ぷよ,
	むにょん,
	連鎖,
	消す,
	お邪魔ぷよ落下,
	オーバーチェック,
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
	int GetScreenId(void);
	void Draw(void);
	void Run(void);
	
	bool overFlag_;
	bool sceneFlag_;
	bool rennsaFlag_;
	bool winFlag_;
	
	Vector2 _offset;


	const int GetID(void);
private:
	
	PuyoMode puyomode_;
	void playerCtl(void);
	void OverCtl(void);
	bool downCheck(sharePuyo& puyo);
	bool SetEraser(PuyoID id, Vector2 pos);
	
	bool PuyoCheck(Vector2 pos);
	void delPuyo(void);

	void ObjDraw(void);
	Permit_Data _pData;
	Permit_Data _drawData;

	const char* name_;

	std::map<conType,std::unique_ptr<Controller>> controller;

	const int blockSize_;
	const Vector2 gridMax;

	std::vector<PuyoID> _dataBase;
	std::vector<PuyoID*> _data;

	std::vector<PuyoID> _EraserdataBase;
	std::vector<PuyoID*> _Eraserdata;
	
	std::vector<PuyoID> _PuyodataBase;
	std::vector<PuyoID*> _Puyodata;

	bool InstancePuyo(void);
	bool InstanceOjyamapuyo(void);

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



