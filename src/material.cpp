#include "material.h"

#include "random.h"
#include "ray.h"

bool diffuse(const Ray& /*inRay*/, const HitResult& hitResult, glm::vec3& attenuation, Ray& scatteredRay)
{
    static RandomUnitVectorGenerator unitGenerator;

    glm::vec3 target = hitResult.position + hitResult.normal + unitGenerator.Generate();

    scatteredRay.origin = hitResult.position;
    scatteredRay.direction = glm::normalize(target - hitResult.position);
    attenuation = hitResult.material->albedo;

    return true;
}

bool metal(const Ray& inRay, const HitResult& hitResult, glm::vec3& attenuation, Ray& scatteredRay)
{
    static RandomInUnitSphereGenerator generator;

    glm::vec3 reflected = glm::reflect(inRay.direction, hitResult.normal);
    scatteredRay.origin = hitResult.position;
    scatteredRay.direction = glm::normalize(reflected + hitResult.material->fuzziness * generator.Generate());
    attenuation = hitResult.material->albedo;
    return glm::dot(reflected, hitResult.normal) > 0.0f;
}

bool dielectric(const Ray& inRay, const HitResult& hitResult, glm::vec3& attenuation, Ray& scatteredRay)
{
    glm::vec3 normal = hitResult.normal;
    float refractIndex = 0.9f; // eta [-1.0f, 1.0f]

    if (glm::dot(hitResult.normal, inRay.direction) > 0.0f)
    {
        normal = -hitResult.normal;
    }

    glm::vec3 refracted = glm::refract(inRay.direction, normal, refractIndex);
    scatteredRay.direction = refracted;
    scatteredRay.origin = hitResult.position;
    attenuation = glm::vec3(1.0f, 1.0f, 1.0f);
    return true;
}

bool scatter(const Ray& inRay, const HitResult& hitResult, glm::vec3& attenuation, Ray& scatteredRay)
{
    assert(hitResult.material != nullptr);

    switch (hitResult.material->type)
    {
    case MaterialType::Diffuse:
        return diffuse(inRay, hitResult, attenuation, scatteredRay);

    case MaterialType::Metal:
        return metal(inRay, hitResult, attenuation, scatteredRay);

    case MaterialType::Dielectric:
        return dielectric(inRay, hitResult, attenuation, scatteredRay);
    }

    return false;
}