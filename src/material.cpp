#include "material.h"

#include "data.h"
#include "random.h"

namespace {

bool Diffuse(const Ray& /*inRay*/, const HitResult& hitResult, const DiffuseData& data, glm::vec3& attenuation,
             Ray& scatteredRay)
{
    static RandomUnitVectorGenerator unitGenerator;

    glm::vec3 target = hitResult.position + hitResult.normal + unitGenerator.Generate();

    scatteredRay.origin = hitResult.position;
    scatteredRay.direction = glm::normalize(target - hitResult.position);
    attenuation = data.albedo;

    return true;
}

bool Metal(const Ray& inRay, const HitResult& hitResult, const MetalData& data, glm::vec3& attenuation,
           Ray& scatteredRay)
{
    static RandomInUnitSphereGenerator generator;

    glm::vec3 reflected = glm::reflect(inRay.direction, hitResult.normal);
    scatteredRay.origin = hitResult.position;
    scatteredRay.direction = glm::normalize(reflected + data.fuzziness * generator.Generate());
    attenuation = data.albedo;
    return glm::dot(reflected, hitResult.normal) > 0.0f;
}

float Schlick(float cosine, float refractIndex)
{
    float r0 = (1.0f - refractIndex) / (1.0f + refractIndex);
    r0 = r0 * r0;
    return r0 + (1.0f - r0) * pow((1.0f - cosine), 5.0f);
}

bool Dielectric(const Ray& inRay, const HitResult& hitResult, const DielectricData& data, glm::vec3& attenuation,
                Ray& scatteredRay)
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
        scatteredRay.origin = hitResult.position;
        scatteredRay.direction = glm::normalize(reflected + data.fuzziness * generator.Generate());
        attenuation = data.albedo;
        return glm::dot(reflected, normal) > 0.0f;
    }

    static RandomFloatGenerator generator;

    float reflectProb = Schlick(cosTheta, schlickIndex);
    if (generator.Generate() < reflectProb)
    {
        glm::vec3 reflected = glm::reflect(inRay.direction, normal);
        scatteredRay.origin = hitResult.position;
        scatteredRay.direction = glm::normalize(reflected + data.fuzziness * generator.Generate());
        attenuation = data.albedo;
        return true;
    }

    glm::vec3 refracted = glm::refract(inRay.direction, normal, refractIndex);
    scatteredRay.direction = refracted;
    scatteredRay.origin = hitResult.position;
    attenuation = glm::vec3(1.0f, 1.0f, 1.0f);
    return true;
}

} // namespace

bool Scatter(const Ray& inRay, const HitResult& hitResult, const Material& material, glm::vec3& attenuation,
             Ray& scatteredRay)
{
    switch (material.type)
    {
    case MaterialType::Diffuse: {
        const DiffuseData& data = std::get<DiffuseData>(material.data);
        return Diffuse(inRay, hitResult, data, attenuation, scatteredRay);
    }

    case MaterialType::Metal: {
        const MetalData& data = std::get<MetalData>(material.data);
        return Metal(inRay, hitResult, data, attenuation, scatteredRay);
    }

    case MaterialType::Dielectric: {
        const DielectricData& data = std::get<DielectricData>(material.data);
        return Dielectric(inRay, hitResult, data, attenuation, scatteredRay);
    }

    case MaterialType::Light:
        return false;
    }

    return false;
}
