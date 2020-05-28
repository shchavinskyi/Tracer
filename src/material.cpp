#include "material.h"

#include "data.h"
#include "random.h"

#include <optional>

namespace {

ScatterResult Diffuse(const HitResult& hitResult, const Material::DiffuseData& data)
{
    static RandomUnitVectorGenerator unitGenerator;

    glm::vec3 target = hitResult.position + hitResult.normal + unitGenerator.Generate();

    return ScatterResult{Ray{hitResult.position, glm::normalize(target - hitResult.position)}, data.albedo};
}

std::optional<ScatterResult> Metal(const Ray& inRay, const HitResult& hitResult, const Material::MetalData& data)
{
    static RandomInUnitSphereGenerator generator;

    glm::vec3 reflected = glm::reflect(inRay.direction, hitResult.normal);
    return glm::dot(reflected, hitResult.normal) < 0.0f
               ? std::nullopt
               : std::make_optional(ScatterResult{
                     Ray{hitResult.position, glm::normalize(reflected + data.fuzziness * generator.Generate())},
                     data.albedo});
}

float Schlick(float cosine, float refractIndex)
{
    float r0 = (1.0f - refractIndex) / (1.0f + refractIndex);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * pow((1.0f - cosine), 5.0f);
}

std::optional<ScatterResult> Dielectric(const Ray& inRay, const HitResult& hitResult,
                                        const Material::DielectricData& data)
{
    glm::vec3 normal = hitResult.normal;
    float refractIndex = 0.0f;
    float schlickIndex = 0.6f;
    if (glm::dot(hitResult.normal, inRay.direction) > 0.0f)
    {
        normal = -hitResult.normal;
        schlickIndex = 1.0f;
    }

    float cosTheta = glm::min(glm::dot(-inRay.direction, normal), 1.0f);
    float sinTheta = sqrt(1.0f - cosTheta * cosTheta);
    if (sinTheta > 1.0f)
    {
        static RandomInUnitSphereGenerator generator;

        glm::vec3 reflected = glm::reflect(inRay.direction, normal);

        return glm::dot(reflected, normal) < 0.0f
                   ? std::nullopt
                   : std::make_optional(ScatterResult{
                         Ray{hitResult.position, glm::normalize(reflected + data.fuzziness * generator.Generate())},
                         data.albedo});
    }

    static RandomFloatGenerator generator;

    float reflectProb = Schlick(cosTheta, schlickIndex);
    if (generator.Generate() < reflectProb)
    {
        glm::vec3 reflected = glm::reflect(inRay.direction, normal);

        return ScatterResult{Ray{hitResult.position, glm::normalize(reflected + data.fuzziness * generator.Generate())},
                             data.albedo};
    }

    glm::vec3 refracted = glm::refract(inRay.direction, normal, refractIndex);

    return ScatterResult{Ray{hitResult.position, glm::normalize(refracted)}, glm::vec3(1.0f, 1.0f, 1.0f)};
}

template <class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...)->overloaded<Ts...>;

} // namespace

std::optional<ScatterResult> Scatter(const Ray& inRay, const HitResult& hitResult, const Material& material)
{
    return std::visit(
        overloaded{
            [&hitResult](const Material::DiffuseData& data) -> std::optional<ScatterResult> {
                return Diffuse(hitResult, data);
            },
            [&inRay, &hitResult](const Material::MetalData& data) { return Metal(inRay, hitResult, data); },
            [&inRay, &hitResult](const Material::DielectricData& data) { return Dielectric(inRay, hitResult, data); },
            [](auto) -> std::optional<ScatterResult> { return std::nullopt; }},
        material.data);
}
