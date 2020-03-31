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
    const struct Material* material;
};

#endif // RAY_H