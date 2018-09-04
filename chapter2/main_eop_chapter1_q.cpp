#include"stdafx.h"
#include <iostream>
#include <string>


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|   Notes:
| 1.    What problem does this solve? Why add a wrapper to just calling a function with some params
|
|
| 2.    std::cout << multiplication(6, 7, multiply_op<int>())<<std::endl;
|                                                          ^
|                                                          these brackets mean "a new object off type multiply_op<int>", and not calling operator()
|
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/



/*-------------------------------------------------------------------------------------------------------------------------------
|
|   check multiple_op() can take parameters of a type that manually implements operator* 
|
-------------------------------------------------------------------------------------------------------------------------------*/


class conc_str {
public:
    std::string x;

    conc_str(std::string y)
    {
        x = y;
    }

    conc_str operator* (const conc_str& y) const
    {
        return x + ' ' + y.x;
    }

    friend std::ostream& operator<<(std::ostream& os, const conc_str& str)
    {
        return os << str.x;
    }
};


/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|
|   Type aliasing : how does it work between diff objects? By the def of square, does it mean it can only be applied to objects who have a ::Domanin member? Is that a precondition?
|   Is ::Domain a member of the object now? Does the type name need to be abstarcted (to Domain)?
|
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/



template<typename T>
struct multiply_op2 {

    T operator() (const T & x, const T & y) {
        return x * y;
    }
};



/*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
|
|   implement multiplication : parameters of the same type to preserve regularity of op. Well, probably wouldn't work otherwise.
|   
-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/




template<typename Op>
// require BinaryOperation(op)
typename Op::Domain multiplication(const typename Op::Domain & x, const typename Op::Domain & y, Op op) {
    return op(x, y);
}



template<typename Op>
// require BinaryOperation(op)
typename Op::Domain square(const typename Op::Domain & x, Op op) {
    return op(x, x);
}

template<typename T>
struct multiply_op {
    using Domain = T;

    Domain operator() (const Domain & x, const Domain & y) {
        return x * y;
    }
};

struct concatenate_strings {
    using Domain = std::string;

    Domain operator() (const Domain & x, const Domain & y) {
        return x + ' ' + y;
    }
};

int main()
{
    /*-------------------------------------------------------------------------------------------------------------------------------
    |
    |   check multiple_op() can take parameters of a type that manually implements operator*
    |
    -------------------------------------------------------------------------------------------------------------------------------*/
    conc_str c{ "g" };
    std::cout << square(c, multiply_op<conc_str>()) << "\n";

    /*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    |
    |   Type aliasing : how does it work between diff objects? By the def of square, does it mean it can only be applied to objects who have a ::Domanin member? Is that a concept?
    |   Is ::Domain a member of the object now?
    |
    -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    //std::cout << square(5, multiply_op2<int>()) << "\n"; type aliasing

    /*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    |
    |   implement multiplication : parameters of the same type to preserve regularity of op. Well, probably wouldn't work otherwise.
    |
    -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/

    std::cout << multiplication(6, 7, multiply_op<int>())<<std::endl;

    std::cout << multiplication("a", "b", concatenate_strings()) << std::endl;


    /*---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    |
    |   Work with Mircea
    |   
    |
    -----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------*/
    std::cout << square(5, multiply_op<int>()) << "\n";

    std::string a{ "a" };
    std::cout << square(a, concatenate_strings()) << "\n";

    return 0;
}

