#pragma once
#include "common/Vector2.h"
#include <map>
#include <functional>
#include <memory>
#include "common/Input/InputID.h"


enum class PuyoID
{
	NON,
	赤,
	緑,
	青,
	黄,
	紫,
	お邪魔ぷよ,
	壁,
	Max
};

enum class PuyonState		// ぷよ状態
{
	stay,					// 止まる
	puyo					// ぷよん
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
	void Draw(Vector2 offset);							// ぷよ描画表示
	bool Run(int no);									// 更新（落ちる速度）
	void Move(InputID id);								// ぷよ移動操作
	bool SetPData(DirBit dirbit);						// 移動制限
	
	bool SetDrawData(DirBit& dirbit);					// むにょん描画制御

	const Vector2& Pos(void);							// ゲットPos
	bool Pos(Vector2 pos);								// Posセット

	const PuyoID& ID(void);								// ゲットぷよID

	const bool& Alive(void);							// ゲット生きるかどうか
	bool Alive(bool flag);								// 生きるかどうかセット
	
	bool Cnt(double cnt);								// ぷよん用カンドセット
	bool puyo(void);									// ぷよんする

	Permit_Data _pData;
	Permit_Data _pDataOld;
	Permit_Data _drawData;

	virtual const Vector2 GetGrid(int size);			// ぷよのマス目をとる
	int SetPuyoCnt(int no);

	bool puyoFlag_;										
private:
	double _cnt;										// ぷよん計算用カンド
	bool _alive;										// 生きるかどうかフラグ

	int _dropCnt;										// 落ちる開始前のカンド
	int _dropInt;										// 落ちる開始前のカンド初期化
	
	void Init(void);									// 初期化
	int color_;											// ぷよIDの色
	Vector2 pos_;										// 座標
	Vector2 _size;										// サイズ

	PuyoID id_;											// ぷよID
	double puyoPos_;									// ぷよん用移動座標
	
	int puyoCnt_;										
};

using sharePuyo = std::shared_ptr<Puyo>;
