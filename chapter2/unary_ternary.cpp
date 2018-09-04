#include"stdafx.h"
#include <iostream>
#include <string>



/* unary regular function*/
template <typename Op>
typename Op::Domain f(const typename Op::Domain & x, Op op)
{
    return op(x);
}

struct unary                                              // weirdly, swapping struct for class doesn't work. why not?
{
    using Domain = std::string;

    Domain operator() (const Domain & x) 
    {
        return x + "x";
    }
};


class Point
{
private:
    int x;
    int y;

public:
    Point(int a, int b)
    {
        x = a;
        y = b;
    }

    Point() : x(0), y(0)
    {

    }


    Point operator+(const Point & a) const
    {
        return Point(x + a.x, y + a.y);
    }

    friend std::ostream& operator<<(std::ostream& os, Point p)
    {
        return os << "( " << p.x << " , " << p.y << " )";
    }
};



template<typename Op>
typename Op::Domain operation_on_three_objects(const typename Op::Domain &  x, const typename Op::Domain & y, Op op)
{
    return op(x, y, Op::Domain());
}

template<typename T>
struct ternary
{
    using Domain = T;
    Domain operator() (const Domain& x, const Domain& y, const Domain& z)
    {
        return x + y + z;
    }
};

struct ternary_two
{
    using Domain = std::string;
    Domain operator()(const Domain& x, const Domain& y, const Domain& z)
    {
        return x + " ; " + y + " ; " + z + " ; ";
    }
};



void main()
{
    std::cout << f("a", unary()) << std::endl;

    unary u;
    std::cout << f("a", u) << std::endl;

    std::cout << operation_on_three_objects( 1, 2, ternary<int>())<<std::endl;
    std::cout << operation_on_three_objects("a", "b", ternary_two())<<std::endl;

    std::cout << operation_on_three_objects(Point(1,2), Point(3,4), ternary<Point>()) << std::endl;

}