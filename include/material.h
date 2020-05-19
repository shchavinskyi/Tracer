#ifndef MATERIAL_H
#define MATERIAL_H

#include <glm/vec3.hpp>
#include <variant>

struct Ray;
struct HitResult;

struct DiffuseData
{
    glm::vec3 albedo;
};

struct MetalData
{
    glm::vec3 albedo;
    float fuzziness;
};

struct DielectricData
{
    glm::vec3 albedo;
    float fuzziness;
};

struct LightData
{
    glm::vec3 emissive;
};

using MaterialData = std::variant<DiffuseData, MetalData, DielectricData, LightData>;

struct Material
{
    MaterialData data;
};

inline Material CreateDiffuseMaterial(const glm::vec3& albedo) { return Material{DiffuseData{albedo}}; }

inline Material CreateMetalMaterial(const glm::vec3& albedo, float fuzziness)
{
    return Material{MetalData{albedo, fuzziness}};
}

inline Material CreateDielectricMaterial(const glm::vec3& albedo, float fuzziness)
{
    return Material{DielectricData{albedo, fuzziness}};
}

inline Material CreateLightMaterial(const glm::vec3& emissive) { return Material{LightData{emissive}}; }

bool Scatter(const Ray& inRay, const HitResult& hitResult, const Material& material, glm::vec3& attenuation,
             Ray& scatteredRay);

#endif // MATERIAL_H
