#define DOCTEST_CONFIG_IMPLEMENT
#include "test.h"

int main(int argc, char** argv)
{
    doctest::Context context;

    context.setOption("abort-after", 5);   // stop test execution after 5 failed assertions
    context.setOption("order-by", "name"); // sort the test cases by their name
    context.setOption("no-breaks", 1);     // don't break in the debugger when assertions fail

    context.applyCommandLine(argc, argv);

    return context.run();
}
