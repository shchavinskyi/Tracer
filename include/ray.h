#ifndef RAY_H
#define RAY_H

#include <glm/vec3.hpp>

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;
};

struct HitResult
{
    glm::vec3 position;
    glm::vec3 normal;
    size_t materialId;
};

#endif // RAY_H