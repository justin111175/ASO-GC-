// ConsoleApplication1.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//


//  I/O ：Input/Output
#include <iostream>
#include "CSample.h"


//基底


int main()
{

	//名前空間　namespace
	CSample *obj1= new CSample();
	CSample *obj2= new CSample();

	// cout/cin:コンソールのInput/Outputの略称
	int num;

	//std::cout << "建立物件a1" << std::endl;
	std::cout << "1つ目の整数を入力してください：" << std::endl;
	std::cin >> num;
	obj1->add(0,num);
	
	std::cout << "2つ目の整数を入力してください：" << std::endl;
	std::cin >> num;
	obj2->add(0, num);

	std::cout << "入力した数値は" << obj1->get(0) << "です\n" << std::endl;
	std::cout << "メンバー関数の呼び出し回数は" << obj1->getCount() << "です\n" << std::endl;



	
	//std::cout << "[入力した数値1]+[入力した数値2]は"<< CSample::StaticGet2_Test(&a1) + CSample::StaticGet2_Test(&a2) <<"です\n" << std::endl;


	//delete obj1;

	//obj1 = nullptr;
	return 0;

}

