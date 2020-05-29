#include "random.h"

#include <chrono>
#include <glm/gtc/constants.hpp>

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

float RandomFloatGenerator::Generate()
{
    return distribution(generator);
}

RandomVectorGenerator::RandomVectorGenerator(float min, float max)
    : floatGenerator(min, max)
{
}

glm::vec3 RandomVectorGenerator::Generate()
{
    return glm::vec3(floatGenerator.Generate(), floatGenerator.Generate(), floatGenerator.Generate());
}

RandomUnitVectorGenerator::RandomUnitVectorGenerator()
    : aGenerator(0.0f, 2.0f * glm::pi<float>())
    , zGenerator(-1.0f, 1.0f)
{
}

glm::vec3 RandomUnitVectorGenerator::Generate()
{
    float a = aGenerator.Generate();
    float z = zGenerator.Generate();
    float r = sqrt(1.0f - z * z);
    return glm::vec3(r * cos(a), r * sin(a), z);
}

glm::vec3 RandomInUnitSphereGenerator::Generate()
{
    while (true)
    {
        glm::vec3 p = vectorGenerator.Generate();
        if (glm::length(p) >= 1.0f)
        {
            continue;
        }
        return p;
    }
}
