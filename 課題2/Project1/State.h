#pragma once
#include "common/Vector2.h"
#include <memory>
#include "Puyo.h"
#include <vector>
#include "common/Input/Controller.h"

#define CHIP_SIZE 48
#define CHIP_MAX_X 6
#define CHIP_MAX_Y 13

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
	
	std::unique_ptr<Puyo> _puyo;



	//int mapData[8][14];

	std::vector<int> dataBase;
	std::vector<int*> data;



	void CtlInit(void);
	Vector2 _pos;


	int screenID;
	void Init(void);

	Vector2 _offset;
	Vector2 _size;
	static int _stateCount;
	int _id;
	int _color;
};



