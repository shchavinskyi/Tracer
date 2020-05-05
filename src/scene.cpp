#include "scene.h"

#include "bvh.h"
#include "objects.h"

#include <glm/gtx/intersect.hpp>
#include <set>
#include <vector>

namespace {
glm::vec3 backgroundColor(const Ray& ray)
{
    return glm::vec3(0.0f, 0.0f, 0.0f);
    /*
    float t = 0.5f * (ray.direction.z + 1.0f);
    const glm::vec3 top{0.5f, 0.7f, 1.0f};
    const glm::vec3 bottom(1.0f, 1.0f, 1.0f);
    return (1.0f - t) * top + t * bottom;
    */
}
} // namespace

void AddSphereAndMaterial(Scene& scene, const Sphere& sphere, const Material& material)
{
    scene.spheresGeometry.push_back(sphere);
    scene.materials.push_back(material);
    scene.spheresMaterial.push_back(scene.materials.size() - 1);
}

void AddTriangleAndMaterial(Scene& scene, const Triangle& triangle, const Material& material)
{
    scene.trianglesGeometry.push_back(triangle);
    scene.materials.push_back(material);
    scene.trianglesMaterial.push_back(scene.materials.size() - 1);
}

void AddSphere(Scene& scene, const Sphere& sphere, size_t materialId)
{
    scene.spheresGeometry.push_back(sphere);
    scene.spheresMaterial.push_back(materialId);
}

void AddTriangle(Scene& scene, const Triangle& triangle, size_t materialId)
{
    scene.trianglesGeometry.push_back(triangle);
    scene.trianglesMaterial.push_back(materialId);
}

void AddXYRect(Scene& scene, const glm::vec3& a, const glm::vec3& b, size_t materialId, bool flip /* = false*/)
{
    assert(a.z == b.z);

    scene.trianglesGeometry.emplace_back(flip ? Triangle{a, glm::vec3(a.x, b.y, a.z), b}
                                              : Triangle{a, b, glm::vec3(a.x, b.y, a.z)});
    scene.trianglesGeometry.emplace_back(flip ? Triangle{b, glm::vec3(b.x, a.y, a.z), a}
                                              : Triangle{b, a, glm::vec3(b.x, a.y, a.z)});
    scene.trianglesMaterial.push_back(materialId);
    scene.trianglesMaterial.push_back(materialId);
}

void AddXZRect(Scene& scene, const glm::vec3& a, const glm::vec3& b, size_t materialId, bool flip /* = false*/)
{
    assert(a.y == b.y);

    scene.trianglesGeometry.emplace_back(Triangle{a, b, glm::vec3(a.x, a.y, b.z)});
    scene.trianglesGeometry.emplace_back(Triangle{b, a, glm::vec3(b.x, a.y, a.z)});
    scene.trianglesMaterial.push_back(materialId);
    scene.trianglesMaterial.push_back(materialId);
}

void AddYZRect(Scene& scene, const glm::vec3& a, const glm::vec3& b, size_t materialId, bool flip /* = false*/)
{
    assert(a.x == b.x);

    scene.trianglesGeometry.emplace_back(flip ? Triangle{a, glm::vec3(a.x, a.y, b.z), b}
                                              : Triangle{a, b, glm::vec3(a.x, a.y, b.z)});
    scene.trianglesGeometry.emplace_back(flip ? Triangle{b, glm::vec3(a.x, b.y, a.z), a}
                                              : Triangle{b, a, glm::vec3(a.x, b.y, a.z)});
    scene.trianglesMaterial.push_back(materialId);
    scene.trianglesMaterial.push_back(materialId);
}

size_t AddMaterial(Scene& scene, const Material& material)
{
    scene.materials.push_back(material);
    return scene.materials.size() - 1;
}

glm::vec3 TracePath(const Ray& ray, int maxDepth, const Scene& scene)
{
    if (maxDepth <= 0)
    {
        return glm::vec3(0, 0, 0);
    }

    bool isHit = false;
    float distanceToClosest = std::numeric_limits<float>::max();

    HitResult hitResult;

    // trace spheres
    for (size_t i = 0; i < scene.spheresGeometry.size(); ++i)
    {
        const Sphere& sphere = scene.spheresGeometry[i];

        glm::vec3 intersectionPosition;
        glm::vec3 intersectionNormal;

        if (glm::intersectRaySphere(ray.origin, ray.direction, sphere.center, sphere.radius, intersectionPosition,
                                    intersectionNormal))
        {
            const float lenghtToIntersection = glm::length(intersectionPosition - ray.origin);
            if (lenghtToIntersection < distanceToClosest && lenghtToIntersection > 0.001f)
            {
                distanceToClosest = lenghtToIntersection;
                hitResult.normal = intersectionNormal;
                hitResult.position = intersectionPosition;
                hitResult.materialId = scene.spheresMaterial[i];
                isHit = true;
            }
        }
    }

    // trace triangles
    for (size_t i = 0; i < scene.trianglesGeometry.size(); ++i)
    {
        const Triangle& triangle = scene.trianglesGeometry[i];
        float distance = std::numeric_limits<float>::min();
        glm::vec2 baryPosition;
        if (glm::intersectRayTriangle(ray.origin, ray.direction, triangle.v1, triangle.v2, triangle.v3, baryPosition,
                                      distance))
        {
            if (distance < distanceToClosest && distance > 0.001f)
            {
                const glm::vec3 edge1 = triangle.v2 - triangle.v1;
                const glm::vec3 edge2 = triangle.v3 - triangle.v1;
                const glm::vec3 position = triangle.v1 + baryPosition.x * edge1 + baryPosition.y * edge2;

                glm::vec3 normal(glm::normalize(glm::cross(edge1, edge2)));

                distanceToClosest = distance;
                hitResult.normal = normal;
                hitResult.position = position;
                hitResult.materialId = scene.trianglesMaterial[i];
                isHit = true;
            }
        }
    }

    if (isHit)
    {
        Ray scattered;
        glm::vec3 attenuation;

        const Material& material = scene.materials[hitResult.materialId];

        if (material.type == MaterialType::Light)
        {
            return material.albedo;
        }
        else if (Scatter(ray, hitResult, material, attenuation, scattered))
            return attenuation * TracePath(scattered, maxDepth - 1, scene);

        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    return backgroundColor(ray);
}