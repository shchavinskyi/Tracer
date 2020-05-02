#ifndef BVH_H
#define BVH_H

#include "logging.h"
#include "ray.h"
#include "scene.h"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>

struct Aabb
{
    glm::vec3 min;
    glm::vec3 max;
};

inline bool Hit(const Ray& ray, const Aabb& aabb)
{
    for (int a = 0; a < 3; a++)
    {
        auto invD = 1.0f / ray.direction[a];
        auto t0 = (aabb.min[a] - ray.origin[a]) * invD;
        auto t1 = (aabb.max[a] - ray.origin[a]) * invD;
        if (invD < 0.0f)
            std::swap(t0, t1);
        if (t1 <= t0)
            return false;
    }

    return true;
}

inline Aabb SphereAabb(const Sphere& sphere)
{
    return Aabb{sphere.center - glm::vec3(sphere.radius), sphere.center + glm::vec3(sphere.radius)};
}

inline Aabb SurroundingAabb(const Aabb& box0, const Aabb& box1)
{
    glm::vec3 small(glm::min(box0.min.x, box1.min.x), glm::min(box0.min.y, box1.min.y),
                    glm::min(box0.min.z, box1.min.z));
    glm::vec3 big(glm::max(box0.max.x, box1.max.x), glm::max(box0.max.y, box1.max.y), glm::max(box0.max.z, box1.max.z));
    return Aabb{small, big};
}

struct BVHNode
{
    Aabb aabb;
    uint32_t objectIndex;
    uint32_t leftNodeIndex;
    uint32_t rightNodeIndex;
};

struct BVHTree
{
    std::vector<BVHNode> nodes;
};

inline bool BoxCompare(const Sphere& left, const Sphere& right, int axis)
{
    Aabb leftBox = SphereAabb(left);
    Aabb rightBox = SphereAabb(right);

    return leftBox.min[axis] < rightBox.min[axis];
}

inline bool BoxXCompare(const Sphere& left, const Sphere& right) { return BoxCompare(left, right, 0); }

inline bool BoxYCompare(const Sphere& left, const Sphere& right) { return BoxCompare(left, right, 1); }

inline bool BoxZCompare(const Sphere& left, const Sphere& right) { return BoxCompare(left, right, 2); }

BVHNode CreateNode(BVHTree& tree, std::vector<Sphere>& spheres, std::size_t start, std::size_t end);

inline BVHTree BuildBVHTree(std::vector<Sphere>& spheres)
{
    TRACE_EXECUTION("BuildBVHTree");

    BVHTree tree;

    tree.nodes.reserve(spheres.size() * 2);

    tree.nodes.push_back(BVHNode{});
    BVHNode& rootNode = tree.nodes[0];
    rootNode = CreateNode(tree, spheres, 0, spheres.size());

    return tree;
}

#endif // BVH_H