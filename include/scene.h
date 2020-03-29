#ifndef _SCENE_
#define _SCENE_

#include <glm/vec3.hpp>
#include <vector>

struct Ray;

struct Sphere
{
    glm::vec3 center;
    float radius;
};

struct Scene
{
    std::vector<Sphere> spheres;
};

#endif // _SCENE_