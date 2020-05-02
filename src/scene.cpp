#include "scene.h"

#include "bvh.h"
#include "ray.h"

#include <glm/gtx/intersect.hpp>

namespace {
glm::vec3 backgroundColor(const Ray& ray)
{
    float t = 0.5f * (ray.direction.y + 1.0f);
    const glm::vec3 top{0.5f, 0.7f, 1.0f};
    const glm::vec3 bottom(1.0f, 1.0f, 1.0f);
    return (1.0f - t) * top + t * bottom;
}
} // namespace

void AddSphere(Scene& scene, Sphere&& sphere, Material&& material)
{
    scene.spheresGeometry.emplace_back(std::move(sphere));
    scene.spheresMaterial.emplace_back(std::move(material));
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

    // TODO save material id somewhere
    // SO number of material can be lower
    int objectIndex = 0;

    for (const Sphere& sphere : scene.spheresGeometry)
    {
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
                hitResult.material = &scene.spheresMaterial[objectIndex];
                isHit = true;
            }
        }

        ++objectIndex;
    }

    if (isHit)
    {
        Ray scattered;
        glm::vec3 attenuation;

        if (scatter(ray, hitResult, attenuation, scattered))
            return attenuation * TracePath(scattered, maxDepth - 1, scene);

        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    return backgroundColor(ray);
}

void TraverseBVH(const Ray& ray, const BVHTree& tree, uint32_t nodeIndex, std::vector<uint32_t>& objectIndexes)
{
    const BVHNode& node = tree.nodes[nodeIndex];

    if (Hit(ray, node.aabb))
    {
        if (node.leftNodeIndex == node.rightNodeIndex)
        {
            // Hit last aabb
            objectIndexes.push_back(node.objectIndex);
            return;
        }
        else
        {
            TraverseBVH(ray, tree, node.leftNodeIndex, objectIndexes);
            TraverseBVH(ray, tree, node.rightNodeIndex, objectIndexes);
        }
    }
}

glm::vec3 TracePathWithBVH(const Ray& ray, int maxDepth, const Scene& scene, const BVHTree& tree)
{
    if (maxDepth <= 0)
    {
        return glm::vec3(0, 0, 0);
    }

    bool isHit = false;
    float distanceToClosest = std::numeric_limits<float>::max();

    HitResult hitResult;

    std::vector<uint32_t> objectIndexes;
    TraverseBVH(ray, tree, 0, objectIndexes);

    for (uint32_t& objectIndex : objectIndexes)
    {
        const Sphere& sphere = scene.spheresGeometry[objectIndex];

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
                hitResult.material = &scene.spheresMaterial[objectIndex];
                isHit = true;
            }
        }
    }

    if (isHit)
    {
        Ray scattered;
        glm::vec3 attenuation;

        if (scatter(ray, hitResult, attenuation, scattered))
            return attenuation * TracePathWithBVH(scattered, maxDepth - 1, scene, tree);

        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    return backgroundColor(ray);
}