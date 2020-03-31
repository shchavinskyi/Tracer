#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/vec3.hpp>

struct Ray;
struct HitResult;

enum class MaterialType
{
    Diffuse,
    Metal
};

struct Material
{
    MaterialType type;
    glm::vec3 albedo;
};

bool scatter(const Ray& inRay, const HitResult& hitResult, glm::vec3& attenuation, Ray& scatteredRay);

#endif // MATERIAL_H