#ifndef OBJECTS_H
#define OBJECTS_H

#include <glm/vec3.hpp>

struct Sphere
{
    glm::vec3 center;
    float radius;
};

struct Triangle
{
    glm::vec3 v1;
    glm::vec3 v2;
    glm::vec3 v3;
};

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;
};

struct HitResult
{
    glm::vec3 position;
    glm::vec3 normal;
    uint32_t materialId;
};

struct Color
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
};

struct RenderBuffer
{
    Color* buffer;
    uint32_t start;
    uint32_t length;
};

#endif // OBJECTS_H
