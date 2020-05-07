#include "bvh.h"

BVHNode CreateNode(BVHTree& tree, std::vector<Sphere>& spheres, std::uint32_t start, std::uint32_t end)
{
    BVHNode result;

    int axis = rand() % 3;
    auto comparator = (axis == 0) ? BoxXCompare : (axis == 1) ? BoxYCompare : BoxZCompare;

    uint32_t objectSpan = end - start;

    if (objectSpan == 1)
    {
        result.leftNodeIndex = result.rightNodeIndex = start;

        // fill aabb
        result.aabb = SphereAabb(spheres[result.leftNodeIndex]);
    }
    else
    {
        std::sort(spheres.begin() + start, spheres.begin() + end, comparator);

        auto mid = start + objectSpan / 2;

        result.leftNodeIndex = static_cast<uint32_t>(tree.nodes.size());
        tree.nodes.push_back(BVHNode{});
        BVHNode& leftNode = tree.nodes[result.leftNodeIndex];
        leftNode = CreateNode(tree, spheres, start, mid);

        result.rightNodeIndex = static_cast<uint32_t>(tree.nodes.size());
        tree.nodes.push_back(BVHNode{});
        BVHNode& rightNode = tree.nodes[result.rightNodeIndex];
        rightNode = CreateNode(tree, spheres, mid, end);

        // fill aabb
        result.aabb = SurroundingAabb(leftNode.aabb, rightNode.aabb);
    }

    return result;
}

BVHTree BuildBVHTree(std::vector<Sphere>& spheres)
{
    TRACE_EXECUTION("BuildBVHTree");

    BVHTree tree;

    tree.nodes.reserve(spheres.size() * 2 + 1);
    tree.nodes.push_back(BVHNode{});
    BVHNode& rootNode = tree.nodes[0];
    rootNode = CreateNode(tree, spheres, 0, static_cast<uint32_t>(spheres.size()));

    return tree;
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
