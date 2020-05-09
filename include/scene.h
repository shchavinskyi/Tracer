#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "material.h"
#include "objects.h"
#include "settings.h"

#include <glm/vec3.hpp>
#include <vector>

struct BVHTree;

struct Scene
{
    Settings settings;
    Camera camera;

    std::vector<Sphere> spheresGeometry;
    std::vector<uint32_t> spheresMaterial;

    std::vector<Triangle> trianglesGeometry;
    std::vector<uint32_t> trianglesMaterial;

    std::vector<Material> materials;
};

void AddSphere(Scene& scene, const Sphere& sphere, uint32_t materialId);

void AddSphereAndMaterial(Scene& scene, const Sphere& sphere, const Material& material);

void AddTriangle(Scene& scene, const Triangle& triangle, uint32_t materialId);

void AddTriangleAndMaterial(Scene& scene, const Triangle& triangle, const Material& material);

void AddXYRect(Scene& scene, const glm::vec3& a, const glm::vec3& b, uint32_t materialId, bool flip = false);
void AddXZRect(Scene& scene, const glm::vec3& a, const glm::vec3& b, uint32_t materialId, bool flip = false);
void AddYZRect(Scene& scene, const glm::vec3& a, const glm::vec3& b, uint32_t materialId, bool flip = false);

uint32_t AddMaterial(Scene& scene, const Material& material);

glm::vec3 TracePath(const Ray& ray, uint32_t maxDepth, const Scene& scene);

void RenderScene(const Scene& scene, RenderBuffer renderBuffer);

void RenderSceneMT(const Scene& scene, RenderBuffer& renderBuffer, uint32_t threadCount);

#endif // SCENE_H
