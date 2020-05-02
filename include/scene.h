#ifndef SCENE_H
#define SCENE_H

#include "material.h"
#include "objects.h"

#include <glm/vec3.hpp>
#include <vector>

struct BVHTree;

struct Scene
{
    std::vector<Sphere> spheresGeometry;
    std::vector<Material> spheresMaterial;
};

void AddSphere(Scene& scene, Sphere&& sphere, Material&& material);

glm::vec3 TracePath(const Ray& ray, int maxDepth, const Scene& scene);

glm::vec3 TracePathWithBVH(const Ray& ray, int maxDepth, const Scene& scene, const BVHTree& tree);

#endif // SCENE_H