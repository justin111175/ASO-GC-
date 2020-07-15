// ConsoleApplication2.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include "Slime.h"
#include "Vector2.h"
int main()
{

    //Monster* _slime = new Slime();

    //int num;


    ////  std::cout << "HP:" << _slime->GetHp()<<"です\n" << std::endl;
    //std::cout << "スライムのHP:" << _slime->GetHP() << "です\n" << std::endl;
    //std::cout << "スライムのHPを？？？加算するか"<< std::endl;

    //std::cin >> num;
    //std::cout << "スライムのHPを" <<num<<"加算"<< std::endl;
    //


    ////オーバーライドのAddHP
    ////_slime->AddHP(100);
    ////std::cout << "スライムのHP:" << _slime->GetHP() << "です\n" << std::endl;
    //


    ////オーバーロードのAddHP
    //_slime->AddHP(num);
    //std::cout << "スライムのHP:" << _slime->GetHP() << "です\n" << std::endl;



   /* delete _slime;*/

    Vector2 vec;
    vec = { 100,100 };

    std::cout << "Vector2のXは" << vec.x << "です\n"<< std::endl;
    std::cout << "Vector2のYは" << vec.y << "です\n" << std::endl;
}

