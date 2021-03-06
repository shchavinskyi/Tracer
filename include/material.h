#ifndef MATERIAL_H
#define MATERIAL_H

#include "data.h"

#include <glm/vec3.hpp>
#include <optional>
#include <variant>

struct Material
{
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

    MaterialData data;

    static inline Material CreateDiffuse(const glm::vec3& albedo);

    static inline Material CreateMetal(const glm::vec3& albedo, float fuzziness);

    static inline Material CreateDielectric(const glm::vec3& albedo, float fuzziness);

    static inline Material CreateLight(const glm::vec3& emissive);
};

struct ScatterResult
{
    Ray ray;
    glm::vec3 attenuation;
};

std::optional<ScatterResult> Scatter(const Ray& inRay, const HitResult& hitResult, const Material& material);

// -- inline functions --

Material Material::CreateDiffuse(const glm::vec3& albedo)
{
    return Material{Material::DiffuseData{albedo}};
}

Material Material::CreateMetal(const glm::vec3& albedo, float fuzziness)
{
    return Material{Material::MetalData{albedo, fuzziness}};
}

Material Material::CreateDielectric(const glm::vec3& albedo, float fuzziness)
{
    return Material{Material::DielectricData{albedo, fuzziness}};
}

Material Material::CreateLight(const glm::vec3& emissive)
{
    return Material{Material::LightData{emissive}};
}

#endif // MATERIAL_H
