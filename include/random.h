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

    float Generate() { return distribution(generator); }

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

    glm::vec3 Generate()
    {
        return glm::vec3(floatGenerator.Generate(), floatGenerator.Generate(), floatGenerator.Generate());
    }

private:
    RandomFloatGenerator floatGenerator;
};

class RandomUnitVectorGenerator
{
public:
    RandomUnitVectorGenerator();

    glm::vec3 Generate()
    {
        float a = aGenerator.Generate();
        float z = zGenerator.Generate();
        float r = sqrt(1.0f - z * z);
        return glm::vec3(r * cos(a), r * sin(a), z);
    }

private:
    RandomFloatGenerator aGenerator;
    RandomFloatGenerator zGenerator;
};

class RandomInUnitSphereGenerator
{
public:
    RandomInUnitSphereGenerator();

    glm::vec3 Generate()
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

private:
    RandomVectorGenerator vectorGenerator;
};

class RandomUnitVectorInHemisphereGenerator
{
public:
    RandomUnitVectorInHemisphereGenerator() = default;

    glm::vec3 GenerateFor(const glm::vec3& normal)
    {
        glm::vec3 inUnitSphere = generator.Generate();
        if (glm::dot(inUnitSphere, normal) > 0.0f)
        {
            return inUnitSphere;
        }
        else
        {
            return -inUnitSphere;
        }
    }

private:
    RandomInUnitSphereGenerator generator;
};

#endif // RANDOM_H_
