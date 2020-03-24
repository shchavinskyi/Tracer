//#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "hello.h"

TEST_CASE("testing sapmple function") {
    CHECK(number(1) == 1);
    CHECK(number(2) == 2);
    CHECK(number(3) == 3);
    CHECK(number(10) == 10);
}