#include"stdafx.h"
#include <iostream>
#include <string>
#include "IntegralTypesProcedures.h"


struct binary_op_concat
{
    using Domain = std::string;

    Domain operator() (const Domain & x, const Domain & y)
    {
        return x + y;
    }
};

struct binary_op_mult
{
    using Domain = long int;

    Domain operator() (const Domain & x, const Domain & y)
    {
        return x * y;
    }
};

struct binary_op_sub
{
    using Domain = long int;

    Domain operator() (const Domain & x, const Domain & y)
    {
        return x - y;
    }
};


template <typename Op>
typename Op::Domain binary(const typename Op::Domain & x, const typename Op::Domain & y, Op op)
{
    return op(x, y);
}

template <typename Op, typename I>
typename Op::Domain power_left_associated(const typename Op::Domain & a, I n, Op op)
{
    if (one(n))
        return a;
    return op(power_left_associated(a, predecessor(n), op), a);
}

template <typename Op, typename I>
typename Op::Domain power_right_associated(const typename Op::Domain & a, I n, Op op)
{
    if (one(n))
        return a;
    return op(a, power_left_associated(a, predecessor(n), op));
}

template <typename Op, typename I>
typename Op::Domain power_0(const typename Op::Domain & a, I n, Op op)
{
    if (one(n))
        return a;
    if (even(n))
        return power_0(op(a, a), half_nonnegative(n), op);

    return op(a, power_0(op(a, a), half_nonnegative(n), op));
}

template <typename Op, typename I>
typename Op::Domain power_1(const typename Op::Domain & a, I n, Op op)
{
    if (one(n))
        return a;

    auto temp = power_0(op(a, a), half_nonnegative(n), op);

    if (odd(n))
        temp = op(temp, a);

    return temp;
}

template <typename Op, typename I>
typename Op::Domain power_accumulate_0(typename Op::Domain r, const typename Op::Domain & a, I n, Op op) //tail-recursive
{
    if (n == 0)
        return r;

    if (odd(n))
        r = op(r, a);

    return power_accumulate_0(r, op(a,a), half_nonnegative(n), op);
}

template <typename Op, typename I>
typename Op::Domain power_accumulate_1(typename Op::Domain r, const typename Op::Domain & a, I n, Op op) 
{
    if (zero(n))
        return r;
    if (one(n))
        return op(r, a);

    if (odd(n))
        r = op(r, a);

    return power_accumulate_1(r, op(a, a), half_nonnegative(n), op);
}

template <typename Op, typename I>
typename Op::Domain power_accumulate_2(typename Op::Domain r, const typename Op::Domain & a, I n, Op op) //reordered
{
    if (odd(n))
    {
        r = op(r, a);
        if (one(n))
            return r;
    }
    else if (zero(n))
        return r;

    return power_accumulate_2(r, op(a, a), half_nonnegative(n), op);
}

template <typename Op, typename I>
typename Op::Domain power_accumulate_3(typename Op::Domain r, typename Op::Domain a, I n, Op op) //strict tail-recursive
{
    if (odd(n))
    {
        r = op(r, a);
        if (one(n))
            return r;
    }
    else if (zero(n))
        return r;

    a = op(a, a);
    n = half_nonnegative(n);

    return power_accumulate_3(r, a, n, op);
}

template <typename Op, typename I>
typename Op::Domain power_accumulate_4(typename Op::Domain r, typename Op::Domain a, I n, Op op) //iterative
{
    while (true)
    {
        if (odd(n))
        {
            r = op(r, a);
            if (one(n))
                return r;
        }
        else if (zero(n))
            return r;

        a = op(a, a);
        n = half_nonnegative(n);
    }
}


template <typename Op, typename I>
typename Op::Domain power_accumulate_positive_0(typename Op::Domain r, typename Op::Domain a, I n, Op op) //iterative; stronger precondition
{
    //precondition: n>0
    while (true)
    {
        if (odd(n))
        {
            r = op(r, a);
            if (one(n))
                return r;
        }

        a = op(a, a);
        n = half_nonnegative(n);
    }
}

template <typename Op, typename I>
typename Op::Domain power_accumulate_5(typename Op::Domain r, typename Op::Domain a, I n, Op op) //iterative
{
    if (zero(n))
        return r;
    return power_accumulate_positive_0(r, a, n, op);
}

template <typename Op, typename I>
typename Op::Domain power_2(typename Op::Domain a, I n, Op op) //iterative
{
    return power_accumulate_5(a, a, predecessor(n), op);
}

template <typename Op, typename I>
typename Op::Domain power_3(typename Op::Domain a, I n, Op op) //iterative
{
    while (even(n))
    {
        a = op(a, a);
        n = half_nonnegative(n);
    }
    n = half_nonnegative(n);
    if (zero(n))
        return a;
    return power_accumulate_positive_0(a, op(a, a), n, op);
}

template <typename I>
std::pair<I, I> fibonacci_matrix_multiply(std::pair<I, I> x, std::pair<I, I> y)
{
    return std::pair<int,int>(x.first * (y.first + y.second) + x.second * y.first, x.first * y.first + x.second * y.second);
}

template <typename I>
I fibonacci(I n)
{
    if (zero(n))
        return 0;

    return (power_3(std::pair<I, I>(0, 1), n, fibonacci_matrix_multiply)).first;
}



void main()
{/*
    std::string a = "a";
    std::string b = "b";

    std::cout<<binary(a, b, binary_op())<<std::endl;*/

    std::string z = "z";
    std::cout << power_left_associated(z, 3, binary_op_concat())<<std::endl;
    std::cout << power_right_associated(z, 3, binary_op_concat()) << std::endl;

    long int number = 2;
    std::cout << power_left_associated(number, 3, binary_op_mult()) << std::endl;
    std::cout << power_right_associated(number, 3, binary_op_mult()) << std::endl;

    /* expecting diff results, as subtraction is not associative */
    std::cout << power_left_associated(number, 3, binary_op_sub()) << std::endl;
    std::cout << power_right_associated(number, 3, binary_op_sub()) << std::endl;

    std::cout << power_0(z, 19, binary_op_concat()) << std::endl;
    std::cout << power_1(z, 19, binary_op_concat()) << std::endl;

    std::string r;
    std::cout << power_accumulate_0(r, z, 19, binary_op_concat()) << std::endl;
    std::cout << power_accumulate_1(r, z, 19, binary_op_concat()) << std::endl;
    std::cout << power_accumulate_2(r, z, 19, binary_op_concat()) << std::endl;
    std::cout << power_accumulate_3(r, z, 19, binary_op_concat()) << std::endl;
    std::cout << power_accumulate_4(r, z, 19, binary_op_concat()) << std::endl;
    std::cout << power_accumulate_positive_0(r, z, 19, binary_op_concat()) << std::endl;
    std::cout << power_accumulate_5(r, z, 19, binary_op_concat()) << std::endl;

    std::cout << power_2(z, 19, binary_op_concat()) << std::endl;
    std::cout << power_3(z, 19, binary_op_concat()) << std::endl;

    std::cout << f(true)<<std::endl;
 //   std::cout << f("str");

    std::pair <int, int> x = std::pair<int, int>(3, 2);
    std::pair <int, int> y = std::pair<int, int>(2, 1);

    std::pair <int, int> t = fibonacci_matrix_multiply(x, y);
    std::cout << t.first << t.second << std::endl;

    fibonacci(3);

}