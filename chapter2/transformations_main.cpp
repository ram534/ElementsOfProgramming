#include"stdafx.h"
#include <iostream>
#include <string>
#include <vector>
#include <set>



template <typename F> 
// Requires:
//      F is a transformation
int distance(typename F::Domain x, typename F::Domain y, F f)
{
    //Precondition: y is reachable from x under f
    int n = 0;
    while (y != x)
    {
        x = f(x);
        n++;
    }

    return n;
}


// First attempt at testing if an orbit has a cycle: but what if we go out of f's domain space?

template <typename F>
typename F::Domain collision_point_nonterminating_orbit(typename  F::Domain x, F f)
{
    typename F::Domain slow = x;
    typename F::Domain fast = f(x);

    while (slow != fast)
    {
        slow = f(slow);
        fast = f(f(fast));
    }
    return fast;
}


// Second attempt:      But what if we go out of f's domain space?
// Solution:            Have a predicate which tells us if f is defined on that value.



template <typename F, typename P>
//Requires:
//      P is a unary predicate
//      F is a transformation
typename F::Domain collision_point(typename  F::Domain x, F f, P p)
{
    // Precondition:
    //    p(x) is true iff x is in the domain space of f
    typename F::Domain slow = x;
    typename F::Domain fast = f(x);

    while (slow != fast)
    {
        slow = f(slow);
        fast = f(fast);
        if (!p(fast))
            return fast;
        fast = f(fast);
        if (!p(fast))
            return fast;
    }
    return fast;
}


template <typename F, typename P>
//Requires:
//      P is a unary predicate
//      F is a transformation
bool is_orbit_terminating(typename F::Domain x, typename F f, typename P p)
{
    // Precondition:
    //    p(x) is true iff x is in the domain space of f
    typename F::Domain point = collision_point(x, f, p);
    return !p(point);
}


template <typename F>
//Requires:
//      F is a transformation
bool circular_nonterminating_orbit(typename F::Domain x, typename F f)
{
    typename F::Domain collision_point = collision_point_nonterminating_orbit(x, f);
    return x == f(collision_point);
}

template <typename F, typename P>
//Requires:
//      P is a unary predicate
//      F is a transformation
bool is_orbit_circular(typename F::Domain x, typename F f, typename P p)
{
    // Precondition:
    //    p(x) is true iff x is in the domain space of f
    typename F::Domain collision_point = collision_point(x, f, p);
    return x == f(collision_point) && p(collision_point);
}


template <typename F>
//Requires:
//      F is a transformation
typename F::Domain convergence_point(typename F::Domain x, typename F::Domain y, typename F f)
{
    //Precondition:
    //      there exists n such that fn(x) = fn(y)
    while (x != y)
    {
        x = f(x);
        y = f(y);
    }
    return x;
}

template <typename F, typename P>
//Requires:
//      P is a unary predicate
//      F is a transformation
typename F::Domain connection_point(typename F::Domain x, typename F f, typename P p)
{
    // Precondition:
    //    p(x) is true iff x is in the domain space of f
    typename F::Domain collision_point = collision_point(x, f, p);
    if (!p(collision_point))
        return collision_point; 
    collision_point = f(collision_point);

    return convergence_point(x, collision_point, f);
}


template <typename F>
//Requires:
//      F is a transformation
typename F::Domain connection_point_nonterminating_orbit(typename F::Domain x, F f)
{
    typename F::Domain collision_point = collision_point_nonterminating_orbit(x, f);

    return convergence_point(x, f(collision_point), f);

}


/* What is a good way of defining and specifying the domain space of a transformation? */
/* Right now I have a predicate defined separately to the domain space definition, which is not good */
template <typename F, typename P>
bool transformation_orbits_intersecting(typename F f, std::vector<typename F::Domain> domain_space, typename P p)
{
    size_t len = domain_space.size();
    for (unsigned int i = 0; i < len - 1; i++)
        for (unsigned int j = i + 1; j < len; j++)
            if (orbit_intersection(f, domain_space[i], domain_space[j], p))
                return true;

    return false;
}

template <typename F, typename P>
bool orbit_intersection(typename F f, typename F::Domain x, typename F::Domain y, typename P p)
{
    typename F::Domain collison_x = collision_point(x, f, p);
    typename F::Domain collison_y = collision_point(y, f, p);

    if (collison_x == collison_y) // they're both terminating orbits
        return true;
    else
    {
        typename F::Domain collison_x_iter = collison_x;
        collison_x_iter = f(collison_x_iter);
        while (collison_x_iter != collison_y)
        {
            collison_x_iter = f(collison_x_iter);
            if (collison_x_iter == collison_x)
                return false;
        }
        return true;
    }
}


struct x_mod_5
{
    using Domain = int;

    Domain operator() (Domain x)
    {
        return ++x % 5;
    }

    static bool x_mod_5_is_in_domain_space(Domain z)
    {
        return (z >= -1 && z <= 4);
    }
};

struct x_plus_plus
{
    using Domain = int;

    Domain operator() (Domain x)
    {
        return ++x;
    }

    static bool x_plus_plus_is_in_range(Domain z)
    {
        return z < 5;
    }
};


struct terminating_transformation
{
    using Domain = int;

    Domain operator() (Domain x)
    {
        if (x % 3 == 1)
            return x*2;
        else if (x % 3 == 2)
            return ++x;
        else return -1;
    }

    static bool terminating_transformation_is_in_domain_space(Domain z)
    {
        return z > 0;
    }
};

int main()
{

    std::cout << distance(1, 0, x_mod_5())<<std::endl;
    std::cout << distance(1, 3, x_mod_5())<< std::endl;


    std::cout << collision_point(-1, x_mod_5(), x_mod_5::x_mod_5_is_in_domain_space) << std::endl;
    std::cout << collision_point(0, x_mod_5(), x_mod_5::x_mod_5_is_in_domain_space) << std::endl;

    std::cout << is_orbit_terminating(0, x_mod_5(), x_mod_5::x_mod_5_is_in_domain_space) << std::endl;

    std::cout << is_orbit_terminating(0, x_plus_plus(), x_plus_plus::x_plus_plus_is_in_range) << std::endl;

    std::cout << circular_nonterminating_orbit(0, x_mod_5())<<std::endl;

    std::cout << connection_point(-1, x_mod_5(), x_mod_5::x_mod_5_is_in_domain_space) << std::endl;
    std::cout << connection_point(0, x_mod_5(), x_mod_5::x_mod_5_is_in_domain_space) << std::endl;

    std::cout << connection_point_nonterminating_orbit(-1, x_mod_5()) << std::endl;
    std::cout << connection_point_nonterminating_orbit(0, x_mod_5()) << std::endl;


    std::cout << transformation_orbits_intersecting(x_mod_5(), std::vector<x_mod_5::Domain>{-1, 0, 1, 2, 3, 4}, x_mod_5::x_mod_5_is_in_domain_space)<<std::endl;   // when do we define the domain space of a transformation? at what point?
    std::cout << transformation_orbits_intersecting(x_mod_5(), std::vector<x_mod_5::Domain>{-1, 0, 1, 2, 3}, x_mod_5::x_mod_5_is_in_domain_space) << std::endl;


    std::cout << transformation_orbits_intersecting(terminating_transformation(), std::vector<terminating_transformation::Domain>{8,9}, terminating_transformation::terminating_transformation_is_in_domain_space) << std::endl;

    return 1;
}