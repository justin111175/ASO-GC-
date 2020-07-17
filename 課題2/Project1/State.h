#pragma once
#include "common/Vector2.h"
#include <memory>
#include "Puyo.h"
#include <vector>
#include "common/Input/Controller.h"


class State
{
public:

	State(Vector2&& _offset, Vector2&& _size);
	~State();
	int GetScreenId(void);
	void Draw(void);
	void Run(void);
	

private:
	
	
	std::map<conType,std::unique_ptr<Controller>> controller;
	
	std::vector<std::unique_ptr<Puyo>> _puyo;

	const int blockSize_;
	const Vector2 gridMax;
	//int mapData[8][14];

	std::vector<PuyoID> _dataBase;
	std::vector<PuyoID*> _data;
	std::vector<PuyoID> _EraserdataBase;
	std::vector<PuyoID*> _Eraserdata;

	bool InstancePuyo(void);

	void SetEraser(void);

	int screenID;
	void Init(void);

	Vector2 _offset;
	Vector2 _size;
	static int _stateCount;
	int _id;
	int _color;
};



