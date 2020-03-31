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

bool scatter(const Ray& inRay, const HitResult& hitResult, glm::vec3& attenuation, Ray& scatteredRay)
{
    assert(hitResult.material != nullptr);

    switch (hitResult.material->type)
    {
    case MaterialType::Diffuse:
        return diffuse(inRay, hitResult, attenuation, scatteredRay);

    case MaterialType::Metal:
        return diffuse(inRay, hitResult, attenuation, scatteredRay);
    }

    return false;
}