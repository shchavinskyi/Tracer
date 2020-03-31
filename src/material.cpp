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
    glm::vec3 reflected = glm::reflect(inRay.direction, hitResult.normal);
    scatteredRay.origin = hitResult.position;
    scatteredRay.direction = reflected;
    attenuation = hitResult.material->albedo;
    return glm::dot(reflected, hitResult.normal) > 0.0f;
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
    }

    return false;
}