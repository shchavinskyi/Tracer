#ifndef _SCENE_
#define _SCENE_

#include <vector>
#include <glm/vec3.hpp>

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