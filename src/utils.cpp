#include "utils.h"

#include <chrono>

#define _USE_MATH_DEFINES
#include <math.h>

using std::chrono::system_clock;

RandomFloatGenerator::RandomFloatGenerator()
    : min(0.0f)
    , max(1.0f)
    , generator(static_cast<uint32_t>(system_clock::now().time_since_epoch().count()))
    , distribution(min, max)
{
}

RandomFloatGenerator::RandomFloatGenerator(float inMin, float inMax)
    : min(inMin)
    , max(inMax)
    , generator(static_cast<uint32_t>(system_clock::now().time_since_epoch().count()))
    , distribution(min, max)
{
}

RandomVectorGenerator::RandomVectorGenerator() {}

RandomVectorGenerator::RandomVectorGenerator(float min, float max)
    : floatGenerator(min, max)
{
}

RandomUnitVectorGenerator::RandomUnitVectorGenerator()
    : aGenerator(0.0f, 2.0f * float(M_PI))
    , zGenerator(-1.0f, 1.0f)
{
}

RandomInUnitSphereGenerator::RandomInUnitSphereGenerator()
    : vectorGenerator(-1.0f, 1.0f)
{
}