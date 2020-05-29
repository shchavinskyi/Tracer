#include "scene.h"

uint32_t AddMaterial(Scene& scene, const Material& material)
{
    scene.materials.push_back(material);
    return uint32_t(scene.materials.size() - 1);
}

void AddSphereAndMaterial(Scene& scene, const Sphere& sphere, const Material& material)
{
    scene.spheresGeometry.push_back(sphere);
    scene.materials.push_back(material);
    scene.spheresMaterial.push_back(uint32_t(scene.materials.size() - 1));
}

void AddTriangleAndMaterial(Scene& scene, const Triangle& triangle, const Material& material)
{
    scene.trianglesGeometry.push_back(triangle);
    scene.materials.push_back(material);
    scene.trianglesMaterial.push_back(uint32_t(scene.materials.size() - 1));
}

void AddSphere(Scene& scene, const Sphere& sphere, uint32_t materialId)
{
    scene.spheresGeometry.push_back(sphere);
    scene.spheresMaterial.push_back(materialId);
}

void AddTriangle(Scene& scene, const Triangle& triangle, uint32_t materialId)
{
    scene.trianglesGeometry.push_back(triangle);
    scene.trianglesMaterial.push_back(materialId);
}

void AddXYRect(Scene& scene, const glm::vec3& a, const glm::vec3& b, uint32_t materialId, bool flip /* = false*/)
{
    scene.trianglesGeometry.emplace_back(flip ? Triangle{a, glm::vec3(a.x, b.y, a.z), b}
                                              : Triangle{a, b, glm::vec3(a.x, b.y, a.z)});
    scene.trianglesGeometry.emplace_back(flip ? Triangle{b, glm::vec3(b.x, a.y, a.z), a}
                                              : Triangle{b, a, glm::vec3(b.x, a.y, a.z)});
    scene.trianglesMaterial.push_back(materialId);
    scene.trianglesMaterial.push_back(materialId);
}

void AddXZRect(Scene& scene, const glm::vec3& a, const glm::vec3& b, uint32_t materialId, bool flip /* = false*/)
{
    scene.trianglesGeometry.emplace_back(flip ? Triangle{a, glm::vec3(a.x, a.y, b.z), b}
                                              : Triangle{a, b, glm::vec3(a.x, a.y, b.z)});
    scene.trianglesGeometry.emplace_back(flip ? Triangle{b, glm::vec3(b.x, a.y, a.z), a}
                                              : Triangle{b, a, glm::vec3(b.x, a.y, a.z)});
    scene.trianglesMaterial.push_back(materialId);
    scene.trianglesMaterial.push_back(materialId);
}

void AddYZRect(Scene& scene, const glm::vec3& a, const glm::vec3& b, uint32_t materialId, bool flip /* = false*/)
{
    scene.trianglesGeometry.emplace_back(flip ? Triangle{a, glm::vec3(a.x, a.y, b.z), b}
                                              : Triangle{a, b, glm::vec3(a.x, a.y, b.z)});
    scene.trianglesGeometry.emplace_back(flip ? Triangle{b, glm::vec3(a.x, b.y, a.z), a}
                                              : Triangle{b, a, glm::vec3(a.x, b.y, a.z)});
    scene.trianglesMaterial.push_back(materialId);
    scene.trianglesMaterial.push_back(materialId);
}
