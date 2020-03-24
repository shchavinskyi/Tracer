#include <iostream>

#include "hello.h"

int main(int /*argc*/, char** /*argv*/)
{

	std::cout << "Hello\n";

	const int n = 6;
	std::cout << n << ' ' << number(n) << '\n';

	return 0;
}