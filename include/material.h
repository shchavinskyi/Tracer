#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/vec3.hpp>

struct Ray;
struct HitResult;

enum class MaterialType
{
    Diffuse,
    Metal,
    Dielectric
};

struct Material
{
    MaterialType type;
    glm::vec3 albedo;
    float fuzziness; // TODO refactor material data
};

bool scatter(const Ray& inRay, const HitResult& hitResult, const Material& material, glm::vec3& attenuation,
             Ray& scatteredRay);

#endif // MATERIAL_H