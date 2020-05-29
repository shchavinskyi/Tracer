#ifndef RANDOM_H_
#define RANDOM_H_

#include <glm/geometric.hpp>
#include <glm/vec3.hpp>
#include <random>

class RandomFloatGenerator
{
public:
    RandomFloatGenerator();
    RandomFloatGenerator(float min, float max);
    float Generate();

private:
    float min;
    float max;
    std::mt19937 generator;
    std::uniform_real_distribution<float> distribution;
};

class RandomVectorGenerator
{
public:
    RandomVectorGenerator() = default;
    RandomVectorGenerator(float min, float max);
    glm::vec3 Generate();

private:
    RandomFloatGenerator floatGenerator;
};

class RandomUnitVectorGenerator
{
    RandomFloatGenerator aGenerator;
    RandomFloatGenerator zGenerator;

public:
    RandomUnitVectorGenerator();
    glm::vec3 Generate();
};

class RandomInUnitSphereGenerator
{
    RandomVectorGenerator vectorGenerator;

public:
    glm::vec3 Generate();
};

class RandomUnitVectorInHemisphereGenerator
{
    RandomInUnitSphereGenerator generator;

public:
    glm::vec3 GenerateFor(const glm::vec3& normal)
    {
        glm::vec3 inUnitSphere = generator.Generate();
        if (glm::dot(inUnitSphere, normal) > 0.0f)
        {
            return inUnitSphere;
        }

        return -inUnitSphere;
    }
};

#endif // RANDOM_H_
