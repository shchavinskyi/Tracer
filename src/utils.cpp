#include "utils.h"

#include <random>

float randomUnitFloat() { return randomFloatInRange(0.0f, 1.0f); }

float randomFloatInRange(float min, float max)
{
    std::random_device device;

    std::mt19937 generator(device());

    std::uniform_real_distribution<float> distribution(min, max);

    return distribution(generator);
}
