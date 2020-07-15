#include "CSample.h"
#include <iostream>

int CSample::count = 0;



void CSample::add(int w_pos,int num)
{
    count++;
    n_num[w_pos] += num;

}

void CSample::set(int w_pos,int num)
{
    count++;

    n_num[w_pos] = num;
    

}

int CSample::get(int w_pos)
{
    count++;

    return n_num[w_pos];
}

//int CSample::get2()
//{
//    return count;
//}

//int CSample::StaticGet2_Test(CSample* p)
//{
//    return p->get2();
//}

int CSample::getCount()
{
    return count;
}

//int CSample::StaticGet2()
//{
//    return n_num->get2();
//}

CSample::CSample(void)
{
    std::cout << "ƒƒ‚ƒŠ[‚É’Ç‰Á" << std::endl;
    
    n_num = new int[2];
    for (int i = 0; i < 2; i++)
    {
        n_num[i] = 0;

    }

}

CSample::~CSample(void)
{
    std::cout << "ƒƒ‚ƒŠ[‚©‚çíœ" << std::endl;
    delete[] n_num;

}


