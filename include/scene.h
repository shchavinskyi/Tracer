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
    std::vector<size_t> spheresMaterial;

    std::vector<Triangle> trianglesGeometry;
    std::vector<size_t> trianglesMaterial;

    std::vector<Material> materials;
};

void AddSphere(Scene& scene, const Sphere& sphere, size_t materialId);

void AddSphereAndMaterial(Scene& scene, const Sphere& sphere, const Material& material);

void AddTriangle(Scene& scene, const Triangle& triangle, size_t materialId);

void AddTriangleAndMaterial(Scene& scene, const Triangle& triangle, const Material& material);

size_t AddMaterial(Scene& scene, const Material& material);

glm::vec3 TracePath(const Ray& ray, int maxDepth, const Scene& scene, const BVHTree& tree);

#endif // SCENE_H