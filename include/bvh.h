#ifndef BVH_H
#define BVH_H

#include "logging.h"
#include "objects.h"
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
    glm::vec3 invD{1.0f / ray.direction.x, 1.0f / ray.direction.y, 1.0f / ray.direction.z};
    glm::vec3 t0s = (aabb.min - ray.origin) * invD;
    glm::vec3 t1s = (aabb.max - ray.origin) * invD;

    glm::vec3 tsmaller = glm::min(t0s, t1s);
    glm::vec3 tbigger = glm::max(t0s, t1s);

    float tmin = glm::max(tsmaller[0], glm::max(tsmaller[1], tsmaller[2]));
    float tmax = glm::min(tbigger[0], glm::min(tbigger[1], tbigger[2]));

    return tmin < tmax;
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
    BVHNode()
        : leftNodeIndex(0)
        , rightNodeIndex(std::numeric_limits<uint32_t>::max())
    {
    }

    Aabb aabb;
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

BVHNode CreateNode(BVHTree& tree, std::vector<Sphere>& spheres, std::uint32_t start, std::uint32_t end);

BVHTree BuildBVHTree(std::vector<Sphere>& spheres);

void TraverseBVH(const Ray& ray, const BVHTree& tree, const BVHNode& node, std::vector<uint32_t>& objectIndexes);

#endif // BVH_H