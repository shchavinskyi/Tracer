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
RandomFloatGenerator floatGenerator;
RandomVectorGenerator colorGenerator(0.0f, 1.0f);

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

    glm::vec3 cameraPosition(-4.0f, 4.0f, 2.5f);
    glm::vec3 at(0.0f, 0.0f, 0.0f);
    glm::vec3 up(0.0f, 0.0f, 1.0f);

    const float aspectRatio = float(imageSize.x) / imageSize.y;
    const float fov = 45.0f;
    Camera camera = cameraFromView(cameraPosition, at, up, fov, aspectRatio);

    Scene scene;

    scene.spheres.emplace_back(SphereObject{Sphere{glm::vec3(0.0f, 0.0f, -1000.0f), 1000.0f},
                                            Material{MaterialType::Diffuse, glm::vec3(0.8f, 0.8f, 0.0f), 0.0f}});

    /*
        scene.spheres.emplace_back(SphereObject{Sphere{glm::vec3(0.0f, 0.0f, 0.5f), 0.5f},
                                                Material{MaterialType::Diffuse, glm::vec3(0.7f, 0.3f, 0.3f), 0.0f}});
        scene.spheres.emplace_back(SphereObject{Sphere{glm::vec3(1.1f, 0.0f, 0.5f), 0.5f},
                                                Material{MaterialType::Metal, glm::vec3(0.8f, 0.6f, 0.2f), 0.3f}});
        scene.spheres.emplace_back(SphereObject{Sphere{glm::vec3(-1.1f, 0.0f, 0.5f), 0.5f},
                                                Material{MaterialType::Dielectric, glm::vec3(0.8f, 0.8f, 0.8f), 0.0f}});

        scene.spheres.emplace_back(SphereObject{Sphere{glm::vec3(0.0f, 1.5f, 0.2f), 0.2f},
                                                Material{MaterialType::Diffuse, glm::vec3(0.9f, 0.1f, 0.1f), 0.5f}});
        scene.spheres.emplace_back(SphereObject{Sphere{glm::vec3(0.0f, -1.5f, 0.2f), 0.2f},
                                                Material{MaterialType::Diffuse, glm::vec3(0.1f, 0.9f, 0.1f), 0.5f}});
    */

    constexpr float width = 7.0f;
    constexpr float minRadiuswidth = 0.15f;
    constexpr float maxRadiuswidth = 0.85f;
    constexpr uint32_t sphereCount = 50;
    for (int i = 0; i < sphereCount; ++i)
    {
        glm::vec3 position;
        position.x = (floatGenerator.Generate() * width * 2.0f) - width;
        position.y = (floatGenerator.Generate() * width * 2.0f) - width;
        float radius = floatGenerator.Generate() * (maxRadiuswidth - minRadiuswidth) + minRadiuswidth;
        position.z = radius;

        glm::vec3 color = colorGenerator.Generate();

        float materialFactor = floatGenerator.Generate();

        if (materialFactor < 0.4f)
        {
            scene.spheres.emplace_back(
                SphereObject{Sphere{position, radius}, Material{MaterialType::Diffuse, color, 0.0f}});
        }
        else if (materialFactor > 0.6f)
        {
            scene.spheres.emplace_back(SphereObject{Sphere{position, radius},
                                                    Material{MaterialType::Metal, color, floatGenerator.Generate()}});
        }
        else
        {
            scene.spheres.emplace_back(
                SphereObject{Sphere{position, radius}, Material{MaterialType::Dielectric, color, 0.0f}});
        }
    }

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