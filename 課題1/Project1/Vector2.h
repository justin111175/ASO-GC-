#pragma once
class Vector2
{
public:
	Vector2();
	Vector2(int x,int y);
	~Vector2();

	int x;
	int y;

	//代入演算子
	Vector2& operator =(const Vector2& vec);
	
	//添え字演算子
	int& operator[](int i);

	// 比較演算子
	bool operator == (const Vector2& vec)const;
	bool operator != (const Vector2& vec)const;
	bool operator > (const Vector2& vec)const;
	bool operator >= (const Vector2& vec)const;
	bool operator <= (const Vector2& vec)const;


	//単項演算子
	Vector2& operator +=(const Vector2& vec);
	Vector2& operator -=(const Vector2& vec);
	Vector2& operator *=(int k);
	Vector2& operator /=(int k);

	Vector2 operator+()const;
	Vector2 operator-()const;


};
//Vector2＝関数の帰り値
//Vector2 operator+(const Vector2& u, int v);

// Vector2　+　int 
Vector2 operator+(const Vector2& u, int v);
// Vector2　-　int 
Vector2 operator-(const Vector2& u, int v);
// Vector2　*　int
Vector2 operator*(const Vector2& u, int v);
// Vector2　/　int
Vector2 operator/(const Vector2& u, int v);
// Vector2　%　int
Vector2 operator%(const Vector2& u, int v);

// int * Vector2　
Vector2 operator*(int v, const Vector2& u);

// Vector2　+　Vector2
Vector2 operator+(const Vector2& u, const Vector2 v);
// Vector2　-　Vector2
Vector2 operator-(const Vector2& u, const Vector2 v);
// Vector2　*　Vector2
Vector2 operator*(const Vector2& u, const Vector2 v);
// Vector2　/　Vector2
Vector2 operator/(const Vector2& u, const Vector2 v);
// Vector2　%　Vector2
Vector2 operator%(const Vector2& u, const Vector2 v);

