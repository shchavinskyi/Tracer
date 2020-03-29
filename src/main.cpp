#include "glm/gtx/intersect.hpp"
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "save.h"
#include "scene.h"
#include "utils.h"

#include <iostream>
#include <limits>
#include <string>
#include <vector>

struct Ray
{
    glm::vec3 origin;
    glm::vec3 direction;
};

struct HitResult
{
    glm::vec3 position;
    glm::vec3 normal;
};

struct Camera
{
    glm::vec3 origin;
    glm::vec3 lowerLeft;
    glm::vec3 horizontal;
    glm::vec3 vertical;
};

glm::vec3 backgroundColor(const Ray& ray)
{
    float t = 0.5f * (ray.direction.y + 1.0f);
    const glm::vec3 top{0.5f, 0.7f, 1.0f};
    const glm::vec3 bottom(1.0f, 1.0f, 1.0f);
    return (1.0f - t) * top + t * bottom;
}

Ray getRay(const Camera& camera, float u, float v)
{
    return Ray{camera.origin, glm::normalize(camera.lowerLeft + u * camera.horizontal + v * camera.vertical)};
}

int main(int /*argc*/, char** /*argv*/)
{
    glm::ivec2 imageSize{800, 400};
    glm::vec3* imageBuffer = static_cast<glm::vec3*>(malloc(sizeof(glm::vec3) * imageSize.x * imageSize.y));

    const int samplesPerPixel = 20;

    Camera camera;
    camera.lowerLeft = glm::vec3(-2.0f, -1.0f, -1.0f);
    camera.horizontal = glm::vec3(4.0f, 0.0f, 0.0f);
    camera.vertical = glm::vec3(0.0f, 2.0f, 0.0f);
    camera.origin = glm::vec3(0.0f, 0.0f, 0.0f);

    Scene scene;
    scene.spheres.emplace_back(Sphere{glm::vec3(0.0f, 0.0f, -1.0f), 0.5f});
    scene.spheres.emplace_back(Sphere{glm::vec3(0.0f, 100.5f, -1.0f), 100.0f});

    for (int y = 0; y < imageSize.y; ++y)
    {
        for (int x = 0; x < imageSize.x; ++x)
        {
            glm::vec3 acumulatedColor(0.0f, 0.0f, 0.0f);

            for (int i = 0; i < samplesPerPixel; ++i)
            {
                float u = (float(x) + randomUnitFloat()) / imageSize.x;
                float v = (float(y) + randomUnitFloat()) / imageSize.y;

                Ray ray = getRay(camera, u, v);

                bool isHit = false;
                glm::length_t distanceToClosest = std::numeric_limits<glm::length_t>::max();
                glm::vec3 colorOfClosest;

                for (const Sphere& sphere : scene.spheres)
                {
                    glm::vec3 intersectionPosition;
                    glm::vec3 intersectionNormal;

                    if (glm::intersectRaySphere(ray.origin, ray.direction, sphere.center, sphere.radius,
                                                intersectionPosition, intersectionNormal))
                    {
                        const glm::length_t lenghtToIntersection =
                            glm::vec3(intersectionPosition - ray.origin).length();
                        if (lenghtToIntersection < distanceToClosest)
                        {
                            distanceToClosest = lenghtToIntersection;
                            glm::vec3 vectorColor = 0.5f * (intersectionNormal + glm::vec3(1.0f, 1.0f, 1.0f));
                            colorOfClosest = vectorColor;
                            isHit = true;
                        }
                    }
                }

                if (!isHit)
                {
                    colorOfClosest = backgroundColor(ray);
                }

                acumulatedColor += colorOfClosest;
            }

            acumulatedColor /= samplesPerPixel;
            imageBuffer[x + y * imageSize.x] = acumulatedColor;
        }
    }

    saveImageBufferToFile(imageSize, imageBuffer, "output.png");

    free(imageBuffer);

    return 0;
}