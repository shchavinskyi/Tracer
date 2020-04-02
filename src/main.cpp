#include "camera.h"
#include "material.h"
#include "random.h"
#include "ray.h"
#include "save.h"
#include "scene.h"
#include "utils.h"

#include <glm/gtx/intersect.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

// TODO find a way to store generators somewhere
RandomUnitVectorInHemisphereGenerator hemisphereGenerator;
RandomUnitVectorGenerator unitGenerator;
RandomInUnitSphereGenerator inSphereGenerator;

RandomFloatGenerator floatGenerator;

glm::vec3 backgroundColor(const Ray& ray)
{
    float t = 0.5f * (ray.direction.y + 1.0f);
    const glm::vec3 top{0.5f, 0.7f, 1.0f};
    const glm::vec3 bottom(1.0f, 1.0f, 1.0f);
    return (1.0f - t) * top + t * bottom;
}

Ray getRay(const Camera& camera, float u, float v)
{
    return Ray{camera.origin,
               glm::normalize(camera.lowerLeft + u * camera.horizontal + v * camera.vertical - camera.origin)};
}

glm::vec3 tracePath(const Ray& ray, int maxDepth, const Scene& scene)
{
    if (maxDepth <= 0)
    {
        return glm::vec3(0, 0, 0);
    }

    bool isHit = false;
    float distanceToClosest = std::numeric_limits<float>::max();

    HitResult hitResult;

    for (const SphereObject& sphere : scene.spheres)
    {
        glm::vec3 intersectionPosition;
        glm::vec3 intersectionNormal;

        if (glm::intersectRaySphere(ray.origin, ray.direction, sphere.geometry.center, sphere.geometry.radius,
                                    intersectionPosition, intersectionNormal))
        {
            const float lenghtToIntersection = glm::length(intersectionPosition - ray.origin);
            if (lenghtToIntersection < distanceToClosest && lenghtToIntersection > 0.001f)
            {
                distanceToClosest = lenghtToIntersection;
                hitResult.normal = intersectionNormal;
                hitResult.position = intersectionPosition;
                hitResult.material = &sphere.material;
                isHit = true;
            }
        }
    }

    if (isHit)
    {
        // glm::vec3 target = hitResult.position + hemisphereGenerator.GenerateFor(hitResult.normal);
        // glm::vec3 target = hitResult.position + hitResult.normal + inSphereGenerator.Generate();
        // glm::vec3 target = hitResult.position + hitResult.normal + unitGenerator.Generate();

        // Ray newRay{hitResult.position, glm::normalize(target - hitResult.position)};
        // return 0.5f * tracePath(newRay, maxDepth - 1, scene);

        Ray scattered;
        glm::vec3 attenuation;

        if (scatter(ray, hitResult, attenuation, scattered))
            return attenuation * tracePath(scattered, maxDepth - 1, scene);

        return glm::vec3(0.0f, 0.0f, 0.0f);
    }

    return backgroundColor(ray);
}

int main(int /*argc*/, char** /*argv*/)
{
    glm::ivec2 imageSize{600, 300};
    glm::vec3* imageBuffer = static_cast<glm::vec3*>(malloc(sizeof(glm::vec3) * imageSize.x * imageSize.y));

    constexpr int samplesPerPixel = 50;
    constexpr int maxBounces = 50;

    glm::vec3 position(-2.5f, -2.0f, 1.5f);
    glm::vec3 at(0.0f, 0.0f, -1.0f);
    glm::vec3 up(0.0f, 1.0f, 0.0f);
    const float aspectRatio = float(imageSize.x) / imageSize.y;
    const float fov = 45.0f;
    Camera camera = cameraFromView(position, at, up, fov, aspectRatio);

    Scene scene;

    scene.spheres.emplace_back(SphereObject{Sphere{glm::vec3(0.0f, 0.0f, -1.0f), 0.5f},
                                            Material{MaterialType::Diffuse, glm::vec3(0.7f, 0.3f, 0.3f), 0.0f}});
    scene.spheres.emplace_back(SphereObject{Sphere{glm::vec3(0.0f, 100.5f, -1.0f), 100.0f},
                                            Material{MaterialType::Diffuse, glm::vec3(0.8f, 0.8f, 0.0f), 0.0f}});

    scene.spheres.emplace_back(SphereObject{Sphere{glm::vec3(1.0f, 0.0f, -1.0f), 0.5f},
                                            Material{MaterialType::Metal, glm::vec3(0.8f, 0.6f, 0.2f), 0.3f}});
    scene.spheres.emplace_back(SphereObject{Sphere{glm::vec3(-1.0f, 0.0f, -1.0f), 0.5f},
                                            Material{MaterialType::Dielectric, glm::vec3(0.8f, 0.8f, 0.8f), 0.0f}});

    for (int y = 0; y < imageSize.y; ++y)
    {
        for (int x = 0; x < imageSize.x; ++x)
        {
            glm::vec3 acumulatedColor(0.0f, 0.0f, 0.0f);

            for (int i = 0; i < samplesPerPixel; ++i)
            {
                float u = (float(x) + floatGenerator.Generate()) / imageSize.x;
                float v = (float(y) + floatGenerator.Generate()) / imageSize.y;

                Ray ray = getRay(camera, u, v);

                acumulatedColor += tracePath(ray, maxBounces, scene);
            }

            acumulatedColor /= samplesPerPixel;
            imageBuffer[x + y * imageSize.x] = acumulatedColor;
        }
    }

    saveImageBufferToFile(imageSize, imageBuffer, "output.png");

    free(imageBuffer);

    return 0;
}