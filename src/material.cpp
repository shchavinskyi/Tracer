#include "material.h"

#include "data.h"
#include "random.h"

namespace {

bool Diffuse(const HitResult& hitResult, const Material::DiffuseData& data, ScatterResult& scatterResult)
{
    static RandomUnitVectorGenerator unitGenerator;

    glm::vec3 target = hitResult.position + hitResult.normal + unitGenerator.Generate();

    scatterResult.ray.origin = hitResult.position;
    scatterResult.ray.direction = glm::normalize(target - hitResult.position);
    scatterResult.attenuation = data.albedo;

    return true;
}

bool Metal(const Ray& inRay, const HitResult& hitResult, const Material::MetalData& data, ScatterResult& scatterResult)
{
    static RandomInUnitSphereGenerator generator;

    glm::vec3 reflected = glm::reflect(inRay.direction, hitResult.normal);
    scatterResult.ray.origin = hitResult.position;
    scatterResult.ray.direction = glm::normalize(reflected + data.fuzziness * generator.Generate());
    scatterResult.attenuation = data.albedo;
    return glm::dot(reflected, hitResult.normal) > 0.0f;
}

float Schlick(float cosine, float refractIndex)
{
    float r0 = (1.0f - refractIndex) / (1.0f + refractIndex);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * pow((1.0f - cosine), 5.0f);
}

bool Dielectric(const Ray& inRay, const HitResult& hitResult, const Material::DielectricData& data,
                ScatterResult& scatterResult)
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
        scatterResult.ray.origin = hitResult.position;
        scatterResult.ray.direction = glm::normalize(reflected + data.fuzziness * generator.Generate());
        scatterResult.attenuation = data.albedo;
        return glm::dot(reflected, normal) > 0.0f;
    }

    static RandomFloatGenerator generator;

    float reflectProb = Schlick(cosTheta, schlickIndex);
    if (generator.Generate() < reflectProb)
    {
        glm::vec3 reflected = glm::reflect(inRay.direction, normal);
        scatterResult.ray.origin = hitResult.position;
        scatterResult.ray.direction = glm::normalize(reflected + data.fuzziness * generator.Generate());
        scatterResult.attenuation = data.albedo;
        return true;
    }

    glm::vec3 refracted = glm::refract(inRay.direction, normal, refractIndex);
    scatterResult.ray.direction = refracted;
    scatterResult.ray.origin = hitResult.position;
    scatterResult.attenuation = glm::vec3(1.0f, 1.0f, 1.0f);
    return true;
}

template <class... Ts>
struct overloaded : Ts...
{
    using Ts::operator()...;
};

template <class... Ts>
overloaded(Ts...)->overloaded<Ts...>;

} // namespace

bool Scatter(const Ray& inRay, const HitResult& hitResult, const Material& material, ScatterResult& scatterResult)
{
    return std::visit(overloaded{[&hitResult, &scatterResult](const Material::DiffuseData& data) {
                                     return Diffuse(hitResult, data, scatterResult);
                                 },
                                 [&inRay, &hitResult, &scatterResult](const Material::MetalData& data) {
                                     return Metal(inRay, hitResult, data, scatterResult);
                                 },
                                 [&inRay, &hitResult, &scatterResult](const Material::DielectricData& data) {
                                     return Dielectric(inRay, hitResult, data, scatterResult);
                                 },
                                 [](auto) { return false; }},
                      material.data);
}
