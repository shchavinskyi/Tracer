#include "scene.h"

#include "bvh.h"
#include "ray.h"

#include <glm/gtx/intersect.hpp>
#include <set>
#include <vector>

namespace {
glm::vec3 backgroundColor(const Ray& ray)
{
    float t = 0.5f * (ray.direction.z + 1.0f);
    const glm::vec3 top{0.5f, 0.7f, 1.0f};
    const glm::vec3 bottom(1.0f, 1.0f, 1.0f);
    return (1.0f - t) * top + t * bottom;
}
} // namespace

void AddSphereAndMaterial(Scene& scene, const Sphere& sphere, const Material& material)
{
    scene.spheresGeometry.push_back(sphere);
    scene.materials.push_back(material);
    scene.spheresMaterial.push_back(scene.materials.size() - 1);
}

void AddSphere(Scene& scene, const Sphere& sphere, size_t materialId)
{
    scene.spheresGeometry.push_back(sphere);
    scene.spheresMaterial.push_back(materialId);
}

size_t AddMaterial(Scene& scene, const Material& material)
{
    scene.materials.push_back(material);
    return scene.materials.size() - 1;
}

void TraverseBVH(const Ray& ray, const BVHTree& tree, const BVHNode& node, std::vector<uint32_t>& objectIndexes)
{
    if (node.leftNodeIndex == node.rightNodeIndex)
    {
        // Hit last aabb
        objectIndexes.push_back(node.leftNodeIndex);
        return;
    }

    const BVHNode& leftNode = tree.nodes[node.leftNodeIndex];
    const BVHNode& rightNode = tree.nodes[node.rightNodeIndex];

    if (Hit(ray, leftNode.aabb))
    {
        TraverseBVH(ray, tree, leftNode, objectIndexes);
    }
    if (Hit(ray, rightNode.aabb))
    {
        TraverseBVH(ray, tree, rightNode, objectIndexes);
    }
}

void TraverseBVH_no_recursion(const Ray& ray, const BVHTree& tree, const BVHNode& node,
                              std::vector<uint32_t>& objectIndexes)
{
    if (node.leftNodeIndex == node.rightNodeIndex)
    {
        // Hit last aabb
        objectIndexes.push_back(node.leftNodeIndex);
        return;
    }

    std::vector<uint32_t> starts;

    starts.reserve(size_t(glm::log2(float(tree.nodes.size()) / 2.0f)));
    starts.push_back(node.rightNodeIndex);
    starts.push_back(node.leftNodeIndex);

    while (!starts.empty())
    {
        BVHNode* currentNode = (BVHNode*)&tree.nodes[starts.back()];

        starts.pop_back();

        while (currentNode)
        {
            if (Hit(ray, currentNode->aabb))
            {
                if (currentNode->leftNodeIndex == currentNode->rightNodeIndex)
                {
                    objectIndexes.push_back(currentNode->leftNodeIndex);
                    currentNode = nullptr;
                }
                else
                {
                    BVHNode* leftNode = (BVHNode*)&tree.nodes[currentNode->leftNodeIndex];
                    BVHNode* rightNode = (BVHNode*)&tree.nodes[currentNode->rightNodeIndex];
                    const bool hitLeft = Hit(ray, leftNode->aabb);
                    const bool hitRight = Hit(ray, rightNode->aabb);
                    if (hitLeft && hitRight)
                    {
                        starts.push_back(currentNode->rightNodeIndex);
                        currentNode = leftNode;
                    }
                    else if (hitLeft)
                    {
                        currentNode = leftNode;
                    }
                    else if (hitRight)
                    {
                        currentNode = rightNode;
                    }
                    else
                    {
                        currentNode = nullptr;
                    }
                }
            }
            else
            {
                currentNode = nullptr;
            }
        }
    }
}

glm::vec3 TracePath(const Ray& ray, int maxDepth, const Scene& scene, const BVHTree& tree)
{
    if (maxDepth <= 0)
    {
        return glm::vec3(0, 0, 0);
    }

    bool isHit = false;
    float distanceToClosest = std::numeric_limits<float>::max();

    HitResult hitResult;

    const BVHNode& rootNode = tree.nodes[0];
    if (Hit(ray, rootNode.aabb))
    {
        std::vector<uint32_t> objectIndexes;

        TraverseBVH(ray, tree, rootNode, objectIndexes);

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
                    hitResult.materialId = scene.spheresMaterial[objectIndex];
                    isHit = true;
                }
            }
        }
    }

    if (isHit)
    {
        Ray scattered;
        glm::vec3 attenuation;

        const Material& material = scene.materials[hitResult.materialId];

        if (scatter(ray, hitResult, material, attenuation, scattered))
            return attenuation * TracePath(scattered, maxDepth - 1, scene, tree);

        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    return backgroundColor(ray);
}