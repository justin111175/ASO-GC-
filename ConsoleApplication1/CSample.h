#pragma once
class CSample
{
public:
	CSample(void);
	~CSample(void);
	void add(int w_pos,int num);

	void set(int w_pos,int num);

	int get(int w_pos);
	//int get2();
	//
	//static int StaticGet2();
	//static int StaticGet2_Test(CSample* p);
	int getCount();


private:
	
	int *n_num;				//’l•Û‚ğ‚·‚é•Ï”

	static int count;



};

