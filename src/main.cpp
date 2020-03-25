#include <iostream>

#include <glm/vec3.hpp> // glm::vec3

#include "hello.h"

int main(int /*argc*/, char** /*argv*/)
{

	glm::vec3 down(-1.0f, 0.0f, 0.0f);

	std::cout << "Hello\n" << down.x;

	const int n = 6;
	std::cout << n << ' ' << number(n) << '\n';

	return 0;
}