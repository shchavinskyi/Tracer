#ifndef SCENE_H
#define SCENE_H

#include "material.h"

#include <glm/vec3.hpp>
#include <vector>

struct Sphere
{
    glm::vec3 center;
    float radius;
};

struct SphereObject
{
    Sphere geometry;
    Material material;
};

struct Scene
{
    std::vector<SphereObject> spheres;
};

#endif // SCENE_H