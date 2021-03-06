#include <iostream>
#include <string>

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

	std::string a{ "a" };
	std::cout << square(a, concatenate_strings()) << "\n";

    return 0;
}

