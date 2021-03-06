#include <iostream>
#include <string>




class conc_str{
    std::string operator* (const std::string& x, const std::string& y)
    {
        return x + ' ' + y;
    }
}

    


template<typename T>
struct multiply_op {
	using Domain = T;

	Domain operator() (const Domain & x, const Domain & y) {
		return x * y;
	}
};

template<typename Op>
// require BinaryOperation(op)
typename Op::Domain square(const typename Op::Domain & x, Op op) {
	return op(x, x);
}

struct concatenate_strings {
	using Domain = std::string;

	Domain operator() (const Domain & x, const Domain & y) {
		return x + ' ' + y;
	}
};

int main()
{
	std::cout << square(5, multiply_op<int>()) << "\n";
    
    std::cout << square("x", multiply_op<conc_str>()) << "\n";
    
    std::string a{ "a" };
	std::cout << square(a, concatenate_strings()) << "\n";

    return 0;
}

