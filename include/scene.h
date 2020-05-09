#ifndef SCENE_H
#define SCENE_H

#include "camera.h"
#include "material.h"
#include "objects.h"

#include <glm/vec3.hpp>
#include <vector>

struct ImageSize
{
    uint32_t width;
    uint32_t height;
};

struct Settings
{
    ImageSize imageSize;
    uint32_t samplesPerPixel;
    uint32_t maxBounces;
};

inline Settings DefaultSettings() { return Settings{{400, 400}, 250, 10}; }

struct Scene
{
    Settings settings;
    Camera camera;

    glm::vec3 backgroundColor;

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

#endif // SCENE_H
