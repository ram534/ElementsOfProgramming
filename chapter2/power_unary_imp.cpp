#include"stdafx.h"
#include <iostream>
#include <string>


/***************************************************************
|
|   Transformations
|
|
*****************************************************************/
template<typename T>
typename T::Domain power_unary(typename T::Domain x, int n, T t)
{
    while (n > 0)
    {
        x = t(x);
        n--;
    }

    return x;
}


struct plusplus
{
    using Domain = int;

    Domain operator() (Domain x)
    {
        return ++x;
    }
};

struct modrand
{
    using Domain = int;
    Domain operator() (Domain x)
    {
        return x % rand();
    }
};

/*
A failed attempt: function is not regular: return type is not the same as input type (int* vs int), so the fct can only be applied once in theory, none in reality 
    (because the return type doesn't match the type of the return value).
                template<typename T>
                struct deref
                {
                    using Domain = T*;

                    Domain operator() (Domain x)
                    {
                        return *x;
                    }
                };
*/
int main()
{
    int* y, x=19;
    y = &x;

    std::cout << power_unary(1, 4, plusplus())<<std::endl;
    std::cout << power_unary(55, 4, modrand()) << std::endl;
    //std::cout << power_unary(y, 1, deref<int>()) << std::endl;

    return 1;
}