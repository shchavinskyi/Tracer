#ifndef SCENE_H
#define SCENE_H

#include "data.h"
#include "material.h"

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
    glm::vec3 backgroundColor;
};

inline Settings DefaultSettings()
{
    return Settings{{400, 400}, 250, 5, glm::vec3(0.5f, 0.7f, 1.0f)};
}

struct Scene
{
    Settings settings = DefaultSettings();
    Camera camera;

    std::vector<Sphere> spheresGeometry;
    std::vector<uint32_t> spheresMaterial;

    std::vector<Triangle> trianglesGeometry;
    std::vector<uint32_t> trianglesMaterial;

    std::vector<Material> materials;
};

uint32_t AddMaterial(Scene& scene, const Material& material);

void AddSphere(Scene& scene, const Sphere& sphere, uint32_t materialId);
void AddSphereAndMaterial(Scene& scene, const Sphere& sphere, const Material& material);

void AddTriangle(Scene& scene, const Triangle& triangle, uint32_t materialId);
void AddTriangleAndMaterial(Scene& scene, const Triangle& triangle, const Material& material);

void AddXYRect(Scene& scene, const glm::vec3& a, const glm::vec3& b, uint32_t materialId, bool flip = false);
void AddXZRect(Scene& scene, const glm::vec3& a, const glm::vec3& b, uint32_t materialId, bool flip = false);
void AddYZRect(Scene& scene, const glm::vec3& a, const glm::vec3& b, uint32_t materialId, bool flip = false);

#endif // SCENE_H
