
/* integral types procedures */

template <typename T>
T f(T i)
{
    static_assert(std::is_integral<T>::value, "Integral required.");
    return i;
}

template<typename T>
T successor(T x)
{
    return x + 1;

}

template<typename T>
T predecessor(T x)
{
    return x - 1;

}

template<typename T>
T twice(T x)
{
    return x * 2;

}

template<typename T>
T half_nonnegative(T x)
{
    //precondition: x >0
    return x / 2;
}

template<typename T>
T binary_scale_down_nonnegative(T n, T k)
{
    return n / std::pow(2, k);
}

template<typename T>
T binary_scale_up_nonnegative(T n, T k)
{
    return n * std::pow(2, k);
}

template<typename T>
T positive(T x)
{
    return x > 0;
}

template<typename T>
T negative(T x)
{
    return x < 0;
}

template<typename T>
T zero(T x)
{
    return x == 0;
}

template<typename T>
T one(T x)
{
    return x == 1;
}

template<typename T>
T odd(T x)
{
    return (x % 2 == 1);
}

template<typename T>
T even(T x)
{
    return !odd(x);
}
#pragma once
