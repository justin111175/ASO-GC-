#pragma once
#include "common/Vector2.h"
#include <memory>
#include "Puyo.h"
#include <vector>
#include "common/Input/Controller.h"

enum class PuyoMode
{
	óéâ∫,
	òAçΩ,
	òAçΩóéâ∫,
	Ç’ÇÊ,
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
	

private:
	PuyoMode puyomode_;
	
	void playerCtl(void);
	bool downCheck(sharedPuyo& puyo);
	bool SetEraser(PuyoID id, Vector2 pos);
	void delPuyo(void);
	Permit_Data _pData;


	std::map<conType,std::unique_ptr<Controller>> controller;
	
	
	std::map<PuyoMode,std::function<void(void)>> test;
	
	std::vector<sharedPuyo> _puyo;

	const int blockSize_;
	const Vector2 gridMax;
	//int mapData[8][14];

	std::vector<PuyoID> _dataBase;
	std::vector<PuyoID*> _data;

	std::vector<PuyoID> _EraserdataBase;
	std::vector<PuyoID*> _Eraserdata;

	bool InstancePuyo(void);

	int _cnt;

	int screenID;
	void Init(void);

	Vector2 _offset;
	Vector2 _size;
	static int _stateCount;
	int _id;
	int _color;
};



