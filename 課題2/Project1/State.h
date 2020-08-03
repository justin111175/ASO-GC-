#pragma once
#include "common/Vector2.h"
#include <memory>
#include "Puyo.h"
#include <vector>
#include "common/Input/Controller.h"
#include "drop.h"
enum class PuyoMode
{
	落下,
	ぷよ,
	むにょん,
	連鎖,
	消す,
	オーバーチェック,
	MAX
};

using sharedPuyo = std::shared_ptr<Puyo>;


class State
{
public:

	State(Vector2&& _offset, Vector2&& _size);
	~State();
	int GetScreenId(void);
	void Draw(void);
	void Run(void);
	
	bool OverFlag;



private:
	friend Drop;
	PuyoMode puyomode_;
	
	void playerCtl(void);
	bool downCheck(sharedPuyo& puyo);
	bool SetEraser(PuyoID id, Vector2 pos);
	void delPuyo(void);
	Permit_Data _pData;
	Permit_Data _drawData;


	std::map<conType,std::unique_ptr<Controller>> controller;
	
	
	std::map<PuyoMode,std::function<void(void)>> puyoMode_;
	
	std::vector<sharedPuyo> _puyo;

	const int blockSize_;
	const Vector2 gridMax;

	std::vector<PuyoID> _dataBase;
	std::vector<PuyoID*> _data;

	std::vector<PuyoID> _EraserdataBase;
	std::vector<PuyoID*> _Eraserdata;

	bool InstancePuyo(void);


	int screenID;
	void Init(void);

	Vector2 _offset;
	Vector2 _size;
	static int _stateCount;
	int _id;
	int _color;
	int tagetID;
};



