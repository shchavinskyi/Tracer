#include "bvh.h"

BVHNode CreateNode(BVHTree& tree, std::vector<Sphere>& spheres, std::size_t start, std::size_t end)
{
    BVHNode result;

    int axis = rand() % 3;
    auto comparator = (axis == 0) ? BoxXCompare : (axis == 1) ? BoxYCompare : BoxZCompare;

    size_t objectSpan = end - start;

    if (objectSpan == 1)
    {
        result.leftNodeIndex = result.rightNodeIndex = start;

        result.objectIndex = start;

        // fill aabb
        result.aabb = SphereAabb(spheres[result.leftNodeIndex]);
    }
    else
    {
        std::sort(spheres.begin() + start, spheres.begin() + end, comparator);

        auto mid = start + objectSpan / 2;

        result.leftNodeIndex = tree.nodes.size();
        tree.nodes.push_back(BVHNode{});
        BVHNode& leftNode = tree.nodes[result.leftNodeIndex];
        leftNode = CreateNode(tree, spheres, start, mid);

        result.rightNodeIndex = tree.nodes.size();
        tree.nodes.push_back(BVHNode{});
        BVHNode& rightNode = tree.nodes[result.rightNodeIndex];
        rightNode = CreateNode(tree, spheres, mid, end);

        // fill aabb
        result.aabb = SurroundingAabb(leftNode.aabb, rightNode.aabb);
    }

    return result;
}